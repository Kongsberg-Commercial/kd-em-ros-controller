#pragma once

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include "ros_kctrl_custom_interfaces/msg/kctrl_version.hpp"
#include "ros_kctrl_custom_interfaces/msg/kctrl_device.hpp"
#include "ros_kctrl_custom_interfaces/msg/kctrl_detected_devices.hpp"
#include "ros_kctrl_custom_interfaces/msg/kctrl_parameter.hpp"
#include "ros_kctrl_custom_interfaces/msg/kctrl_parameters.hpp"
#include "ros_kctrl_custom_interfaces/msg/kctrl_multicast.hpp"
#include "ros_kctrl_custom_interfaces/msg/kctrl_status_entry.hpp"
#include "ros_kctrl_custom_interfaces/msg/kctrl_status.hpp"
#include "ros_kctrl_custom_interfaces/msg/kctrl_info_warn_error.hpp"
#include "ros_kctrl_custom_interfaces/msg/kctrl_device_disconnected.hpp"
#include "ros_kctrl_custom_interfaces/msg/kctrl_pu_params.hpp"
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
    void handle_kssis_499(const std::string& msg);
    void handle_kssis_998(const std::string& msg);
    void handle_kssis_993(const std::string& msg);

    // Helpers
    void publish_string(const rclcpp::Publisher<std_msgs::msg::String>::SharedPtr& pub, const std::string& data);
    std::vector<ros_kctrl_custom_interfaces::msg::KctrlDevice> parse_devices(const std::string& msg);

    // Members
    std::thread thread_;
    std::atomic<bool> running_;

    // Formatted publishers
    rclcpp::Publisher<ros_kctrl_custom_interfaces::msg::KctrlVersion>::SharedPtr pub_version_;
    rclcpp::Publisher<ros_kctrl_custom_interfaces::msg::KctrlDetectedDevices>::SharedPtr pub_devices_;
    rclcpp::Publisher<ros_kctrl_custom_interfaces::msg::KctrlParameters>::SharedPtr pub_param_;
    rclcpp::Publisher<ros_kctrl_custom_interfaces::msg::KctrlMulticast>::SharedPtr pub_multicast_;
    rclcpp::Publisher<ros_kctrl_custom_interfaces::msg::KctrlStatus>::SharedPtr pub_status_;
    rclcpp::Publisher<ros_kctrl_custom_interfaces::msg::KctrlInfoWarnError>::SharedPtr pub_info_warn_error_;
    rclcpp::Publisher<ros_kctrl_custom_interfaces::msg::KctrlDeviceDisconnected>::SharedPtr pub_device_disconnected_;
    rclcpp::Publisher<ros_kctrl_custom_interfaces::msg::KctrlPUParams>::SharedPtr pub_pu_params_;

    // Raw publishers
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_version_raw_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_devices_raw_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_param_raw_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_multicast_raw_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_status_raw_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_info_warn_error_raw_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_device_disconnected_raw_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_pu_params_raw_;
};