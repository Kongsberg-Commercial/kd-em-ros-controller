#pragma once
#include <string>

class Ros2CtrlUdpClient {
public:
    Ros2CtrlUdpClient(const std::string& ip, int port);

    bool send_start_sounder(const std::string& sounder_name);
    bool send_start_pinging(const std::string& sounder_name);
    bool send_stop_pinging(const std::string& sounder_name);
    bool send_start_water_column(const std::string& name);
    bool send_stop_water_column(const std::string& name);
    bool send_start_stave(const std::string& name);
    bool send_stop_stave(const std::string& name);

private:
    std::string ip_;
    int port_;

    bool send_udp_cmd(const std::string& msg, const std::string& action, const std::string& sounder_name);
};