#pragma once

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/bool.hpp>
#include <thread>
#include <atomic>
#include <string>
#include <sstream>
#include <iomanip>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <mutex>
#include <vector>
#include <optional>

#include "ros_otter_custom_interfaces/srv/drift_mode.hpp"
#include "ros_otter_custom_interfaces/srv/manual_mode.hpp"
#include "ros_otter_custom_interfaces/srv/course_mode.hpp"
#include "ros_otter_custom_interfaces/srv/leg_mode.hpp"
#include "ros_otter_custom_interfaces/srv/station_mode.hpp"
#include "ros_otter_custom_interfaces/srv/set_parameters.hpp"

#include "ros_otter_custom_interfaces/msg/gps_info.hpp"
#include "ros_otter_custom_interfaces/msg/imu_info.hpp"
#include "ros_otter_custom_interfaces/msg/mode_info.hpp"
#include "ros_otter_custom_interfaces/msg/error_info.hpp"
#include "ros_otter_custom_interfaces/msg/otter_info.hpp"

class OtterTCPNode : public rclcpp::Node {
public:
    OtterTCPNode();
    ~OtterTCPNode();

private:
    void connect_socket();
    void receive_loop();
    void process_line(const std::string& line);

    // Parsing helpers
    std::optional<ros_otter_custom_interfaces::msg::GpsInfo> parse_gps(const std::string& line);
    std::optional<ros_otter_custom_interfaces::msg::ImuInfo> parse_imu(const std::string& line);
    std::optional<ros_otter_custom_interfaces::msg::ModeInfo> parse_mode(const std::string& line);
    std::optional<ros_otter_custom_interfaces::msg::ErrorInfo> parse_error(const std::string& line);
    std::optional<ros_otter_custom_interfaces::msg::OtterInfo> parse_otter(const std::string& line);

    // Helper: calculate NMEA checksum
    std::string compute_checksum(const std::string& msg);
    bool send_nmea(const std::string &msg);
    
    // Helper for leg endpoint detection
    bool at_leg_endpoint(double curr_lat, const std::string& curr_lat_dir,
                        double curr_lon, const std::string& curr_lon_dir);

    // Service handlers
    void handle_drift(const std::shared_ptr<ros_otter_custom_interfaces::srv::DriftMode::Request> request,
                      std::shared_ptr<ros_otter_custom_interfaces::srv::DriftMode::Response> response);
    void handle_manual(const std::shared_ptr<ros_otter_custom_interfaces::srv::ManualMode::Request> request,
                       std::shared_ptr<ros_otter_custom_interfaces::srv::ManualMode::Response> response);
    void handle_course(const std::shared_ptr<ros_otter_custom_interfaces::srv::CourseMode::Request> request,
                       std::shared_ptr<ros_otter_custom_interfaces::srv::CourseMode::Response> response);
    void handle_leg(const std::shared_ptr<ros_otter_custom_interfaces::srv::LegMode::Request> request,
                    std::shared_ptr<ros_otter_custom_interfaces::srv::LegMode::Response> response);
    void handle_station(const std::shared_ptr<ros_otter_custom_interfaces::srv::StationMode::Request> request,
                        std::shared_ptr<ros_otter_custom_interfaces::srv::StationMode::Response> response);
    void handle_parameters(const std::shared_ptr<ros_otter_custom_interfaces::srv::SetParameters::Request> request,
                           std::shared_ptr<ros_otter_custom_interfaces::srv::SetParameters::Response> response);

    // Member variables
    std::string ip_;
    int port_;
    int sockfd_;
    std::atomic<bool> stop_;
    std::thread receiver_thread_;
    std::mutex sock_mutex_;

    // Service servers
    rclcpp::Service<ros_otter_custom_interfaces::srv::DriftMode>::SharedPtr drift_srv_;
    rclcpp::Service<ros_otter_custom_interfaces::srv::ManualMode>::SharedPtr manual_srv_;
    rclcpp::Service<ros_otter_custom_interfaces::srv::CourseMode>::SharedPtr course_srv_;
    rclcpp::Service<ros_otter_custom_interfaces::srv::LegMode>::SharedPtr leg_srv_;
    rclcpp::Service<ros_otter_custom_interfaces::srv::StationMode>::SharedPtr station_srv_;
    rclcpp::Service<ros_otter_custom_interfaces::srv::SetParameters>::SharedPtr param_srv_;

    // Service clients
    rclcpp::Client<ros_otter_custom_interfaces::srv::LegMode>::SharedPtr ppnode_leg_client_;

    // Publishers
    rclcpp::Publisher<ros_otter_custom_interfaces::msg::GpsInfo>::SharedPtr gps_pub_;
    rclcpp::Publisher<ros_otter_custom_interfaces::msg::ImuInfo>::SharedPtr imu_pub_;
    rclcpp::Publisher<ros_otter_custom_interfaces::msg::ModeInfo>::SharedPtr mode_pub_;
    rclcpp::Publisher<ros_otter_custom_interfaces::msg::ErrorInfo>::SharedPtr error_pub_;
    rclcpp::Publisher<ros_otter_custom_interfaces::msg::OtterInfo>::SharedPtr otter_pub_;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr leg_status_pub_;
    
    // Leg tracking variables
    bool leg_active_;
    double leg_end_lat_;
    double leg_end_lon_;
    std::string leg_end_lat_dir_;
    std::string leg_end_lon_dir_;
};
