#pragma once
#include <string>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <limits>
#include <vector>
#include <iostream>

// Enable debug output temporarily
#define DEBUG_INTERSECTION

// Structure to represent a lat/lon point
struct LatLonPoint {
    double lat;
    double lon;
    
    LatLonPoint(double latitude = 0.0, double longitude = 0.0) 
        : lat(latitude), lon(longitude) {}
};

// Structure to represent a point in local Cartesian coordinates (meters)
struct CartesianPoint {
    double x;
    double y;
    
    CartesianPoint(double x_coord = 0.0, double y_coord = 0.0) 
        : x(x_coord), y(y_coord) {}
};

// Helper to convert decimal degrees to ddmm.mm (NMEA format)
inline std::string decDegToNmeaLat(double latitude) {
    char dir;
    if (latitude >= 0) {
        dir = 'N';
    } else {
        dir = 'S';
    }
    latitude = std::fabs(latitude); // cmath func: floating-point absolute value
    int deg = static_cast<int>(latitude); // extract degrees
    double min = (latitude - deg) * 60.0; // calculate the minutes from degree scaled by 60

    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << deg
        << std::fixed << std::setprecision(3)
        << std::setw(6) << min;
    return oss.str() + "," + dir;
}

inline std::string decDegToNmeaLon(double longitude) {
    char dir;
    if (longitude >= 0) {
        dir = 'E';
    } else {
        dir = 'W';
    }
    longitude = std::fabs(longitude); // cmath func: floating-point absolute value
    int deg = static_cast<int>(longitude); // extract degrees
    double min = (longitude - deg) * 60.0; // calculate the minutes from degree scaled by 60

    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(3) << deg
        << std::fixed << std::setprecision(3)
        << std::setw(6) << min;
    return oss.str() + "," + dir;
}

// Coordinate transformation functions for accurate intersection calculations
// Convert lat/lon to local Cartesian coordinates (East-North-Up system)
// Uses simple equirectangular projection for small areas
// reference_point: The origin point for the local coordinate system
inline CartesianPoint latLonToCartesian(const LatLonPoint& point, const LatLonPoint& reference_point) {
    const double R = 6371000.0; // Earth's radius in meters
    
    // Convert degrees to radians
    double lat_rad = point.lat * M_PI / 180.0;
    double lon_rad = point.lon * M_PI / 180.0;
    double ref_lat_rad = reference_point.lat * M_PI / 180.0;
    double ref_lon_rad = reference_point.lon * M_PI / 180.0;
    
    // Calculate differences
    double dlat = lat_rad - ref_lat_rad;
    double dlon = lon_rad - ref_lon_rad;
    
    // Convert to Cartesian coordinates
    double x = R * dlon * std::cos(ref_lat_rad);  // Eastward
    double y = R * dlat;                          // Northward
    
    return CartesianPoint(x, y);
}

// Convert local Cartesian coordinates back to lat/lon
inline LatLonPoint cartesianToLatLon(const CartesianPoint& point, const LatLonPoint& reference_point) {
    const double R = 6371000.0; // Earth's radius in meters
    
    // Convert reference point to radians
    double ref_lat_rad = reference_point.lat * M_PI / 180.0;
    double ref_lon_rad = reference_point.lon * M_PI / 180.0;
    
    // Calculate lat/lon differences
    double dlat = point.y / R;
    double dlon = point.x / (R * std::cos(ref_lat_rad));
    
    // Convert back to degrees
    double lat = (ref_lat_rad + dlat) * 180.0 / M_PI;
    double lon = (ref_lon_rad + dlon) * 180.0 / M_PI;
    
    return LatLonPoint(lat, lon);
}

// Compute NMEA 0183 checksum (returns two uppercase hex digits)
inline std::string nmeaChecksum(const std::string& nmea_no_dollars, bool include_star = false) {
    uint8_t checksum = 0;
    for (const auto& c : nmea_no_dollars) {
        checksum ^= static_cast<uint8_t>(c);
    }
    std::ostringstream oss;
    if (include_star)
        oss << "*";
    oss << std::uppercase << std::hex << std::setfill('0') << std::setw(2) << int(checksum);
    return oss.str();
}

// Helper function to calculate distance between two lat/lon points (in meters)
// Uses Haversine formula for accurate distance calculation on sphere
inline double calculate_distance(const LatLonPoint& p1, const LatLonPoint& p2) {
    const double R = 6371000.0; // Earth's radius in meters
    double lat1_rad = p1.lat * M_PI / 180.0;
    double lat2_rad = p2.lat * M_PI / 180.0;
    double dlat_rad = (p2.lat - p1.lat) * M_PI / 180.0;
    double dlon_rad = (p2.lon - p1.lon) * M_PI / 180.0;
    
    double a = std::sin(dlat_rad/2) * std::sin(dlat_rad/2) +
               std::cos(lat1_rad) * std::cos(lat2_rad) *
               std::sin(dlon_rad/2) * std::sin(dlon_rad/2);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1-a));
    
    return R * c;
}

// Helper function to calculate bearing between two points (in degrees)
// Returns bearing from 0-360 degrees (0 = North, 90 = East, etc.)
inline double calculate_bearing(const LatLonPoint& from, const LatLonPoint& to) {
    double lat1_rad = from.lat * M_PI / 180.0;
    double lat2_rad = to.lat * M_PI / 180.0;
    double dlon_rad = (to.lon - from.lon) * M_PI / 180.0;
    
    double y = std::sin(dlon_rad) * std::cos(lat2_rad);
    double x = std::cos(lat1_rad) * std::sin(lat2_rad) -
               std::sin(lat1_rad) * std::cos(lat2_rad) * std::cos(dlon_rad);
    
    double bearing_rad = std::atan2(y, x);
    return std::fmod(bearing_rad * 180.0 / M_PI + 360.0, 360.0);
}

// Helper function to calculate a point at distance and bearing from origin
// distance_m: distance in meters, bearing_deg: bearing in degrees (0-360)
inline LatLonPoint calculate_point_at_distance_bearing(const LatLonPoint& origin, double distance_m, double bearing_deg) {
    const double R = 6371000.0; // Earth's radius in meters
    double lat1_rad = origin.lat * M_PI / 180.0;
    double lon1_rad = origin.lon * M_PI / 180.0;
    double bearing_rad = bearing_deg * M_PI / 180.0;
    
    double lat2_rad = std::asin(std::sin(lat1_rad) * std::cos(distance_m / R) +
                               std::cos(lat1_rad) * std::sin(distance_m / R) * std::cos(bearing_rad));
    double lon2_rad = lon1_rad + std::atan2(std::sin(bearing_rad) * std::sin(distance_m / R) * std::cos(lat1_rad),
                                           std::cos(distance_m / R) - std::sin(lat1_rad) * std::sin(lat2_rad));
    
    return LatLonPoint(lat2_rad * 180.0 / M_PI, lon2_rad * 180.0 / M_PI);
}

/*  Helper function to find intersection between a ray and a line segment
    Returns true if intersection exists, false otherwise
        ray_origin: starting point of the ray
        bearing_degrees: bearing direction of the ray in degrees
        seg_start, seg_end: endpoints of the line segment
        intersection: output parameter for the intersection point
        t: output parameter for the distance along the ray (optional) */
inline bool findRaySegmentIntersection(const LatLonPoint& ray_origin, double bearing_degrees,
                                       const LatLonPoint& seg_start, const LatLonPoint& seg_end,
                                       LatLonPoint& intersection, double* t = nullptr) {
    // Use the ray origin as the reference point for coordinate transformation
    LatLonPoint reference_point = ray_origin;
    
    // Transform all points to local Cartesian coordinates
    CartesianPoint ray_origin_cart = latLonToCartesian(ray_origin, reference_point);
    CartesianPoint seg_start_cart = latLonToCartesian(seg_start, reference_point);
    CartesianPoint seg_end_cart = latLonToCartesian(seg_end, reference_point);
    
    // Calculate ray direction vector in Cartesian space from bearing
    double bearing_rad = bearing_degrees * M_PI / 180.0;
    double ray_dx = std::sin(bearing_rad);  // East component
    double ray_dy = std::cos(bearing_rad);  // North component
    
    // Calculate segment direction vector
    double seg_dx = seg_end_cart.x - seg_start_cart.x;
    double seg_dy = seg_end_cart.y - seg_start_cart.y;
    
    #ifdef DEBUG_INTERSECTION
    std::cout << "Intersection calculation:" << std::endl;
    std::cout << "  Ray origin (lat/lon): (" << ray_origin.lat << ", " << ray_origin.lon << ")" << std::endl;
    std::cout << "  Ray origin (cart): (" << ray_origin_cart.x << ", " << ray_origin_cart.y << ")" << std::endl;
    std::cout << "  Ray bearing: " << bearing_degrees << " degrees" << std::endl;
    std::cout << "  Ray direction: (" << ray_dx << ", " << ray_dy << ")" << std::endl;
    std::cout << "  Segment start (lat/lon): (" << seg_start.lat << ", " << seg_start.lon << ")" << std::endl;
    std::cout << "  Segment end (lat/lon): (" << seg_end.lat << ", " << seg_end.lon << ")" << std::endl;
    std::cout << "  Segment start (cart): (" << seg_start_cart.x << ", " << seg_start_cart.y << ")" << std::endl;
    std::cout << "  Segment end (cart): (" << seg_end_cart.x << ", " << seg_end_cart.y << ")" << std::endl;
    std::cout << "  Segment direction: (" << seg_dx << ", " << seg_dy << ")" << std::endl;
    #endif
    
    // Using cross product approach to handle edge cases better
    double cross_product = ray_dx * seg_dy - ray_dy * seg_dx;
    
    if (std::abs(cross_product) < 1e-12) {
        // Lines are parallel or collinear
        #ifdef DEBUG_INTERSECTION
        std::cout << "PARALLEL LINES: cross_product=" << cross_product << std::endl;
        #endif
        return false;
    }
    
    // Calculate intersection parameters
    double dx = seg_start_cart.x - ray_origin_cart.x;
    double dy = seg_start_cart.y - ray_origin_cart.y;
    
    double t_ray = (dx * seg_dy - dy * seg_dx) / cross_product;
    double s_segment = (dx * ray_dy - dy * ray_dx) / cross_product;
    
    #ifdef DEBUG_INTERSECTION
    std::cout << "  t_ray: " << t_ray << ", s_segment: " << s_segment << std::endl;
    std::cout << "  cross_product: " << cross_product << std::endl;
    std::cout << "  dx: " << dx << ", dy: " << dy << std::endl;
    #endif
    
    // Check if intersection is valid (on ray and on segment)
    // Use more relaxed tolerances to handle floating point precision issues
    const double EPSILON = 1e-8;
    if (t_ray >= -EPSILON && s_segment >= -EPSILON && s_segment <= 1.0 + EPSILON) {
        // Calculate intersection point in Cartesian space
        CartesianPoint intersection_cart;
        intersection_cart.x = ray_origin_cart.x + t_ray * ray_dx;
        intersection_cart.y = ray_origin_cart.y + t_ray * ray_dy;
        
        // Convert back to lat/lon
        intersection = cartesianToLatLon(intersection_cart, reference_point);
        
        #ifdef DEBUG_INTERSECTION
        std::cout << "INTERSECTION FOUND: t=" << t_ray << ", s=" << s_segment << std::endl;
        std::cout << "  Ray bearing: " << bearing_degrees << " degrees" << std::endl;
        std::cout << "  Ray direction: (" << ray_dx << ", " << ray_dy << ")" << std::endl;
        std::cout << "  Cartesian intersection: (" << intersection_cart.x << ", " << intersection_cart.y << ")" << std::endl;
        std::cout << "  Lat/Lon intersection: (" << intersection.lat << ", " << intersection.lon << ")" << std::endl;
        #endif
        
        if (t) *t = t_ray;
        return true;
    }
    
    #ifdef DEBUG_INTERSECTION
    std::cout << "NO INTERSECTION: t=" << t_ray << ", s=" << s_segment << " (need t>=0 and 0<=s<=1)" << std::endl;
    std::cout << "  Ray bearing: " << bearing_degrees << " degrees" << std::endl;
    std::cout << "  Ray direction: (" << ray_dx << ", " << ray_dy << ")" << std::endl;
    #endif
    
    return false;
}

/*  Helper function to find the nearest intersection of a ray with a polygon
    Returns the nearest intersection point along the ray direction
    If no intersection is found, returns the ray_origin */
inline LatLonPoint findNearestPolygonIntersection(const std::vector<LatLonPoint>& polygon, 
                                                 const LatLonPoint& ray_origin, 
                                                 double bearing_degrees) {
    if (polygon.empty()) {
        return ray_origin;
    }
    
    LatLonPoint nearest_intersection = ray_origin;
    double min_distance = std::numeric_limits<double>::max();
    bool found_intersection = false;
    int intersected_edge = -1;
    
    // Debug output
    #ifdef DEBUG_INTERSECTION
    std::cout << "Finding intersection from origin (" << ray_origin.lat << ", " << ray_origin.lon 
              << ") with bearing " << bearing_degrees << " degrees" << std::endl;
    std::cout << "Polygon has " << polygon.size() << " vertices" << std::endl;
    #endif
    
    // Check intersection with each edge of the polygon
    for (size_t i = 0; i < polygon.size(); ++i) {
        LatLonPoint seg_start = polygon[i];
        LatLonPoint seg_end = polygon[(i + 1) % polygon.size()]; // Wrap around to first vertex
        
        LatLonPoint intersection;
        double t;
        if (findRaySegmentIntersection(ray_origin, bearing_degrees, seg_start, seg_end, intersection, &t)) {
            double distance = calculate_distance(ray_origin, intersection);
            
            #ifdef DEBUG_INTERSECTION
            std::cout << "Edge " << i << " intersects at (" << intersection.lat << ", " << intersection.lon 
                      << ") distance: " << distance << " t: " << t << std::endl;
            #endif
            
            // Only consider intersections that are at least 50m away to avoid self-intersection
            // and ensure we get meaningful survey legs
            if (distance > 1.0 && distance < min_distance) {
                min_distance = distance;
                nearest_intersection = intersection;
                found_intersection = true;
                intersected_edge = static_cast<int>(i);
            }
        } else {
            #ifdef DEBUG_INTERSECTION
            std::cout << "Edge " << i << " (" << seg_start.lat << ", " << seg_start.lon 
                      << ") to (" << seg_end.lat << ", " << seg_end.lon << ") - no intersection" << std::endl;
            #endif
        }
    }
    
    #ifdef DEBUG_INTERSECTION
    if (found_intersection) {
        std::cout << "Nearest intersection found at (" << nearest_intersection.lat << ", " << nearest_intersection.lon 
                  << ") distance: " << min_distance << " on edge " << intersected_edge << std::endl;
    } else {
        std::cout << "No intersection found, returning fallback point" << std::endl;
    }
    #endif
    
    // If no intersection found, return a point far along the ray
    if (!found_intersection) {
        return calculate_point_at_distance_bearing(ray_origin, 1000.0, bearing_degrees);
    }
    
    return nearest_intersection;
}

/*  Helper function to find the nearest intersection of a ray with a polygon
    Returns the nearest intersection point along the ray direction
    If no intersection is found, returns the ray_origin
    exclude_edge_index: edge index to exclude from intersection calculations (-1 to include all edges) */
inline LatLonPoint findNearestPolygonIntersection(const std::vector<LatLonPoint>& polygon, 
                                                 const LatLonPoint& ray_origin, 
                                                 double bearing_degrees,
                                                 int exclude_edge_index = -1) {
    if (polygon.empty()) {
        return ray_origin;
    }
    
    LatLonPoint nearest_intersection = ray_origin;
    double min_distance = std::numeric_limits<double>::max();
    bool found_intersection = false;
    int intersected_edge = -1;
    
    // Debug output
    #ifdef DEBUG_INTERSECTION
    std::cout << "Finding intersection from origin (" << ray_origin.lat << ", " << ray_origin.lon 
              << ") with bearing " << bearing_degrees << " degrees";
    if (exclude_edge_index >= 0) {
        std::cout << " (excluding edge " << exclude_edge_index << ")";
    }
    std::cout << std::endl;
    std::cout << "Polygon has " << polygon.size() << " vertices" << std::endl;
    #endif
    
    // Check intersection with each edge of the polygon
    for (size_t i = 0; i < polygon.size(); ++i) {
        // Skip the excluded edge
        if (exclude_edge_index >= 0 && static_cast<int>(i) == exclude_edge_index) {
            #ifdef DEBUG_INTERSECTION
            std::cout << "Skipping edge " << i << " (excluded starting edge)" << std::endl;
            #endif
            continue;
        }
        
        LatLonPoint seg_start = polygon[i];
        LatLonPoint seg_end = polygon[(i + 1) % polygon.size()]; // Wrap around to first vertex
        
        LatLonPoint intersection;
        double t;
        if (findRaySegmentIntersection(ray_origin, bearing_degrees, seg_start, seg_end, intersection, &t)) {
            double distance = calculate_distance(ray_origin, intersection);
            
            #ifdef DEBUG_INTERSECTION
            std::cout << "Edge " << i << " intersects at (" << intersection.lat << ", " << intersection.lon 
                      << ") distance: " << distance << " t: " << t << std::endl;
            #endif
            
            // Only consider intersections that are at least 25m away to avoid self-intersection
            // and ensure we get meaningful survey legs (reduced from 50m to be more permissive)
            if (distance > 1.0 && distance < min_distance) {
                min_distance = distance;
                nearest_intersection = intersection;
                found_intersection = true;
                intersected_edge = static_cast<int>(i);
            }
        } else {
            #ifdef DEBUG_INTERSECTION
            std::cout << "Edge " << i << " (" << seg_start.lat << ", " << seg_start.lon 
                      << ") to (" << seg_end.lat << ", " << seg_end.lon << ") - no intersection" << std::endl;
            #endif
        }
    }
    
    #ifdef DEBUG_INTERSECTION
    if (found_intersection) {
        std::cout << "Nearest intersection found at (" << nearest_intersection.lat << ", " << nearest_intersection.lon 
                  << ") distance: " << min_distance << " on edge " << intersected_edge << std::endl;
    } else {
        std::cout << "No intersection found, returning fallback point" << std::endl;
    }
    #endif
    
    // If no intersection found, return a point far along the ray
    if (!found_intersection) {
        return calculate_point_at_distance_bearing(ray_origin, 1000.0, bearing_degrees);
    }
    
    return nearest_intersection;
}

// Helper function to find which edge of the polygon was intersected
// Returns the index of the edge, or -1 if no intersection found
inline int findIntersectedEdgeIndex(const std::vector<LatLonPoint>& polygon, 
                                   const LatLonPoint& ray_origin, 
                                   double bearing_degrees,
                                   LatLonPoint& intersection,
                                   LatLonPoint& edge_start,
                                   LatLonPoint& edge_end) {
    if (polygon.empty()) {
        return -1;
    }
    
    double min_distance = std::numeric_limits<double>::max();
    int intersected_edge_index = -1;
    
    // Check intersection with each edge of the polygon
    for (size_t i = 0; i < polygon.size(); ++i) {
        LatLonPoint seg_start = polygon[i];
        LatLonPoint seg_end = polygon[(i + 1) % polygon.size()];
        
        LatLonPoint temp_intersection;
        double t;
        if (findRaySegmentIntersection(ray_origin, bearing_degrees, seg_start, seg_end, temp_intersection, &t)) {
            double distance = calculate_distance(ray_origin, temp_intersection);
            if (distance < min_distance && distance > 25.0) { // Must be at least 25m away (consistent with other checks)
                min_distance = distance;
                intersection = temp_intersection;
                edge_start = seg_start;
                edge_end = seg_end;
                intersected_edge_index = static_cast<int>(i);
            }
        }
    }
    
    return intersected_edge_index;
}

// Helper function to determine the correct edge for displacement when near a vertex
// Returns the edge index and bearing to move along the "unexplored" edge
inline std::pair<size_t, double> getCorrectEdgeForDisplacement(
    const std::vector<LatLonPoint>& polygon_vertices,
    const LatLonPoint& current_point,
    size_t last_edge_index,
    bool move_forward,
    double vertex_threshold = 20.0) {
    
    // Find if we're near a vertex
    size_t nearest_vertex_index = 0;
    double min_vertex_distance = std::numeric_limits<double>::max();
    
    for (size_t i = 0; i < polygon_vertices.size(); ++i) {
        double dist = calculate_distance(current_point, polygon_vertices[i]);
        if (dist < min_vertex_distance) {
            min_vertex_distance = dist;
            nearest_vertex_index = i;
        }
    }
    
    // If we're near a vertex, we need to choose the correct edge
    if (min_vertex_distance < vertex_threshold) {
        size_t target_edge_index;
        
        if (move_forward) {
            // Moving forward: choose the edge that starts from this vertex
            target_edge_index = nearest_vertex_index;
        } else {
            // Moving backward: choose the edge that ends at this vertex
            target_edge_index = (nearest_vertex_index - 1 + polygon_vertices.size()) % polygon_vertices.size();
        }
        
        // Calculate the bearing for the target edge
        LatLonPoint edge_start = polygon_vertices[target_edge_index];
        LatLonPoint edge_end = polygon_vertices[(target_edge_index + 1) % polygon_vertices.size()];
        
        double edge_bearing;
        if (move_forward) {
            edge_bearing = calculate_bearing(edge_start, edge_end);
        } else {
            edge_bearing = calculate_bearing(edge_end, edge_start);
        }
        
        return std::make_pair(target_edge_index, edge_bearing);
    } else {
        // We're not near a vertex, use the current edge
        LatLonPoint edge_start = polygon_vertices[last_edge_index];
        LatLonPoint edge_end = polygon_vertices[(last_edge_index + 1) % polygon_vertices.size()];
        
        double edge_bearing;
        if (move_forward) {
            edge_bearing = calculate_bearing(edge_start, edge_end);
        } else {
            edge_bearing = calculate_bearing(edge_end, edge_start);
        }
        
        return std::make_pair(last_edge_index, edge_bearing);
    }
}

// Helper function to find the next unvisited edge from a vertex
// Returns the bearing to move along the next edge of the polygon
inline double getNextEdgeBearing(const std::vector<LatLonPoint>& polygon_vertices,
                                const LatLonPoint& current_point, 
                                const LatLonPoint& last_edge_start, 
                                const LatLonPoint& last_edge_end) {
    // Find which vertex we're closest to
    double min_distance = std::numeric_limits<double>::max();
    size_t closest_vertex_index = 0;
    
    for (size_t i = 0; i < polygon_vertices.size(); ++i) {
        double distance = calculate_distance(current_point, polygon_vertices[i]);
        if (distance < min_distance) {
            min_distance = distance;
            closest_vertex_index = i;
        }
    }
    
    // If we're very close to a vertex (within 10m), use that vertex
    if (min_distance < 10.0) {
        LatLonPoint vertex = polygon_vertices[closest_vertex_index];
        
        // Get the two neighboring vertices
        size_t prev_index = (closest_vertex_index == 0) ? 
            polygon_vertices.size() - 1 : closest_vertex_index - 1;
        size_t next_index = (closest_vertex_index + 1) % polygon_vertices.size();
        
        LatLonPoint prev_vertex = polygon_vertices[prev_index];
        LatLonPoint next_vertex = polygon_vertices[next_index];
        
        // Determine which edge we just came from
        double dist_to_prev = calculate_distance(last_edge_start, prev_vertex) + 
                             calculate_distance(last_edge_end, prev_vertex);
        double dist_to_next = calculate_distance(last_edge_start, next_vertex) + 
                             calculate_distance(last_edge_end, next_vertex);
        
        // Choose the edge we haven't been on (the one farther from last edge)
        if (dist_to_prev < dist_to_next) {
            // We came from prev_vertex, so next edge is to next_vertex
            return calculate_bearing(vertex, next_vertex);
        } else {
            // We came from next_vertex, so next edge is to prev_vertex
            return calculate_bearing(vertex, prev_vertex);
        }
    }
    
    // If not close to a vertex, use the current edge bearing
    return calculate_bearing(last_edge_start, last_edge_end);
}


// Helper function to calculate perpendicular bearing to an edge
// Returns the bearing perpendicular to the edge (90 degrees rotated from edge bearing)
// inward: if true, returns inward perpendicular; if false, returns outward perpendicular
inline double calculatePerpendicularBearing(const LatLonPoint& edge_start, const LatLonPoint& edge_end, bool inward = true) {
    double edge_bearing = calculate_bearing(edge_start, edge_end);
    
    // Calculate perpendicular bearing (90 degrees rotation)
    double perpendicular_bearing;
    if (inward) {
        // Rotate 90 degrees counterclockwise (inward to polygon)
        perpendicular_bearing = edge_bearing - 90.0;
    } else {
        // Rotate 90 degrees clockwise (outward from polygon)
        perpendicular_bearing = edge_bearing + 90.0;
    }
    
    // Normalize to 0-360 degrees
    while (perpendicular_bearing < 0) perpendicular_bearing += 360.0;
    while (perpendicular_bearing >= 360) perpendicular_bearing -= 360.0;
    
    return perpendicular_bearing;
}

// Helper function to determine survey bearing based on current edge for alternating pattern
// Returns alternating bearing for proper lawnmower pattern
inline double getSurveyBearing(int current_edge_index, int starting_edge_index, 
                              int polygon_size, double odd_bearing, double even_bearing) {
    // Calculate edge distance from starting edge
    int edge_distance = (current_edge_index - starting_edge_index + polygon_size) % polygon_size;
    
    // Alternate between odd and even bearing
    if (edge_distance % 2 == 0) {
        // Even distance from starting edge: use even bearing
        return even_bearing;
    } else {
        // Odd distance from starting edge: use odd bearing
        return odd_bearing;
    }
}

// Helper function to check if a point is inside a polygon using ray casting algorithm
inline bool isPointInPolygon(const LatLonPoint& point, const std::vector<LatLonPoint>& polygon) {
    if (polygon.size() < 3) return false;
    
    int crossings = 0;
    size_t n = polygon.size();
    
    for (size_t i = 0; i < n; i++) {
        size_t j = (i + 1) % n;
        
        // Check if ray from point to the right crosses edge (i,j)
        if (((polygon[i].lat <= point.lat) && (point.lat < polygon[j].lat)) ||
            ((polygon[j].lat <= point.lat) && (point.lat < polygon[i].lat))) {
            
            // Calculate x-coordinate of intersection
            double x_intersect = polygon[i].lon + (point.lat - polygon[i].lat) * 
                                (polygon[j].lon - polygon[i].lon) / (polygon[j].lat - polygon[i].lat);
            
            if (point.lon < x_intersect) {
                crossings++;
            }
        }
    }
    
    return (crossings % 2) == 1; // Odd number of crossings means inside
}

// Helper function to find the closest point on a line segment to a given point
inline LatLonPoint findClosestPointOnEdge(const LatLonPoint& point, const LatLonPoint& edge_start, const LatLonPoint& edge_end) {
    // Use the point as reference for coordinate transformation
    LatLonPoint reference = point;
    
    // Transform to Cartesian coordinates
    CartesianPoint point_cart = latLonToCartesian(point, reference);
    CartesianPoint edge_start_cart = latLonToCartesian(edge_start, reference);
    CartesianPoint edge_end_cart = latLonToCartesian(edge_end, reference);
    
    // Vector from edge_start to edge_end
    double edge_dx = edge_end_cart.x - edge_start_cart.x;
    double edge_dy = edge_end_cart.y - edge_start_cart.y;
    
    // Vector from edge_start to point
    double point_dx = point_cart.x - edge_start_cart.x;
    double point_dy = point_cart.y - edge_start_cart.y;
    
    // Calculate the projection parameter
    double edge_length_sq = edge_dx * edge_dx + edge_dy * edge_dy;
    
    if (edge_length_sq == 0.0) {
        // Edge is a point, return edge_start
        return edge_start;
    }
    
    double t = (point_dx * edge_dx + point_dy * edge_dy) / edge_length_sq;
    
    // Clamp t to [0, 1] to stay on the edge segment
    t = std::max(0.0, std::min(1.0, t));
    
    // Calculate the closest point in Cartesian coordinates
    CartesianPoint closest_cart;
    closest_cart.x = edge_start_cart.x + t * edge_dx;
    closest_cart.y = edge_start_cart.y + t * edge_dy;
    
    // Convert back to lat/lon
    return cartesianToLatLon(closest_cart, reference);
}

// Helper function to move along polygon perimeter by a specified distance
inline LatLonPoint moveAlongPolygonPerimeter(const std::vector<LatLonPoint>& polygon_vertices,
                                            const LatLonPoint& start_point,
                                            int start_edge_index,
                                            double distance_to_move,
                                            bool move_forward) {
    if (polygon_vertices.empty() || start_edge_index < 0 || start_edge_index >= static_cast<int>(polygon_vertices.size())) {
        return start_point;
    }
    
    LatLonPoint current_point = start_point;
    double remaining_distance = distance_to_move;
    int current_edge_index = start_edge_index;
    
    while (remaining_distance > 1.0) { // Continue until we've moved the required distance (within 1m)
        // Get current edge endpoints
        LatLonPoint edge_start = polygon_vertices[current_edge_index];
        LatLonPoint edge_end = polygon_vertices[(current_edge_index + 1) % polygon_vertices.size()];
        
        // Calculate distance from current point to edge end (in movement direction)
        double distance_to_edge_end;
        LatLonPoint target_vertex;
        
        if (move_forward) {
            distance_to_edge_end = calculate_distance(current_point, edge_end);
            target_vertex = edge_end;
        } else {
            distance_to_edge_end = calculate_distance(current_point, edge_start);
            target_vertex = edge_start;
        }
        
        if (remaining_distance <= distance_to_edge_end) {
            // We can complete the movement on this edge
            double bearing = calculate_bearing(current_point, target_vertex);
            return calculate_point_at_distance_bearing(current_point, remaining_distance, bearing);
        } else {
            // Move to the edge endpoint and continue on next edge
            current_point = target_vertex;
            remaining_distance -= distance_to_edge_end;
            
            // Move to next edge
            if (move_forward) {
                current_edge_index = (current_edge_index + 1) % polygon_vertices.size();
            } else {
                current_edge_index = (current_edge_index - 1 + polygon_vertices.size()) % polygon_vertices.size();
            }
        }
    }
    
    return current_point;
}
