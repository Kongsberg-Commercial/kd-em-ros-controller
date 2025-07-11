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
    : Node("leg_publisher"), survey_started_(false)
    {
        // Publisher on /path topic
        pub_ = this->create_publisher<std_msgs::msg::String>("/path", 10);

        // Timer to publish every 1 second
        timer_ = this->create_wall_timer(
            std::chrono::seconds(1),
            std::bind(&LegPublisher::publish_leg, this)
        );
    }

private:
    // Mission boundary vertices (polygon)
    std::vector<LatLonPoint> mission_boundary_vertices_;
    
    // User defined start point
    LatLonPoint initial_start_point_;
    
    // Calculated survey parameters
    LatLonPoint nearest_vertice_start_;
    LatLonPoint farther_neighbor_vertice_;
    LatLonPoint survey_start_point_;
    
    // Survey parameters
    double swath_width_;  // in meters
    bool survey_started_;
    
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
        int nearest_index = 0;
        
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
        
        // and choose the farther one
        if (dist_to_prev > dist_to_next) {
            farther_neighbor_vertice_ = mission_boundary_vertices_[prev_index];
        } else {
            farther_neighbor_vertice_ = mission_boundary_vertices_[next_index];
        }
        
        RCLCPP_INFO(this->get_logger(), "Farther neighbor vertex: %.6f Lat, %.6f Lon", 
                    farther_neighbor_vertice_.lat, farther_neighbor_vertice_.lon);


        // Calculate initial vector leg (bearing from nearest to farther neighbor)
        double initial_vector_bearing = calculate_bearing(nearest_vertice_start_, farther_neighbor_vertice_);
        RCLCPP_INFO(this->get_logger(), "Initial vector bearing: %.2f degrees", initial_vector_bearing);
        

        // Calculate survey start point (swath width distance along the vector)
        survey_start_point_ = calculate_point_at_distance_bearing(nearest_vertice_start_, swath_width_, initial_vector_bearing);
        RCLCPP_INFO(this->get_logger(), "Survey start point: %.6f Lat, %.6f Lon", 
                    survey_start_point_.lat, survey_start_point_.lon);
        
        // Survey not started yet
        survey_started_ = false;
        RCLCPP_INFO(this->get_logger(), "Survey started: %s", survey_started_ ? "true" : "false");
    }
    void publish_leg()
    {
        // Use calculated survey parameters instead of hardcoded values
        // This is currently just placeholder of a published leg for now...
        double lat0 = nearest_vertice_start_.lat;     // Nearest vertex start
        double lon0 = nearest_vertice_start_.lon;     
        double lat1 = survey_start_point_.lat;        // Survey start point
        double lon1 = survey_start_point_.lon;        
        double speed = 2.5;                           // Speed in meters/second

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
        
        // Log survey status
        if (!survey_started_) {
            RCLCPP_INFO(this->get_logger(), "Survey not started yet. Publishing nearest vertex to survey start leg.");
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