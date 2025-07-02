#pragma once

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <thread>
#include <atomic>
#include <string>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>

class Ros2UdpReceiver : public rclcpp::Node {
public:
    Ros2UdpReceiver();
    ~Ros2UdpReceiver() override;

private:
    void send_handshake();
    void udp_listen();

    // KSSIS Handlers
    void handle_kssis_1(const std::string& msg);
    void handle_kssis_12(const std::string& msg);
    void handle_kssis_401(const std::string& msg);
    void handle_kssis_455(const std::string& msg);
    void handle_kssis_616(const std::string& msg);
    void handle_kssis_812(const std::string& msg);
    void handle_kssis_813(const std::string& msg);
    void handle_kssis_499(const std::string& msg);
    void handle_kssis_998(const std::string& msg);

    // Helpers
    void publish_string(const rclcpp::Publisher<std_msgs::msg::String>::SharedPtr& pub, const std::string& data);
    std::string parse_and_pretty_print_devices(const std::string& msg);

    // Members
    std::thread thread_;
    std::atomic<bool> running_;

    // Formatted publishers
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_version_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_devices_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_param_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_multicast_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_status_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_unknown812_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_unknown813_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_info_warn_error_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_device_disconnected_;

    // Raw publishers
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_version_raw_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_devices_raw_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_param_raw_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_multicast_raw_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_status_raw_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_unknown812_raw_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_unknown813_raw_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_info_warn_error_raw_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_device_disconnected_raw_;
};