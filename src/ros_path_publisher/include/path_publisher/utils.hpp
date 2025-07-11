#pragma once
#include <string>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <limits>
#include <vector>

// Structure to represent a lat/lon point
struct LatLonPoint {
    double lat;
    double lon;
    
    LatLonPoint(double latitude = 0.0, double longitude = 0.0) 
        : lat(latitude), lon(longitude) {}
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
        ray_direction: direction vector of the ray (normalized bearing in lat/lon space)
        seg_start, seg_end: endpoints of the line segment
        intersection: output parameter for the intersection point
        t: output parameter for the distance along the ray (optional) */
inline bool findRaySegmentIntersection(const LatLonPoint& ray_origin, const LatLonPoint& ray_direction,
                                       const LatLonPoint& seg_start, const LatLonPoint& seg_end,
                                       LatLonPoint& intersection, double* t = nullptr) {
    // Convert to local coordinate system for easier calculation
    // We'll work in a small local area where lat/lon can be treated as x/y
    
    // Calculate direction vectors
    double ray_dx = ray_direction.lon - ray_origin.lon;
    double ray_dy = ray_direction.lat - ray_origin.lat;
    double seg_dx = seg_end.lon - seg_start.lon;
    double seg_dy = seg_end.lat - seg_start.lat;
    
    // Calculate vector from segment start to ray origin
    double start_to_origin_dx = ray_origin.lon - seg_start.lon;
    double start_to_origin_dy = ray_origin.lat - seg_start.lat;
    
    // Solve the intersection using parametric equations:
    // Ray: ray_origin + t * ray_direction
    // Segment: seg_start + s * (seg_end - seg_start)
    // Where t >= 0 (ray extends forward) and 0 <= s <= 1 (point is on segment)
    
    double denominator = ray_dx * seg_dy - ray_dy * seg_dx;
    if (std::abs(denominator) < 1e-10) {
        // Lines are parallel or collinear
        return false;
    }
    
    double t_ray = (start_to_origin_dx * seg_dy - start_to_origin_dy * seg_dx) / denominator;
    double s_segment = (start_to_origin_dx * ray_dy - start_to_origin_dy * ray_dx) / denominator;
    
    // Check if intersection is valid (on ray and on segment)
    if (t_ray >= 0 && s_segment >= 0 && s_segment <= 1) {
        intersection.lat = ray_origin.lat + t_ray * ray_dy;
        intersection.lon = ray_origin.lon + t_ray * ray_dx;
        if (t) *t = t_ray;
        return true;
    }
    
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
    
    // Create a point along the ray direction (1 degree away for direction calculation)
    LatLonPoint ray_direction = calculate_point_at_distance_bearing(ray_origin, 111000.0, bearing_degrees); // ~1 degree
    
    LatLonPoint nearest_intersection = ray_origin;
    double min_distance = std::numeric_limits<double>::max();
    bool found_intersection = false;
    
    // Check intersection with each edge of the polygon
    for (size_t i = 0; i < polygon.size(); ++i) {
        LatLonPoint seg_start = polygon[i];
        LatLonPoint seg_end = polygon[(i + 1) % polygon.size()]; // Wrap around to first vertex
        
        LatLonPoint intersection;
        double t;
        if (findRaySegmentIntersection(ray_origin, ray_direction, seg_start, seg_end, intersection, &t)) {
            double distance = calculate_distance(ray_origin, intersection);
            if (distance < min_distance) {
                min_distance = distance;
                nearest_intersection = intersection;
                found_intersection = true;
            }
        }
    }
    
    return found_intersection ? nearest_intersection : ray_origin;
}