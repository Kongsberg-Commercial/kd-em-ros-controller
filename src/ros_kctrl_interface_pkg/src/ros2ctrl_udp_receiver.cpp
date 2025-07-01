#include "ros_kctrl_interface_pkg/ros2ctrl_udp_receiver.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <regex>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

Ros2UdpReceiver::Ros2UdpReceiver()
: Node("ros2_udp_receiver"), running_(true)
{
    pub_version_      = this->create_publisher<std_msgs::msg::String>("kctrl/version", 10);
    pub_devices_      = this->create_publisher<std_msgs::msg::String>("kctrl/detected_devices", 10);
    pub_param_        = this->create_publisher<std_msgs::msg::String>("kctrl/parameters", 10);
    pub_multicast_    = this->create_publisher<std_msgs::msg::String>("kctrl/multicast", 10);
    pub_status_       = this->create_publisher<std_msgs::msg::String>("kctrl/status", 10);
    pub_unknown812_   = this->create_publisher<std_msgs::msg::String>("kctrl/unknown812", 10);
    pub_unknown813_   = this->create_publisher<std_msgs::msg::String>("kctrl/unknown813", 10);

    RCLCPP_INFO(this->get_logger(), "ROS2 UDP Receiver Node started.");

    send_handshake();
    thread_ = std::thread([this]() { udp_listen(); });
}

Ros2UdpReceiver::~Ros2UdpReceiver() {
    running_ = false;
    if (thread_.joinable()) thread_.join();
}

// --- UDP HANDSHAKE ---
void Ros2UdpReceiver::send_handshake() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        RCLCPP_ERROR(this->get_logger(), "UDP handshake socket creation failed");
        return;
    }
    sockaddr_in destaddr{};
    destaddr.sin_family = AF_INET;
    destaddr.sin_port = htons(4001); // K-Controller handshake port
    std::string kctrl_host_ip = "192.168.48.1"; // Adjust to your Windows host IP
    inet_pton(AF_INET, kctrl_host_ip.c_str(), &destaddr.sin_addr);

    std::string msg = "Hello";
    ssize_t sent = sendto(sockfd, msg.c_str(), msg.size(), 0,
                          (sockaddr*)&destaddr, sizeof(destaddr));
    if (sent < 0) {
        RCLCPP_ERROR(this->get_logger(), "UDP handshake sendto failed");
    }
    close(sockfd);
}

// --- UDP LISTEN & ROUTER ---
void Ros2UdpReceiver::udp_listen() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        RCLCPP_ERROR(this->get_logger(), "UDP socket creation failed");
        return;
    }

    sockaddr_in servaddr{};
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(4002);

    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        RCLCPP_ERROR(this->get_logger(), "UDP socket bind failed");
        close(sockfd);
        return;
    }
    RCLCPP_INFO(this->get_logger(), "UDP socket bound to 0.0.0.0:4002");

    char buffer[4096];
    while (running_) {
        ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer)-1, 0, nullptr, nullptr);
        if (n > 0) {
            buffer[n] = '\0';
            std::string msg(buffer);

            // Route by KSSIS number
            std::smatch match;
            if (std::regex_search(msg, match, std::regex(R"(\$KSSIS,(\d+),)"))) {
                int code = std::stoi(match[1]);
                switch (code) {
                    case 1: handle_kssis_1(msg); break;
                    case 12: handle_kssis_12(msg); break;
                    case 401: handle_kssis_401(msg); break;
                    case 455: handle_kssis_455(msg); break;
                    case 616: handle_kssis_616(msg); break;
                    case 812: handle_kssis_812(msg); break;
                    case 813: handle_kssis_813(msg); break;
                    default: break;
                }
            }
        }
    }
    close(sockfd);
}

// --- KSSIS HANDLER FUNCTIONS ---
void Ros2UdpReceiver::handle_kssis_1(const std::string& msg) {
    size_t pos = msg.find("KCTRL_VER=");
    std::string version = (pos != std::string::npos) ? msg.substr(pos + 10) : "Unknown";
    publish_string(pub_version_, "K-Controller Version: " + version);
}
void Ros2UdpReceiver::handle_kssis_12(const std::string& msg) {
    publish_string(pub_devices_, parse_and_pretty_print_devices(msg));
}
void Ros2UdpReceiver::handle_kssis_401(const std::string& msg) {
    size_t start = msg.find('[');
    size_t end = msg.rfind(']');
    if (start == std::string::npos || end == std::string::npos || end <= start) {
        publish_string(pub_param_, "Invalid parameter datagram.");
        return;
    }
    std::string json_str = msg.substr(start, end - start + 1);
    std::ostringstream oss;
    try {
        auto j = json::parse(json_str);
        oss << "Parameters:\n";
        for (const auto& param : j) {
            oss << "  " << (param.contains("label") ? param["label"].get<std::string>() : param["id"].get<std::string>()) 
                << " = " << param["value"].get<std::string>() << "\n";
        }
    } catch (...) {
        oss << "Failed to parse parameter JSON.";
    }
    publish_string(pub_param_, oss.str());
}
void Ros2UdpReceiver::handle_kssis_455(const std::string& msg) {
    std::stringstream ss(msg);
    std::string token;
    std::vector<std::string> parts;
    while (std::getline(ss, token, ',')) {
        parts.push_back(token);
    }
    std::ostringstream oss;
    if (parts.size() >= 5) {
        oss << "Sounder: " << parts[2] << "\n";
        oss << "  Multicast Addr: " << parts[3] << "\n";
        oss << "  Multicast Port: " << parts[4];
    } else {
        oss << "Invalid multicast datagram.";
    }
    publish_string(pub_multicast_, oss.str());
}
void Ros2UdpReceiver::handle_kssis_616(const std::string& msg) {
    size_t start = msg.find(',', 9); // skip "$KSSIS,616,"
    if (start == std::string::npos) {
        publish_string(pub_status_, "Invalid status datagram.");
        return;
    }
    std::string rest = msg.substr(start + 1);
    std::stringstream ss(rest);
    std::string token;
    std::ostringstream oss;
    oss << "Status:\n";
    while (std::getline(ss, token, ',')) {
        size_t eq = token.find('=');
        if (eq != std::string::npos) {
            std::string key = token.substr(0, eq);
            std::string val = token.substr(eq + 1);
            size_t semi = val.find(';');
            if (semi != std::string::npos) val = val.substr(0, semi);
            oss << "  " << key << " = " << val << "\n";
        }
    }
    publish_string(pub_status_, oss.str());
}
void Ros2UdpReceiver::handle_kssis_812(const std::string& msg) {
    publish_string(pub_unknown812_, "[KSSIS,812] Received data (not parsed).");
}
void Ros2UdpReceiver::handle_kssis_813(const std::string& msg) {
    publish_string(pub_unknown813_, "[KSSIS,813] Received message.");
}

// --- HELPERS ---
void Ros2UdpReceiver::publish_string(const rclcpp::Publisher<std_msgs::msg::String>::SharedPtr& pub, const std::string& data) {
    auto msg = std_msgs::msg::String();
    msg.data = data;
    pub->publish(msg);
}
std::string Ros2UdpReceiver::parse_and_pretty_print_devices(const std::string& msg) {
    std::ostringstream oss;
    size_t start = msg.find("$KSSIS,12,");
    if (start == std::string::npos) return "Invalid device datagram.";
    size_t dataStart = start + std::string("$KSSIS,12,").length();
    std::string body = msg.substr(dataStart);

    // Remove trailing $$$ if present
    if (body.size() >= 3 && body.compare(body.size() - 3, 3, "$$$") == 0) {
        body = body.substr(0, body.size() - 3);
    }

    // Split each device by $$$
    std::vector<std::string> deviceEntries;
    size_t pos = 0, found;
    while ((found = body.find("$$$", pos)) != std::string::npos) {
        deviceEntries.push_back(body.substr(pos, found - pos));
        pos = found + 3;
    }
    deviceEntries.push_back(body.substr(pos));

    int deviceCount = 1;
    for (const auto& deviceEntry : deviceEntries) {
        if (deviceEntry.empty()) continue;

        std::map<std::string, std::string> deviceInfo;
        size_t fieldPos = 0, fieldFound;
        while ((fieldFound = deviceEntry.find("~~", fieldPos)) != std::string::npos) {
            std::string pair = deviceEntry.substr(fieldPos, fieldFound - fieldPos);
            size_t eq = pair.find('=');
            if (eq != std::string::npos) {
                std::string key = pair.substr(0, eq);
                std::string value = pair.substr(eq + 1);
                deviceInfo[key] = value;
            }
            fieldPos = fieldFound + 2;
        }
        std::string lastPair = deviceEntry.substr(fieldPos);
        size_t eq = lastPair.find('=');
        if (eq != std::string::npos) {
            std::string key = lastPair.substr(0, eq);
            std::string value = lastPair.substr(eq + 1);
            deviceInfo[key] = value;
        }

        // Output device info
        oss << "-------------------- Device #" << deviceCount++ << " --------------------\n";
        if (deviceInfo.count("NAME"))         oss << "  Name           : " << deviceInfo["NAME"] << '\n';
        if (deviceInfo.count("IP"))           oss << "  IP Address     : " << deviceInfo["IP"] << '\n';
        if (deviceInfo.count("TYPE"))         oss << "  Type           : " << deviceInfo["TYPE"] << '\n';
        if (deviceInfo.count("SYSTEM_ID"))    oss << "  System ID      : " << deviceInfo["SYSTEM_ID"] << '\n';
        if (deviceInfo.count("STARTED"))      oss << "  Started        : " << (deviceInfo["STARTED"] == "1" ? "Yes" : "No") << '\n';
        if (deviceInfo.count("IS_PINGING"))   oss << "  Pinging        : " << (deviceInfo["IS_PINGING"] == "1" ? "Yes" : "No") << '\n';
        if (deviceInfo.count("DATA_MACST_IP"))oss << "  Data MC Addr   : " << deviceInfo["DATA_MACST_IP"] << '\n';
        if (deviceInfo.count("DATA_MACST_PORT")) oss << "  Data MC Port   : " << deviceInfo["DATA_MACST_PORT"] << '\n';
        if (deviceInfo.count("CMDPORT_PORT")) oss << "  Command Port   : " << deviceInfo["CMDPORT_PORT"] << '\n';
        if (deviceInfo.count("SYSTEM_DESC"))  oss << "  Description    : " << deviceInfo["SYSTEM_DESC"] << '\n';
        if (deviceInfo.count("LICENSE"))      oss << "  License        : " << deviceInfo["LICENSE"] << '\n';
        oss << "--------------------------------------------------------\n";
    }
    return oss.str();
}

// main
int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<Ros2UdpReceiver>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}