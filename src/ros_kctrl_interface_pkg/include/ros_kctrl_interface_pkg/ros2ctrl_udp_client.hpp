#pragma once
#include <string>
#include <vector>
#include <utility>

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
    bool send_shutdown();
    bool send_request_pu_parameters(const std::string& sounder_name, const std::vector<std::string>& parameter_names);
    bool send_set_pu_parameters(const std::string& sounder_name, const std::vector<std::pair<std::string, std::string>>& param_value_pairs);
    bool send_request_installation_and_runtime_parameters(const std::string& sounder_name);
    bool send_request_multicast_address(const std::string& sounder_name);
    bool send_recording_control(const std::string& sounder_name, bool turn_on);
    bool send_update_recording_path(const std::string& sounder_name, const std::string& folder_path);
    bool send_disconnect_sounder(const std::string& sounder_name);
    bool send_export_pu_parameters(const std::string& sounder_name, const std::string& filename);
    bool send_request_detected_sounders();
    bool send_import_pu_parameters(const std::string& sounder_name, const std::string& pu_parameters_xml);

private:
    std::string ip_;
    int port_;

    bool send_udp_cmd(const std::string& msg, const std::string& action, const std::string& sounder_name);
};