#ifndef PPNODE_PATH_PLANNER_HPP
#define PPNODE_PATH_PLANNER_HPP

#include "ppnode/utils.hpp"
#include <vector>
#include <utility>

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
    
public:
    explicit SimplePathPlanner(double offset_distance = ppnode_utils::config::PATH_OFFSET_METERS)
        : offset_distance_(offset_distance) {}
    
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
};

} // namespace ppnode

#endif // PPNODE_PATH_PLANNER_HPP
