#include <ros_otter_control_pkg/otter_tcp_node.hpp>
#include <std_msgs/msg/bool.hpp>

OtterTCPNode::OtterTCPNode()
    : Node("otter_tcp_node"), stop_(false) {
    // Declare and get parameters from config file
    ip_ = this->declare_parameter("device_ip", "192.168.53.3");
    port_ = this->declare_parameter("device_port", 2009);
    
    // Additional configurable parameters
    auto connection_timeout = this->declare_parameter("connection_timeout_sec", 10);
    auto retry_interval = this->declare_parameter("retry_interval_sec", 5);
    auto position_threshold = this->declare_parameter("position_threshold", 0.0001);
    auto auto_leg_progression = this->declare_parameter("auto_leg_progression", true);
    auto initial_leg_number = this->declare_parameter("initial_leg_number", 1);
    
    RCLCPP_INFO(this->get_logger(), "Starting OtterTCPNode with IP: %s, Port: %d", ip_.c_str(), port_);
    RCLCPP_INFO(this->get_logger(), "Position threshold: %.6f degrees", position_threshold);
    RCLCPP_INFO(this->get_logger(), "Auto leg progression: %s", auto_leg_progression ? "enabled" : "disabled");
    
    connect_socket();
    receiver_thread_ = std::thread(&OtterTCPNode::receive_loop, this);

    // Service servers for each maneuver mode
    drift_srv_ = this->create_service<ros_otter_custom_interfaces::srv::DriftMode>("set_drift_mode",
        std::bind(&OtterTCPNode::handle_drift, this, std::placeholders::_1, std::placeholders::_2));
    manual_srv_ = this->create_service<ros_otter_custom_interfaces::srv::ManualMode>("set_manual_mode",
        std::bind(&OtterTCPNode::handle_manual, this, std::placeholders::_1, std::placeholders::_2));
    course_srv_ = this->create_service<ros_otter_custom_interfaces::srv::CourseMode>("set_course_mode",
        std::bind(&OtterTCPNode::handle_course, this, std::placeholders::_1, std::placeholders::_2));
    leg_srv_ = this->create_service<ros_otter_custom_interfaces::srv::LegMode>("set_leg_mode",
        std::bind(&OtterTCPNode::handle_leg, this, std::placeholders::_1, std::placeholders::_2));
    station_srv_ = this->create_service<ros_otter_custom_interfaces::srv::StationMode>("set_station_mode",
        std::bind(&OtterTCPNode::handle_station, this, std::placeholders::_1, std::placeholders::_2));
    param_srv_ = this->create_service<ros_otter_custom_interfaces::srv::SetParameters>("set_parameters",
        std::bind(&OtterTCPNode::handle_parameters, this, std::placeholders::_1, std::placeholders::_2));

    // Service client
    ppnode_leg_client_ = this->create_client<ros_otter_custom_interfaces::srv::LegMode>("leg_srv");

    // Publishers for each incoming TCP message type
    gps_pub_   = this->create_publisher<ros_otter_custom_interfaces::msg::GpsInfo>("gps_info", 10);
    imu_pub_   = this->create_publisher<ros_otter_custom_interfaces::msg::ImuInfo>("imu_info", 10);
    mode_pub_  = this->create_publisher<ros_otter_custom_interfaces::msg::ModeInfo>("mode_info", 10);
    error_pub_ = this->create_publisher<ros_otter_custom_interfaces::msg::ErrorInfo>("error_info", 10);
    otter_pub_ = this->create_publisher<ros_otter_custom_interfaces::msg::OtterInfo>("otter_info", 10);

    // Publisher for leg_status topic
    leg_status_pub_ = this->create_publisher<std_msgs::msg::Bool>("/leg_status", 10);

    // Initialize leg tracking variables
    leg_active_ = false;
    leg_end_lat_ = 0.0;
    leg_end_lon_ = 0.0;
    leg_end_lat_dir_ = "";
    leg_end_lon_dir_ = "";
    current_leg_number_ = initial_leg_number;
    
    // Start the automatic leg progression if enabled
    if (auto_leg_progression) {
        start_leg_progression();
    }
}

OtterTCPNode::~OtterTCPNode() {
    stop_ = true;
    close(sockfd_);
    if (receiver_thread_.joinable()) receiver_thread_.join();
}

void OtterTCPNode::connect_socket() {
    sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_);
    inet_pton(AF_INET, ip_.c_str(), &serv_addr.sin_addr);
    if (connect(sockfd_, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        RCLCPP_ERROR(this->get_logger(), "Connection to %s:%d failed", ip_.c_str(), port_);
    } else {
        RCLCPP_INFO(this->get_logger(), "Connected to %s:%d", ip_.c_str(), port_);
    }
}

void OtterTCPNode::receive_loop() {
    char buffer[1024];
    std::string line;
    while (!stop_) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(sockfd_, &readfds);
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 100000; // 100 ms timeout

        int retval = select(sockfd_ + 1, &readfds, NULL, NULL, &tv);
        if (retval > 0 && FD_ISSET(sockfd_, &readfds)) {
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
}

void OtterTCPNode::process_line(const std::string& line) {
    RCLCPP_INFO(this->get_logger(), "Received: %s", line.c_str());
    if (line.rfind("$PMARGPS", 0) == 0) {
        auto msg = parse_gps(line);
        if (msg) {
            gps_pub_->publish(*msg);

            // Check if leg is active and endpoint reached
            if (leg_active_ && at_leg_endpoint(msg->lat, msg->lat_dir, msg->lon, msg->lon_dir)) {
                std_msgs::msg::Bool status_msg;
                status_msg.data = false;
                leg_status_pub_->publish(status_msg);
                leg_active_ = false;
                RCLCPP_INFO(this->get_logger(), "Leg %d ended, set /leg_status to false.", current_leg_number_);
                
                // Start next leg automatically
                current_leg_number_++;
                start_next_leg();
            }
        }
    } else if (line.rfind("$PMARIMU", 0) == 0) {
        auto msg = parse_imu(line);
        if (msg) imu_pub_->publish(*msg);
    } else if (line.rfind("$PMARMOD", 0) == 0) {
        auto msg = parse_mode(line);
        if (msg) {
            mode_pub_->publish(*msg);
        } else {
            RCLCPP_WARN(this->get_logger(), "Failed to parse PMARMOD message: %s", line.c_str());
        }
    } else if (line.rfind("$PMARERR", 0) == 0) {
        auto msg = parse_error(line);
        if (msg) error_pub_->publish(*msg);
    } else if (line.rfind("$PMAROTR", 0) == 0) {
        auto msg = parse_otter(line);
        if (msg) otter_pub_->publish(*msg);
    }
}

// --- Parsing helpers for each message type ---

std::optional<ros_otter_custom_interfaces::msg::GpsInfo> OtterTCPNode::parse_gps(const std::string& line) {
    auto star_idx = line.find('*');
    std::string content = (star_idx != std::string::npos) ? line.substr(9, star_idx-9) : line.substr(9);
    std::stringstream ss(content);
    std::string item;
    std::vector<std::string> tokens;
    while (std::getline(ss, item, ',')) tokens.push_back(item);
    if (tokens.size() < 8) return std::nullopt;
    ros_otter_custom_interfaces::msg::GpsInfo msg;
    try {
        msg.time = std::stod(tokens[0]);
        msg.lat = std::stod(tokens[1]);
        msg.lat_dir = tokens[2];
        msg.lon = std::stod(tokens[3]);
        msg.lon_dir = tokens[4];
        msg.alt = std::stod(tokens[5]);
        msg.sog = std::stod(tokens[6]);
        msg.cog = std::stod(tokens[7]);
    } catch (...) { return std::nullopt; }
    return msg;
}

std::optional<ros_otter_custom_interfaces::msg::ImuInfo> OtterTCPNode::parse_imu(const std::string& line) {
    auto star_idx = line.find('*');
    std::string content = (star_idx != std::string::npos) ? line.substr(9, star_idx-9) : line.substr(9);
    std::stringstream ss(content);
    std::string item;
    std::vector<std::string> tokens;
    while (std::getline(ss, item, ',')) tokens.push_back(item);
    if (tokens.size() < 6) return std::nullopt;
    ros_otter_custom_interfaces::msg::ImuInfo msg;
    try {
        msg.roll = std::stod(tokens[0]);
        msg.pitch = std::stod(tokens[1]);
        msg.yaw = std::stod(tokens[2]);
        msg.p = std::stod(tokens[3]);
        msg.q = std::stod(tokens[4]);
        msg.r = std::stod(tokens[5]);
    } catch (...) { return std::nullopt; }
    return msg;
}

std::optional<ros_otter_custom_interfaces::msg::ModeInfo> OtterTCPNode::parse_mode(const std::string& line) {
    auto star_idx = line.find('*');
    std::string content = (star_idx != std::string::npos) ? line.substr(9, star_idx-9) : line.substr(9);
    std::stringstream ss(content);
    std::string item;
    std::vector<std::string> tokens;
    while (std::getline(ss, item, ',')) tokens.push_back(item);
    if (tokens.size() < 2) {
        return std::nullopt;
    }
    ros_otter_custom_interfaces::msg::ModeInfo msg;
    try {
        msg.mode = tokens[0];
        msg.fuel_capacity = std::stof(tokens[1]);
    } catch (...) {
        return std::nullopt;
    }
    return msg;
}

std::optional<ros_otter_custom_interfaces::msg::ErrorInfo> OtterTCPNode::parse_error(const std::string& line) {
    auto star_idx = line.find('*');
    std::string content = (star_idx != std::string::npos) ? line.substr(9, star_idx-9) : line.substr(9);
    ros_otter_custom_interfaces::msg::ErrorInfo msg;
    try {
        msg.description = content;
    } catch (...) { return std::nullopt; }
    return msg;
}

std::optional<ros_otter_custom_interfaces::msg::OtterInfo> OtterTCPNode::parse_otter(const std::string& line) {
    auto star_idx = line.find('*');
    std::string content = (star_idx != std::string::npos) ? line.substr(9, star_idx-9) : line.substr(9);
    std::stringstream ss(content);
    std::string item;
    std::vector<std::string> tokens;
    while (std::getline(ss, item, ',')) tokens.push_back(item);
    if (tokens.size() < 8) return std::nullopt;
    ros_otter_custom_interfaces::msg::OtterInfo msg;
    try {
        msg.rpm_port = std::stof(tokens[0]);
        msg.rpm_stb = std::stof(tokens[1]);
        msg.temp_port = std::stof(tokens[2]);
        msg.temp_stb = std::stof(tokens[3]);
        msg.power_port = std::stof(tokens[4]);
        msg.power_stb = std::stof(tokens[5]);
        msg.power_total = std::stof(tokens[6]);
        msg.batteries_count = static_cast<uint8_t>(std::stoi(tokens[7]));
    } catch (...) { return std::nullopt; }
    return msg;
}

std::string OtterTCPNode::compute_checksum(const std::string& msg) {
    uint8_t chk = 0;
    for (char c : msg) chk ^= c;
    std::ostringstream oss;
    oss << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << (int)chk;
    return oss.str();
}

bool OtterTCPNode::send_nmea(const std::string &msg) {
    std::lock_guard<std::mutex> lock(sock_mutex_);
    std::string framed = "$" + msg + "*" + compute_checksum(msg) + "\r\n";
    ssize_t ret = send(sockfd_, framed.c_str(), framed.size(), 0);
    if (ret != (ssize_t)framed.size()) {
        RCLCPP_ERROR(this->get_logger(), "Failed to send TCP message.");
        return false;
    }
    return true;
}

// Helper for endpoint detection
bool OtterTCPNode::at_leg_endpoint(double curr_lat, const std::string& curr_lat_dir,
                                   double curr_lon, const std::string& curr_lon_dir) {
    double threshold = 0.0001; // ~11 meters at equator, adjust if needed
    return leg_active_ &&
           curr_lat_dir == leg_end_lat_dir_ && curr_lon_dir == leg_end_lon_dir_ &&
           std::abs(curr_lat - leg_end_lat_) < threshold &&
           std::abs(curr_lon - leg_end_lon_) < threshold;
}

void OtterTCPNode::start_leg_progression() {
    RCLCPP_INFO(this->get_logger(), "Starting automatic leg progression with leg 1");
    start_next_leg();
}

void OtterTCPNode::start_next_leg() {
    if (!leg_active_) {
        RCLCPP_INFO(this->get_logger(), "Attempting to start leg %d", current_leg_number_);
        request_leg_from_ppnode(current_leg_number_);
    }
}

void OtterTCPNode::request_leg_from_ppnode(int leg_number) {
    // Check if ppnode service is available
    if (!ppnode_leg_client_->wait_for_service(std::chrono::seconds(1))) {
        RCLCPP_WARN(this->get_logger(), "ppnode leg_srv service not available, retrying in 5 seconds...");
        
        // Create a timer to retry after 5 seconds
        auto timer = this->create_wall_timer(
            std::chrono::seconds(5),
            [this, leg_number]() {
                this->request_leg_from_ppnode(leg_number);
            }
        );
        
        // Store timer to prevent it from being destroyed
        retry_timers_.push_back(timer);
        return;
    }
    
    // Create request to ppnode
    auto ppnode_request = std::make_shared<ros_otter_custom_interfaces::srv::LegMode::Request>();
    ppnode_request->leg_number = leg_number;
    
    RCLCPP_INFO(this->get_logger(), "Requesting leg %d from ppnode...", leg_number);
    
    // Call ppnode service synchronously with timeout
    auto future = ppnode_leg_client_->async_send_request(ppnode_request);
    
    // Wait for response with timeout
    if (rclcpp::spin_until_future_complete(this->get_node_base_interface(), future, std::chrono::seconds(10)) 
        == rclcpp::FutureReturnCode::SUCCESS) {
        
        auto ppnode_response = future.get();
        
        if (ppnode_response->success) {
            // Use coordinates from ppnode response
            double lat0 = ppnode_response->lat0;
            std::string lat0_dir = ppnode_response->lat0_dir;
            double lon0 = ppnode_response->lon0;
            std::string lon0_dir = ppnode_response->lon0_dir;
            double lat1 = ppnode_response->lat1;
            std::string lat1_dir = ppnode_response->lat1_dir;
            double lon1 = ppnode_response->lon1;
            std::string lon1_dir = ppnode_response->lon1_dir;
            double speed = ppnode_response->speed;
            
            // Build NMEA message with the coordinates from ppnode
            std::ostringstream oss;
            oss << "PMARLEG," 
                << lat0 << "," << lat0_dir << ","
                << lon0 << "," << lon0_dir << ","
                << lat1 << "," << lat1_dir << ","
                << lon1 << "," << lon1_dir << ","
                << speed;
            
            bool ok = send_nmea(oss.str());
            
            if (ok) {
                // Publish leg_status true and store endpoint
                std_msgs::msg::Bool status_msg;
                status_msg.data = true;
                leg_status_pub_->publish(status_msg);

                leg_end_lat_ = lat1;
                leg_end_lat_dir_ = lat1_dir;
                leg_end_lon_ = lon1;
                leg_end_lon_dir_ = lon1_dir;
                leg_active_ = true;

                RCLCPP_INFO(this->get_logger(), "Successfully started leg %d", leg_number);
                RCLCPP_INFO(this->get_logger(), "Leg started, set /leg_status to true.");
            } else {
                RCLCPP_ERROR(this->get_logger(), "Failed to send NMEA command for leg %d, retrying in 5 seconds...", leg_number);
                // Retry after 5 seconds
                auto retry_timer = this->create_wall_timer(
                    std::chrono::seconds(5),
                    [this, leg_number]() {
                        this->request_leg_from_ppnode(leg_number);
                    }
                );
                retry_timers_.push_back(retry_timer);
            }
        } else {
            if (ppnode_response->message.find("No more legs") != std::string::npos || 
                ppnode_response->message.find("completed") != std::string::npos) {
                RCLCPP_INFO(this->get_logger(), "All legs completed: %s", ppnode_response->message.c_str());
            } else {
                RCLCPP_WARN(this->get_logger(), "ppnode failed for leg %d: %s, retrying in 5 seconds...", 
                           leg_number, ppnode_response->message.c_str());
                // Retry after 5 seconds
                auto retry_timer = this->create_wall_timer(
                    std::chrono::seconds(5),
                    [this, leg_number]() {
                        this->request_leg_from_ppnode(leg_number);
                    }
                );
                retry_timers_.push_back(retry_timer);
            }
        }
    } else {
        RCLCPP_ERROR(this->get_logger(), "Timeout calling ppnode for leg %d, retrying in 5 seconds...", leg_number);
        // Retry after 5 seconds
        auto retry_timer = this->create_wall_timer(
            std::chrono::seconds(5),
            [this, leg_number]() {
                this->request_leg_from_ppnode(leg_number);
            }
        );
        retry_timers_.push_back(retry_timer);
    }
}

// Service handlers

void OtterTCPNode::handle_drift(const std::shared_ptr<ros_otter_custom_interfaces::srv::DriftMode::Request> request,
                                std::shared_ptr<ros_otter_custom_interfaces::srv::DriftMode::Response> response) {
    (void)request;
    bool ok = send_nmea("PMARABT");
    response->success = ok;
    response->message = ok ? "Drift mode command sent." : "Failed to send drift command.";
}

void OtterTCPNode::handle_manual(const std::shared_ptr<ros_otter_custom_interfaces::srv::ManualMode::Request> request,
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

void OtterTCPNode::handle_course(const std::shared_ptr<ros_otter_custom_interfaces::srv::CourseMode::Request> request,
                                 std::shared_ptr<ros_otter_custom_interfaces::srv::CourseMode::Response> response) {
    std::ostringstream oss;
    oss << "PMARCRS," << request->course << "," << request->speed;
    bool ok = send_nmea(oss.str());
    response->success = ok;
    response->message = ok ? "Course mode command sent." : "Failed to send course mode command.";
}

void OtterTCPNode::handle_leg(const std::shared_ptr<ros_otter_custom_interfaces::srv::LegMode::Request> request,
                              std::shared_ptr<ros_otter_custom_interfaces::srv::LegMode::Response> response) {
    
    // TODO: Leg selection/iterative logic TO BE IMPLEMENTED || currently leg_number from external client
    RCLCPP_INFO(this->get_logger(), "Received leg mode request for leg number: %ld", request->leg_number);
    
    // Check if ppnode service is available
    if (!ppnode_leg_client_->wait_for_service(std::chrono::seconds(2))) {
        RCLCPP_ERROR(this->get_logger(), "ppnode leg_srv service not available");
        response->success = false;
        response->message = "ppnode leg_srv service not available";
        return;
    }
    
    // Create request to ppnode
    auto ppnode_request = std::make_shared<ros_otter_custom_interfaces::srv::LegMode::Request>();
    ppnode_request->leg_number = request->leg_number;
    
    // Call ppnode service synchronously
    auto future = ppnode_leg_client_->async_send_request(ppnode_request);
    
    // Wait for response with timeout
    if (rclcpp::spin_until_future_complete(this->get_node_base_interface(), future, std::chrono::seconds(5)) 
        == rclcpp::FutureReturnCode::SUCCESS) {
        
        auto ppnode_response = future.get();
        
        if (ppnode_response->success) {
            // Use coordinates from ppnode response
            double lat0 = ppnode_response->lat0;
            std::string lat0_dir = ppnode_response->lat0_dir;
            double lon0 = ppnode_response->lon0;
            std::string lon0_dir = ppnode_response->lon0_dir;
            double lat1 = ppnode_response->lat1;
            std::string lat1_dir = ppnode_response->lat1_dir;
            double lon1 = ppnode_response->lon1;
            std::string lon1_dir = ppnode_response->lon1_dir;
            double speed = ppnode_response->speed;
            
            // Build NMEA message with the coordinates from ppnode
            std::ostringstream oss;
            oss << "PMARLEG," 
                << lat0 << "," << lat0_dir << ","
                << lon0 << "," << lon0_dir << ","
                << lat1 << "," << lat1_dir << ","
                << lon1 << "," << lon1_dir << ","
                << speed;
            
            bool ok = send_nmea(oss.str());
            
            // Fill response with the coordinates that were sent
            response->lat0 = lat0;
            response->lat0_dir = lat0_dir;
            response->lon0 = lon0;
            response->lon0_dir = lon0_dir;
            response->lat1 = lat1;
            response->lat1_dir = lat1_dir;
            response->lon1 = lon1;
            response->lon1_dir = lon1_dir;
            response->speed = speed;
            response->success = ok;
            response->message = ok ? 
                ("Leg mode command sent with coordinates from ppnode: " + ppnode_response->message) : 
                "Failed to send leg mode command";
            
            // Publish leg_status true and store endpoint
            std_msgs::msg::Bool status_msg;
            status_msg.data = true;
            leg_status_pub_->publish(status_msg);

            leg_end_lat_ = lat1;
            leg_end_lat_dir_ = lat1_dir;
            leg_end_lon_ = lon1;
            leg_end_lon_dir_ = lon1_dir;
            leg_active_ = true;

            RCLCPP_INFO(this->get_logger(), "Successfully called ppnode and sent NMEA for leg %ld", request->leg_number);
            RCLCPP_INFO(this->get_logger(), "Leg started, set /leg_status to true.");
        } else {
            RCLCPP_ERROR(this->get_logger(), "ppnode failed to calculate coordinates: %s", ppnode_response->message.c_str());
            response->success = false;
            response->message = "ppnode failed: " + ppnode_response->message;
        }
        
    } else {
        RCLCPP_ERROR(this->get_logger(), "Failed to call ppnode leg_srv service (timeout)");
        response->success = false;
        response->message = "Timeout calling ppnode leg_srv service";
    }
}

void OtterTCPNode::handle_station(const std::shared_ptr<ros_otter_custom_interfaces::srv::StationMode::Request> request,
                                  std::shared_ptr<ros_otter_custom_interfaces::srv::StationMode::Response> response) {
    std::ostringstream oss;
    oss << "PMARSTA," << request->lat << "," << request->lat_dir << ","
        << request->lon << "," << request->lon_dir << "," << request->speed;
    bool ok = send_nmea(oss.str());
    response->success = ok;
    response->message = ok ? "Station mode command sent." : "Failed to send station mode command.";
}

void OtterTCPNode::handle_parameters(const std::shared_ptr<ros_otter_custom_interfaces::srv::SetParameters::Request> request,
                                     std::shared_ptr<ros_otter_custom_interfaces::srv::SetParameters::Response> response) {
    std::ostringstream oss;
    oss << "PMARPRM," << request->lookahead_distance << ","
        << request->radius_in << "," << request->radius_out;
    bool ok = send_nmea(oss.str());
    response->success = ok;
    response->message = ok ? "Set parameters command sent." : "Failed to send set parameters command.";
}

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<OtterTCPNode>());
    rclcpp::shutdown();
    return 0;
}