#pragma once
#include <string>
#include <memory>

/**
 * Simple UDP communication helper class
 * Hides all the complex socket programming details
 */
class SimpleUDP {
public:
    /**
     * Create a UDP sender
     * @param target_ip IP address to send to (e.g., "192.168.1.100")
     * @param target_port Port number to send to (e.g., 4001)
     */
    static std::unique_ptr<SimpleUDP> createSender(const std::string& target_ip, int target_port);
    
    /**
     * Create a UDP receiver
     * @param listen_port Port to listen on (e.g., 4002)
     */
    static std::unique_ptr<SimpleUDP> createReceiver(int listen_port);
    
    /**
     * Send a message (only works for senders)
     * @param message The text message to send
     * @return true if sent successfully, false otherwise
     */
    bool send(const std::string& message);
    
    /**
     * Receive a message (only works for receivers)
     * @param message_out The received message will be stored here
     * @param timeout_ms How long to wait for a message (0 = don't wait, -1 = wait forever)
     * @return true if message received, false if timeout or error
     */
    bool receive(std::string& message_out, int timeout_ms = 0);
    
    /**
     * Check if this UDP object is valid (successfully created)
     */
    bool isValid() const { return socket_fd_ >= 0; }
    
    /**
     * Get error message if something went wrong
     */
    std::string getLastError() const { return last_error_; }
    
    ~SimpleUDP();

private:
    SimpleUDP(int socket_fd, bool is_sender);
    
    int socket_fd_ = -1;
    bool is_sender_ = false;
    std::string target_ip_;
    int target_port_ = 0;
    std::string last_error_;
    
    // Internal socket address storage
    void* target_addr_ = nullptr;
};
