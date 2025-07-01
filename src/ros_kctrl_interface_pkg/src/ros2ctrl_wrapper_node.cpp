#include "rclcpp/rclcpp.hpp"
#include "ros_kctrl_custom_interfaces/srv/sounder_command.hpp"
#include "ros_kctrl_interface_pkg/ros2ctrl_udp_client.hpp"

class Ros2CtrlWrapperNode : public rclcpp::Node {
public:
    Ros2CtrlWrapperNode() : Node("ros2_ctrl_wrapper_node") {
        this->declare_parameter<std::string>("kctrl_ip", "127.0.0.1");
        this->declare_parameter<int>("kctrl_port", 14002);

        std::string kctrl_ip = this->get_parameter("kctrl_ip").as_string();
        int kctrl_port = this->get_parameter("kctrl_port").as_int();

        udp_client_ = std::make_shared<Ros2CtrlUdpClient>(kctrl_ip, kctrl_port);

        // Existing services
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

        // New services based on the diagram
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

    rclcpp::Service<ros_kctrl_custom_interfaces::srv::SounderCommand>::SharedPtr start_sounder_srv_;
    rclcpp::Service<ros_kctrl_custom_interfaces::srv::SounderCommand>::SharedPtr start_pinging_srv_;
    rclcpp::Service<ros_kctrl_custom_interfaces::srv::SounderCommand>::SharedPtr stop_pinging_srv_;

    rclcpp::Service<ros_kctrl_custom_interfaces::srv::SounderCommand>::SharedPtr start_water_column_srv_;
    rclcpp::Service<ros_kctrl_custom_interfaces::srv::SounderCommand>::SharedPtr stop_water_column_srv_;
    rclcpp::Service<ros_kctrl_custom_interfaces::srv::SounderCommand>::SharedPtr start_stave_srv_;
    rclcpp::Service<ros_kctrl_custom_interfaces::srv::SounderCommand>::SharedPtr stop_stave_srv_;

    std::shared_ptr<Ros2CtrlUdpClient> udp_client_;
};

int main(int argc, char ** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Ros2CtrlWrapperNode>());
    rclcpp::shutdown();
    return 0;
}