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
     * @brief Validate polygon for path planning
     * @param polygon Vector of Cartesian polygon vertices
     * @return true if polygon is valid for path planning
     */
    virtual bool validatePolygon(const std::vector<ppnode_utils::CartesianPoint>& polygon) const {
        return polygon.size() >= ppnode_utils::config::MIN_POLYGON_VERTICES;
    }
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
    calculatePath(const std::vector<ppnode_utils::CartesianPoint>& polygon) const override {
        if (!validatePolygon(polygon)) {
            throw std::runtime_error("Invalid polygon for path calculation");
        }
        
        // Calculate centroid
        double sum_x = 0.0, sum_y = 0.0;
        for (const auto& point : polygon) {
            sum_x += point.x;
            sum_y += point.y;
        }
        
        ppnode_utils::CartesianPoint centroid(sum_x / polygon.size(), sum_y / polygon.size());
        
        // Create second point offset in x direction
        ppnode_utils::CartesianPoint offset_point(centroid.x + offset_distance_, centroid.y);
        
        return std::make_pair(centroid, offset_point);
    }
    
    /**
     * @brief Set the offset distance for the second point
     * @param distance Offset distance in meters
     */
    void setOffsetDistance(double distance) {
        offset_distance_ = distance;
    }
    
    /**
     * @brief Get the current offset distance
     * @return Offset distance in meters
     */
    double getOffsetDistance() const {
        return offset_distance_;
    }
};

} // namespace ppnode

#endif // PPNODE_PATH_PLANNER_HPP
