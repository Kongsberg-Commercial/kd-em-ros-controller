#include "ppnode/path_planner.hpp"
#include <stdexcept>
#include <numeric>

namespace ppnode {

bool PathPlanner::validatePolygon(const std::vector<ppnode_utils::CartesianPoint>& polygon) const {
    return polygon.size() >= ppnode_utils::config::MIN_POLYGON_VERTICES;
}

std::pair<ppnode_utils::CartesianPoint, ppnode_utils::CartesianPoint> 
SimplePathPlanner::calculatePath(const std::vector<ppnode_utils::CartesianPoint>& polygon) const {
    if (!validatePolygon(polygon)) {
        throw std::runtime_error("Invalid polygon for path calculation");
    }
    
    // Calculate centroid using more robust approach
    double sum_x = 0.0, sum_y = 0.0;
    for (const auto& point : polygon) {
        sum_x += point.x;
        sum_y += point.y;
    }
    
    const double inv_size = 1.0 / static_cast<double>(polygon.size());
    ppnode_utils::CartesianPoint centroid(sum_x * inv_size, sum_y * inv_size);
    
    // Create second point offset in x direction
    ppnode_utils::CartesianPoint offset_point(centroid.x + offset_distance_, centroid.y);
    
    return std::make_pair(centroid, offset_point);
}

void SimplePathPlanner::setOffsetDistance(double distance) {
    if (distance <= 0.0) {
        throw std::invalid_argument("Offset distance must be positive");
    }
    offset_distance_ = distance;
}

double SimplePathPlanner::getOffsetDistance() const {
    return offset_distance_;
}

ppnode_utils::CartesianPoint SimplePathPlanner::calculateCentroid(
    const std::vector<ppnode_utils::CartesianPoint>& polygon) const {
    if (polygon.empty()) {
        throw std::invalid_argument("Cannot calculate centroid of empty polygon");
    }
    
    double sum_x = 0.0, sum_y = 0.0;
    for (const auto& point : polygon) {
        sum_x += point.x;
        sum_y += point.y;
    }
    
    const double inv_size = 1.0 / static_cast<double>(polygon.size());
    return ppnode_utils::CartesianPoint(sum_x * inv_size, sum_y * inv_size);
}

double SimplePathPlanner::calculatePolygonArea(
    const std::vector<ppnode_utils::CartesianPoint>& polygon) const {
    if (polygon.size() < 3) {
        return 0.0;
    }
    
    double area = 0.0;
    const size_t n = polygon.size();
    
    for (size_t i = 0; i < n; ++i) {
        const size_t j = (i + 1) % n;
        area += polygon[i].x * polygon[j].y;
        area -= polygon[j].x * polygon[i].y;
    }
    
    return std::abs(area) / 2.0;
}

} // namespace ppnode
