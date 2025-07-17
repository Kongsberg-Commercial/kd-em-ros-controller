#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <geometry_msgs/msg/point.hpp>
#include <vector>
#include <limits>
#include "path_publisher/utils.hpp"

class LegPublisher : public rclcpp::Node
{
public:
    LegPublisher()
    : Node("leg_publisher"), survey_started_(false), polygon_data_received_(false), current_leg_index_(0), last_intersected_edge_index_(0)
    {
        // Declare parameters for configurable values
        this->declare_parameter<double>("overlap_percentage", 50.0);
        this->declare_parameter<double>("swath_width", 50.0);
        
        // Publisher on /path topic
        pub_ = this->create_publisher<std_msgs::msg::String>("/path", 10);
        
        // Publisher for debug data
        debug_pub_ = this->create_publisher<std_msgs::msg::String>("/debug_data", 10);

        // Subscription to receive polygon data from webapp
        polygon_sub_ = this->create_subscription<std_msgs::msg::String>(
            "polygon_data", 10,
            std::bind(&LegPublisher::polygon_data_callback, this, std::placeholders::_1)
        );
        
        // Subscription to receive leg requests
        leg_request_sub_ = this->create_subscription<std_msgs::msg::String>(
            "leg_request", 10,
            std::bind(&LegPublisher::leg_request_callback, this, std::placeholders::_1)
        );
        
        RCLCPP_INFO(this->get_logger(), "LegPublisher initialized");
        RCLCPP_INFO(this->get_logger(), "- Waiting for polygon data on 'polygon_data' topic");
        RCLCPP_INFO(this->get_logger(), "- Waiting for leg requests on 'leg_request' topic");
        RCLCPP_INFO(this->get_logger(), "- Will publish path legs on '/path' topic");
    }

private:
    // Mission boundary vertices (polygon)
    std::vector<LatLonPoint> mission_boundary_vertices_;
    
    // User defined start point
    LatLonPoint initial_start_point_;
    
    // Calculated survey parameters
    LatLonPoint nearest_vertice_start_;
    LatLonPoint closer_neighbor_vertice_;
    LatLonPoint survey_start_point_;
    LatLonPoint survey_end_point_;  // Intersection point with polygon boundary
    
    // Survey parameters
    double swath_width_;  // in meters
    double initial_vector_bearing_;  // bearing from nearest to closer neighbor
    double odd_bearing_;   // bearing for odd legs (original direction)
    double even_bearing_;  // bearing for even legs (opposite direction)
    double overlap_percentage_;  // percentage of swath width for overlap (50% default)
    bool survey_started_;
    
    // Last survey leg end point (for continuing survey)
    LatLonPoint last_survey_leg_end_point_;
    LatLonPoint last_boundary_edge_start_;  // Start of the edge where last intersection occurred
    LatLonPoint last_boundary_edge_end_;    // End of the edge where last intersection occurred
    
    // Subscription to receive polygon data from webapp
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr polygon_sub_;
    
    // Subscription to receive leg requests
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr leg_request_sub_;
    
    // Flag to track if we have received polygon data
    bool polygon_data_received_;
    
    // Current leg index for tracking survey progress
    int current_leg_index_;
    
    // Starting edge index for alternating pattern
    int starting_edge_index_;
    
    // Last intersected edge index for tracking progression
    int last_intersected_edge_index_;

    // Check if survey is complete based on coverage and geometry
    bool is_survey_complete() {
        // Calculate polygon perimeter
        double polygon_perimeter = 0.0;
        for (size_t i = 0; i < mission_boundary_vertices_.size(); ++i) {
            size_t next_i = (i + 1) % mission_boundary_vertices_.size();
            polygon_perimeter += calculate_distance(mission_boundary_vertices_[i], mission_boundary_vertices_[next_i]);
        }
        
        // Calculate effective swath width (accounting for overlap)
        double effective_swath_width = swath_width_ * (1.0 - overlap_percentage_ / 100.0);
        
        // Calculate coverage distance
        double coverage_distance = current_leg_index_ * effective_swath_width;
        double coverage_percentage = (coverage_distance / polygon_perimeter) * 100.0;
        
        RCLCPP_INFO(this->get_logger(), 
                   "Survey progress: Leg %d, Perimeter: %.2f m, Coverage: %.1f%%, Effective swath: %.2f m", 
                   current_leg_index_, polygon_perimeter, coverage_percentage, effective_swath_width);
        
        // Consider survey complete if we've covered >90% of the polygon perimeter
        return coverage_percentage > 90.0;
    }

    void initialize_mission_data() {
        // Mission boundary vertices and initial start point should already be set from webapp data
        // Just set default survey parameters here
        
        // Get configurable parameters
        swath_width_ = this->get_parameter("swath_width").as_double();
        overlap_percentage_ = this->get_parameter("overlap_percentage").as_double();
        
        // Validate parameters
        if (swath_width_ <= 0.0) {
            RCLCPP_WARN(this->get_logger(), "Invalid swath_width: %.2f, using default 50.0", swath_width_);
            swath_width_ = 50.0;
        }
        
        if (overlap_percentage_ < 0.0 || overlap_percentage_ > 100.0) {
            RCLCPP_WARN(this->get_logger(), "Invalid overlap_percentage: %.2f, using default 50.0", overlap_percentage_);
            overlap_percentage_ = 50.0;
        }
        
        RCLCPP_INFO(this->get_logger(), "Mission data initialized");
        RCLCPP_INFO(this->get_logger(), "Mission boundary has %zu vertices", mission_boundary_vertices_.size());
        RCLCPP_INFO(this->get_logger(), "Initial start point: %.6f Lat, %.6f Lon", initial_start_point_.lat, initial_start_point_.lon);
        RCLCPP_INFO(this->get_logger(), "Swath width: %.2f meters", swath_width_);
    }
    
    void calculate_survey_parameters() {
        if (mission_boundary_vertices_.empty()) {
            RCLCPP_ERROR(this->get_logger(), "No mission boundary vertices available");
            return;
        }
        
        // Find nearest vertex from initial start point
        double min_distance = std::numeric_limits<double>::max();
        size_t nearest_index = 0;
        
        for (size_t i = 0; i < mission_boundary_vertices_.size(); ++i) {
            double distance = calculate_distance(initial_start_point_, mission_boundary_vertices_[i]);
            if (distance < min_distance) {
                min_distance = distance;
                nearest_index = i;
            }
        }
        
        nearest_vertice_start_ = mission_boundary_vertices_[nearest_index];
        RCLCPP_INFO(this->get_logger(), "Nearest vertex: %.6f Lat, %.6f Lon | (distance: %.2f m)", 
                    nearest_vertice_start_.lat, nearest_vertice_start_.lon, min_distance);

    
        // Find farther neighboring vertex
        size_t prev_index;
        if (nearest_index == 0) {
            prev_index = mission_boundary_vertices_.size() - 1;
        } else {
            prev_index = (nearest_index - 1 + mission_boundary_vertices_.size()) % mission_boundary_vertices_.size();
        }

        size_t next_index;
        if (nearest_index == mission_boundary_vertices_.size() - 1) {
            next_index = 0;
        } else {
            next_index = (nearest_index + 1) % mission_boundary_vertices_.size();
        }
        // Log previous and next vertex indices
        RCLCPP_INFO(this->get_logger(), "Previous vertex index: %zu, Next vertex index: %zu", prev_index, next_index);
        
        // Calculate distances to previous and next vertices
        double dist_to_prev = calculate_distance(nearest_vertice_start_, mission_boundary_vertices_[prev_index]);
        double dist_to_next = calculate_distance(nearest_vertice_start_, mission_boundary_vertices_[next_index]);
        
        // and choose the closer one
        if (dist_to_prev < dist_to_next) {
            closer_neighbor_vertice_ = mission_boundary_vertices_[prev_index];
        } else {
            closer_neighbor_vertice_ = mission_boundary_vertices_[next_index];
        }
        
        RCLCPP_INFO(this->get_logger(), "Closer neighbor vertex: %.6f Lat, %.6f Lon", 
                    closer_neighbor_vertice_.lat, closer_neighbor_vertice_.lon);


        // Calculate initial vector leg (bearing from nearest to closer neighbor)
        initial_vector_bearing_ = calculate_bearing(nearest_vertice_start_, closer_neighbor_vertice_);
        RCLCPP_INFO(this->get_logger(), "Initial vector bearing: %.2f degrees", initial_vector_bearing_);
        
        // Pre-calculate odd and even bearings for alternating pattern
        odd_bearing_ = initial_vector_bearing_;
        even_bearing_ = std::fmod(initial_vector_bearing_ + 180.0, 360.0);
        RCLCPP_INFO(this->get_logger(), "Odd bearing (original): %.2f degrees", odd_bearing_);
        RCLCPP_INFO(this->get_logger(), "Even bearing (opposite): %.2f degrees", even_bearing_);
        
        // Set survey start point directly at the nearest vertex
        survey_start_point_ = nearest_vertice_start_;
        RCLCPP_INFO(this->get_logger(), "Survey start point: %.6f Lat, %.6f Lon", 
                    survey_start_point_.lat, survey_start_point_.lon);
        
        // For first leg, set end point directly to closer neighbor vertex (not ray intersection)
        survey_end_point_ = closer_neighbor_vertice_;
        last_survey_leg_end_point_ = survey_end_point_;
        
        // Set the edge information for the first leg (from nearest to closer neighbor)
        last_boundary_edge_start_ = nearest_vertice_start_;
        last_boundary_edge_end_ = closer_neighbor_vertice_;
        
        // Find and store the starting edge index for alternating pattern
        starting_edge_index_ = nearest_index; // The edge starts from this vertex
        
        // Calculate distance from survey start to end point
        double survey_leg_distance = calculate_distance(survey_start_point_, survey_end_point_);
        RCLCPP_INFO(this->get_logger(), "Survey end point (closer neighbor): %.6f Lat, %.6f Lon", 
                    survey_end_point_.lat, survey_end_point_.lon);
        RCLCPP_INFO(this->get_logger(), "Survey leg distance: %.2f meters", survey_leg_distance);
        
        // Survey not started yet
        survey_started_ = false;
        RCLCPP_INFO(this->get_logger(), "Survey started: %s", survey_started_ ? "true" : "false");
    }


    // Callback function to handle polygon data from webapp
    void polygon_data_callback(const std_msgs::msg::String::SharedPtr msg) {
        RCLCPP_INFO(this->get_logger(), "Received polygon data: %s", msg->data.c_str());
        
        try {
            // Parse JSON data (simplified parsing)
            // Expected format: {"vertices":[[lat1,lon1],[lat2,lon2],...], "start":[lat,lon]}
            std::string data = msg->data;
            
            // TODO: Replace with proper JSON parsing (like nlohmann/json)
            // For now, extract vertices and start point manually
            
            // Clear existing data
            mission_boundary_vertices_.clear();
            
            // Simple parsing - find vertices array
            size_t vertices_start = data.find("\"vertices\":");
            if (vertices_start != std::string::npos) {
                // Extract coordinates from the received data
                // This is a simplified parser - in production, use a proper JSON library
                
                // For now, let's use the received data structure
                // Example: {"vertices":[[59.42953,10.46894],[59.43134,10.47675],...], "start":[59.42752,10.46615]}
                
                // Parse vertices (simplified - assumes the format is correct)
                size_t bracket_start = data.find("[[", vertices_start);
                size_t bracket_end = data.find("]]", bracket_start);
                
                if (bracket_start != std::string::npos && bracket_end != std::string::npos) {
                    std::string vertices_str = data.substr(bracket_start + 2, bracket_end - bracket_start - 2);
                    
                    // Split by "],["
                    std::vector<std::string> vertex_pairs;
                    size_t pos = 0;
                    while ((pos = vertices_str.find("],[")) != std::string::npos) {
                        vertex_pairs.push_back(vertices_str.substr(0, pos));
                        vertices_str.erase(0, pos + 3);
                    }
                    vertex_pairs.push_back(vertices_str); // Add the last pair
                    
                    // Parse each vertex pair
                    for (const auto& pair : vertex_pairs) {
                        size_t comma_pos = pair.find(',');
                        if (comma_pos != std::string::npos) {
                            double lat = std::stod(pair.substr(0, comma_pos));
                            double lon = std::stod(pair.substr(comma_pos + 1));
                            mission_boundary_vertices_.push_back(LatLonPoint(lat, lon));
                            // RCLCPP_INFO(this->get_logger(), "Parsed vertex: %.6f Lat, %.6f Lon", lat, lon);
                        }
                    }
                }
            }
            
            // Parse start point
            size_t start_pos = data.find("\"start\":");
            if (start_pos != std::string::npos) {
                size_t bracket_start = data.find("[", start_pos);
                size_t bracket_end = data.find("]", bracket_start);
                
                if (bracket_start != std::string::npos && bracket_end != std::string::npos) {
                    std::string start_str = data.substr(bracket_start + 1, bracket_end - bracket_start - 1);
                    size_t comma_pos = start_str.find(',');
                    if (comma_pos != std::string::npos) {
                        double lat = std::stod(start_str.substr(0, comma_pos));
                        double lon = std::stod(start_str.substr(comma_pos + 1));
                        initial_start_point_ = LatLonPoint(lat, lon);
                        // RCLCPP_INFO(this->get_logger(), "Parsed start point: %.6f Lat, %.6f Lon", lat, lon);
                    }
                }
            }
            
            polygon_data_received_ = true;
            
            // Initialize mission data and calculate survey parameters
            initialize_mission_data();
            calculate_survey_parameters();
            
            RCLCPP_INFO(this->get_logger(), "Polygon data processed. Starting survey...");
            
        } catch (const std::exception& e) {
            RCLCPP_ERROR(this->get_logger(), "Failed to parse polygon data: %s", e.what());
        }
    }

    // Callback function to handle leg requests
    void leg_request_callback(const std_msgs::msg::String::SharedPtr msg) {
        RCLCPP_INFO(this->get_logger(), "Received leg request: %s", msg->data.c_str());
        
        if (!polygon_data_received_) {
            RCLCPP_WARN(this->get_logger(), "Cannot publish leg - no polygon data received yet");
            return;
        }
        
        // Check if we've reached the end of the survey
        if (survey_started_ && is_survey_complete()) {
            RCLCPP_INFO(this->get_logger(), "Survey completed based on coverage analysis - no more legs to publish");
            return;
        }
        
        // Also check for reasonable maximum leg count to prevent infinite surveys
        if (survey_started_ && current_leg_index_ >= 100) {
            RCLCPP_WARN(this->get_logger(), "Maximum leg count reached (100) - stopping survey");
            return;
        }
        
        // Increment the leg index before publishing to ensure consistent state
        current_leg_index_++;
        
        // Publish the next leg
        publish_leg();
        
        RCLCPP_INFO(this->get_logger(), "Published leg %d", current_leg_index_);
    }


    // Publish the survey leg
    void publish_leg()
    {
        // Only publish if we have received polygon data
        if (!polygon_data_received_) {
            return;
        }
        
        double lat0, lon0, lat1, lon1;
        
        // If survey has not started, publish the first leg from survey start point to closer neighbor vertex
        if (!survey_started_) {
            // First leg: from survey start point to closer neighbor vertex
            lat0 = survey_start_point_.lat;
            lon0 = survey_start_point_.lon;
            lat1 = survey_end_point_.lat;
            lon1 = survey_end_point_.lon;
            
            // Initialize the last intersected edge index with the starting edge
            last_intersected_edge_index_ = starting_edge_index_;
            
            // Publish debug data for first leg
            std::ostringstream debug_json;
            debug_json << "{";
            debug_json << "\"leg_number\":" << current_leg_index_ << ",";
            debug_json << "\"start_point\":[" << lat0 << "," << lon0 << "],";
            debug_json << "\"end_point\":[" << lat1 << "," << lon1 << "],";
            debug_json << "\"survey_bearing\":" << initial_vector_bearing_ << ",";
            debug_json << "\"leg_distance\":" << calculate_distance(LatLonPoint(lat0, lon0), LatLonPoint(lat1, lon1)) << ",";
            debug_json << "\"polygon_vertices\":[";
            for (size_t i = 0; i < mission_boundary_vertices_.size(); ++i) {
                debug_json << "[" << mission_boundary_vertices_[i].lat << "," << mission_boundary_vertices_[i].lon << "]";
                if (i < mission_boundary_vertices_.size() - 1) debug_json << ",";
            }
            debug_json << "],";
            debug_json << "\"intersection_found\":true";
            debug_json << "}";
            
            std_msgs::msg::String debug_msg;
            debug_msg.data = debug_json.str();
            debug_pub_->publish(debug_msg);
            
            RCLCPP_INFO(this->get_logger(), "Publishing first survey leg: start to closer neighbor vertex");
        } 

        // If survey has started, calculate the next leg based on overlap percentage and last leg end
        else { 
            // Subsequent legs: calculate next survey line using geometry-based approach
            
            // Calculate effective displacement distance (swath width minus overlap)
            double effective_swath_width = swath_width_ * (1.0 - overlap_percentage_ / 100.0);
            
            // Find the closest point on the polygon boundary to the last leg end point
            LatLonPoint closest_boundary_point;
            int closest_edge_index = -1;
            double min_distance_to_boundary = std::numeric_limits<double>::max();
            
            // Check distance to each edge of the polygon
            for (size_t i = 0; i < mission_boundary_vertices_.size(); ++i) {
                LatLonPoint edge_start = mission_boundary_vertices_[i];
                LatLonPoint edge_end = mission_boundary_vertices_[(i + 1) % mission_boundary_vertices_.size()];
                
                // Find closest point on this edge to the last leg end point
                LatLonPoint closest_point_on_edge = findClosestPointOnEdge(last_survey_leg_end_point_, edge_start, edge_end);
                double distance_to_edge = calculate_distance(last_survey_leg_end_point_, closest_point_on_edge);
                
                if (distance_to_edge < min_distance_to_boundary) {
                    min_distance_to_boundary = distance_to_edge;
                    closest_boundary_point = closest_point_on_edge;
                    closest_edge_index = static_cast<int>(i);
                }
            }
            
            RCLCPP_INFO(this->get_logger(), "Closest boundary point: (%.6f, %.6f) on edge %d, distance: %.2f m", 
                       closest_boundary_point.lat, closest_boundary_point.lon, closest_edge_index, min_distance_to_boundary);
            
            // Calculate the direction to move along the polygon perimeter
            // For lawnmower pattern: odd legs move in one direction, even legs in opposite direction
            bool move_forward = (current_leg_index_ % 2 == 1);
            
            // Move along the polygon perimeter by the effective swath width
            LatLonPoint displaced_point = moveAlongPolygonPerimeter(
                mission_boundary_vertices_, 
                closest_boundary_point, 
                closest_edge_index, 
                effective_swath_width, 
                move_forward);
            
            RCLCPP_INFO(this->get_logger(), "Leg %d: Moving %s along polygon perimeter by %.2f m", 
                       current_leg_index_, move_forward ? "forward" : "backward", effective_swath_width);
            RCLCPP_INFO(this->get_logger(), "Displaced to: (%.6f, %.6f)", displaced_point.lat, displaced_point.lon);
            
            // Use the displaced point as the start of the next survey leg
            LatLonPoint start_leg_point = displaced_point;
            
            // Get the correct survey bearing based on current leg index (alternating pattern)
            double survey_bearing;
            if (current_leg_index_ % 2 == 1) {
                // Odd leg numbers (1, 3, 5, ...) use the original bearing (same as first leg)
                survey_bearing = odd_bearing_;
            } else {
                // Even leg numbers (2, 4, 6, ...) use the opposite bearing (opposite to first leg)
                survey_bearing = even_bearing_;
            }
            
            RCLCPP_INFO(this->get_logger(), "Leg %d: Using %s bearing %.2f degrees", 
                       current_leg_index_, 
                       (current_leg_index_ % 2 == 1) ? "odd" : "even", 
                       survey_bearing);
            
            // Use the displaced point as the start of the survey leg
            LatLonPoint perpendicular_point = start_leg_point;
            
            RCLCPP_INFO(this->get_logger(), "Survey leg start point: (%.6f, %.6f)", 
                       perpendicular_point.lat, perpendicular_point.lon);
            
            // Find the end point by intersecting the survey bearing with the polygon
            LatLonPoint end_leg_point = findNearestPolygonIntersection(mission_boundary_vertices_, 
                                                                      perpendicular_point, 
                                                                      survey_bearing,
                                                                      -1); // Don't exclude any edge
            
            // Check if a valid intersection was found
            double leg_distance = calculate_distance(perpendicular_point, end_leg_point);
            if (leg_distance < 25.0) {
                RCLCPP_WARN(this->get_logger(), "No valid polygon intersection found (distance: %.2f m)", leg_distance);
                RCLCPP_WARN(this->get_logger(), "Survey bearing: %.2f degrees", survey_bearing);
                RCLCPP_WARN(this->get_logger(), "Start point: (%.6f, %.6f)", perpendicular_point.lat, perpendicular_point.lon);
                
                // Check if survey is complete
                if (is_survey_complete()) {
                    RCLCPP_INFO(this->get_logger(), "Survey complete - no more valid intersections needed");
                    return;
                }
                
                RCLCPP_ERROR(this->get_logger(), "Cannot find valid intersection and survey not complete");
                return;
            }
            
            RCLCPP_INFO(this->get_logger(), "Found intersection: Start: (%.6f, %.6f), End: (%.6f, %.6f)", 
                       perpendicular_point.lat, perpendicular_point.lon, end_leg_point.lat, end_leg_point.lon);
            RCLCPP_INFO(this->get_logger(), "Survey bearing: %.2f degrees, Leg distance: %.2f meters", 
                       survey_bearing, leg_distance);
            
            // Publish debug data for visualization
            std::ostringstream debug_json;
            debug_json << "{";
            debug_json << "\"leg_number\":" << current_leg_index_ << ",";
            debug_json << "\"start_point\":[" << perpendicular_point.lat << "," << perpendicular_point.lon << "],";
            debug_json << "\"end_point\":[" << end_leg_point.lat << "," << end_leg_point.lon << "],";
            debug_json << "\"survey_bearing\":" << survey_bearing << ",";
            debug_json << "\"closest_edge_index\":" << closest_edge_index << ",";
            debug_json << "\"effective_swath_width\":" << effective_swath_width << ",";
            debug_json << "\"leg_distance\":" << leg_distance << ",";
            debug_json << "\"move_forward\":" << (move_forward ? "true" : "false") << ",";
            debug_json << "\"polygon_vertices\":[";
            for (size_t i = 0; i < mission_boundary_vertices_.size(); ++i) {
                debug_json << "[" << mission_boundary_vertices_[i].lat << "," << mission_boundary_vertices_[i].lon << "]";
                if (i < mission_boundary_vertices_.size() - 1) debug_json << ",";
            }
            debug_json << "],";
            debug_json << "\"intersection_found\":" << (leg_distance >= 25.0 ? "true" : "false");
            debug_json << "}";
            
            std_msgs::msg::String debug_msg;
            debug_msg.data = debug_json.str();
            debug_pub_->publish(debug_msg);
            
            // Update state for next iteration
            last_survey_leg_end_point_ = end_leg_point;
            
            // Update the last intersected edge index based on where the end point is
            LatLonPoint temp_intersection;
            int intersected_edge_index = findIntersectedEdgeIndex(mission_boundary_vertices_, 
                                                                 perpendicular_point, 
                                                                 survey_bearing,
                                                                 temp_intersection,
                                                                 last_boundary_edge_start_,
                                                                 last_boundary_edge_end_);
            
            if (intersected_edge_index >= 0) {
                last_intersected_edge_index_ = intersected_edge_index;
            }
            
            lat0 = perpendicular_point.lat;
            lon0 = perpendicular_point.lon;
            lat1 = end_leg_point.lat;
            lon1 = end_leg_point.lon;
            
            RCLCPP_INFO(this->get_logger(), "Publishing survey leg %d: effective swath %.2f m", 
                       current_leg_index_, effective_swath_width);
            RCLCPP_INFO(this->get_logger(), "Survey pattern: %s bearing %.2f degrees", 
                       (current_leg_index_ % 2 == 1) ? "odd" : "even", survey_bearing);
            if (intersected_edge_index >= 0) {
                RCLCPP_INFO(this->get_logger(), "Intersected edge: %d", intersected_edge_index);
            }
        }
        
        double speed = 2.5;  // Speed in meters/second

        // Format each field
        std::string lat0_nmea = decDegToNmeaLat(lat0);   
        std::string lon0_nmea = decDegToNmeaLon(lon0);   
        std::string lat1_nmea = decDegToNmeaLat(lat1);
        std::string lon1_nmea = decDegToNmeaLon(lon1);

        // Compose message body (excluding $ and *checksum)
        std::ostringstream nmea_body;
        nmea_body << "PMARLEG,"
                  << lat0_nmea << ","
                  << lon0_nmea << ","
                  << lat1_nmea << ","
                  << lon1_nmea << ","
                  << std::fixed << std::setprecision(2) << speed;

        std::string nmea_payload = nmea_body.str();

        // Compute checksum
        std::string checksum = nmeaChecksum(nmea_payload);

        // Compose full message: $ + body + * + checksum + <CR><LF>
        std::string nmea_msg = "$" + nmea_payload + "*" + checksum + "\r\n";

        // Publish
        std_msgs::msg::String msg;
        msg.data = nmea_msg;
        pub_->publish(msg);

        RCLCPP_INFO(this->get_logger(), "Published: %s", nmea_msg.c_str());
        
        // Log survey status and update state
        if (!survey_started_) {
            RCLCPP_INFO(this->get_logger(), "First survey leg published. Survey started = true");
            survey_started_ = true;  // Set to true after publishing first leg
        } else {
            RCLCPP_INFO(this->get_logger(), "Subsequent survey leg published. Survey continuing...");
        }
    }

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr debug_pub_;
};

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<LegPublisher>());
    rclcpp::shutdown();
    return 0;
}

// ros2 topic pub --once leg_request std_msgs/msg/String '{data: "next"}'

/* 
Worked!:D

Received vertices: [
  [ 59.42907768917837, 10.468919277191164 ],
  [ 59.431718645652595, 10.471708774566652 ],
  [ 59.431369440091224, 10.478832721710207 ],
  [ 59.42757159693651, 10.479390621185305 ],
  [ 59.42569434466917, 10.475871562957765 ],
  [ 59.427135035932494, 10.471408367156984 ]
]
Received start position: [ 59.424821168605554, 10.471665859222414 ]
Published polygon to ROS2 topic.
*/
