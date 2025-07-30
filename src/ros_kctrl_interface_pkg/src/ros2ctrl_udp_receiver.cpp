#include "ros_kctrl_interface_pkg/ros2ctrl_udp_receiver.hpp"
#include "ros_kctrl_interface_pkg/simple_udp.hpp"
#include <sstream>
#include <regex>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Ros2UdpReceiver::Ros2UdpReceiver()
: Node("ros2_udp_receiver"), running_(true)
{
    // Set up publishers for custom messages and raw strings
    pub_version_      = this->create_publisher<ros_kctrl_custom_interfaces::msg::KctrlVersion>("kctrl/version", 10);
    pub_version_raw_  = this->create_publisher<std_msgs::msg::String>("kctrl/version_raw", 10);

    pub_devices_      = this->create_publisher<ros_kctrl_custom_interfaces::msg::KctrlDetectedDevices>("kctrl/detected_devices", 10);
    pub_devices_raw_  = this->create_publisher<std_msgs::msg::String>("kctrl/detected_devices_raw", 10);

    pub_param_        = this->create_publisher<ros_kctrl_custom_interfaces::msg::KctrlParameters>("kctrl/parameters", 10);
    pub_param_raw_    = this->create_publisher<std_msgs::msg::String>("kctrl/parameters_raw", 10);

    pub_multicast_    = this->create_publisher<ros_kctrl_custom_interfaces::msg::KctrlMulticast>("kctrl/multicast", 10);
    pub_multicast_raw_= this->create_publisher<std_msgs::msg::String>("kctrl/multicast_raw", 10);

    pub_status_       = this->create_publisher<ros_kctrl_custom_interfaces::msg::KctrlStatus>("kctrl/status", 10);
    pub_status_raw_   = this->create_publisher<std_msgs::msg::String>("kctrl/status_raw", 10);

    pub_info_warn_error_ = this->create_publisher<ros_kctrl_custom_interfaces::msg::KctrlInfoWarnError>("kctrl/info_warn_error", 10);
    pub_info_warn_error_raw_ = this->create_publisher<std_msgs::msg::String>("kctrl/info_warn_error_raw", 10);

    pub_device_disconnected_ = this->create_publisher<ros_kctrl_custom_interfaces::msg::KctrlDeviceDisconnected>("kctrl/device_disconnected", 10);
    pub_device_disconnected_raw_ = this->create_publisher<std_msgs::msg::String>("kctrl/device_disconnected_raw", 10);

    pub_pu_params_ = this->create_publisher<ros_kctrl_custom_interfaces::msg::KctrlPUParams>("kctrl/pu_params", 10);
    pub_pu_params_raw_ = this->create_publisher<std_msgs::msg::String>("kctrl/pu_params_raw", 10);

    // Declare configurable parameters
    this->declare_parameter<std::string>("kctrl_ip", "192.168.53.3");
    this->declare_parameter<int>("kctrl_port", 4001);
    this->declare_parameter<int>("listen_port", 4002);

    // Get parameter values
    std::string kctrl_ip = this->get_parameter("kctrl_ip").as_string();
    int kctrl_port = this->get_parameter("kctrl_port").as_int();
    int listen_port = this->get_parameter("listen_port").as_int();

    RCLCPP_INFO(this->get_logger(), "ROS2 UDP Receiver Node started.");
    RCLCPP_INFO(this->get_logger(), "K-Controller at: %s:%d", kctrl_ip.c_str(), kctrl_port);
    RCLCPP_INFO(this->get_logger(), "Listening on port: %d", listen_port);

    // Set up UDP communication - much simpler now!
    handshake_sender_ = SimpleUDP::createSender(kctrl_ip, kctrl_port);
    message_receiver_ = SimpleUDP::createReceiver(listen_port);

    // Send handshake and start listening
    send_handshake();
    thread_ = std::thread([this]() { udp_listen(); });
}

Ros2UdpReceiver::~Ros2UdpReceiver() {
    running_ = false;
    if (thread_.joinable()) thread_.join();
}

void Ros2UdpReceiver::send_handshake() {
    if (!handshake_sender_ || !handshake_sender_->isValid()) {
        RCLCPP_ERROR(this->get_logger(), "Handshake sender not available");
        return;
    }
    
    bool success = handshake_sender_->send("Hello");
    if (!success) {
        RCLCPP_ERROR(this->get_logger(), "Failed to send handshake: %s", 
                     handshake_sender_->getLastError().c_str());
    } else {
        RCLCPP_INFO(this->get_logger(), "Handshake sent successfully");
    }
}

void Ros2UdpReceiver::udp_listen() {
    if (!message_receiver_ || !message_receiver_->isValid()) {
        RCLCPP_ERROR(this->get_logger(), "Message receiver not available");
        return;
    }
    
    RCLCPP_INFO(this->get_logger(), "UDP listening started on port 4002");

    while (running_) {
        std::string received_message;
        
        // Try to receive a message (with 100ms timeout to allow clean shutdown)
        bool got_message = message_receiver_->receive(received_message, 100);
        
        if (got_message) {
            // Route message by KSSIS number - same logic as before
            std::smatch match;
            if (std::regex_search(received_message, match, std::regex(R"(\$KSSIS,(\d+),)"))) {
                int code = std::stoi(match[1]);
                switch (code) {
                    case 1: handle_kssis_1(received_message); break;
                    case 12: handle_kssis_12(received_message); break;
                    case 401: handle_kssis_401(received_message); break;
                    case 455: handle_kssis_455(received_message); break;
                    case 616: handle_kssis_616(received_message); break;
                    case 499: handle_kssis_499(received_message); break;
                    case 998: handle_kssis_998(received_message); break;
                    case 993: handle_kssis_993(received_message); break;
                    default: break;
                }
            }
        }
        // If no message received, just continue the loop (timeout is normal)
    }
    
    RCLCPP_INFO(this->get_logger(), "UDP listening stopped");
}

// --- KSSIS HANDLER FUNCTIONS ---
void Ros2UdpReceiver::handle_kssis_1(const std::string& msg) {
    publish_string(pub_version_raw_, msg);
    
    size_t pos = msg.find("KCTRL_VER=");
    std::string version = (pos != std::string::npos) ? msg.substr(pos + 10) : "Unknown";
    
    auto version_msg = ros_kctrl_custom_interfaces::msg::KctrlVersion();
    version_msg.version = version;
    pub_version_->publish(version_msg);
}
void Ros2UdpReceiver::handle_kssis_12(const std::string& msg) {
    publish_string(pub_devices_raw_, msg);
    
    auto devices_msg = ros_kctrl_custom_interfaces::msg::KctrlDetectedDevices();
    devices_msg.devices = parse_devices(msg);
    pub_devices_->publish(devices_msg);
}
void Ros2UdpReceiver::handle_kssis_401(const std::string& msg) {
    publish_string(pub_param_raw_, msg);
    
    size_t start = msg.find('[');
    size_t end = msg.rfind(']');
    if (start == std::string::npos || end == std::string::npos || end <= start) {
        auto param_msg = ros_kctrl_custom_interfaces::msg::KctrlParameters();
        pub_param_->publish(param_msg);
        return;
    }
    
    std::string json_str = msg.substr(start, end - start + 1);
    auto param_msg = ros_kctrl_custom_interfaces::msg::KctrlParameters();
    
    try {
        auto j = json::parse(json_str);
        for (const auto& param : j) {
            auto kctrl_param = ros_kctrl_custom_interfaces::msg::KctrlParameter();
            kctrl_param.name = param["id"].get<std::string>();
            kctrl_param.value = param["value"].get<std::string>();
            param_msg.parameters.push_back(kctrl_param);
        }
    } catch (...) {
        // If parsing fails, publish empty message
    }
    pub_param_->publish(param_msg);
}
void Ros2UdpReceiver::handle_kssis_455(const std::string& msg) {
    publish_string(pub_multicast_raw_, msg);
    
    std::stringstream ss(msg);
    std::string token;
    std::vector<std::string> parts;
    while (std::getline(ss, token, ',')) {
        parts.push_back(token);
    }
    
    auto multicast_msg = ros_kctrl_custom_interfaces::msg::KctrlMulticast();
    if (parts.size() >= 5) {
        multicast_msg.sounder = parts[2];
        multicast_msg.multicast_addr = parts[3];
        multicast_msg.multicast_port = parts[4];
    }
    pub_multicast_->publish(multicast_msg);
}
void Ros2UdpReceiver::handle_kssis_616(const std::string& msg) {
    publish_string(pub_status_raw_, msg);
    
    size_t start = msg.find(',', 9);
    if (start == std::string::npos) {
        auto status_msg = ros_kctrl_custom_interfaces::msg::KctrlStatus();
        pub_status_->publish(status_msg);
        return;
    }
    
    std::string rest = msg.substr(start + 1);
    std::stringstream ss(rest);
    std::string token;
    auto status_msg = ros_kctrl_custom_interfaces::msg::KctrlStatus();
    
    while (std::getline(ss, token, ',')) {
        size_t eq = token.find('=');
        if (eq != std::string::npos) {
            std::string key = token.substr(0, eq);
            std::string val = token.substr(eq + 1);
            size_t semi = val.find(';');
            if (semi != std::string::npos) val = val.substr(0, semi);
            
            auto status_entry = ros_kctrl_custom_interfaces::msg::KctrlStatusEntry();
            status_entry.key = key;
            status_entry.value = val;
            status_msg.entries.push_back(status_entry);
        }
    }
    pub_status_->publish(status_msg);
}

void Ros2UdpReceiver::handle_kssis_499(const std::string& msg) {
    publish_string(pub_info_warn_error_raw_, msg);

    std::string device, level, text, timestamp;

    std::stringstream ss(msg);
    std::string part;
    std::vector<std::string> tokens;
    while (std::getline(ss, part, ',')) {
        tokens.push_back(part);
    }

    for (const auto& token : tokens) {
        if (token.rfind("$KSSIS", 0) == 0) continue; 
        else if (token.find("499") == 0) continue;   
        else if (device.empty()) { device = token; continue; }

        auto eq = token.find('=');
        if (eq == std::string::npos) continue;
        std::string key = token.substr(0, eq);
        std::string val = token.substr(eq + 1);
        if (key == "TIMESTAMP") timestamp = val;
        else if (key == "LEVEL") level = val;
        else if (key == "MSG") text = val;
    }

    auto info_msg = ros_kctrl_custom_interfaces::msg::KctrlInfoWarnError();
    info_msg.device = device;
    info_msg.timestamp = timestamp;
    info_msg.level = level;
    info_msg.text = text;
    pub_info_warn_error_->publish(info_msg);
}
void Ros2UdpReceiver::handle_kssis_998(const std::string& msg) {
    publish_string(pub_device_disconnected_raw_, msg);

    // Example: $KSSIS,998,EM2040_38
    std::stringstream ss(msg);
    std::string token;
    std::vector<std::string> tokens;
    while (std::getline(ss, token, ',')) {
        tokens.push_back(token);
    }

    std::string device = (tokens.size() >= 3) ? tokens[2] : "Unknown";

    auto disconnect_msg = ros_kctrl_custom_interfaces::msg::KctrlDeviceDisconnected();
    disconnect_msg.device = device;
    pub_device_disconnected_->publish(disconnect_msg);
}
void Ros2UdpReceiver::handle_kssis_993(const std::string& msg) {
    publish_string(pub_pu_params_raw_, msg);

    std::stringstream ss(msg);
    std::string token;
    std::vector<std::string> tokens;
    while (std::getline(ss, token, ',')) {
        tokens.push_back(token);
    }

    std::string device = (tokens.size() >= 3) ? tokens[2] : "Unknown";

    size_t xml_start = msg.find('<');
    std::string xml_str = (xml_start != std::string::npos) ? msg.substr(xml_start) : "[Ingen XML funnet]";

    // ---- NEW: Write XML to file ----
    std::string xml_file_path = "/home/arveds/ros2_ws/src/ros_kctrl_interface_pkg/include/pu_params.xml"; // Change path as needed
    if (xml_start != std::string::npos) {
        std::ofstream xml_file(xml_file_path, std::ios::trunc);
        if (xml_file.is_open()) {
            xml_file << xml_str;
            xml_file.close();
            RCLCPP_INFO(this->get_logger(), "Wrote PU parameters XML to %s", xml_file_path.c_str());
        } else {
            RCLCPP_WARN(this->get_logger(), "Failed to open %s for writing PU parameters XML.", xml_file_path.c_str());
        }
    }

    auto pu_params_msg = ros_kctrl_custom_interfaces::msg::KctrlPUParams();
    pu_params_msg.device = device;
    pu_params_msg.xml = xml_str;
    pub_pu_params_->publish(pu_params_msg);
}
// --- HELPERS ---
void Ros2UdpReceiver::publish_string(const rclcpp::Publisher<std_msgs::msg::String>::SharedPtr& pub, const std::string& data) {
    auto msg = std_msgs::msg::String();
    msg.data = data;
    pub->publish(msg);
}

std::vector<ros_kctrl_custom_interfaces::msg::KctrlDevice> Ros2UdpReceiver::parse_devices(const std::string& msg) {
    std::vector<ros_kctrl_custom_interfaces::msg::KctrlDevice> devices;
    
    size_t start = msg.find("$KSSIS,12,");
    if (start == std::string::npos) return devices;
    size_t dataStart = start + std::string("$KSSIS,12,").length();
    std::string body = msg.substr(dataStart);

    if (body.size() >= 3 && body.compare(body.size() - 3, 3, "$$$") == 0) {
        body = body.substr(0, body.size() - 3);
    }

    std::vector<std::string> deviceEntries;
    size_t pos = 0, found;
    while ((found = body.find("$$$", pos)) != std::string::npos) {
        deviceEntries.push_back(body.substr(pos, found - pos));
        pos = found + 3;
    }
    deviceEntries.push_back(body.substr(pos));

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

        auto device = ros_kctrl_custom_interfaces::msg::KctrlDevice();
        device.name = deviceInfo.count("NAME") ? deviceInfo["NAME"] : "";
        device.ip = deviceInfo.count("IP") ? deviceInfo["IP"] : "";
        device.type = deviceInfo.count("TYPE") ? deviceInfo["TYPE"] : "";
        device.system_id = deviceInfo.count("SYSTEM_ID") ? deviceInfo["SYSTEM_ID"] : "";
        device.started = deviceInfo.count("STARTED") ? deviceInfo["STARTED"] : "";
        device.is_pinging = deviceInfo.count("IS_PINGING") ? deviceInfo["IS_PINGING"] : "";
        device.data_macst_ip = deviceInfo.count("DATA_MACST_IP") ? deviceInfo["DATA_MACST_IP"] : "";
        device.data_macst_port = deviceInfo.count("DATA_MACST_PORT") ? deviceInfo["DATA_MACST_PORT"] : "";
        device.cmdport_port = deviceInfo.count("CMDPORT_PORT") ? deviceInfo["CMDPORT_PORT"] : "";
        device.system_desc = deviceInfo.count("SYSTEM_DESC") ? deviceInfo["SYSTEM_DESC"] : "";
        device.license = deviceInfo.count("LICENSE") ? deviceInfo["LICENSE"] : "";
        
        devices.push_back(device);
    }
    return devices;
}

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<Ros2UdpReceiver>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
