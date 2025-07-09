#include <rclcpp/rclcpp.hpp>
#include <thread>
#include <atomic>
#include <string>
#include <sstream>
#include <iomanip>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <mutex>

#include "ros_otter_custom_interfaces/srv/drift_mode.hpp"
#include "ros_otter_custom_interfaces/srv/manual_mode.hpp"
#include "ros_otter_custom_interfaces/srv/course_mode.hpp"
#include "ros_otter_custom_interfaces/srv/leg_mode.hpp"
#include "ros_otter_custom_interfaces/srv/station_mode.hpp"
#include "ros_otter_custom_interfaces/srv/set_parameters.hpp"

using std::placeholders::_1;
using std::placeholders::_2;

class OtterTCPNode : public rclcpp::Node {
public:
    OtterTCPNode()
    : Node("otter_tcp_node"), stop_(false) {
        ip_ = this->declare_parameter("device_ip", "192.168.48.211");
        port_ = this->declare_parameter("device_port", 2009);
        connect_socket();
        receiver_thread_ = std::thread(&OtterTCPNode::receive_loop, this);

        // Service servers for each maneuver mode
        drift_srv_ = this->create_service<ros_otter_custom_interfaces::srv::DriftMode>("set_drift_mode",
            std::bind(&OtterTCPNode::handle_drift, this, _1, _2));
        manual_srv_ = this->create_service<ros_otter_custom_interfaces::srv::ManualMode>("set_manual_mode",
            std::bind(&OtterTCPNode::handle_manual, this, _1, _2));
        course_srv_ = this->create_service<ros_otter_custom_interfaces::srv::CourseMode>("set_course_mode",
            std::bind(&OtterTCPNode::handle_course, this, _1, _2));
        leg_srv_ = this->create_service<ros_otter_custom_interfaces::srv::LegMode>("set_leg_mode",
            std::bind(&OtterTCPNode::handle_leg, this, _1, _2));
        station_srv_ = this->create_service<ros_otter_custom_interfaces::srv::StationMode>("set_station_mode",
            std::bind(&OtterTCPNode::handle_station, this, _1, _2));
        param_srv_ = this->create_service<ros_otter_custom_interfaces::srv::SetParameters>("set_parameters",
            std::bind(&OtterTCPNode::handle_parameters, this, _1, _2));
    }

    ~OtterTCPNode() {
        stop_ = true;
        if (receiver_thread_.joinable()) receiver_thread_.join();
        close(sockfd_);
    }

private:
    std::string ip_;
    int port_;
    int sockfd_;
    std::atomic<bool> stop_;
    std::thread receiver_thread_;
    std::mutex sock_mutex_;

    // Services
    rclcpp::Service<ros_otter_custom_interfaces::srv::DriftMode>::SharedPtr drift_srv_;
    rclcpp::Service<ros_otter_custom_interfaces::srv::ManualMode>::SharedPtr manual_srv_;
    rclcpp::Service<ros_otter_custom_interfaces::srv::CourseMode>::SharedPtr course_srv_;
    rclcpp::Service<ros_otter_custom_interfaces::srv::LegMode>::SharedPtr leg_srv_;
    rclcpp::Service<ros_otter_custom_interfaces::srv::StationMode>::SharedPtr station_srv_;
    rclcpp::Service<ros_otter_custom_interfaces::srv::SetParameters>::SharedPtr param_srv_;

    void connect_socket() {
        sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in serv_addr;
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port_);
        inet_pton(AF_INET, ip_.c_str(), &serv_addr.sin_addr);
        if (connect(sockfd_, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
            RCLCPP_ERROR(this->get_logger(), "Connection to %s:%d failed", ip_.c_str(), port_);
            // throw std::runtime_error("TCP connection failed");
        } else {
            RCLCPP_INFO(this->get_logger(), "Connected to %s:%d", ip_.c_str(), port_);
        }
    }

    void receive_loop() {
        char buffer[1024];
        std::string line;
        while (!stop_) {
            ssize_t n = recv(sockfd_, buffer, sizeof(buffer), 0);
            if (n > 0) {
                for (ssize_t i = 0; i < n; ++i) {
                    char c = buffer[i];
                    if (c == '\n') {
                        process_line(line);
                        line.clear();
                    } else if (c != '\r') {
                        line += c;
                    }
                }
            }
        }
    }

    void process_line(const std::string& line) {
        RCLCPP_INFO(this->get_logger(), "Received: %s", line.c_str());
        // TODO: Publish parsed data to topics if needed
    }

    // Helper: calculate NMEA checksum
    std::string compute_checksum(const std::string& msg) {
        uint8_t chk = 0;
        for (char c : msg) chk ^= c;
        std::ostringstream oss;
        oss << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << (int)chk;
        return oss.str();
    }

    // Helper: send NMEA string over TCP
    bool send_nmea(const std::string &msg) {
        std::lock_guard<std::mutex> lock(sock_mutex_);
        std::string framed = "$" + msg + "*" + compute_checksum(msg) + "\r\n";
        ssize_t ret = send(sockfd_, framed.c_str(), framed.size(), 0);
        if (ret != (ssize_t)framed.size()) {
            RCLCPP_ERROR(this->get_logger(), "Failed to send TCP message.");
            return false;
        }
        return true;
    }

    // Service handlers
    void handle_drift(const std::shared_ptr<ros_otter_custom_interfaces::srv::DriftMode::Request> request,
                      std::shared_ptr<ros_otter_custom_interfaces::srv::DriftMode::Response> response) {
        (void)request;
        bool ok = send_nmea("PMARABT");
        response->success = ok;
        response->message = ok ? "Drift mode command sent." : "Failed to send drift command.";
    }

    void handle_manual(const std::shared_ptr<ros_otter_custom_interfaces::srv::ManualMode::Request> request,
                       std::shared_ptr<ros_otter_custom_interfaces::srv::ManualMode::Response> response) {
        std::ostringstream oss;
        oss << "PMARMAN," 
            << request->force_x << "," 
            << request->force_y << "," 
            << request->torque_z;
        bool ok = send_nmea(oss.str());
        response->success = ok;
        response->message = ok ? "Manual mode command sent." : "Failed to send manual mode command.";
    }

    void handle_course(const std::shared_ptr<ros_otter_custom_interfaces::srv::CourseMode::Request> request,
                       std::shared_ptr<ros_otter_custom_interfaces::srv::CourseMode::Response> response) {
        std::ostringstream oss;
        oss << "PMARCRS," << request->course << "," << request->speed;
        bool ok = send_nmea(oss.str());
        response->success = ok;
        response->message = ok ? "Course mode command sent." : "Failed to send course mode command.";
    }

    void handle_leg(const std::shared_ptr<ros_otter_custom_interfaces::srv::LegMode::Request> request,
                    std::shared_ptr<ros_otter_custom_interfaces::srv::LegMode::Response> response) {
        std::ostringstream oss;
        oss << "PMARLEG," 
            << request->lat0 << "," << request->lat0_dir << ","
            << request->lon0 << "," << request->lon0_dir << ","
            << request->lat1 << "," << request->lat1_dir << ","
            << request->lon1 << "," << request->lon1_dir << ","
            << request->speed;
        bool ok = send_nmea(oss.str());
        response->success = ok;
        response->message = ok ? "Leg mode command sent." : "Failed to send leg mode command.";
    }

    void handle_station(const std::shared_ptr<ros_otter_custom_interfaces::srv::StationMode::Request> request,
                        std::shared_ptr<ros_otter_custom_interfaces::srv::StationMode::Response> response) {
        std::ostringstream oss;
        oss << "PMARSTA," << request->lat << "," << request->lat_dir << ","
            << request->lon << "," << request->lon_dir << "," << request->speed;
        bool ok = send_nmea(oss.str());
        response->success = ok;
        response->message = ok ? "Station mode command sent." : "Failed to send station mode command.";
    }

    void handle_parameters(const std::shared_ptr<ros_otter_custom_interfaces::srv::SetParameters::Request> request,
                           std::shared_ptr<ros_otter_custom_interfaces::srv::SetParameters::Response> response) {
        std::ostringstream oss;
        oss << "PMARPRM," << request->lookahead_distance << ","
            << request->radius_in << "," << request->radius_out;
        bool ok = send_nmea(oss.str());
        response->success = ok;
        response->message = ok ? "Set parameters command sent." : "Failed to send set parameters command.";
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<OtterTCPNode>());
    rclcpp::shutdown();
    return 0;
}