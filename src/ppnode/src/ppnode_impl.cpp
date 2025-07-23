#include "ppnode/ppnode.hpp"

namespace ppnode
{

// Sets subscribers, publishers, services, and path planning algorithm
PpNode::PpNode(const rclcpp::NodeOptions & options)
: Node("pp_node", options), ref_lat_(0.0), ref_lon_(0.0), ref_point_set_(false),
  boat_position_available_(false), survey_received_(false)
{
    RCLCPP_INFO(this->get_logger(), "===== Path Planning Node Initializing =====");
    
    // Initialize path planner with default algorithm (centroid + offset)
    path_planner_ = std::make_unique<SimplePathPlanner>();
    
    // Create subscriber for survey info (polygon vertices + start point)
    survey_sub_ = this->create_subscription<ros_otter_custom_interfaces::msg::SurveyInfo>(
        "polygon_vertices", ppnode_utils::config::DEFAULT_QUEUE_SIZE,
        std::bind(&PpNode::surveyInfoCallback, this, std::placeholders::_1));
    
    // Create subscriber for boat GPS position
    gps_sub_ = this->create_subscription<ros_otter_custom_interfaces::msg::GpsInfo>(
        "gps_info", ppnode_utils::config::DEFAULT_QUEUE_SIZE,
        std::bind(&PpNode::boatGpsCallback, this, std::placeholders::_1));
    
    // Create service server for OTTER_TCP_node leg requests | OBS Leg number is requested
    leg_srv_ = this->create_service<ros_otter_custom_interfaces::srv::LegMode>(
        "leg_srv",
        std::bind(&PpNode::legServiceCallback, this, 
                  std::placeholders::_1, std::placeholders::_2));
    
    // Create publishers for complete paths
    path_gps_pub_ = this->create_publisher<ros_otter_custom_interfaces::msg::Paths>(
        "path_gps", ppnode_utils::config::DEFAULT_QUEUE_SIZE);
    
    path_cartesian_pub_ = this->create_publisher<ros_otter_custom_interfaces::msg::Paths>(
        "path_cartesian", ppnode_utils::config::DEFAULT_QUEUE_SIZE);
    
    RCLCPP_INFO(this->get_logger(), "Configured components:");
    RCLCPP_INFO(this->get_logger(), "  Service: leg_srv (for OTTER_TCP_node)");
    RCLCPP_INFO(this->get_logger(), "  Publishers: path_gps, path_cartesian");
    RCLCPP_INFO(this->get_logger(), "===== Path Planning Node Ready =====");
}


// Callback for boat GPS position
void PpNode::boatGpsCallback(const ros_otter_custom_interfaces::msg::GpsInfo::SharedPtr msg)
{
    try {
        // Update current boat position
        current_boat_position_ = ppnode_utils::gpsInfoToGpsPoint(*msg);
        
        // Set initial boat position if this is the first GPS message
        // This becomes our coordinate system reference point if no survey received yet
        if (!boat_position_available_) {
            initial_boat_position_ = current_boat_position_;
            boat_position_available_ = true;
            
            RCLCPP_INFO(this->get_logger(), "Initial boat position set: lat=%.6f, lon=%.6f", 
                        initial_boat_position_.lat, initial_boat_position_.lon);
            
            // Set as reference point if survey hasn't been received yet
            // Boat position has priority over survey start point for coordinate reference
            if (!survey_received_) {
                setReferencePoint(initial_boat_position_);
            }
        }
    } catch (const std::exception& e) {
        RCLCPP_ERROR(this->get_logger(), "Error processing boat GPS: %s", e.what());
    }
}


// Callback for survey polygon information
void PpNode::surveyInfoCallback(const ros_otter_custom_interfaces::msg::SurveyInfo::SharedPtr msg)
{
    RCLCPP_DEBUG(this->get_logger(), "Received survey info with %zu polygon vertices", 
                msg->vertices.size());
    
    try {
        survey_received_ = true;
        
        // Parse polygon vertices from the Vertex array (lat/lon to internal format)
        polygon_vertices_gps_ = parseSurveyPolygon(msg->vertices);
        
        // Determine reference point priority: boat position > survey start point
        ppnode_utils::GpsPoint reference_point;
        if (boat_position_available_) {
            reference_point = initial_boat_position_;
            RCLCPP_INFO(this->get_logger(), "Using boat position as reference point");
        } else {
            reference_point = ppnode_utils::GpsPoint(msg->start.latitude, msg->start.longitude);
            if (!reference_point.isValid()) {
                RCLCPP_ERROR(this->get_logger(), "Invalid survey start point coordinates");
                return;
            }
            RCLCPP_INFO(this->get_logger(), "Using survey start point as reference point");
        }
        
        setReferencePoint(reference_point);
        
        // Convert all polygon vertices to Cartesian for path planning algorithms
        // Path planning works in local Cartesian coordinates (meters) not GPS degrees
        polygon_vertices_cartesian_.clear();
        polygon_vertices_cartesian_ = ppnode_utils::gpsPolygonToCartesian(
            polygon_vertices_gps_, ref_lat_, ref_lon_);
        
        // Log vertex conversions coordinate transform
        for (size_t i = 0; i < polygon_vertices_gps_.size(); ++i) {
            const auto& gps_point = polygon_vertices_gps_[i];
            const auto& cart_point = polygon_vertices_cartesian_[i];
            RCLCPP_DEBUG(this->get_logger(), "Vertex %zu: GPS(%.6f, %.6f) -> Cartesian(%.2f, %.2f)", 
                        i, gps_point.lat, gps_point.lon, cart_point.x, cart_point.y);
        }
        
        // Process polygon if we have minimum vertices for path planning
        if (hasMinimumVertices()) {
            processPolygonAndPublishPath();
        }
    } catch (const std::exception& e) {
        RCLCPP_ERROR(this->get_logger(), "Error processing survey info: %s", e.what());
    }
}


// Parse survey polygon from /polygon_vertices topic to internal GPS points
std::vector<ppnode_utils::GpsPoint> PpNode::parseSurveyPolygon(
    const std::vector<ros_otter_custom_interfaces::msg::Vertex>& vertices) const
{
    try {
        std::vector<ppnode_utils::GpsPoint> gps_vertices = 
            ppnode_utils::vertexArrayToGpsPoints(vertices);
        
        RCLCPP_DEBUG(this->get_logger(), "Parsed %zu polygon vertices", gps_vertices.size());
        return gps_vertices;
    } catch (const std::exception& e) {
        RCLCPP_ERROR(this->get_logger(), "Error parsing survey polygon: %s", e.what());
        return {};
    }
}


// Set reference point - boat position || surveyinfo start point
void PpNode::setReferencePoint(const ppnode_utils::GpsPoint& point)
{
    if (!point.isValid()) {
        RCLCPP_ERROR(this->get_logger(), "Invalid GPS coordinates for reference point");
        return;
    }
    
    ref_lat_ = point.lat;
    ref_lon_ = point.lon;
    ref_point_set_ = true;
    
    RCLCPP_DEBUG(this->get_logger(), "Reference point set to: lat=%.6f, lon=%.6f", 
                ref_lat_, ref_lon_);
}


// Calculate and store complete multi-leg path from polygon vertices
void PpNode::calculateAndStorePath()
{
    if (!path_planner_) {
        throw std::runtime_error("Path planner not initialized");
    }
    
    // Calculate multi-leg path using the configured algorithm
    auto leg_pairs = path_planner_->calculateMultiLegPath(polygon_vertices_cartesian_, MAX_LEGS);
    
    // Convert to ROS message format and store for lookup
    calculated_legs_.clear();
    calculated_legs_.reserve(leg_pairs.size());
    
    for (const auto& [start_cart, end_cart] : leg_pairs) {
        ros_otter_custom_interfaces::msg::Leg leg;
        
        // Convert Cartesian coordinates to GPS for storage
        ppnode_utils::GpsPoint start_gps = ppnode_utils::cartesianToGps(start_cart, ref_lat_, ref_lon_);
        ppnode_utils::GpsPoint end_gps = ppnode_utils::cartesianToGps(end_cart, ref_lat_, ref_lon_);
        
        // Fill start vertex
        leg.start.latitude = start_gps.lat;
        leg.start.longitude = start_gps.lon;
        
        // Fill end vertex
        leg.end.latitude = end_gps.lat;
        leg.end.longitude = end_gps.lon;
        
        calculated_legs_.push_back(leg);
    }
    
    RCLCPP_INFO(this->get_logger(), "Calculated %zu legs for survey path", calculated_legs_.size());
}


// Process polygon and publish complete multi-leg path
void PpNode::processPolygonAndPublishPath()
{
    if (!hasMinimumVertices()) {
        RCLCPP_WARN(this->get_logger(), "Need at least %zu vertices to form a polygon", 
                    ppnode_utils::config::MIN_POLYGON_VERTICES);
        return;
    }
    
    RCLCPP_DEBUG(this->get_logger(), "Processing polygon with %zu vertices for multi-leg path", 
                polygon_vertices_cartesian_.size());
    
    try {
        // Calculate and store the complete path
        calculateAndStorePath();
        
        // Publish the complete path for external consumption
        publishCompletePath();
        
        RCLCPP_INFO(this->get_logger(), "Successfully published complete %zu-leg path", 
                    calculated_legs_.size());
    } catch (const std::exception& e) {
        RCLCPP_ERROR(this->get_logger(), "Failed to process polygon for multi-leg path: %s", e.what());
    }
}


// Publish the complete calculated path in both coordinate systems
void PpNode::publishCompletePath()
{
    if (calculated_legs_.empty()) {
        RCLCPP_WARN(this->get_logger(), "No calculated legs to publish");
        return;
    }
    
    try {
        // Create GPS path message
        ros_otter_custom_interfaces::msg::Paths gps_paths;
        gps_paths.paths = calculated_legs_;  // Already in GPS coordinates
        path_gps_pub_->publish(gps_paths);
        
        // Create Cartesian path message
        ros_otter_custom_interfaces::msg::Paths cartesian_paths;
        cartesian_paths.paths.reserve(calculated_legs_.size());
        
        for (const auto& gps_leg : calculated_legs_) {
            ros_otter_custom_interfaces::msg::Leg cartesian_leg;
            
            // Convert GPS coordinates back to Cartesian for publication
            ppnode_utils::GpsPoint start_gps(gps_leg.start.latitude, gps_leg.start.longitude);
            ppnode_utils::GpsPoint end_gps(gps_leg.end.latitude, gps_leg.end.longitude);
            
            ppnode_utils::CartesianPoint start_cart = ppnode_utils::gpsToCartesian(start_gps, ref_lat_, ref_lon_);
            ppnode_utils::CartesianPoint end_cart = ppnode_utils::gpsToCartesian(end_gps, ref_lat_, ref_lon_);
            
            // Store as latitude/longitude fields for consistency (represents x/y in Cartesian)
            cartesian_leg.start.latitude = start_cart.x;
            cartesian_leg.start.longitude = start_cart.y;
            cartesian_leg.end.latitude = end_cart.x;
            cartesian_leg.end.longitude = end_cart.y;
            
            cartesian_paths.paths.push_back(cartesian_leg);
        }
        
        path_cartesian_pub_->publish(cartesian_paths);
        
        RCLCPP_DEBUG(this->get_logger(), "Published complete path with %zu legs to both topics", 
                    calculated_legs_.size());
    } catch (const std::exception& e) {
        RCLCPP_ERROR(this->get_logger(), "Error publishing complete path: %s", e.what());
    }
}


// Service callback for OTTER_TCP_node leg requests (lookup table)
void PpNode::legServiceCallback(
    const std::shared_ptr<ros_otter_custom_interfaces::srv::LegMode::Request> request,
    std::shared_ptr<ros_otter_custom_interfaces::srv::LegMode::Response> response)
{
    RCLCPP_INFO(this->get_logger(), "Leg service called for leg number: %ld", request->leg_number);
    
    try {
        int64_t leg_number = request->leg_number;
        
        // Validate that we have calculated path available
        if (!hasCalculatedPath()) {
            response->success = false;
            response->message = "No calculated path available. Please send polygon vertices first.";
            RCLCPP_WARN(this->get_logger(), "Leg service called but no calculated path available");
            return;
        }
        
        // Validate leg number is within valid range (1-based indexing)
        if (leg_number < 1 || leg_number > static_cast<int64_t>(calculated_legs_.size())) {
            response->success = false;
            response->message = "Invalid leg number. Available legs: 1 to " + 
                              std::to_string(calculated_legs_.size());
            RCLCPP_WARN(this->get_logger(), "Invalid leg number %ld requested. Available: 1-%zu", 
                       leg_number, calculated_legs_.size());
            return;
        }
        
        // Lookup the requested leg (convert to 0-based indexing)
        const auto& requested_leg = calculated_legs_[leg_number - 1];
        
        // Fill response with GPS coordinates in LegMode.srv format
        // Start point
        response->lat0 = std::abs(requested_leg.start.latitude);
        response->lat0_dir = (requested_leg.start.latitude >= 0) ? "N" : "S";
        response->lon0 = std::abs(requested_leg.start.longitude);
        response->lon0_dir = (requested_leg.start.longitude >= 0) ? "E" : "W";
        
        // End point
        response->lat1 = std::abs(requested_leg.end.latitude);
        response->lat1_dir = (requested_leg.end.latitude >= 0) ? "N" : "S";
        response->lon1 = std::abs(requested_leg.end.longitude);
        response->lon1_dir = (requested_leg.end.longitude >= 0) ? "E" : "W";
        
        response->speed = ppnode_utils::config::DEFAULT_SPEED_MS;  // Default survey speed in m/s
        response->success = true;
        response->message = "Leg " + std::to_string(leg_number) + " coordinates retrieved from lookup table";
        
        RCLCPP_INFO(this->get_logger(), "Leg service response for leg %ld (lookup):", leg_number);
        RCLCPP_INFO(this->get_logger(), "  Start: %.6f°%s, %.6f°%s", 
                    response->lat0, response->lat0_dir.c_str(),
                    response->lon0, response->lon0_dir.c_str());
        RCLCPP_INFO(this->get_logger(), "  End: %.6f°%s, %.6f°%s", 
                    response->lat1, response->lat1_dir.c_str(),
                    response->lon1, response->lon1_dir.c_str());
        RCLCPP_INFO(this->get_logger(), "  Speed: %.2f m/s", response->speed);
        
    } catch (const std::exception& e) {
        response->success = false;
        response->message = std::string("Error retrieving leg coordinates: ") + e.what();
        RCLCPP_ERROR(this->get_logger(), "Leg service failed: %s", response->message.c_str());
    }
}


// Utility functions for validation and configuration

// Validate GPS coordinates
bool PpNode::validateGpsCoordinates(double lat, double lon) const
{
    return ppnode_utils::validateGpsCoordinates(lat, lon);
}

// Validate polygon vertices
bool PpNode::validatePolygonVertices(const std::vector<ppnode_utils::GpsPoint>& vertices) const
{
    return ppnode_utils::validatePolygonVertices(vertices);
}


/**
 * @brief Set a custom path planning algorithm
 * Allows runtime switching of path planning strategies for different survey types
 * @param planner Unique pointer to a PathPlanner implementation
 */
void PpNode::setPathPlanner(std::unique_ptr<PathPlanner> planner)
{
    if (planner) {
        path_planner_ = std::move(planner);
        RCLCPP_INFO(this->get_logger(), "Custom path planner set");
    } else {
        RCLCPP_WARN(this->get_logger(), "Attempted to set null path planner");
    }
}

}  // namespace ppnode
