#include "em_data_analyzer/pointcloud_analyzer.hpp"

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<em_data_analyzer::PointCloudAnalyzer>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
