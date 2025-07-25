#include "em_data_analyzer/pointcloud_analyzer.hpp"
#include <sensor_msgs/point_cloud2_iterator.hpp>
#include <std_msgs/msg/bool.hpp>
#include <limits>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <chrono>

namespace em_data_analyzer {

PointCloudAnalyzer::PointCloudAnalyzer()
: Node("pointcloud_analyzer"),
  leg_active_(false),
  global_proj_initialized_(false)
{
    load_config();
    setup_subscriptions_and_publishers();
    
    RCLCPP_INFO(this->get_logger(), "PointCloud Analyzer initialized");
}

void PointCloudAnalyzer::load_config()
{
    // Declare parameters with defaults
    this->declare_parameter("topics.pointcloud", config_.pointcloud_topic);
    this->declare_parameter("topics.gps", config_.gps_topic);
    this->declare_parameter("topics.leg_status", config_.leg_status_topic);
    this->declare_parameter("topics.stats_output", config_.stats_topic);
    this->declare_parameter("topics.shadow_output", config_.shadow_topic);
    this->declare_parameter("topics.viz_pointcloud", config_.viz_pointcloud_topic);
    this->declare_parameter("topics.viz_gps_path", config_.viz_gps_path_topic);
    this->declare_parameter("topics.viz_shadows", config_.viz_shadows_topic);
    
    this->declare_parameter("queue_sizes.pointcloud", config_.pointcloud_queue_size);
    this->declare_parameter("queue_sizes.gps", config_.gps_queue_size);
    this->declare_parameter("queue_sizes.leg_status", config_.leg_status_queue_size);
    this->declare_parameter("queue_sizes.stats_output", config_.stats_queue_size);
    this->declare_parameter("queue_sizes.shadow_output", config_.shadow_queue_size);
    this->declare_parameter("queue_sizes.viz", config_.viz_queue_size);
    
    this->declare_parameter("gps.buffer_size", static_cast<int>(config_.gps_buffer_size));
    this->declare_parameter("shadow_detection.grid_cell_size", config_.grid_cell_size);
    this->declare_parameter("shadow_detection.density_threshold", config_.shadow_density_threshold);
    this->declare_parameter("shadow_detection.edge_exclusion_factor", config_.edge_exclusion_factor);
    this->declare_parameter("shadow_detection.along_track_exclusion", config_.along_track_exclusion);
    this->declare_parameter("visualization.max_accumulated_points", static_cast<int>(config_.max_accumulated_points));
    this->declare_parameter("visualization.point_decimation_distance", config_.point_decimation_distance);
    
    // Get parameters
    config_.pointcloud_topic = this->get_parameter("topics.pointcloud").as_string();
    config_.gps_topic = this->get_parameter("topics.gps").as_string();
    config_.leg_status_topic = this->get_parameter("topics.leg_status").as_string();
    config_.stats_topic = this->get_parameter("topics.stats_output").as_string();
    config_.shadow_topic = this->get_parameter("topics.shadow_output").as_string();
    config_.viz_pointcloud_topic = this->get_parameter("topics.viz_pointcloud").as_string();
    config_.viz_gps_path_topic = this->get_parameter("topics.viz_gps_path").as_string();
    config_.viz_shadows_topic = this->get_parameter("topics.viz_shadows").as_string();
    
    config_.pointcloud_queue_size = this->get_parameter("queue_sizes.pointcloud").as_int();
    config_.gps_queue_size = this->get_parameter("queue_sizes.gps").as_int();
    config_.leg_status_queue_size = this->get_parameter("queue_sizes.leg_status").as_int();
    config_.stats_queue_size = this->get_parameter("queue_sizes.stats_output").as_int();
    config_.shadow_queue_size = this->get_parameter("queue_sizes.shadow_output").as_int();
    config_.viz_queue_size = this->get_parameter("queue_sizes.viz").as_int();
    
    config_.gps_buffer_size = this->get_parameter("gps.buffer_size").as_int();
    config_.grid_cell_size = this->get_parameter("shadow_detection.grid_cell_size").as_double();
    config_.shadow_density_threshold = this->get_parameter("shadow_detection.density_threshold").as_double();
    config_.edge_exclusion_factor = this->get_parameter("shadow_detection.edge_exclusion_factor").as_double();
    config_.along_track_exclusion = this->get_parameter("shadow_detection.along_track_exclusion").as_double();
    config_.max_accumulated_points = this->get_parameter("visualization.max_accumulated_points").as_int();
    config_.point_decimation_distance = this->get_parameter("visualization.point_decimation_distance").as_double();
    
    RCLCPP_INFO(this->get_logger(), "Configuration loaded: grid_cell_size=%.1f, density_threshold=%.2f", 
                config_.grid_cell_size, config_.shadow_density_threshold);
}

void PointCloudAnalyzer::setup_subscriptions_and_publishers()
{
    subscription_ = this->create_subscription<sensor_msgs::msg::PointCloud2>(
        config_.pointcloud_topic,
        config_.pointcloud_queue_size,
        std::bind(&PointCloudAnalyzer::pointcloud_callback, this, std::placeholders::_1)
    );

    gps_sub_ = this->create_subscription<ros_otter_custom_interfaces::msg::GpsInfo>(
        config_.gps_topic, 
        config_.gps_queue_size,
        std::bind(&PointCloudAnalyzer::gps_callback, this, std::placeholders::_1)
    );

    leg_status_subscription_ = this->create_subscription<std_msgs::msg::Bool>(
        config_.leg_status_topic,
        config_.leg_status_queue_size,
        std::bind(&PointCloudAnalyzer::leg_status_callback, this, std::placeholders::_1)
    );

    stats_publisher_ = this->create_publisher<ros_otter_custom_interfaces::msg::SwathLineStats>(
        config_.stats_topic, 
        config_.stats_queue_size
    );

    shadow_publisher_ = this->create_publisher<ros_otter_custom_interfaces::msg::ShadowAnalysis>(
        config_.shadow_topic,
        config_.shadow_queue_size
    );

    // Visualization publishers
    viz_pointcloud_publisher_ = this->create_publisher<sensor_msgs::msg::PointCloud2>(
        config_.viz_pointcloud_topic, config_.viz_queue_size);
    
    viz_gps_path_publisher_ = this->create_publisher<visualization_msgs::msg::MarkerArray>(
        config_.viz_gps_path_topic, config_.viz_queue_size);
    
    viz_shadows_publisher_ = this->create_publisher<visualization_msgs::msg::MarkerArray>(
        config_.viz_shadows_topic, config_.viz_queue_size);
}

void PointCloudAnalyzer::gps_callback(const ros_otter_custom_interfaces::msg::GpsInfo::SharedPtr msg)
{
    gps_buffer_.push_back(msg);
    if (gps_buffer_.size() > config_.gps_buffer_size) {
        gps_buffer_.pop_front();
    }
}

template<typename T>
typename T::SharedPtr PointCloudAnalyzer::find_closest_gps(
    const std::deque<typename T::SharedPtr>& buffer, 
    const rclcpp::Time& stamp)
{
    typename T::SharedPtr closest = nullptr;
    rclcpp::Duration min_diff = rclcpp::Duration::from_nanoseconds(std::numeric_limits<int64_t>::max());

    for (const auto& msg : buffer) {
        if (!msg) continue;
        rclcpp::Time msg_time(static_cast<uint64_t>(msg->time * 1e9), RCL_SYSTEM_TIME);
        rclcpp::Duration diff = (msg_time > stamp) ? (msg_time - stamp) : (stamp - msg_time);
        if (diff < min_diff) {
            min_diff = diff;
            closest = msg;
        }
    }
    return closest;
}

void PointCloudAnalyzer::pointcloud_callback(const sensor_msgs::msg::PointCloud2::SharedPtr msg)
{
    // Sync with closest GPS
    rclcpp::Time stamp(msg->header.stamp.sec, msg->header.stamp.nanosec, RCL_SYSTEM_TIME);
    auto gps = find_closest_gps<ros_otter_custom_interfaces::msg::GpsInfo>(gps_buffer_, stamp);
    
    if (!gps) {
        RCLCPP_WARN(this->get_logger(), "No matching GPS for pointcloud at time %.3f", stamp.seconds());
        return;
    }

    process_pointcloud_data(msg, gps);
}

void PointCloudAnalyzer::process_pointcloud_data(
    const sensor_msgs::msg::PointCloud2::SharedPtr msg,
    const ros_otter_custom_interfaces::msg::GpsInfo::SharedPtr gps)
{
    // Set up ENU projection for this ping at vessel's position
    GeographicLib::LocalCartesian proj(gps->lat, gps->lon, gps->alt);

    // Initialize global projection on first pointcloud
    if (!global_proj_initialized_) {
        global_proj_ = GeographicLib::LocalCartesian(gps->lat, gps->lon, gps->alt);
        global_proj_initialized_ = true;
    }

    auto [outer_left, outer_right] = find_swath_boundaries(msg);
    auto depths = extract_depths(msg);
    double swath_length = calculate_swath_length(outer_left, outer_right);

    double min_depth = *std::min_element(depths.begin(), depths.end());
    double max_depth = *std::max_element(depths.begin(), depths.end());
    double mean_depth = std::accumulate(depths.begin(), depths.end(), 0.0) / depths.size();

    // If vessel is driving a leg, collect stats and points
    if (leg_active_) {
        swath_lengths_.push_back(swath_length);
        swath_min_depths_.push_back(min_depth);
        swath_max_depths_.push_back(max_depth);
        swath_mean_depths_.push_back(mean_depth);

        // Store all points for shadow analysis
        sensor_msgs::PointCloud2ConstIterator<float> iter_x(*msg, "x");
        sensor_msgs::PointCloud2ConstIterator<float> iter_y(*msg, "y");
        sensor_msgs::PointCloud2ConstIterator<float> iter_z(*msg, "z");

        for (size_t i = 0; i < msg->width * msg->height; ++i, ++iter_x, ++iter_y, ++iter_z) {
            double x = *iter_x;
            double y = *iter_y;
            double z = *iter_z;

            // Transform to global coordinates
            double lat, lon, h;
            proj.Reverse(x, y, z, lat, lon, h);
            
            // Convert to global ENU for shadow analysis
            double enu_x, enu_y, enu_z;
            global_proj_.Forward(lat, lon, h, enu_x, enu_y, enu_z);

            all_leg_points_.push_back({enu_x, enu_y, enu_z, lat, lon, h});
        }
    }
    
    // Add GPS point to path for visualization
    double enu_x, enu_y, enu_z;
    global_proj_.Forward(gps->lat, gps->lon, gps->alt, enu_x, enu_y, enu_z);
    gps_path_points_.push_back({enu_x, enu_y, enu_z, gps->lat, gps->lon, gps->alt});
    
    // Add points to accumulation for visualization
    add_points_to_accumulation(msg, gps);
    
    // Publish visualizations
    publish_accumulated_pointcloud();
    publish_gps_path();
}

void PointCloudAnalyzer::add_points_to_accumulation(
    const sensor_msgs::msg::PointCloud2::SharedPtr msg,
    const ros_otter_custom_interfaces::msg::GpsInfo::SharedPtr gps)
{
    GeographicLib::LocalCartesian proj(gps->lat, gps->lon, gps->alt);
    
    sensor_msgs::PointCloud2ConstIterator<float> iter_x(*msg, "x");
    sensor_msgs::PointCloud2ConstIterator<float> iter_y(*msg, "y");
    sensor_msgs::PointCloud2ConstIterator<float> iter_z(*msg, "z");

    for (size_t i = 0; i < msg->width * msg->height; ++i, ++iter_x, ++iter_y, ++iter_z) {
        double x = *iter_x;
        double y = *iter_y;
        double z = *iter_z;

        // Transform to global coordinates
        double lat, lon, h;
        proj.Reverse(x, y, z, lat, lon, h);
        
        // Convert to global ENU for visualization
        if (global_proj_initialized_) {
            double enu_x, enu_y, enu_z;
            global_proj_.Forward(lat, lon, h, enu_x, enu_y, enu_z);
            
            GeoPoint new_point = {enu_x, enu_y, enu_z, lat, lon, h};
            
            // Add point if it's far enough from existing points (decimation)
            if (should_add_point(new_point)) {
                accumulated_points_.push_back(new_point);
                
                // Limit total accumulated points
                if (accumulated_points_.size() > config_.max_accumulated_points) {
                    accumulated_points_.erase(accumulated_points_.begin(), 
                        accumulated_points_.begin() + accumulated_points_.size() / 10);
                }
            }
        }
    }
}

bool PointCloudAnalyzer::should_add_point(const GeoPoint& new_point)
{
    if (accumulated_points_.empty()) return true;
    
    // Simple decimation: check distance to last few points
    size_t check_count = std::min(size_t(50), accumulated_points_.size());
    for (size_t i = accumulated_points_.size() - check_count; i < accumulated_points_.size(); ++i) {
        const auto& existing = accumulated_points_[i];
        double distance = std::sqrt(
            std::pow(new_point.x - existing.x, 2) +
            std::pow(new_point.y - existing.y, 2)
        );
        if (distance < config_.point_decimation_distance) {
            return false;
        }
    }
    return true;
}

void PointCloudAnalyzer::publish_accumulated_pointcloud()
{
    if (accumulated_points_.empty()) return;
    
    auto pc_msg = create_pointcloud_message(accumulated_points_);
    viz_pointcloud_publisher_->publish(pc_msg);
}

sensor_msgs::msg::PointCloud2 PointCloudAnalyzer::create_pointcloud_message(const std::vector<GeoPoint>& points)
{
    sensor_msgs::msg::PointCloud2 cloud_msg;
    cloud_msg.header.stamp = this->get_clock()->now();
    cloud_msg.header.frame_id = "map";
    cloud_msg.height = 1;
    cloud_msg.width = points.size();
    cloud_msg.is_dense = true;
    
    sensor_msgs::PointCloud2Modifier modifier(cloud_msg);
    modifier.setPointCloud2Fields(4,
        "x", 1, sensor_msgs::msg::PointField::FLOAT32,
        "y", 1, sensor_msgs::msg::PointField::FLOAT32,
        "z", 1, sensor_msgs::msg::PointField::FLOAT32,
        "intensity", 1, sensor_msgs::msg::PointField::FLOAT32);
    
    sensor_msgs::PointCloud2Iterator<float> iter_x(cloud_msg, "x");
    sensor_msgs::PointCloud2Iterator<float> iter_y(cloud_msg, "y");
    sensor_msgs::PointCloud2Iterator<float> iter_z(cloud_msg, "z");
    sensor_msgs::PointCloud2Iterator<float> iter_intensity(cloud_msg, "intensity");
    
    for (const auto& point : points) {
        *iter_x = static_cast<float>(point.x);
        *iter_y = static_cast<float>(point.y);
        *iter_z = static_cast<float>(point.z);
        *iter_intensity = std::abs(static_cast<float>(point.z)); // Use depth as intensity
        
        ++iter_x; ++iter_y; ++iter_z; ++iter_intensity;
    }
    
    return cloud_msg;
}

void PointCloudAnalyzer::publish_gps_path()
{
    if (gps_path_points_.size() < 2) return;
    
    visualization_msgs::msg::MarkerArray marker_array;
    
    // GPS path line
    visualization_msgs::msg::Marker path_marker;
    path_marker.header.frame_id = "map";
    path_marker.header.stamp = this->get_clock()->now();
    path_marker.ns = "gps_path";
    path_marker.id = 0;
    path_marker.type = visualization_msgs::msg::Marker::LINE_STRIP;
    path_marker.action = visualization_msgs::msg::Marker::ADD;
    path_marker.scale.x = 2.0; // Line width
    path_marker.color.r = 0.0;
    path_marker.color.g = 1.0;
    path_marker.color.b = 0.0;
    path_marker.color.a = 0.8;
    
    for (const auto& gps_point : gps_path_points_) {
        geometry_msgs::msg::Point point;
        point.x = gps_point.x;
        point.y = gps_point.y;
        point.z = gps_point.z;
        path_marker.points.push_back(point);
    }
    
    marker_array.markers.push_back(path_marker);
    
    // Current vessel position
    if (!gps_path_points_.empty()) {
        visualization_msgs::msg::Marker vessel_marker;
        vessel_marker.header.frame_id = "map";
        vessel_marker.header.stamp = this->get_clock()->now();
        vessel_marker.ns = "vessel";
        vessel_marker.id = 1;
        vessel_marker.type = visualization_msgs::msg::Marker::ARROW;
        vessel_marker.action = visualization_msgs::msg::Marker::ADD;
        
        const auto& current_pos = gps_path_points_.back();
        vessel_marker.pose.position.x = current_pos.x;
        vessel_marker.pose.position.y = current_pos.y;
        vessel_marker.pose.position.z = current_pos.z;
        vessel_marker.pose.orientation.w = 1.0;
        
        vessel_marker.scale.x = 10.0;
        vessel_marker.scale.y = 3.0;
        vessel_marker.scale.z = 3.0;
        vessel_marker.color.r = 1.0;
        vessel_marker.color.g = 0.0;
        vessel_marker.color.b = 0.0;
        vessel_marker.color.a = 1.0;
        
        marker_array.markers.push_back(vessel_marker);
    }
    
    viz_gps_path_publisher_->publish(marker_array);
}

void PointCloudAnalyzer::publish_shadow_visualization(const std::vector<std::pair<double, double>>& shadow_cells)
{
    visualization_msgs::msg::MarkerArray marker_array;
    
    for (size_t i = 0; i < shadow_cells.size(); ++i) {
        visualization_msgs::msg::Marker shadow_marker;
        shadow_marker.header.frame_id = "map";
        shadow_marker.header.stamp = this->get_clock()->now();
        shadow_marker.ns = "shadows";
        shadow_marker.id = static_cast<int>(i);
        shadow_marker.type = visualization_msgs::msg::Marker::CUBE;
        shadow_marker.action = visualization_msgs::msg::Marker::ADD;
        
        shadow_marker.pose.position.x = shadow_cells[i].first;
        shadow_marker.pose.position.y = shadow_cells[i].second;
        shadow_marker.pose.position.z = 0.0;
        shadow_marker.pose.orientation.w = 1.0;
        
        shadow_marker.scale.x = config_.grid_cell_size;
        shadow_marker.scale.y = config_.grid_cell_size;
        shadow_marker.scale.z = 1.0;
        
        shadow_marker.color.r = 1.0;
        shadow_marker.color.g = 0.0;
        shadow_marker.color.b = 1.0; // Magenta for shadows
        shadow_marker.color.a = 0.5;
        
        marker_array.markers.push_back(shadow_marker);
    }
    
    viz_shadows_publisher_->publish(marker_array);
}

void PointCloudAnalyzer::analyze_holes_and_shadows()
{
    if (all_leg_points_.empty()) {
        RCLCPP_WARN(this->get_logger(), "No point data for shadow/hole analysis.");
        return;
    }

    // Calculate mean swath width from collected swath statistics
    double mean_swath_width = 0.0;
    if (!swath_lengths_.empty()) {
        mean_swath_width = std::accumulate(swath_lengths_.begin(), swath_lengths_.end(), 0.0) / swath_lengths_.size();
    } else {
        RCLCPP_WARN(this->get_logger(), "No swath width data available for shadow analysis.");
        return;
    }

    // Find bounds in ENU
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

    // Create grid
    int nx = static_cast<int>((max_x - min_x) / config_.grid_cell_size) + 1;
    int ny = static_cast<int>((max_y - min_y) / config_.grid_cell_size) + 1;
    std::vector<std::vector<int>> grid(nx, std::vector<int>(ny, 0));

    // Fill grid
    for (const auto& pt : all_leg_points_) {
        int ix = static_cast<int>((pt.x - min_x) / config_.grid_cell_size);
        int iy = static_cast<int>((pt.y - min_y) / config_.grid_cell_size);
        if (ix >= 0 && ix < nx && iy >= 0 && iy < ny) {
            grid[ix][iy]++;
        }
    }

    // Calculate exclusion zone based on mean swath width
    // Exclude cells within specified fraction of mean swath width from the edges
    double exclusion_distance = mean_swath_width * config_.edge_exclusion_factor;
    int exclusion_cells_y = static_cast<int>(exclusion_distance / config_.grid_cell_size);
    
    // For along-track direction, exclude a smaller margin
    int exclusion_cells_x = static_cast<int>(config_.along_track_exclusion / config_.grid_cell_size);

    RCLCPP_INFO(this->get_logger(), 
        "Shadow analysis: mean_swath_width=%.1fm, exclusion_distance=%.1fm, excluding %d cells from Y edges, %d cells from X edges",
        mean_swath_width, exclusion_distance, exclusion_cells_y, exclusion_cells_x);

    // Analyze grid for shadows, excluding edge areas
    int total_points = all_leg_points_.size();
    int analyzed_cells = 0;
    int excluded_cells = 0;
    
    // Calculate mean density only for the analyzed area
    int points_in_analyzed_area = 0;
    for (int ix = exclusion_cells_x; ix < nx - exclusion_cells_x; ++ix) {
        for (int iy = exclusion_cells_y; iy < ny - exclusion_cells_y; ++iy) {
            points_in_analyzed_area += grid[ix][iy];
            analyzed_cells++;
        }
    }
    
    if (analyzed_cells == 0) {
        RCLCPP_WARN(this->get_logger(), "No cells left to analyze after excluding edges.");
        return;
    }
    
    double mean_density = double(points_in_analyzed_area) / double(analyzed_cells);

    std::vector<std::pair<double, double>> shadow_cells;
    for (int ix = 0; ix < nx; ++ix) {
        for (int iy = 0; iy < ny; ++iy) {
            // Check if cell is in exclusion zone
            bool in_exclusion_zone = (ix < exclusion_cells_x || ix >= nx - exclusion_cells_x ||
                                    iy < exclusion_cells_y || iy >= ny - exclusion_cells_y);
            
            if (in_exclusion_zone) {
                excluded_cells++;
                continue; // Skip edge cells
            }
            
            // Check for shadow/low density
            if (grid[ix][iy] < mean_density * config_.shadow_density_threshold) {
                double cell_x = min_x + ix * config_.grid_cell_size + config_.grid_cell_size/2.0;
                double cell_y = min_y + iy * config_.grid_cell_size + config_.grid_cell_size/2.0;
                shadow_cells.push_back({cell_x, cell_y});
            }
        }
    }

    RCLCPP_INFO(this->get_logger(), 
        "Shadow analysis complete: %d total cells, %d analyzed, %d excluded, %zu shadows detected (mean_density=%.2f)",
        nx * ny, analyzed_cells, excluded_cells, shadow_cells.size(), mean_density);

    // Publish shadow visualization
    publish_shadow_visualization(shadow_cells);
    
    // Publish shadow analysis
    publish_shadow_analysis(shadow_cells, analyzed_cells, mean_density);
}

void PointCloudAnalyzer::reset_leg_data()
{
    swath_lengths_.clear();
    swath_min_depths_.clear();
    swath_max_depths_.clear();
    swath_mean_depths_.clear();
    all_leg_points_.clear();
    // Don't clear visualization data when leg resets - keep accumulated data
    // accumulated_points_.clear(); 
    // gps_path_points_.clear();
}

std::string PointCloudAnalyzer::generate_leg_id()
{
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::ostringstream oss;
    oss << "leg_" << std::put_time(std::localtime(&time_t), "%Y%m%d_%H%M%S")
        << "_" << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}

void PointCloudAnalyzer::publish_shadow_analysis(
    const std::vector<std::pair<double, double>>& shadow_cells,
    int total_cells, 
    double mean_density)
{
    ros_otter_custom_interfaces::msg::ShadowAnalysis shadow_msg;
    
    // Header
    shadow_msg.header.stamp = this->get_clock()->now();
    shadow_msg.header.frame_id = "enu";
    
    // Leg information
    shadow_msg.leg_id = current_leg_id_;
    shadow_msg.total_cells_analyzed = total_cells;
    shadow_msg.shadow_cells_detected = shadow_cells.size();
    shadow_msg.mean_point_density = mean_density;
    
    // Analysis parameters
    shadow_msg.grid_cell_size = config_.grid_cell_size;
    shadow_msg.density_threshold = config_.shadow_density_threshold;
    
    // Convert shadow cells to message format
    shadow_msg.shadow_cells.reserve(shadow_cells.size());
    
    for (size_t i = 0; i < shadow_cells.size(); ++i) {
        ros_otter_custom_interfaces::msg::ShadowCell cell;
        
        // ENU coordinates
        cell.enu_x = shadow_cells[i].first;
        cell.enu_y = shadow_cells[i].second;
        
        // Convert to geographic coordinates
        if (global_proj_initialized_) {
            double lat, lon, h;
            global_proj_.Reverse(cell.enu_x, cell.enu_y, 0.0, lat, lon, h);
            cell.latitude = lat;
            cell.longitude = lon;
        } else {
            cell.latitude = 0.0;
            cell.longitude = 0.0;
            RCLCPP_WARN(this->get_logger(), "Cannot convert shadow cell to lat/lon - projection not initialized");
        }
        
        // Cell properties
        cell.cell_size = config_.grid_cell_size;
        cell.point_count = 0; // We don't track individual cell counts in current implementation
        cell.density_ratio = 0.0; // Below threshold, exact ratio not calculated
        
        // Metadata
        cell.detection_time = shadow_msg.header.stamp;
        cell.leg_id = current_leg_id_;
        
        shadow_msg.shadow_cells.push_back(cell);
    }
    
    shadow_publisher_->publish(shadow_msg);
    
    RCLCPP_INFO(this->get_logger(), 
        "Published shadow analysis for leg %s: %zu shadows detected from %d total cells",
        current_leg_id_.c_str(), shadow_cells.size(), total_cells);
}

void PointCloudAnalyzer::leg_status_callback(const std_msgs::msg::Bool::SharedPtr msg)
{
    if (msg->data && !leg_active_) {
        start_leg();
    } else if (!msg->data && leg_active_) {
        end_leg();
    }
}

void PointCloudAnalyzer::start_leg()
{
    leg_active_ = true;
    current_leg_id_ = generate_leg_id();
    reset_leg_data();
    RCLCPP_INFO(this->get_logger(), "Leg started: %s, collecting swath statistics.", current_leg_id_.c_str());
}

void PointCloudAnalyzer::end_leg()
{
    leg_active_ = false;
    RCLCPP_INFO(this->get_logger(), "Leg ended: %s, analyzing collected swath statistics.", current_leg_id_.c_str());
    publish_stats();
    analyze_holes_and_shadows();
}

std::pair<geometry_msgs::msg::Point, geometry_msgs::msg::Point> 
PointCloudAnalyzer::find_swath_boundaries(const sensor_msgs::msg::PointCloud2::SharedPtr msg)
{
    double min_y = std::numeric_limits<double>::max();
    double max_y = std::numeric_limits<double>::lowest();
    geometry_msgs::msg::Point outer_left, outer_right;

    sensor_msgs::PointCloud2ConstIterator<float> iter_x(*msg, "x");
    sensor_msgs::PointCloud2ConstIterator<float> iter_y(*msg, "y");
    sensor_msgs::PointCloud2ConstIterator<float> iter_z(*msg, "z");

    for (size_t i = 0; i < msg->width * msg->height; ++i, ++iter_x, ++iter_y, ++iter_z) {
        double y = *iter_y;
        
        if (y < min_y) {
            min_y = y;
            outer_left.x = *iter_x;
            outer_left.y = *iter_y;
            outer_left.z = *iter_z;
        }
        if (y > max_y) {
            max_y = y;
            outer_right.x = *iter_x;
            outer_right.y = *iter_y;
            outer_right.z = *iter_z;
        }
    }
    
    return {outer_left, outer_right};
}

std::vector<double> PointCloudAnalyzer::extract_depths(const sensor_msgs::msg::PointCloud2::SharedPtr msg)
{
    std::vector<double> depths;
    depths.reserve(msg->width * msg->height);
    
    sensor_msgs::PointCloud2ConstIterator<float> iter_z(*msg, "z");
    for (size_t i = 0; i < msg->width * msg->height; ++i, ++iter_z) {
        depths.push_back(*iter_z);
    }
    
    return depths;
}

double PointCloudAnalyzer::calculate_swath_length(
    const geometry_msgs::msg::Point& left, 
    const geometry_msgs::msg::Point& right)
{
    return std::sqrt(
        std::pow(left.x - right.x, 2) +
        std::pow(left.y - right.y, 2) +
        std::pow(left.z - right.z, 2)
    );
}

void PointCloudAnalyzer::publish_stats()
{
    if (swath_lengths_.empty()) {
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

} // namespace em_data_analyzer
