#ifndef PPNODE__PPNODE_HPP_
#define PPNODE__PPNODE_HPP_

#include "rclcpp/rclcpp.hpp"
#include "ros_otter_custom_interfaces/msg/gps_info.hpp"
#include "ros_otter_custom_interfaces/msg/survey_info.hpp"
#include "ros_otter_custom_interfaces/msg/vertex.hpp"
#include "ros_otter_custom_interfaces/srv/leg_mode.hpp"
#include "geometry_msgs/msg/point.hpp"
#include "std_msgs/msg/string.hpp"
#include "ppnode/utils.hpp"
#include "ppnode/path_planner.hpp"
#include <vector>
#include <memory>

namespace ppnode
{

/**
 * @brief Path Planning Node for ROS2
 * 
 * This node acts as a service server for OTTER_TCP_node, providing leg coordinates
 * based on polygon vertices received from external sources (e.g., web app).
 * 
 * Service: leg_srv (ros_otter_custom_interfaces::srv::LegMode)
 * - Receives requests from OTTER_TCP_node
 * - Returns start/end coordinates and speed in LegMode.srv format
 * 
 * Subscribers:
 * - polygon_vertices (ros_otter_custom_interfaces::msg::SurveyInfo): Survey polygon and start point
 * - gps_info (ros_otter_custom_interfaces::msg::GpsInfo): Real-time boat GPS position
 * 
 * Reference Point Strategy:
 * - Prefers boat's initial position (from gps_info) as local coordinate reference
 * - Falls back to survey start point if boat GPS unavailable
 * - Uses this reference for GPS ↔ Cartesian coordinate conversions
 * 
 * Publishers (for debugging/visualization):
 * - result_points_gps: GPS coordinates of calculated points
 * - result_points_cartesian: Cartesian coordinates of calculated points
 */
class PpNode : public rclcpp::Node
{
private:
    // Subscribers
    rclcpp::Subscription<ros_otter_custom_interfaces::msg::SurveyInfo>::SharedPtr survey_sub_;
    rclcpp::Subscription<ros_otter_custom_interfaces::msg::GpsInfo>::SharedPtr gps_sub_;
    
    // Service server for leg_srv (called by OTTER_TCP_node)
    rclcpp::Service<ros_otter_custom_interfaces::srv::LegMode>::SharedPtr leg_srv_;
    
    // Publishers for result points (debugging/monitoring)
    rclcpp::Publisher<ros_otter_custom_interfaces::msg::GpsInfo>::SharedPtr result_gps_pub_;
    rclcpp::Publisher<geometry_msgs::msg::Point>::SharedPtr result_cartesian_pub_;
    
    // Storage for polygon vertices and boat position
    std::vector<ppnode_utils::GpsPoint> polygon_vertices_gps_;
    std::vector<ppnode_utils::CartesianPoint> polygon_vertices_cartesian_;
    ppnode_utils::GpsPoint initial_boat_position_;
    ppnode_utils::GpsPoint current_boat_position_;
    
    // Reference point for coordinate conversion (boat's initial position)
    double ref_lat_;
    double ref_lon_;
    bool ref_point_set_;
    bool boat_position_available_;
    bool survey_received_;
    
    // Path planning algorithm
    std::unique_ptr<PathPlanner> path_planner_;
    
    // Private member functions
    
    /** @brief Callback for receiving survey info (polygon vertices + start point) */
    void surveyInfoCallback(const ros_otter_custom_interfaces::msg::SurveyInfo::SharedPtr msg);
    
    /** @brief Callback for receiving boat GPS position */
    void boatGpsCallback(const ros_otter_custom_interfaces::msg::GpsInfo::SharedPtr msg);
    
    /** @brief Process collected polygon vertices and publish result points */
    void processPolygonAndPublishResults();
    
    /** @brief Publish a single result point in both GPS and Cartesian formats */
    void publishResultPoint(const ppnode_utils::CartesianPoint& cart_point, int point_id);
    
    /** @brief Service callback for OTTER_TCP_node leg requests */
    void legServiceCallback(
        const std::shared_ptr<ros_otter_custom_interfaces::srv::LegMode::Request> request,
        std::shared_ptr<ros_otter_custom_interfaces::srv::LegMode::Response> response);
    
    // Helper functions for coordinate calculations
    
    /** @brief Calculate start and end points from polygon vertices using path planner */
    std::pair<ppnode_utils::CartesianPoint, ppnode_utils::CartesianPoint> calculatePathPoints() const;
    
    /** @brief Check if we have minimum vertices for polygon processing */
    bool hasMinimumVertices() const { 
        return polygon_vertices_gps_.size() >= ppnode_utils::config::MIN_POLYGON_VERTICES; 
    }
    
    /** @brief Set reference point from boat position or survey start point */
    void setReferencePoint(const ppnode_utils::GpsPoint& point);
    
    /** @brief Convert SurveyInfo Vertex array to vector of GpsPoints */
    std::vector<ppnode_utils::GpsPoint> parseSurveyPolygon(
        const std::vector<ros_otter_custom_interfaces::msg::Vertex>& vertices) const;
    
    /** @brief Validate GPS coordinates */
    bool validateGpsCoordinates(double lat, double lon) const;
    
    /** @brief Validate polygon vertices */
    bool validatePolygonVertices(const std::vector<ppnode_utils::GpsPoint>& vertices) const;
    
public:
    explicit PpNode(const rclcpp::NodeOptions & options = rclcpp::NodeOptions());
    virtual ~PpNode() = default;
    
    /** @brief Set a custom path planning algorithm */
    void setPathPlanner(std::unique_ptr<PathPlanner> planner);
};

}  // namespace ppnode

#endif  // PPNODE__PPNODE_HPP_
