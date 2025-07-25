#pragma once

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <geometry_msgs/msg/point.hpp>
#include <std_msgs/msg/bool.hpp>
#include <visualization_msgs/msg/marker.hpp>
#include <visualization_msgs/msg/marker_array.hpp>
#include <sensor_msgs/point_cloud2_iterator.hpp>
#include "ros_otter_custom_interfaces/msg/swath_line_stats.hpp"
#include "ros_otter_custom_interfaces/msg/gps_info.hpp"
#include "ros_otter_custom_interfaces/msg/shadow_analysis.hpp"
#include <vector>
#include <deque>
#include <GeographicLib/LocalCartesian.hpp>

namespace em_data_analyzer {

struct GeoPoint {
    double x; // ENU
    double y;
    double z;
    double lat; // Global coordinates
    double lon;
    double alt;
};

struct AnalyzerConfig {
    double shadow_density_threshold = 0.1;  // 10% of expected density
    double grid_cell_size = 10.0;           // meters
    double edge_exclusion_factor = 0.1;     // Exclude edges within this fraction of mean swath width
    double along_track_exclusion = 20.0;    // Fixed exclusion distance along track (meters)
    size_t gps_buffer_size = 100;
    std::string pointcloud_topic = "/PointcloudFromKMALL";
    std::string gps_topic = "gps_info";
    std::string leg_status_topic = "/leg_status";
    std::string stats_topic = "/swath_line_stats";
    std::string shadow_topic = "/shadow_analysis";
    std::string viz_pointcloud_topic = "/viz/accumulated_pointcloud";
    std::string viz_gps_path_topic = "/viz/gps_path";
    std::string viz_shadows_topic = "/viz/shadows";
    int pointcloud_queue_size = 10;
    int gps_queue_size = 100;
    int leg_status_queue_size = 10;
    int stats_queue_size = 10;
    int shadow_queue_size = 10;
    int viz_queue_size = 10;
    size_t max_accumulated_points = 100000; // Limit accumulated points
    double point_decimation_distance = 1.0; // Minimum distance between accumulated points
};

class PointCloudAnalyzer : public rclcpp::Node
{
public:
    PointCloudAnalyzer();

private:
    // Configuration
    AnalyzerConfig config_;
    
    // Buffer for GPS messages
    std::deque<ros_otter_custom_interfaces::msg::GpsInfo::SharedPtr> gps_buffer_;

    // ROS2 subscriptions and publisher
    rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr subscription_;
    rclcpp::Subscription<ros_otter_custom_interfaces::msg::GpsInfo>::SharedPtr gps_sub_;
    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr leg_status_subscription_;
    rclcpp::Publisher<ros_otter_custom_interfaces::msg::SwathLineStats>::SharedPtr stats_publisher_;
    rclcpp::Publisher<ros_otter_custom_interfaces::msg::ShadowAnalysis>::SharedPtr shadow_publisher_;

    // Visualization publishers
    rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr viz_pointcloud_publisher_;
    rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr viz_gps_path_publisher_;
    rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr viz_shadows_publisher_;
    
    // State
    bool leg_active_;
    GeographicLib::LocalCartesian global_proj_;
    bool global_proj_initialized_;
    std::string current_leg_id_;

    // Data collected during a leg
    std::vector<double> swath_lengths_;
    std::vector<double> swath_min_depths_;
    std::vector<double> swath_max_depths_;
    std::vector<double> swath_mean_depths_;
    std::vector<GeoPoint> all_leg_points_;
    
    // Accumulated data for visualization
    std::vector<GeoPoint> accumulated_points_;
    std::vector<GeoPoint> gps_path_points_;

    // Methods
    void load_config();
    void setup_subscriptions_and_publishers();
    
    void gps_callback(const ros_otter_custom_interfaces::msg::GpsInfo::SharedPtr msg);
    void pointcloud_callback(const sensor_msgs::msg::PointCloud2::SharedPtr msg);
    void leg_status_callback(const std_msgs::msg::Bool::SharedPtr msg);
    
    template<typename T>
    typename T::SharedPtr find_closest_gps(const std::deque<typename T::SharedPtr>& buffer, 
                                          const rclcpp::Time& stamp);
    
    void process_pointcloud_data(const sensor_msgs::msg::PointCloud2::SharedPtr msg,
                               const ros_otter_custom_interfaces::msg::GpsInfo::SharedPtr gps);
    
    void start_leg();
    void end_leg();
    void publish_stats();
    void analyze_holes_and_shadows();
    void publish_shadow_analysis(const std::vector<std::pair<double, double>>& shadow_cells,
                               int total_cells, double mean_density);
    
    void reset_leg_data();
    std::pair<geometry_msgs::msg::Point, geometry_msgs::msg::Point> 
        find_swath_boundaries(const sensor_msgs::msg::PointCloud2::SharedPtr msg);
    std::vector<double> extract_depths(const sensor_msgs::msg::PointCloud2::SharedPtr msg);
    double calculate_swath_length(const geometry_msgs::msg::Point& left, 
                                const geometry_msgs::msg::Point& right);
    std::string generate_leg_id();

    // New visualization methods
    void publish_accumulated_pointcloud();
    void publish_gps_path();
    void publish_shadow_visualization(const std::vector<std::pair<double, double>>& shadow_cells);
    void add_points_to_accumulation(const sensor_msgs::msg::PointCloud2::SharedPtr msg,
                                   const ros_otter_custom_interfaces::msg::GpsInfo::SharedPtr gps);
    bool should_add_point(const GeoPoint& new_point);
    sensor_msgs::msg::PointCloud2 create_pointcloud_message(const std::vector<GeoPoint>& points);
};

} // namespace em_data_analyzer
