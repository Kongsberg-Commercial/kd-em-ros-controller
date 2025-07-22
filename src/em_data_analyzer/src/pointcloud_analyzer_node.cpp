#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <sensor_msgs/point_cloud2_iterator.hpp>
#include <geometry_msgs/msg/point.hpp>
#include <std_msgs/msg/bool.hpp>
#include "ros_otter_custom_interfaces/msg/swath_line_stats.hpp"
#include "ros_otter_custom_interfaces/msg/gps_info.hpp"
#include <vector>
#include <array>
#include <limits>
#include <cmath>
#include <numeric>
#include <sstream>
#include <deque>
#include <algorithm>
#include <GeographicLib/LocalCartesian.hpp>

struct GeoPoint {
    double x; // ENU
    double y;
    double z;
    double lat; // Global coordinates
    double lon;
    double alt;
};

class PointCloudAnalyzer : public rclcpp::Node
{
public:
    PointCloudAnalyzer()
    : Node("pointcloud_analyzer"),
      leg_active_(false)
    {
        // PointCloud2 subscriber
        subscription_ = this->create_subscription<sensor_msgs::msg::PointCloud2>(
            "/PointcloudFromKMALL",
            10,
            std::bind(&PointCloudAnalyzer::pointcloud_callback, this, std::placeholders::_1)
        );

        // GPS subscriber
        gps_sub_ = this->create_subscription<ros_otter_custom_interfaces::msg::GpsInfo>(
            "gps_info", 100,
            std::bind(&PointCloudAnalyzer::gps_callback, this, std::placeholders::_1));

        leg_status_subscription_ = this->create_subscription<std_msgs::msg::Bool>(
            "/leg_status",
            10,
            std::bind(&PointCloudAnalyzer::leg_status_callback, this, std::placeholders::_1)
        );

        stats_publisher_ = this->create_publisher<ros_otter_custom_interfaces::msg::SwathLineStats>("/swath_line_stats", 10);
    }

private:
    // Buffer for GPS messages (keep last 100 for safety)
    std::deque<ros_otter_custom_interfaces::msg::GpsInfo::SharedPtr> gps_buffer_;
    const size_t buffer_size_ = 100;

    void gps_callback(const ros_otter_custom_interfaces::msg::GpsInfo::SharedPtr msg)
    {
        gps_buffer_.push_back(msg);
        if (gps_buffer_.size() > buffer_size_) gps_buffer_.pop_front();
    }

    // Find the closest GPS message in a buffer by timestamp
    template<typename T>
    typename T::SharedPtr find_closest(const std::deque<typename T::SharedPtr>& buffer, const rclcpp::Time& stamp)
    {
        typename T::SharedPtr closest = nullptr;
        rclcpp::Duration min_diff = rclcpp::Duration::from_nanoseconds(std::numeric_limits<int64_t>::max());

        for (const auto& msg : buffer) {
            if (!msg) continue;
            rclcpp::Time msg_time(static_cast<uint64_t>(msg->time * 1e9), RCL_SYSTEM_TIME); // use system time
            rclcpp::Duration diff = (msg_time > stamp) ? (msg_time - stamp) : (stamp - msg_time);
            if (diff < min_diff) {
                min_diff = diff;
                closest = msg;
            }
        }
        return closest;
    }

    // Called for each pointcloud ping
    void pointcloud_callback(const sensor_msgs::msg::PointCloud2::SharedPtr msg)
    {
        // --- Sync with closest GPS ---
        // Convert header.stamp to system time for fair comparison with GPS epoch time
        rclcpp::Time stamp(msg->header.stamp.sec, msg->header.stamp.nanosec, RCL_SYSTEM_TIME);
        auto gps = find_closest<ros_otter_custom_interfaces::msg::GpsInfo>(gps_buffer_, stamp);
        if (!gps) {
            RCLCPP_WARN(this->get_logger(), "No matching GPS for pointcloud at time %.3f", stamp.seconds());
            return;
        }

        // Set up ENU projection for this ping at vessel's position
        GeographicLib::LocalCartesian proj(gps->lat, gps->lon, gps->alt);

        double min_y = std::numeric_limits<double>::max();
        double max_y = std::numeric_limits<double>::lowest();
        std::vector<double> depths;
        geometry_msgs::msg::Point outer_left, outer_right;

        sensor_msgs::PointCloud2ConstIterator<float> iter_x(*msg, "x");
        sensor_msgs::PointCloud2ConstIterator<float> iter_y(*msg, "y");
        sensor_msgs::PointCloud2ConstIterator<float> iter_z(*msg, "z");

        for (size_t i = 0; i < msg->width * msg->height; ++i, ++iter_x, ++iter_y, ++iter_z)
        {
            double x = *iter_x;
            double y = *iter_y;
            double z = *iter_z;

            if (y < min_y) {
                min_y = y;
                outer_left.x = x;
                outer_left.y = y;
                outer_left.z = z;
            }
            if (y > max_y) {
                max_y = y;
                outer_right.x = x;
                outer_right.y = y;
                outer_right.z = z;
            }

            depths.push_back(z);

            if (leg_active_) {
                // Transform to global coordinates using per-ping GPS
                double lat, lon, h;
                proj.Reverse(x, y, z, lat, lon, h);
                // Also store back-transformed ENU for global grid analysis
                double enu_x, enu_y, enu_z;
                // For shadow analysis, use ENU with respect to a fixed origin for the leg.
                if (!global_proj_initialized_) {
                    // On first point of the leg, set global ENU origin
                    global_proj_ = GeographicLib::LocalCartesian(gps->lat, gps->lon, gps->alt);
                    global_proj_initialized_ = true;
                }
                global_proj_.Forward(lat, lon, h, enu_x, enu_y, enu_z);

                all_leg_points_.push_back({enu_x, enu_y, enu_z, lat, lon, h});
            }
        }

        double swath_length = std::sqrt(
            std::pow(outer_left.x - outer_right.x, 2) +
            std::pow(outer_left.y - outer_right.y, 2) +
            std::pow(outer_left.z - outer_right.z, 2)
        );

        double min_depth = *std::min_element(depths.begin(), depths.end());
        double max_depth = *std::max_element(depths.begin(), depths.end());
        double mean_depth = std::accumulate(depths.begin(), depths.end(), 0.0) / depths.size();

        // If vessel is driving a leg, collect stats
        if (leg_active_) {
            swath_lengths_.push_back(swath_length);
            swath_min_depths_.push_back(min_depth);
            swath_max_depths_.push_back(max_depth);
            swath_mean_depths_.push_back(mean_depth);
        }
    }

    // Called when leg status updates
    void leg_status_callback(const std_msgs::msg::Bool::SharedPtr msg)
    {
        if (msg->data && !leg_active_) {
            // Leg started
            leg_active_ = true;
            // Clear previous data
            swath_lengths_.clear();
            swath_min_depths_.clear();
            swath_max_depths_.clear();
            swath_mean_depths_.clear();
            all_leg_points_.clear();
            global_proj_initialized_ = false;
            RCLCPP_INFO(this->get_logger(), "Leg started, collecting swath statistics.");
        } else if (!msg->data && leg_active_) {
            // Leg ended
            leg_active_ = false;
            RCLCPP_INFO(this->get_logger(), "Leg ended, analyzing collected swath statistics.");
            publish_stats();
            analyze_holes_and_shadows();
        }
    }

    void publish_stats()
    {
        if (swath_lengths_.empty())
        {
            RCLCPP_WARN(this->get_logger(), "No swath data collected during leg.");
            return;
        }

        double min_swath = *std::min_element(swath_lengths_.begin(), swath_lengths_.end());
        double max_swath = *std::max_element(swath_lengths_.begin(), swath_lengths_.end());
        double avg_swath = std::accumulate(swath_lengths_.begin(), swath_lengths_.end(), 0.0) / swath_lengths_.size();

        double min_depth = *std::min_element(swath_min_depths_.begin(), swath_min_depths_.end());
        double max_depth = *std::max_element(swath_max_depths_.begin(), swath_max_depths_.end());
        double avg_depth = std::accumulate(swath_mean_depths_.begin(), swath_mean_depths_.end(), 0.0) / swath_mean_depths_.size();

        ros_otter_custom_interfaces::msg::SwathLineStats stats_msg;
        stats_msg.min_swath_width = min_swath;
        stats_msg.max_swath_width = max_swath;
        stats_msg.avg_swath_width = avg_swath;
        stats_msg.min_depth = min_depth;
        stats_msg.max_depth = max_depth;
        stats_msg.avg_depth = avg_depth;

        stats_publisher_->publish(stats_msg);

        RCLCPP_INFO(this->get_logger(),
            "Published SwathLineStats: min_swath=%.2f, max_swath=%.2f, avg_swath=%.2f, min_depth=%.2f, max_depth=%.2f, avg_depth=%.2f",
            min_swath, max_swath, avg_swath, min_depth, max_depth, avg_depth);
    }

    // Analyze leg for holes/shadows in point density, output both ENU and lat/lon of shadow cell centers
    void analyze_holes_and_shadows()
    {
        if (all_leg_points_.empty()) {
            RCLCPP_WARN(this->get_logger(), "No point data for shadow/hole analysis.");
            return;
        }

        // 1. Find bounds in ENU (all_leg_points_ is now in common global ENU frame)
        double min_x = std::numeric_limits<double>::max();
        double max_x = std::numeric_limits<double>::lowest();
        double min_y = std::numeric_limits<double>::max();
        double max_y = std::numeric_limits<double>::lowest();

        for (const auto& pt : all_leg_points_) {
            min_x = std::min(min_x, pt.x);
            max_x = std::max(max_x, pt.x);
            min_y = std::min(min_y, pt.y);
            max_y = std::max(max_y, pt.y);
        }

        // 2. Create grid
        double cell_size = 10.0; // meters
        int nx = static_cast<int>((max_x - min_x) / cell_size) + 1;
        int ny = static_cast<int>((max_y - min_y) / cell_size) + 1;
        std::vector<std::vector<int>> grid(nx, std::vector<int>(ny, 0));

        // 3. Fill grid
        for (const auto& pt : all_leg_points_) {
            int ix = static_cast<int>((pt.x - min_x) / cell_size);
            int iy = static_cast<int>((pt.y - min_y) / cell_size);
            if (ix >= 0 && ix < nx && iy >= 0 && iy < ny)
                grid[ix][iy]++;
        }

        // 4. Compute mean/threshold
        int total_points = all_leg_points_.size();
        int total_cells = nx * ny;
        double mean_density = double(total_points) / double(total_cells);

        std::vector<std::pair<double, double>> shadow_cells;
        for (int ix = 0; ix < nx; ++ix) {
            for (int iy = 0; iy < ny; ++iy) {
                if (grid[ix][iy] < mean_density * 0.1) { // 10% of expected density
                    double cell_x = min_x + ix * cell_size + cell_size/2.0;
                    double cell_y = min_y + iy * cell_size + cell_size/2.0;
                    shadow_cells.push_back({cell_x, cell_y});
                }
            }
        }

        RCLCPP_INFO(this->get_logger(), "Detected %zu shadow/low-density areas in leg.", shadow_cells.size());

        // --- Step 5: Convert shadow cell centers from ENU to Lat/Lon using common global_proj_ ---
        if (!global_proj_initialized_) {
            RCLCPP_WARN(this->get_logger(), "Global ENU origin not initialized, cannot convert to lat/lon.");
            for (size_t i = 0; i < shadow_cells.size(); ++i) {
                RCLCPP_INFO(this->get_logger(), "Shadow cell #%zu at (x=%.2f, y=%.2f) [No lat/lon conversion]", i, shadow_cells[i].first, shadow_cells[i].second);
            }
            return;
        }

        for (size_t i = 0; i < shadow_cells.size(); ++i) {
            double cell_x = shadow_cells[i].first;
            double cell_y = shadow_cells[i].second;
            double cell_z = 0.0;

            double lat, lon, h;
            global_proj_.Reverse(cell_x, cell_y, cell_z, lat, lon, h);

            RCLCPP_INFO(this->get_logger(),
                "Shadow cell #%zu at ENU(x=%.2f, y=%.2f) -> (lat=%.8f, lon=%.8f)",
                i, cell_x, cell_y, lat, lon);
        }
    }

    // ROS2 subscriptions and publisher
    rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr subscription_;
    rclcpp::Subscription<ros_otter_custom_interfaces::msg::GpsInfo>::SharedPtr gps_sub_;
    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr leg_status_subscription_;
    rclcpp::Publisher<ros_otter_custom_interfaces::msg::SwathLineStats>::SharedPtr stats_publisher_;

    // State
    bool leg_active_;
    GeographicLib::LocalCartesian global_proj_;
    bool global_proj_initialized_ = false;

    // Data collected during a leg
    std::vector<double> swath_lengths_;
    std::vector<double> swath_min_depths_;
    std::vector<double> swath_max_depths_;
    std::vector<double> swath_mean_depths_;
    std::vector<GeoPoint> all_leg_points_; // Store x, y, z (ENU global), lat, lon, alt for each point in leg
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<PointCloudAnalyzer>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}