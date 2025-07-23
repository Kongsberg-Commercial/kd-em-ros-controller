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

std::vector<std::pair<ppnode_utils::CartesianPoint, ppnode_utils::CartesianPoint>>
SimplePathPlanner::calculateMultiLegPath(const std::vector<ppnode_utils::CartesianPoint>& polygon, size_t max_legs) const {
    if (!validatePolygon(polygon)) {
        throw std::runtime_error("Invalid polygon for multi-leg path calculation");
    }
    
    if (max_legs == 0) {
        throw std::invalid_argument("max_legs must be greater than 0");
    }
    
    std::vector<std::pair<ppnode_utils::CartesianPoint, ppnode_utils::CartesianPoint>> legs;
    
    // Calculate polygon bounds for grid-based survey pattern
    auto [min_x, max_x, min_y, max_y] = calculatePolygonBounds(polygon);
    
    // Generate survey legs based on polygon size and orientation
    double width = max_x - min_x;
    double height = max_y - min_y;
    
    // Determine primary survey direction (along longer axis)
    bool survey_along_x = width >= height;
    
    if (survey_along_x) {
        // Survey with horizontal legs (north-south tracks)
        double leg_spacing = width / (max_legs + 1);
        
        for (size_t i = 0; i < max_legs && i < max_legs; ++i) {
            double x_pos = min_x + leg_spacing * (i + 1);
            
            // Create vertical leg from south to north
            ppnode_utils::CartesianPoint start(x_pos, min_y + height * 0.1);  // 10% margin
            ppnode_utils::CartesianPoint end(x_pos, max_y - height * 0.1);    // 10% margin
            
            // Alternate direction for efficient survey pattern
            if (i % 2 == 1) {
                std::swap(start, end);
            }
            
            legs.emplace_back(start, end);
        }
    } else {
        // Survey with vertical legs (east-west tracks)
        double leg_spacing = height / (max_legs + 1);
        
        for (size_t i = 0; i < max_legs && i < max_legs; ++i) {
            double y_pos = min_y + leg_spacing * (i + 1);
            
            // Create horizontal leg from west to east
            ppnode_utils::CartesianPoint start(min_x + width * 0.1, y_pos);   // 10% margin
            ppnode_utils::CartesianPoint end(max_x - width * 0.1, y_pos);     // 10% margin
            
            // Alternate direction for efficient survey pattern
            if (i % 2 == 1) {
                std::swap(start, end);
            }
            
            legs.emplace_back(start, end);
        }
    }
    
    // If no legs generated, fall back to simple centroid-based leg
    if (legs.empty()) {
        auto simple_path = calculatePath(polygon);
        legs.emplace_back(simple_path.first, simple_path.second);
    }
    
    return legs;
}

std::tuple<double, double, double, double> SimplePathPlanner::calculatePolygonBounds(
    const std::vector<ppnode_utils::CartesianPoint>& polygon) const {
    if (polygon.empty()) {
        return {0.0, 0.0, 0.0, 0.0};
    }
    
    double min_x = polygon[0].x, max_x = polygon[0].x;
    double min_y = polygon[0].y, max_y = polygon[0].y;
    
    for (const auto& point : polygon) {
        min_x = std::min(min_x, point.x);
        max_x = std::max(max_x, point.x);
        min_y = std::min(min_y, point.y);
        max_y = std::max(max_y, point.y);
    }
    
    return {min_x, max_x, min_y, max_y};
}

} // namespace ppnode
