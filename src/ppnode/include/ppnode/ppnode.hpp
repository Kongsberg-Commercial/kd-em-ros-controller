#ifndef PPNODE__PPNODE_HPP_
#define PPNODE__PPNODE_HPP_

#include "rclcpp/rclcpp.hpp"
#include "ros_otter_custom_interfaces/msg/gps_info.hpp"
#include "ros_otter_custom_interfaces/msg/survey_info.hpp"
#include "ros_otter_custom_interfaces/msg/vertex.hpp"
#include "ros_otter_custom_interfaces/msg/leg.hpp"
#include "ros_otter_custom_interfaces/msg/paths.hpp"
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
 * Publishers:
 * - path_gps (ros_otter_custom_interfaces::msg::Paths): Complete path in GPS coordinates
 * - path_cartesian (ros_otter_custom_interfaces::msg::Paths): Complete path in Cartesian coordinates
 */
class PpNode : public rclcpp::Node
{
private:
    // Subscribers
    rclcpp::Subscription<ros_otter_custom_interfaces::msg::SurveyInfo>::SharedPtr survey_sub_;
    rclcpp::Subscription<ros_otter_custom_interfaces::msg::GpsInfo>::SharedPtr gps_sub_;
    
    // Service server for leg_srv (called by OTTER_TCP_node)
    rclcpp::Service<ros_otter_custom_interfaces::srv::LegMode>::SharedPtr leg_srv_;
    
    // Publishers for complete paths
    rclcpp::Publisher<ros_otter_custom_interfaces::msg::Paths>::SharedPtr path_gps_pub_;
    rclcpp::Publisher<ros_otter_custom_interfaces::msg::Paths>::SharedPtr path_cartesian_pub_;
    
    // Storage for polygon vertices and boat position
    std::vector<ppnode_utils::GpsPoint> polygon_vertices_gps_;
    std::vector<ppnode_utils::CartesianPoint> polygon_vertices_cartesian_;
    ppnode_utils::GpsPoint initial_boat_position_;
    ppnode_utils::GpsPoint current_boat_position_;
    
    // Pre-calculated path storage (lookup table for leg requests)
    std::vector<ros_otter_custom_interfaces::msg::Leg> calculated_legs_;
    static constexpr size_t MAX_LEGS = 5;  // Maximum number of legs in path
    
    // Reference point for coordinate conversion (boat's initial position)
    double ref_lat_;
    double ref_lon_;
    bool ref_point_set_;
    bool boat_position_available_;
    bool survey_received_;
    
    // Path planning algorithm
    std::unique_ptr<PathPlanner> path_planner_;
    
    // Private member functions
    
    /**
     * @brief Callback for boat GPS position updates
     * Tracks the boat's current location and sets the initial position for reference
     * Priority: Uses the first GPS reading as the coordinate reference point
     */
    void boatGpsCallback(const ros_otter_custom_interfaces::msg::GpsInfo::SharedPtr msg);
    
    /**
     * @brief Callback for receiving survey info (polygon vertices + start point)
     * Processes the survey area definition and calculates the coordinate transformation
     * Receives this from the polygon_vertices topic from the backend web app
     * This is the main data input that defines where the boat should survey
     */
    void surveyInfoCallback(const ros_otter_custom_interfaces::msg::SurveyInfo::SharedPtr msg);
    
    /**
     * @brief Parse survey polygon from ROS message format to internal GPS points
     * Converts SurveyInfo Vertex[] message array to vector of GpsPoint
     */
    std::vector<ppnode_utils::GpsPoint> parseSurveyPolygon(
    const std::vector<ros_otter_custom_interfaces::msg::Vertex>& vertices) const;

    /**
     * @brief Set the GPS reference point for coordinate transformations
     * This point becomes the origin (0,0) of the local Cartesian coordinate system
     * Critical for accurate GPS-to-Cartesian conversions in path planning
     */
    void setReferencePoint(const ppnode_utils::GpsPoint& point);
    
    /**
     * @brief Calculate complete path with multiple legs from polygon vertices
     * Uses the configured path planning algorithm to generate up to MAX_LEGS legs
     * Stores results in calculated_legs_ for lookup table access
     */
    void calculateAndStorePath();
    
    /**
     * @brief Process polygon and publish complete path
     * Main processing function that calculates full path and publishes it
     * Called automatically when survey data is received and polygon is valid
     */
    void processPolygonAndPublishPath();
    
    /**
     * @brief Publish the complete calculated path in both coordinate systems
     * Publishes to path_gps and path_cartesian topics for external consumption
     */
    void publishCompletePath();
    
    /**
     * @brief Convert Cartesian leg coordinates to GPS leg coordinates
     * Helper function for coordinate system conversion of leg data
     */
    ros_otter_custom_interfaces::msg::Leg cartesianLegToGpsLeg(
        const ros_otter_custom_interfaces::msg::Leg& cartesian_leg) const;
    
    /**
     * @brief Service callback for boat control system leg requests (lookup table)
     * Called by OTTER_TCP_node when it needs coordinates for a specific leg number
     * Uses pre-calculated legs as lookup table instead of recalculating
     * Returns GPS coordinates in the specific format required by the boat control system
     */
    void legServiceCallback(
        const std::shared_ptr<ros_otter_custom_interfaces::srv::LegMode::Request> request,
        std::shared_ptr<ros_otter_custom_interfaces::srv::LegMode::Response> response);
    
    // Helper functions for path management
    
    /** @brief Check if calculated path is available for leg requests */
    bool hasCalculatedPath() const { 
        return !calculated_legs_.empty(); 
    }
    
    /** @brief Get number of legs in the calculated path */
    size_t getPathLegCount() const {
        return calculated_legs_.size();
    }
    
    
    
    // Helper functions for coordinate calculations
    
    /** @brief Check if we have minimum vertices for polygon processing */
    bool hasMinimumVertices() const { 
        return polygon_vertices_gps_.size() >= ppnode_utils::config::MIN_POLYGON_VERTICES; 
    }
    
    /**
     * @brief Validate GPS coordinates are within reasonable bounds
     * Wrapper for utility function to maintain interface consistency
     */
    bool validateGpsCoordinates(double lat, double lon) const;
    
    /**
     * @brief Validate polygon vertices for path planning
     * Ensures polygon has enough vertices and coordinates are valid
     */
    bool validatePolygonVertices(const std::vector<ppnode_utils::GpsPoint>& vertices) const;
    


public:
    explicit PpNode(const rclcpp::NodeOptions & options = rclcpp::NodeOptions());
    virtual ~PpNode() = default;
    
    /** @brief Set a custom path planning algorithm */
    void setPathPlanner(std::unique_ptr<PathPlanner> planner);
};

}  // namespace ppnode

#endif  // PPNODE__PPNODE_HPP_
