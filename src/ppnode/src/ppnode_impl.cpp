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
    
    // Create publishers for debugging/visualization
    result_gps_pub_ = this->create_publisher<ros_otter_custom_interfaces::msg::GpsInfo>(
        "result_points_gps", ppnode_utils::config::DEFAULT_QUEUE_SIZE);
    
    result_cartesian_pub_ = this->create_publisher<geometry_msgs::msg::Point>(
        "result_points_cartesian", ppnode_utils::config::DEFAULT_QUEUE_SIZE);
    
    RCLCPP_INFO(this->get_logger(), "Configured components:");
    RCLCPP_INFO(this->get_logger(), "  Service: leg_srv (for OTTER_TCP_node)");
    RCLCPP_INFO(this->get_logger(), "  Publishers: result_points_gps, result_points_cartesian");
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
        
        // Process polygon if we have minimum vertices
        if (hasMinimumVertices()) {
            processPolygonAndPublishResults();
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


// Calculate start & end points from polygon vertices using path planner
std::pair<ppnode_utils::CartesianPoint, ppnode_utils::CartesianPoint> PpNode::calculatePathPoints() const
{
    if (!path_planner_) {
        throw std::runtime_error("Path planner not initialized");
    }
    
    return path_planner_->calculatePath(polygon_vertices_cartesian_);
}


// Process collected polygon vertices and publish result points
void PpNode::processPolygonAndPublishResults()
{
    if (!hasMinimumVertices()) {
        RCLCPP_WARN(this->get_logger(), "Need at least %zu vertices to form a polygon", 
                    ppnode_utils::config::MIN_POLYGON_VERTICES);
        return;
    }
    
    RCLCPP_DEBUG(this->get_logger(), "Processing polygon with %zu vertices", 
                polygon_vertices_cartesian_.size());
    
    try {
        // Calculate the path using the configured algorithm
        auto [start_point, end_point] = calculatePathPoints();
        
        // Publish both result points for debugging/visualization tools
        publishResultPoint(start_point, 1);
        publishResultPoint(end_point, 2);
        
        RCLCPP_DEBUG(this->get_logger(), "Successfully published result points for visualization");
    } catch (const std::exception& e) {
        RCLCPP_ERROR(this->get_logger(), "Failed to process polygon: %s", e.what());
    }
}


// Publish a single result point in both GPS and Cartesian formats
void PpNode::publishResultPoint(const ppnode_utils::CartesianPoint& cart_point, int point_id)
{
    try {
        // Publish Cartesian coordinates
        auto cart_msg = geometry_msgs::msg::Point();
        cart_msg.x = cart_point.x;
        cart_msg.y = cart_point.y;
        cart_msg.z = 0.0;  // Assuming 2D plane
        result_cartesian_pub_->publish(cart_msg);
        
        // Convert to GPS coords and publish
        ppnode_utils::GpsPoint gps_point = ppnode_utils::cartesianToGps(cart_point, ref_lat_, ref_lon_);
        auto gps_msg = ppnode_utils::gpsPointToGpsInfo(gps_point, this->now().seconds());
        result_gps_pub_->publish(gps_msg);
        
        RCLCPP_DEBUG(this->get_logger(), "Point %d - Cartesian: (%.2f, %.2f), GPS: (%.6f, %.6f)", 
                    point_id, cart_point.x, cart_point.y, gps_point.lat, gps_point.lon);
    } catch (const std::exception& e) {
        RCLCPP_ERROR(this->get_logger(), "Error publishing result point %d: %s", point_id, e.what());
    }
}


// Service callback for OTTER_TCP_node leg requests
void PpNode::legServiceCallback(
    const std::shared_ptr<ros_otter_custom_interfaces::srv::LegMode::Request> request,
    std::shared_ptr<ros_otter_custom_interfaces::srv::LegMode::Response> response)
{
    RCLCPP_INFO(this->get_logger(), "Leg service called for leg number: %ld", request->leg_number);
    
    try {
        // Store leg number for future use (not used in current implementation)
        int64_t leg_number = request->leg_number;
        (void)leg_number; // Suppress unused variable warning for now
        
        
        // Validate that we have polygon vertices
        if (!hasMinimumVertices()) {
            response->success = false;
            response->message = "No polygon vertices available for path calculation";
            RCLCPP_WARN(this->get_logger(), "Leg service called but insufficient polygon vertices");
            return;
        }
        
        // Calculate path points using the configured path planning algorithm
        auto [start_cartesian, end_cartesian] = calculatePathPoints();
        
        // Convert Cartesian path points back to GPS coordinates for boat navigation
        ppnode_utils::GpsPoint start_gps = ppnode_utils::cartesianToGps(start_cartesian, ref_lat_, ref_lon_);
        ppnode_utils::GpsPoint end_gps = ppnode_utils::cartesianToGps(end_cartesian, ref_lat_, ref_lon_);
        
        // Fill response with GPS coordinates in LegMode.srv format
        response->lat0 = std::abs(start_gps.lat);
        response->lat0_dir = (start_gps.lat >= 0) ? "N" : "S";
        response->lon0 = std::abs(start_gps.lon);
        response->lon0_dir = (start_gps.lon >= 0) ? "E" : "W";
        
        response->lat1 = std::abs(end_gps.lat);
        response->lat1_dir = (end_gps.lat >= 0) ? "N" : "S";
        response->lon1 = std::abs(end_gps.lon);
        response->lon1_dir = (end_gps.lon >= 0) ? "E" : "W";
        
        response->speed = ppnode_utils::config::DEFAULT_SPEED_MS;  // Default survey speed in m/s
        response->success = true;
        response->message = "Leg coordinates calculated successfully from polygon vertices";
        
        // Also publish result points for debugging/visualization
        publishResultPoint(start_cartesian, 1);
        publishResultPoint(end_cartesian, 2);
        
        RCLCPP_INFO(this->get_logger(), "Leg service response for leg %ld:", leg_number);
        RCLCPP_INFO(this->get_logger(), "  Start: %.6f°%s, %.6f°%s", 
                    response->lat0, response->lat0_dir.c_str(),
                    response->lon0, response->lon0_dir.c_str());
        RCLCPP_INFO(this->get_logger(), "  End: %.6f°%s, %.6f°%s", 
                    response->lat1, response->lat1_dir.c_str(),
                    response->lon1, response->lon1_dir.c_str());
        RCLCPP_INFO(this->get_logger(), "  Speed: %.2f m/s", response->speed);
        
    } catch (const std::exception& e) {
        response->success = false;
        response->message = std::string("Error calculating leg coordinates: ") + e.what();
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
