#include "ros_kctrl_interface_pkg/ros2ctrl_udp_client.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

Ros2CtrlUdpClient::Ros2CtrlUdpClient(const std::string& ip, int port)
    : ip_(ip), port_(port) {}

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

bool Ros2CtrlUdpClient::send_udp_cmd(const std::string& msg, const std::string& action, const std::string& sounder_name) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "[UDP] Socket creation failed\n";
        return false;
    }
    sockaddr_in servaddr{};
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port_);
    if (inet_pton(AF_INET, ip_.c_str(), &servaddr.sin_addr) <= 0) {
        std::cerr << "[UDP] Invalid address/ Address not supported\n";
        close(sockfd);
        return false;
    }

    ssize_t sent = sendto(sockfd, msg.c_str(), msg.size(), 0,
                          (sockaddr*)&servaddr, sizeof(servaddr));
    close(sockfd);

    if (sent < 0) {
        std::cerr << "[UDP] sendto failed\n";
        return false;
    }
    std::cout << "[UDP] Sent " << action << " for: " << sounder_name << " to " << ip_ << ":" << port_ << std::endl;
    return true;
}