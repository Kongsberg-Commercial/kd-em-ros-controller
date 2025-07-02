#include "rclcpp/rclcpp.hpp"
#include "ros_kctrl_custom_interfaces/srv/sounder_command.hpp"
#include "ros_kctrl_custom_interfaces/srv/request_pu_parameters.hpp"
#include "ros_kctrl_interface_pkg/ros2ctrl_udp_client.hpp"
#include "ros_kctrl_custom_interfaces/srv/set_pu_parameters.hpp"
#include "ros_kctrl_custom_interfaces/srv/recording_control.hpp"
#include "ros_kctrl_custom_interfaces/srv/update_recording_path.hpp"

class Ros2CtrlWrapperNode : public rclcpp::Node {
public:
    Ros2CtrlWrapperNode() : Node("ros2_ctrl_wrapper_node") {
        this->declare_parameter<std::string>("kctrl_ip", "192.168.48.1");
        this->declare_parameter<int>("kctrl_port", 14002);

        std::string kctrl_ip = this->get_parameter("kctrl_ip").as_string();
        int kctrl_port = this->get_parameter("kctrl_port").as_int();

        udp_client_ = std::make_shared<Ros2CtrlUdpClient>(kctrl_ip, kctrl_port);

        start_sounder_srv_ = this->create_service<ros_kctrl_custom_interfaces::srv::SounderCommand>(
            "kctrl/start_sounder",
            std::bind(&Ros2CtrlWrapperNode::startSounderCallback, this, std::placeholders::_1, std::placeholders::_2)
        );
        start_pinging_srv_ = this->create_service<ros_kctrl_custom_interfaces::srv::SounderCommand>(
            "kctrl/start_pinging",
            std::bind(&Ros2CtrlWrapperNode::startPingingCallback, this, std::placeholders::_1, std::placeholders::_2)
        );
        stop_pinging_srv_ = this->create_service<ros_kctrl_custom_interfaces::srv::SounderCommand>(
            "kctrl/stop_pinging",
            std::bind(&Ros2CtrlWrapperNode::stopPingingCallback, this, std::placeholders::_1, std::placeholders::_2)
        );
        start_water_column_srv_ = this->create_service<ros_kctrl_custom_interfaces::srv::SounderCommand>(
            "kctrl/start_water_column",
            std::bind(&Ros2CtrlWrapperNode::startWaterColumnCallback, this, std::placeholders::_1, std::placeholders::_2)
        );
        stop_water_column_srv_ = this->create_service<ros_kctrl_custom_interfaces::srv::SounderCommand>(
            "kctrl/stop_water_column",
            std::bind(&Ros2CtrlWrapperNode::stopWaterColumnCallback, this, std::placeholders::_1, std::placeholders::_2)
        );
        start_stave_srv_ = this->create_service<ros_kctrl_custom_interfaces::srv::SounderCommand>(
            "kctrl/start_stave",
            std::bind(&Ros2CtrlWrapperNode::startStaveCallback, this, std::placeholders::_1, std::placeholders::_2)
        );
        stop_stave_srv_ = this->create_service<ros_kctrl_custom_interfaces::srv::SounderCommand>(
            "kctrl/stop_stave",
            std::bind(&Ros2CtrlWrapperNode::stopStaveCallback, this, std::placeholders::_1, std::placeholders::_2)
        );
        shutdown_srv_ = this->create_service<ros_kctrl_custom_interfaces::srv::SounderCommand>(
            "kctrl/shutdown",
            std::bind(&Ros2CtrlWrapperNode::shutdownCallback, this, std::placeholders::_1, std::placeholders::_2)
        );
        request_pu_parameters_srv_ = this->create_service<ros_kctrl_custom_interfaces::srv::RequestPUParameters>(
            "kctrl/request_pu_parameters",
            std::bind(&Ros2CtrlWrapperNode::requestPUParametersCallback, this, std::placeholders::_1, std::placeholders::_2)
        );
        set_pu_parameters_srv_ = this->create_service<ros_kctrl_custom_interfaces::srv::SetPUParameters>(
            "kctrl/set_pu_parameters",
            std::bind(&Ros2CtrlWrapperNode::setPUParametersCallback, this, std::placeholders::_1, std::placeholders::_2)
        );
        request_install_runtime_parameters_srv_ = this->create_service<ros_kctrl_custom_interfaces::srv::SounderCommand>(
            "kctrl/request_install_runtime_parameters",
            std::bind(&Ros2CtrlWrapperNode::requestInstallRuntimeParametersCallback, this, std::placeholders::_1, std::placeholders::_2)
        );
        request_multicast_address_srv_ = this->create_service<ros_kctrl_custom_interfaces::srv::SounderCommand>(
            "kctrl/request_multicast_address",
            std::bind(&Ros2CtrlWrapperNode::requestMulticastAddressCallback, this, std::placeholders::_1, std::placeholders::_2)
        );
        recording_control_srv_ = this->create_service<ros_kctrl_custom_interfaces::srv::RecordingControl>(
            "kctrl/recording_control",
            std::bind(&Ros2CtrlWrapperNode::recordingControlCallback, this, std::placeholders::_1, std::placeholders::_2)
        );
        update_recording_path_srv_ = this->create_service<ros_kctrl_custom_interfaces::srv::UpdateRecordingPath>(
            "kctrl/update_recording_path",
            std::bind(&Ros2CtrlWrapperNode::updateRecordingPathCallback, this, std::placeholders::_1, std::placeholders::_2)
        );
        disconnect_sounder_srv_ = this->create_service<ros_kctrl_custom_interfaces::srv::SounderCommand>(
            "kctrl/disconnect_sounder",
            std::bind(&Ros2CtrlWrapperNode::disconnectSounderCallback, this, std::placeholders::_1, std::placeholders::_2)
        );

        RCLCPP_INFO(this->get_logger(), "ROS2_Ctrl Wrapper Node started.");
    }

private:
    void startSounderCallback(
        const std::shared_ptr<ros_kctrl_custom_interfaces::srv::SounderCommand::Request> req,
        std::shared_ptr<ros_kctrl_custom_interfaces::srv::SounderCommand::Response> res)
    {
        bool ok = udp_client_->send_start_sounder(req->sounder_name);
        res->success = ok;
        res->message = ok ? "Start command sent" : "Failed to send start command";
    }

    void startPingingCallback(
        const std::shared_ptr<ros_kctrl_custom_interfaces::srv::SounderCommand::Request> req,
        std::shared_ptr<ros_kctrl_custom_interfaces::srv::SounderCommand::Response> res)
    {
        bool ok = udp_client_->send_start_pinging(req->sounder_name);
        res->success = ok;
        res->message = ok ? "Start pinging sent" : "Failed to send pinging command";
    }

    void stopPingingCallback(
        const std::shared_ptr<ros_kctrl_custom_interfaces::srv::SounderCommand::Request> req,
        std::shared_ptr<ros_kctrl_custom_interfaces::srv::SounderCommand::Response> res)
    {
        bool ok = udp_client_->send_stop_pinging(req->sounder_name);
        res->success = ok;
        res->message = ok ? "Stop pinging sent" : "Failed to send stop pinging";
    }

    void startWaterColumnCallback(
        const std::shared_ptr<ros_kctrl_custom_interfaces::srv::SounderCommand::Request> req,
        std::shared_ptr<ros_kctrl_custom_interfaces::srv::SounderCommand::Response> res)
    {
        bool ok = udp_client_->send_start_water_column(req->sounder_name);
        res->success = ok;
        res->message = ok ? "Start water column command sent" : "Failed to send start water column command";
    }

    void stopWaterColumnCallback(
        const std::shared_ptr<ros_kctrl_custom_interfaces::srv::SounderCommand::Request> req,
        std::shared_ptr<ros_kctrl_custom_interfaces::srv::SounderCommand::Response> res)
    {
        bool ok = udp_client_->send_stop_water_column(req->sounder_name);
        res->success = ok;
        res->message = ok ? "Stop water column command sent" : "Failed to send stop water column command";
    }

    void startStaveCallback(
        const std::shared_ptr<ros_kctrl_custom_interfaces::srv::SounderCommand::Request> req,
        std::shared_ptr<ros_kctrl_custom_interfaces::srv::SounderCommand::Response> res)
    {
        bool ok = udp_client_->send_start_stave(req->sounder_name);
        res->success = ok;
        res->message = ok ? "Start stave command sent" : "Failed to send start stave command";
    }

    void stopStaveCallback(
        const std::shared_ptr<ros_kctrl_custom_interfaces::srv::SounderCommand::Request> req,
        std::shared_ptr<ros_kctrl_custom_interfaces::srv::SounderCommand::Response> res)
    {
        bool ok = udp_client_->send_stop_stave(req->sounder_name);
        res->success = ok;
        res->message = ok ? "Stop stave command sent" : "Failed to send stop stave command";
    }

    void shutdownCallback(
        const std::shared_ptr<ros_kctrl_custom_interfaces::srv::SounderCommand::Request> /*req*/,
        std::shared_ptr<ros_kctrl_custom_interfaces::srv::SounderCommand::Response> res)
    {
        bool ok = udp_client_->send_shutdown();
        res->success = ok;
        res->message = ok ? "Shutdown command sent" : "Failed to send shutdown command";
    }

    void requestPUParametersCallback(
        const std::shared_ptr<ros_kctrl_custom_interfaces::srv::RequestPUParameters::Request> req,
        std::shared_ptr<ros_kctrl_custom_interfaces::srv::RequestPUParameters::Response> res)
    {
        bool ok = udp_client_->send_request_pu_parameters(req->sounder_name, req->param_names);
        res->success = ok;
        res->message = ok ? "Request params info sent" : "Failed to send request params info";
    }

    void setPUParametersCallback(
        const std::shared_ptr<ros_kctrl_custom_interfaces::srv::SetPUParameters::Request> req,
        std::shared_ptr<ros_kctrl_custom_interfaces::srv::SetPUParameters::Response> res)
    {
        if (req->param_names.size() != req->param_values.size()) {
            res->success = false;
            res->message = "param_names and param_values must have the same length.";
            return;
        }
        std::vector<std::pair<std::string, std::string>> pairs;
        for (size_t i = 0; i < req->param_names.size(); ++i) {
            pairs.emplace_back(req->param_names[i], req->param_values[i]);
        }
        bool ok = udp_client_->send_set_pu_parameters(req->sounder_name, pairs);
        res->success = ok;
        res->message = ok ? "Set params command sent" : "Failed to send set params command";
    }

    void requestInstallRuntimeParametersCallback(
        const std::shared_ptr<ros_kctrl_custom_interfaces::srv::SounderCommand::Request> req,
        std::shared_ptr<ros_kctrl_custom_interfaces::srv::SounderCommand::Response> res)
    {
        bool ok = udp_client_->send_request_installation_and_runtime_parameters(req->sounder_name);
        res->success = ok;
        res->message = ok ? "Install/runtime params request sent" : "Failed to send install/runtime params request";
    }
    
    void requestMulticastAddressCallback(
        const std::shared_ptr<ros_kctrl_custom_interfaces::srv::SounderCommand::Request> req,
        std::shared_ptr<ros_kctrl_custom_interfaces::srv::SounderCommand::Response> res)
    {
        bool ok = udp_client_->send_request_multicast_address(req->sounder_name);
        res->success = ok;
        res->message = ok ? "Multicast address request sent" : "Failed to send multicast address request";
    }

    void recordingControlCallback(
        const std::shared_ptr<ros_kctrl_custom_interfaces::srv::RecordingControl::Request> req,
        std::shared_ptr<ros_kctrl_custom_interfaces::srv::RecordingControl::Response> res)
    {
        bool ok = udp_client_->send_recording_control(req->sounder_name, req->turn_on);
        res->success = ok;
        res->message = ok ? "Recording control command sent" : "Failed to send recording control command";
    }

    void updateRecordingPathCallback(
        const std::shared_ptr<ros_kctrl_custom_interfaces::srv::UpdateRecordingPath::Request> req,
        std::shared_ptr<ros_kctrl_custom_interfaces::srv::UpdateRecordingPath::Response> res)
    {
        bool ok = udp_client_->send_update_recording_path(req->sounder_name, req->folder_path);
        res->success = ok;
        res->message = ok ? "Update recording path command sent" : "Failed to send update recording path command";
    }

    void disconnectSounderCallback(
        const std::shared_ptr<ros_kctrl_custom_interfaces::srv::SounderCommand::Request> req,
        std::shared_ptr<ros_kctrl_custom_interfaces::srv::SounderCommand::Response> res)
    {
        bool ok = udp_client_->send_disconnect_sounder(req->sounder_name);
        res->success = ok;
        res->message = ok ? "Disconnect sounder command sent" : "Failed to send disconnect sounder command";
    }

    rclcpp::Service<ros_kctrl_custom_interfaces::srv::SounderCommand>::SharedPtr start_sounder_srv_;
    rclcpp::Service<ros_kctrl_custom_interfaces::srv::SounderCommand>::SharedPtr start_pinging_srv_;
    rclcpp::Service<ros_kctrl_custom_interfaces::srv::SounderCommand>::SharedPtr stop_pinging_srv_;
    rclcpp::Service<ros_kctrl_custom_interfaces::srv::SounderCommand>::SharedPtr start_water_column_srv_;
    rclcpp::Service<ros_kctrl_custom_interfaces::srv::SounderCommand>::SharedPtr stop_water_column_srv_;
    rclcpp::Service<ros_kctrl_custom_interfaces::srv::SounderCommand>::SharedPtr start_stave_srv_;
    rclcpp::Service<ros_kctrl_custom_interfaces::srv::SounderCommand>::SharedPtr stop_stave_srv_;
    rclcpp::Service<ros_kctrl_custom_interfaces::srv::SounderCommand>::SharedPtr shutdown_srv_;
    rclcpp::Service<ros_kctrl_custom_interfaces::srv::RequestPUParameters>::SharedPtr request_pu_parameters_srv_;
    rclcpp::Service<ros_kctrl_custom_interfaces::srv::SetPUParameters>::SharedPtr set_pu_parameters_srv_;
    rclcpp::Service<ros_kctrl_custom_interfaces::srv::SounderCommand>::SharedPtr request_install_runtime_parameters_srv_;
    rclcpp::Service<ros_kctrl_custom_interfaces::srv::SounderCommand>::SharedPtr request_multicast_address_srv_;
    rclcpp::Service<ros_kctrl_custom_interfaces::srv::RecordingControl>::SharedPtr recording_control_srv_;
    rclcpp::Service<ros_kctrl_custom_interfaces::srv::UpdateRecordingPath>::SharedPtr update_recording_path_srv_;
    rclcpp::Service<ros_kctrl_custom_interfaces::srv::SounderCommand>::SharedPtr disconnect_sounder_srv_;

    std::shared_ptr<Ros2CtrlUdpClient> udp_client_;
};

int main(int argc, char ** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Ros2CtrlWrapperNode>());
    rclcpp::shutdown();
    return 0;
}