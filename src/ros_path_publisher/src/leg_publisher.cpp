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
    : Node("leg_publisher"), survey_started_(false), polygon_data_received_(false)
    {
        // Publisher on /path topic
        pub_ = this->create_publisher<std_msgs::msg::String>("/path", 10);

        // Timer to publish every 1 second
        timer_ = this->create_wall_timer(
            std::chrono::seconds(1),
            std::bind(&LegPublisher::publish_leg, this)
        );
        
        // Subscription to receive polygon data from webapp
        polygon_sub_ = this->create_subscription<std_msgs::msg::String>(
            "polygon_data", 10,
            std::bind(&LegPublisher::polygon_data_callback, this, std::placeholders::_1)
        );
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
    double overlap_percentage_;  // percentage of swath width for overlap (50% default)
    bool survey_started_;
    
    // Last survey leg end point (for continuing survey)
    LatLonPoint last_survey_leg_end_point_;
    LatLonPoint last_boundary_edge_start_;  // Start of the edge where last intersection occurred
    LatLonPoint last_boundary_edge_end_;    // End of the edge where last intersection occurred
    
    // Subscription to receive polygon data from webapp
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr polygon_sub_;
    
    // Flag to track if we have received polygon data
    bool polygon_data_received_;

    void initialize_mission_data() {
        // Example mission boundary vertices (polygon)
        // TODO: Get these from webapp
        mission_boundary_vertices_ = {
            LatLonPoint(37.7749, -122.4194),  // Point 1
            LatLonPoint(37.7849, -122.4094),  // Point 2
            LatLonPoint(37.7849, -122.4294),  // Point 3
            LatLonPoint(37.7749, -122.4294)   // Point 4
        };
        
        // Example initial start point
        // TODO: Get this from webapp
        initial_start_point_ = LatLonPoint(37.7750, -122.4200);
        
        // Example swath width (in meters)
        // TODO: Get this from topic
        swath_width_ = 50.0;
        
        // Overlap percentage for survey legs (50% default)
        // TODO: Might Change This or Make It Dynamic | TBD
        overlap_percentage_ = 50.0;
        
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
            // For now, let's trigger survey calculation with existing data
            polygon_data_received_ = true;
            
            // Initialize mission data and calculate survey parameters
            initialize_mission_data();
            calculate_survey_parameters();
            
            RCLCPP_INFO(this->get_logger(), "Polygon data processed. Starting survey...");
            
        } catch (const std::exception& e) {
            RCLCPP_ERROR(this->get_logger(), "Failed to parse polygon data: %s", e.what());
        }
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
            
            RCLCPP_INFO(this->get_logger(), "Publishing first survey leg: start to closer neighbor vertex");
        } 

        // If survey has started, calculate the next leg based on overlap percentage and last leg end
        else { 
            // Subsequent legs: calculate next survey line
            
            // Calculate start point: overlap_percentage% of swath width from last end point along the boundary edge
            double overlap_distance = (overlap_percentage_ / 100.0) * swath_width_;
            
            // Get the correct edge bearing - use next edge if at/near a vertex
            double edge_bearing = getNextEdgeBearing(mission_boundary_vertices_,
                                                   last_survey_leg_end_point_, 
                                                   last_boundary_edge_start_, 
                                                   last_boundary_edge_end_);
            
            LatLonPoint start_leg_point = calculate_point_at_distance_bearing(last_survey_leg_end_point_, 
                                                                            overlap_distance, 
                                                                            edge_bearing);
            
            // Calculate end point: ray in opposite direction (180 degrees rotated)
            double opposite_bearing = std::fmod(initial_vector_bearing_ + 180.0, 360.0);
            
            LatLonPoint end_leg_point = findNearestPolygonIntersection(mission_boundary_vertices_, 
                                                                      start_leg_point, 
                                                                      opposite_bearing);
            
            // Update last survey leg end point for next iteration
            last_survey_leg_end_point_ = end_leg_point;
            
            // Update boundary edge information for next iteration
            // Find which edge the new end point intersected
            LatLonPoint temp_intersection;
            int intersected_edge_index = findIntersectedEdgeIndex(mission_boundary_vertices_, 
                                                                 start_leg_point, 
                                                                 opposite_bearing,
                                                                 temp_intersection,
                                                                 last_boundary_edge_start_,
                                                                 last_boundary_edge_end_);
            
            lat0 = start_leg_point.lat;
            lon0 = start_leg_point.lon;
            lat1 = end_leg_point.lat;
            lon1 = end_leg_point.lon;
            
            RCLCPP_INFO(this->get_logger(), "Publishing subsequent survey leg: %.2f%% overlap", overlap_percentage_);
            RCLCPP_INFO(this->get_logger(), "Edge bearing used: %.2f degrees", edge_bearing);
            RCLCPP_INFO(this->get_logger(), "Opposite bearing: %.2f degrees", opposite_bearing);
            if (intersected_edge_index >= 0) {
                RCLCPP_INFO(this->get_logger(), "Intersected edge index: %d", intersected_edge_index);
            } else {
                RCLCPP_WARN(this->get_logger(), "No edge intersection found for boundary tracking");
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
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<LegPublisher>());
    rclcpp::shutdown();
    return 0;
}
