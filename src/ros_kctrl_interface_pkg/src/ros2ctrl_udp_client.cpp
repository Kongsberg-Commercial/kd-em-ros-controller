#include "ros_kctrl_interface_pkg/ros2ctrl_udp_client.hpp"
#include "ros_kctrl_interface_pkg/simple_udp.hpp"
#include <iostream>
#include <sstream>
#include <vector>

Ros2CtrlUdpClient::Ros2CtrlUdpClient(const std::string& ip, int port)
    : ip_(ip), port_(port) {
    // Create a UDP sender that connects to the specified IP and port
    udp_sender_ = SimpleUDP::createSender(ip, port);
    
    if (!udp_sender_ || !udp_sender_->isValid()) {
        std::cerr << "[UDP Client] Failed to create UDP sender to " << ip << ":" << port << std::endl;
        if (udp_sender_) {
            std::cerr << "[UDP Client] Error: " << udp_sender_->getLastError() << std::endl;
        }
    } else {
        std::cout << "[UDP Client] Ready to send commands to " << ip << ":" << port << std::endl;
    }
}

bool Ros2CtrlUdpClient::send_start_sounder(const std::string& sounder_name) {
    return send_udp_cmd("$KSSIS,13," + sounder_name, "start sounder", sounder_name);
}

bool Ros2CtrlUdpClient::send_start_pinging(const std::string& sounder_name) {
    return send_udp_cmd("$KSSIS,458," + sounder_name, "start pinging", sounder_name);
}

bool Ros2CtrlUdpClient::send_stop_pinging(const std::string& sounder_name) {
    return send_udp_cmd("$KSSIS,457," + sounder_name, "stop pinging", sounder_name);
}

bool Ros2CtrlUdpClient::send_start_water_column(const std::string& sounder_name) {
    return send_udp_cmd("$KSSIS,461," + sounder_name, "start water column", sounder_name);
}

bool Ros2CtrlUdpClient::send_stop_water_column(const std::string& sounder_name) {
    return send_udp_cmd("$KSSIS,460," + sounder_name, "stop water column", sounder_name);
}

bool Ros2CtrlUdpClient::send_start_stave(const std::string& sounder_name) {
    return send_udp_cmd("$KSSIS,463," + sounder_name, "start stave", sounder_name);
}

bool Ros2CtrlUdpClient::send_stop_stave(const std::string& sounder_name) {
    return send_udp_cmd("$KSSIS,462," + sounder_name, "stop stave", sounder_name);
}

bool Ros2CtrlUdpClient::send_shutdown() {
    return send_udp_cmd("$KSSIS,24", "shutdown", "");
}

bool Ros2CtrlUdpClient::send_request_pu_parameters(const std::string& sounder_name, const std::vector<std::string>& param_names) {
    std::ostringstream oss;
    oss << "$KSSIS,261," << sounder_name;
    for (const auto& param : param_names) {
        oss << "," << param;
    }
    return send_udp_cmd(oss.str(), "request params info", sounder_name);
}

bool Ros2CtrlUdpClient::send_set_pu_parameters(
    const std::string& sounder_name,
    const std::vector<std::pair<std::string, std::string>>& param_value_pairs)
{
    std::ostringstream oss;
    oss << "$KSSIS,262," << sounder_name;
    for (const auto& pv : param_value_pairs) {
        oss << "," << pv.first << "=" << pv.second;
    }
    return send_udp_cmd(oss.str(), "set pu parameters", sounder_name);
}

bool Ros2CtrlUdpClient::send_request_installation_and_runtime_parameters(const std::string& sounder_name) {
    std::string msg = "$KSSIS,454," + sounder_name;
    return send_udp_cmd(msg, "request installation and runtime params", sounder_name);
}

bool Ros2CtrlUdpClient::send_request_multicast_address(const std::string& sounder_name) {
    std::string msg = "$KSSIS,456," + sounder_name;
    return send_udp_cmd(msg, "request multicast address", sounder_name);
}

bool Ros2CtrlUdpClient::send_recording_control(const std::string& sounder_name, bool turn_on) {
    std::string msg = "$KSSIS,473," + sounder_name + "," + (turn_on ? "ON" : "OFF");
    return send_udp_cmd(msg, "recording control", sounder_name);
}

bool Ros2CtrlUdpClient::send_update_recording_path(const std::string& sounder_name, const std::string& folder_path) {
    std::string msg = "$KSSIS,474," + sounder_name + "," + folder_path;
    return send_udp_cmd(msg, "update recording path", sounder_name);
}

bool Ros2CtrlUdpClient::send_disconnect_sounder(const std::string& sounder_name) {
    std::string msg = "$KSSIS,99992," + sounder_name;
    return send_udp_cmd(msg, "disconnect sounder", sounder_name);
}

bool Ros2CtrlUdpClient::send_export_pu_parameters(const std::string& sounder_name, const std::string& filename) {
    std::ostringstream oss;
    oss << "$KSSIS,991," << sounder_name;
    if (!filename.empty()) {
        oss << ",FILENAME=" << filename;
    }
    return send_udp_cmd(oss.str(), "export pu parameters", sounder_name);
}

bool Ros2CtrlUdpClient::send_request_detected_sounders() {
    return send_udp_cmd("$KSSIS,997", "request detected sounders", "");
}

bool Ros2CtrlUdpClient::send_import_pu_parameters(const std::string& sounder_name, const std::string& pu_parameters_xml) {
    std::ostringstream oss;
    oss << "$KSSIS,992," << sounder_name << "," << pu_parameters_xml;
    return send_udp_cmd(oss.str(), "import pu parameters", sounder_name);
}

bool Ros2CtrlUdpClient::send_udp_cmd(const std::string& msg, const std::string& action, const std::string& sounder_name) {
    // Check if UDP sender is ready
    if (!udp_sender_ || !udp_sender_->isValid()) {
        std::cerr << "[UDP] UDP sender not available" << std::endl;
        return false;
    }
    
    // Send the message - that's it! No complex socket code needed.
    bool success = udp_sender_->send(msg);
    
    if (!success) {
        std::cerr << "[UDP] Failed to send message: " << udp_sender_->getLastError() << std::endl;
        return false;
    }
    
    std::cout << "[UDP] Sent " << action << " for: " << sounder_name << " to " << ip_ << ":" << port_ << std::endl;
    return true;
}