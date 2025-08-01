#include "ros_kctrl_interface_pkg/simple_udp.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <memory>

SimpleUDP::SimpleUDP(int socket_fd, bool is_sender) 
    : socket_fd_(socket_fd), is_sender_(is_sender) {
}

SimpleUDP::~SimpleUDP() {
    if (socket_fd_ >= 0) {
        close(socket_fd_);
    }
    if (target_addr_) {
        delete static_cast<sockaddr_in*>(target_addr_);
    }
}

std::unique_ptr<SimpleUDP> SimpleUDP::createSender(const std::string& target_ip, int target_port) {
    // Create socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        return nullptr;
    }
    
    auto udp = std::unique_ptr<SimpleUDP>(new SimpleUDP(sockfd, true));
    
    // Store target information
    udp->target_ip_ = target_ip;
    udp->target_port_ = target_port;
    
    // Set up target address
    auto* addr = new sockaddr_in{};
    addr->sin_family = AF_INET;
    addr->sin_port = htons(target_port);
    
    if (inet_pton(AF_INET, target_ip.c_str(), &addr->sin_addr) <= 0) {
        udp->last_error_ = "Invalid IP address: " + target_ip;
        delete addr;
        return nullptr;
    }
    
    udp->target_addr_ = addr;
    return udp;
}

std::unique_ptr<SimpleUDP> SimpleUDP::createReceiver(int listen_port) {
    // Create socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        return nullptr;
    }
    
    // Set up listening address
    sockaddr_in servaddr{};
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;  // Listen on all network interfaces
    servaddr.sin_port = htons(listen_port);
    
    // Bind socket to port
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        close(sockfd);
        return nullptr;
    }
    
    return std::unique_ptr<SimpleUDP>(new SimpleUDP(sockfd, false));
}

bool SimpleUDP::send(const std::string& message) {
    if (!is_sender_ || socket_fd_ < 0 || !target_addr_) {
        last_error_ = "Not a valid sender";
        return false;
    }
    
    auto* addr = static_cast<sockaddr_in*>(target_addr_);
    ssize_t sent = sendto(socket_fd_, message.c_str(), message.size(), 0,
                          (sockaddr*)addr, sizeof(*addr));
    
    if (sent < 0) {
        last_error_ = "Failed to send message";
        return false;
    }
    
    return true;
}

bool SimpleUDP::receive(std::string& message_out, int timeout_ms) {
    if (is_sender_ || socket_fd_ < 0) {
        last_error_ = "Not a valid receiver";
        return false;
    }
    
    // Set timeout if specified
    if (timeout_ms > 0) {
        struct timeval tv;
        tv.tv_sec = timeout_ms / 1000;
        tv.tv_usec = (timeout_ms % 1000) * 1000;
        setsockopt(socket_fd_, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    }
    
    char buffer[4096];
    ssize_t received = recvfrom(socket_fd_, buffer, sizeof(buffer) - 1, 0, nullptr, nullptr);
    
    if (received < 0) {
        last_error_ = "Failed to receive message";
        return false;
    }
    
    buffer[received] = '\0';
    message_out = std::string(buffer);
    return true;
}
