#ifndef PPNODE_PATH_PLANNER_HPP
#define PPNODE_PATH_PLANNER_HPP

#include "ppnode/utils.hpp"
#include <rclcpp/rclcpp.hpp>
#include <vector>
#include <utility>
#include <optional>
#include <tuple>

namespace ppnode {

/**
 * @brief Abstract base class for path planning algorithms
 */
class PathPlanner {
public:
    virtual ~PathPlanner() = default;
    
    /**
     * @brief Calculate a path from polygon vertices
     * @param polygon Vector of Cartesian polygon vertices
     * @return Pair of start and end points for the path
     * @throws std::runtime_error if path calculation fails
     */
    virtual std::pair<ppnode_utils::CartesianPoint, ppnode_utils::CartesianPoint> 
    calculatePath(const std::vector<ppnode_utils::CartesianPoint>& polygon) const = 0;
    
    /**
     * @brief Calculate multiple legs from polygon vertices
     * @param polygon Vector of Cartesian polygon vertices
     * @param max_legs Maximum number of legs to generate
     * @return Vector of leg pairs (start, end) in Cartesian coordinates
     * @throws std::runtime_error if path calculation fails
     */
    virtual std::vector<std::pair<ppnode_utils::CartesianPoint, ppnode_utils::CartesianPoint>>
    calculateMultiLegPath(const std::vector<ppnode_utils::CartesianPoint>& polygon, size_t max_legs) const = 0;
    
    /**
     * @brief Validate polygon for path planning
     * @param polygon Vector of Cartesian polygon vertices
     * @return true if polygon is valid for path planning
     */
    virtual bool validatePolygon(const std::vector<ppnode_utils::CartesianPoint>& polygon) const;
};

/**
 * @brief Simple path planner that creates a path from centroid with offset
 */
class SimplePathPlanner : public PathPlanner {
private:
    double offset_distance_;
    rclcpp::Logger logger_;
    
public:
    explicit SimplePathPlanner(double offset_distance = ppnode_utils::config::PATH_OFFSET_METERS)
        : offset_distance_(offset_distance), logger_(rclcpp::get_logger("simple_path_planner")) {}
    
    explicit SimplePathPlanner(const rclcpp::Logger& logger, double offset_distance = ppnode_utils::config::PATH_OFFSET_METERS)
        : offset_distance_(offset_distance), logger_(logger) {}
    
    /**
     * @brief Calculate path using centroid + offset method
     * @param polygon Vector of Cartesian polygon vertices
     * @return Pair of centroid and offset point
     * @throws std::runtime_error if polygon is invalid
     */
    std::pair<ppnode_utils::CartesianPoint, ppnode_utils::CartesianPoint> 
    calculatePath(const std::vector<ppnode_utils::CartesianPoint>& polygon) const override;
    
    /**
     * @brief Calculate multiple legs using grid-based survey pattern
     * @param polygon Vector of Cartesian polygon vertices
     * @param max_legs Maximum number of legs to generate
     * @return Vector of leg pairs in Cartesian coordinates
     * @throws std::runtime_error if polygon is invalid
     */
    std::vector<std::pair<ppnode_utils::CartesianPoint, ppnode_utils::CartesianPoint>>
    calculateMultiLegPath(const std::vector<ppnode_utils::CartesianPoint>& polygon, size_t max_legs) const override;
    
    /**
     * @brief Set the offset distance for the second point
     * @param distance Offset distance in meters
     * @throws std::invalid_argument if distance is not positive
     */
    void setOffsetDistance(double distance);
    
    /**
     * @brief Get the current offset distance
     * @return Offset distance in meters
     */
    double getOffsetDistance() const;

private:
    /**
     * @brief Calculate centroid of polygon
     * @param polygon Vector of Cartesian polygon vertices
     * @return Centroid point
     * @throws std::invalid_argument if polygon is empty
     */
    ppnode_utils::CartesianPoint calculateCentroid(
        const std::vector<ppnode_utils::CartesianPoint>& polygon) const;
    
    /**
     * @brief Calculate area of polygon using shoelace formula
     * @param polygon Vector of Cartesian polygon vertices
     * @return Area in square meters
     */
    double calculatePolygonArea(
        const std::vector<ppnode_utils::CartesianPoint>& polygon) const;
    
    /**
     * @brief Calculate bounding box of polygon
     * @param polygon Vector of Cartesian polygon vertices
     * @return Tuple of (min_x, max_x, min_y, max_y)
     */
    std::tuple<double, double, double, double> calculatePolygonBounds(
        const std::vector<ppnode_utils::CartesianPoint>& polygon) const;
        
    // Helper functions for lawnmower pattern algorithm
    
    /**
     * @brief Find the index of the nearest vertex to a given point
     * @param polygon Vector of polygon vertices
     * @param point Reference point
     * @return Index of nearest vertex
     */
    size_t findNearestVertexIndex(const std::vector<ppnode_utils::CartesianPoint>& polygon, 
                                  const ppnode_utils::CartesianPoint& point) const;
    
    /**
     * @brief Calculate Euclidean distance between two points
     * @param p1 First point
     * @param p2 Second point
     * @return Distance in meters
     */
    double calculateDistance(const ppnode_utils::CartesianPoint& p1, 
                            const ppnode_utils::CartesianPoint& p2) const;
    
    /**
     * @brief Calculate vector from one point to another
     * @param from Starting point
     * @param to Ending point
     * @return Vector as CartesianPoint
     */
    ppnode_utils::CartesianPoint vectorFromTo(const ppnode_utils::CartesianPoint& from, 
                                             const ppnode_utils::CartesianPoint& to) const;
    
    /**
     * @brief Normalize a vector to unit length
     * @param vector Input vector
     * @return Normalized vector
     */
    ppnode_utils::CartesianPoint normalizeVector(const ppnode_utils::CartesianPoint& vector) const;
    
    /**
     * @brief Move along polygon boundary by a specified distance
     * @param polygon Vector of polygon vertices
     * @param start_point Starting position on boundary
     * @param distance Distance to move in meters
     * @param forward_direction True for CCW, false for CW
     * @return New position or nullopt if can't move that distance
     */
    std::optional<ppnode_utils::CartesianPoint> moveAlongBoundary(
        const std::vector<ppnode_utils::CartesianPoint>& polygon, 
        const ppnode_utils::CartesianPoint& start_point,
        double distance, 
        bool forward_direction) const;
    
    /**
     * @brief Calculate distance from point to line segment
     * @param point Point to measure from
     * @param line_start Start of line segment
     * @param line_end End of line segment
     * @return Distance to closest point on line segment
     */
    double distanceToLineSegment(const ppnode_utils::CartesianPoint& point,
                                const ppnode_utils::CartesianPoint& line_start,
                                const ppnode_utils::CartesianPoint& line_end) const;
    
    /**
     * @brief Find intersection of line with polygon boundary
     * @param polygon Vector of polygon vertices
     * @param start_point Starting point of line
     * @param direction Normalized direction vector
     * @return Intersection point or nullopt if no intersection
     */
    std::optional<ppnode_utils::CartesianPoint> findPolygonIntersection(
        const std::vector<ppnode_utils::CartesianPoint>& polygon,
        const ppnode_utils::CartesianPoint& start_point,
        const ppnode_utils::CartesianPoint& direction) const;
    
    /**
     * @brief Find intersection between two line segments
     * @param p1 Start of first line segment
     * @param p2 End of first line segment
     * @param p3 Start of second line segment
     * @param p4 End of second line segment
     * @return Intersection point or nullopt if no intersection
     */
    std::optional<ppnode_utils::CartesianPoint> lineSegmentIntersection(
        const ppnode_utils::CartesianPoint& p1, const ppnode_utils::CartesianPoint& p2,
        const ppnode_utils::CartesianPoint& p3, const ppnode_utils::CartesianPoint& p4) const;
};

} // namespace ppnode

#endif // PPNODE_PATH_PLANNER_HPP
