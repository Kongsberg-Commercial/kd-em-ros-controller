#include "ppnode/ppnode.hpp"
#include "rclcpp/rclcpp.hpp"

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    
    // Create the path planning node
    auto node = std::make_shared<ppnode::PpNode>();
    
    RCLCPP_INFO(node->get_logger(), "Starting Path Planning Node...");
    
    // Spin the node
    rclcpp::spin(node);
    
    RCLCPP_INFO(node->get_logger(), "Shutting down Path Planning Node...");
    rclcpp::shutdown();
    return 0;
}