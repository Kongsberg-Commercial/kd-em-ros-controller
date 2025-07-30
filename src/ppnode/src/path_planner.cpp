#include "ppnode/path_planner.hpp"
#include <rclcpp/rclcpp.hpp>
#include <stdexcept>
#include <numeric>
#include <cmath>
#include <algorithm>

namespace ppnode {

// Configuration constants for lawnmower pattern
static constexpr double DEFAULT_SWATH_WIDTH_M = 50.0;
static constexpr double DEFAULT_OVERLAP_PERCENT = 0.0;

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

    // Step 1: Find nearest vertex to start position (using first polygon vertex as reference)
    ppnode_utils::CartesianPoint start_reference = polygon[0];  // Default to first vertex
    size_t nearest_vertex_idx = findNearestVertexIndex(polygon, start_reference);
    RCLCPP_WARN(logger_, "Nearest vertex index: %zu", nearest_vertex_idx);

    // Step 2: Determine direction - find closest neighbor (previous or next)
    size_t prev_idx = (nearest_vertex_idx == 0) ? polygon.size() - 1 : nearest_vertex_idx - 1;
    size_t next_idx = (nearest_vertex_idx + 1) % polygon.size();

    double dist_to_prev = calculateDistance(polygon[nearest_vertex_idx], polygon[prev_idx]);
    double dist_to_next = calculateDistance(polygon[nearest_vertex_idx], polygon[next_idx]);
    RCLCPP_WARN(logger_, "Distance to previous: %.2f, Distance to next: %.2f", dist_to_prev, dist_to_next);

    // Choose direction and establish traversal order
    bool traverse_forward = (dist_to_next <= dist_to_prev);  // true = CCW, false = CW
    size_t target_vertex_idx = traverse_forward ? next_idx : prev_idx;
    RCLCPP_WARN(logger_, "Traversal direction: %s", traverse_forward ? "Forward" : "Backward");

    // Step 3: Create first leg (nearest vertex to closest neighbor)
    ppnode_utils::CartesianPoint leg1_start = polygon[nearest_vertex_idx];
    ppnode_utils::CartesianPoint leg1_end = polygon[target_vertex_idx];
    legs.emplace_back(leg1_start, leg1_end);
    RCLCPP_WARN(logger_, "First leg: Start(%.2f, %.2f) -> End(%.2f, %.2f)", 
                 leg1_start.x, leg1_start.y, leg1_end.x, leg1_end.y);

    // Step 4: Calculate swath width with overlap
    double effective_swath_width = DEFAULT_SWATH_WIDTH_M * (1.0 - DEFAULT_OVERLAP_PERCENT / 100.0);
    RCLCPP_WARN(logger_, "Effective swath width: %.2f", effective_swath_width);

    // Step 5: Pre-calculate direction vectors (outside loop for efficiency)
    ppnode_utils::CartesianPoint leg1_direction = normalizeVector(vectorFromTo(leg1_start, leg1_end));
    ppnode_utils::CartesianPoint leg1_reverse_direction = normalizeVector(vectorFromTo(leg1_end, leg1_start));

    // Step 6: Generate subsequent legs with proper lawnmower pattern
    ppnode_utils::CartesianPoint current_leg_end = leg1_end;
    bool even_direction = true;  // Start with leg 2 (first even-numbered leg)

    for (size_t leg_num = 1; leg_num < max_legs; ++leg_num) {
        RCLCPP_WARN(logger_, "Generating leg %zu", leg_num + 1);

        // Find the direction to move along polygon boundary from current leg end
        size_t current_vertex_idx = findNearestVertexIndex(polygon, current_leg_end);
        RCLCPP_WARN(logger_, "Current vertex index: %zu", current_vertex_idx); // DOES THIS EXCLUDE ITSELF? IF SAY THE POINT IT AT A VERTEX TO BEGIN WITH?

        // Determine which neighboring vertex to move toward (continue in traverse_forward direction)
        size_t next_boundary_vertex_idx;
        if (traverse_forward) {
            next_boundary_vertex_idx = (current_vertex_idx + 1) % polygon.size();
        } else {
            next_boundary_vertex_idx = (current_vertex_idx == 0) ? polygon.size() - 1 : current_vertex_idx - 1;
        }
        RCLCPP_WARN(logger_, "Next boundary vertex index: %zu", next_boundary_vertex_idx);

        // Calculate direction along polygon boundary
        ppnode_utils::CartesianPoint boundary_direction = normalizeVector(
            vectorFromTo(polygon[current_vertex_idx], polygon[next_boundary_vertex_idx])
        );
        // NOT SURE ABOUT THAT TRAVERSE_FORWARD THINGY ... HAS TO BE SOME SMARTER AND EASIER WAY TO FIND NEXT BOUNDARY VERTEX IDX
        RCLCPP_WARN(logger_, "Boundary direction: (%.2f, %.2f)", boundary_direction.x, boundary_direction.y);

        // Move along boundary by swath width to get next leg start
        ppnode_utils::CartesianPoint next_start(
            current_leg_end.x + boundary_direction.x * effective_swath_width,
            current_leg_end.y + boundary_direction.y * effective_swath_width
        );
        RCLCPP_WARN(logger_, "Next start point: (%.2f, %.2f)", next_start.x, next_start.y);

        // Calculate leg direction (alternate between forward and reverse)
        ppnode_utils::CartesianPoint leg_direction;
        if (even_direction) {
            leg_direction = leg1_reverse_direction; // Even legs (2,4,6...) use reverse direction
        } else {
            leg_direction = leg1_direction;         // Odd legs (3,5,7...) use original direction
        }
        
        RCLCPP_WARN(logger_, "Leg direction: (%.2f, %.2f)", leg_direction.x, leg_direction.y);
        RCLCPP_WARN(logger_, "original leg direction: (%.2f, %.2f)", leg1_direction.x, leg1_direction.y);
        RCLCPP_WARN(logger_, "reverse leg direction: (%.2f, %.2f)", leg1_reverse_direction.x, leg1_reverse_direction.y);
        


        // Find intersection with polygon boundary
        auto intersection = findPolygonIntersection(polygon, next_start, leg_direction);
        if (intersection.has_value()) {
            legs.emplace_back(next_start, intersection.value());
            current_leg_end = intersection.value();
            RCLCPP_WARN(logger_, "Leg %zu: Start(%.2f, %.2f) -> End(%.2f, %.2f)", 
                         leg_num + 1, next_start.x, next_start.y, current_leg_end.x, current_leg_end.y);
        } else {
            RCLCPP_WARN(logger_, "No valid intersection found for leg %zu", leg_num + 1);
            break;  // No valid intersection found
        }

        even_direction = !even_direction;  // Alternate direction for next leg
    }

    // Fallback: if no legs generated, create simple centroid-based leg
    if (legs.empty()) {
        auto simple_path = calculatePath(polygon);
        legs.emplace_back(simple_path.first, simple_path.second);
        RCLCPP_WARN(logger_, "Fallback: Created simple path: Start(%.2f, %.2f) -> End(%.2f, %.2f)", 
                     simple_path.first.x, simple_path.first.y, simple_path.second.x, simple_path.second.y);
    }

    return legs;
}

// Helper functions for lawnmower pattern algorithm

size_t SimplePathPlanner::findNearestVertexIndex(const std::vector<ppnode_utils::CartesianPoint>& polygon, 
                                                 const ppnode_utils::CartesianPoint& point) const {
    if (polygon.empty()) {
        throw std::runtime_error("Cannot find nearest vertex in empty polygon");
    }
    
    size_t nearest_idx = 0;
    double min_distance = calculateDistance(point, polygon[0]);
    
    for (size_t i = 1; i < polygon.size(); ++i) {
        double distance = calculateDistance(point, polygon[i]);
        if (distance < min_distance) {
            min_distance = distance;
            nearest_idx = i;
        }
    }
    
    return nearest_idx;
}

double SimplePathPlanner::calculateDistance(const ppnode_utils::CartesianPoint& p1, 
                                          const ppnode_utils::CartesianPoint& p2) const {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    return std::sqrt(dx * dx + dy * dy);
}

ppnode_utils::CartesianPoint SimplePathPlanner::vectorFromTo(const ppnode_utils::CartesianPoint& from, 
                                                           const ppnode_utils::CartesianPoint& to) const {
    return ppnode_utils::CartesianPoint(to.x - from.x, to.y - from.y);
}

ppnode_utils::CartesianPoint SimplePathPlanner::normalizeVector(const ppnode_utils::CartesianPoint& vector) const {
    double length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
    if (length < 1e-10) {
        return ppnode_utils::CartesianPoint(1.0, 0.0);  // Default to unit x vector
    }
    return ppnode_utils::CartesianPoint(vector.x / length, vector.y / length);
}

std::optional<ppnode_utils::CartesianPoint> SimplePathPlanner::moveAlongBoundary(
    const std::vector<ppnode_utils::CartesianPoint>& polygon, 
    const ppnode_utils::CartesianPoint& start_point,
    double distance, 
    bool forward_direction) const {
    
    // Find the closest edge to the start point
    size_t closest_edge_idx = 0;
    double min_distance_to_edge = std::numeric_limits<double>::max();
    
    for (size_t i = 0; i < polygon.size(); ++i) {
        size_t next_i = (i + 1) % polygon.size();
        double dist = distanceToLineSegment(start_point, polygon[i], polygon[next_i]);
        
        if (dist < min_distance_to_edge) {
            min_distance_to_edge = dist;
            closest_edge_idx = i;
        }
    }
    
    // Move along the boundary from the closest edge
    size_t current_vertex = closest_edge_idx;
    double remaining_distance = distance;
    
    // Traverse the polygon boundary
    for (size_t step = 0; step < polygon.size() && remaining_distance > 0; ++step) {
        size_t next_vertex = forward_direction ? 
            (current_vertex + 1) % polygon.size() : 
            (current_vertex == 0 ? polygon.size() - 1 : current_vertex - 1);
        
        double edge_length = calculateDistance(polygon[current_vertex], polygon[next_vertex]);
        
        if (remaining_distance <= edge_length) {
            // Final position is on this edge
            double ratio = remaining_distance / edge_length;
            return ppnode_utils::CartesianPoint(
                polygon[current_vertex].x + ratio * (polygon[next_vertex].x - polygon[current_vertex].x),
                polygon[current_vertex].y + ratio * (polygon[next_vertex].y - polygon[current_vertex].y)
            );
        }
        
        remaining_distance -= edge_length;
        current_vertex = next_vertex;
    }
    
    return std::nullopt;  // Could not move the full distance
}

double SimplePathPlanner::distanceToLineSegment(const ppnode_utils::CartesianPoint& point,
                                              const ppnode_utils::CartesianPoint& line_start,
                                              const ppnode_utils::CartesianPoint& line_end) const {
    double line_length_sq = std::pow(line_end.x - line_start.x, 2) + std::pow(line_end.y - line_start.y, 2);
    
    if (line_length_sq < 1e-10) {
        // Line segment is a point
        return calculateDistance(point, line_start);
    }
    
    // Calculate projection parameter
    double t = ((point.x - line_start.x) * (line_end.x - line_start.x) + 
                (point.y - line_start.y) * (line_end.y - line_start.y)) / line_length_sq;
    
    t = std::max(0.0, std::min(1.0, t));  // Clamp to line segment
    
    // Find closest point on line segment
    ppnode_utils::CartesianPoint closest(
        line_start.x + t * (line_end.x - line_start.x),
        line_start.y + t * (line_end.y - line_start.y)
    );
    
    return calculateDistance(point, closest);
}

std::optional<ppnode_utils::CartesianPoint> SimplePathPlanner::findPolygonIntersection(
    const std::vector<ppnode_utils::CartesianPoint>& polygon,
    const ppnode_utils::CartesianPoint& start_point,
    const ppnode_utils::CartesianPoint& direction) const {
    
    // Calculate line length (use bounding box approach)
    auto [min_x, max_x, min_y, max_y] = calculatePolygonBounds(polygon);
    double max_dimension = std::max(max_x - min_x, max_y - min_y);
    double line_length = 2.0 * max_dimension;
    
    // Create end point of line segment
    ppnode_utils::CartesianPoint end_point(
        start_point.x + direction.x * line_length,
        start_point.y + direction.y * line_length
    );
    
    // Find intersections with all polygon edges
    std::vector<std::pair<ppnode_utils::CartesianPoint, double>> intersections;
    
    RCLCPP_WARN(logger_, "Checking intersections for line from (%.2f, %.2f) in direction (%.2f, %.2f)", 
               start_point.x, start_point.y, direction.x, direction.y);
    RCLCPP_WARN(logger_, "Line extends to (%.2f, %.2f)", end_point.x, end_point.y);
    
    for (size_t i = 0; i < polygon.size(); ++i) {
        size_t next_i = (i + 1) % polygon.size();
        
        RCLCPP_WARN(logger_, "Edge %zu: from (%.2f, %.2f) to (%.2f, %.2f)", 
                   i, polygon[i].x, polygon[i].y, polygon[next_i].x, polygon[next_i].y);
        
        auto intersection = lineSegmentIntersection(start_point, end_point, polygon[i], polygon[next_i]);
        if (intersection.has_value()) {
            double distance = calculateDistance(start_point, intersection.value());
            RCLCPP_WARN(logger_, "Edge %zu: intersection at (%.2f, %.2f), distance: %.6f", 
                       i, intersection.value().x, intersection.value().y, distance);
            if (distance > 1.0) {  // Use 1.0 meter as minimum meaningful distance instead of 1e-6
                intersections.emplace_back(intersection.value(), distance);
                RCLCPP_WARN(logger_, "Edge %zu: intersection ACCEPTED (distance > 1.0m)", i);
            } else {
                RCLCPP_WARN(logger_, "Edge %zu: intersection REJECTED (distance <= 1.0m, likely start point)", i);
            }
        } else {
            RCLCPP_WARN(logger_, "Edge %zu: NO intersection found", i);
        }
    }
    
    // Return the closest intersection
    if (!intersections.empty()) {
        auto closest = std::min_element(intersections.begin(), intersections.end(),
            [](const auto& a, const auto& b) { return a.second < b.second; });
        RCLCPP_WARN(logger_, "Returning closest intersection at (%.2f, %.2f) with distance %.6f", 
                   closest->first.x, closest->first.y, closest->second);
        return closest->first;
    }
    
    RCLCPP_WARN(logger_, "No valid intersections found (all were too close to start point)");
    return std::nullopt;
}

std::optional<ppnode_utils::CartesianPoint> SimplePathPlanner::lineSegmentIntersection(
    const ppnode_utils::CartesianPoint& p1, const ppnode_utils::CartesianPoint& p2,
    const ppnode_utils::CartesianPoint& p3, const ppnode_utils::CartesianPoint& p4) const {
    
    double denom = (p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x);
    
    if (std::abs(denom) < 1e-10) {
        return std::nullopt;  // Lines are parallel
    }
    
    double t = ((p1.x - p3.x) * (p3.y - p4.y) - (p1.y - p3.y) * (p3.x - p4.x)) / denom;
    double u = -((p1.x - p2.x) * (p1.y - p3.y) - (p1.y - p2.y) * (p1.x - p3.x)) / denom;
    
    if (t >= 0.0 && t <= 1.0 && u >= 0.0 && u <= 1.0) {
        // Lines intersect
        RCLCPP_WARN(logger_, "Line segments intersect at (%.2f, %.2f)", 
                     p1.x + t * (p2.x - p1.x), p1.y + t * (p2.y - p1.y));
        return ppnode_utils::CartesianPoint(
            p1.x + t * (p2.x - p1.x),
            p1.y + t * (p2.y - p1.y)
        );
    }
    
    return std::nullopt;  // No intersection within line segments
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
