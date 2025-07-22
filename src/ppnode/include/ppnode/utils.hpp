#ifndef PPNODE_UTILS_HPP
#define PPNODE_UTILS_HPP

#include <cmath>
#include <vector>
#include <stdexcept>
#include "ros_otter_custom_interfaces/msg/gps_info.hpp"
#include "ros_otter_custom_interfaces/msg/vertex.hpp"

namespace ppnode_utils {

// Configuration constants
namespace config {
    constexpr double EARTH_RADIUS = 6371000.0; // Earth radius in meters
    constexpr double DEG_TO_RAD = M_PI / 180.0;
    constexpr double RAD_TO_DEG = 180.0 / M_PI;
    constexpr size_t MIN_POLYGON_VERTICES = 3; // Minimum vertices required for a polygon
    constexpr double DEFAULT_SPEED_MS = 2.0; // Default speed in m/s
    constexpr double PATH_OFFSET_METERS = 10.0; // Default offset for second point in meters
    constexpr int DEFAULT_QUEUE_SIZE = 10; // Default ROS2 queue size
    
    // GPS validation limits
    constexpr double MIN_LATITUDE = -90.0;
    constexpr double MAX_LATITUDE = 90.0;
    constexpr double MIN_LONGITUDE = -180.0;
    constexpr double MAX_LONGITUDE = 180.0;
}

// Structure to represent a 2D Cartesian point
struct CartesianPoint {
    double x;
    double y;
    
    CartesianPoint() : x(0.0), y(0.0) {}
    CartesianPoint(double x_val, double y_val) : x(x_val), y(y_val) {}
    
    // Operator overloads for convenience
    CartesianPoint operator+(const CartesianPoint& other) const {
        return CartesianPoint(x + other.x, y + other.y);
    }
    
    CartesianPoint operator-(const CartesianPoint& other) const {
        return CartesianPoint(x - other.x, y - other.y);
    }
};

// Structure to represent a GPS coordinate
struct GpsPoint {
    double lat;
    double lon;
    
    GpsPoint() : lat(0.0), lon(0.0) {}
    GpsPoint(double lat_val, double lon_val) : lat(lat_val), lon(lon_val) {}
    
    // Validation method
    bool isValid() const {
        return lat >= config::MIN_LATITUDE && lat <= config::MAX_LATITUDE &&
               lon >= config::MIN_LONGITUDE && lon <= config::MAX_LONGITUDE;
    }
};

// Validation functions
inline bool validateGpsCoordinates(double lat, double lon) {
    return lat >= config::MIN_LATITUDE && lat <= config::MAX_LATITUDE &&
           lon >= config::MIN_LONGITUDE && lon <= config::MAX_LONGITUDE;
}

inline bool validatePolygonVertices(const std::vector<GpsPoint>& vertices) {
    if (vertices.size() < config::MIN_POLYGON_VERTICES) {
        return false;
    }
    
    for (const auto& vertex : vertices) {
        if (!vertex.isValid()) {
            return false;
        }
    }
    
    return true;
}

/**
 * Convert GPS coordinates (lat/lon) to local Cartesian coordinates
 * Uses a simple equirectangular projection with a reference point
 * @param gps_point The GPS coordinate to convert
 * @param ref_lat Reference latitude for the local coordinate system (degrees)
 * @param ref_lon Reference longitude for the local coordinate system (degrees)
 * @return CartesianPoint in meters from the reference point
 * @throws std::invalid_argument if GPS coordinates are invalid
 */
inline CartesianPoint gpsToCartesian(const GpsPoint& gps_point, double ref_lat, double ref_lon) {
    if (!gps_point.isValid() || !validateGpsCoordinates(ref_lat, ref_lon)) {
        throw std::invalid_argument("Invalid GPS coordinates for conversion");
    }
    
    double lat_rad = gps_point.lat * config::DEG_TO_RAD;
    double lon_rad = gps_point.lon * config::DEG_TO_RAD;
    double ref_lat_rad = ref_lat * config::DEG_TO_RAD;
    double ref_lon_rad = ref_lon * config::DEG_TO_RAD;
    
    // Simple equirectangular projection
    double x = config::EARTH_RADIUS * (lon_rad - ref_lon_rad) * std::cos(ref_lat_rad);
    double y = config::EARTH_RADIUS * (lat_rad - ref_lat_rad);
    
    return CartesianPoint(x, y);
}

/**
 * Convert local Cartesian coordinates back to GPS coordinates
 * @param cart_point The Cartesian coordinate to convert
 * @param ref_lat Reference latitude for the local coordinate system (degrees)
 * @param ref_lon Reference longitude for the local coordinate system (degrees)
 * @return GpsPoint in decimal degrees
 * @throws std::invalid_argument if reference coordinates are invalid
 */
inline GpsPoint cartesianToGps(const CartesianPoint& cart_point, double ref_lat, double ref_lon) {
    if (!validateGpsCoordinates(ref_lat, ref_lon)) {
        throw std::invalid_argument("Invalid reference GPS coordinates for conversion");
    }
    
    double ref_lat_rad = ref_lat * config::DEG_TO_RAD;
    double ref_lon_rad = ref_lon * config::DEG_TO_RAD;
    
    // Reverse equirectangular projection
    double lat_rad = ref_lat_rad + (cart_point.y / config::EARTH_RADIUS);
    double lon_rad = ref_lon_rad + (cart_point.x / (config::EARTH_RADIUS * std::cos(ref_lat_rad)));
    
    double lat = lat_rad * config::RAD_TO_DEG;
    double lon = lon_rad * config::RAD_TO_DEG;
    
    return GpsPoint(lat, lon);
}

/**
 * Convert GpsInfo message to GpsPoint
 * @param gps_info The ROS2 GpsInfo message
 * @return GpsPoint with decimal degree coordinates
 * @throws std::invalid_argument if GPS coordinates are invalid
 */
inline GpsPoint gpsInfoToGpsPoint(const ros_otter_custom_interfaces::msg::GpsInfo& gps_info) {
    double lat = gps_info.lat;
    double lon = gps_info.lon;
    
    // Handle direction indicators - make negative if South or West
    if (gps_info.lat_dir == "S") {
        lat = -lat;
    }
    if (gps_info.lon_dir == "W") {
        lon = -lon;
    }
    
    GpsPoint point(lat, lon);
    if (!point.isValid()) {
        throw std::invalid_argument("Invalid GPS coordinates in GpsInfo message");
    }
    
    return point;
}

/**
 * Convert GpsPoint to GpsInfo message
 * @param gps_point The GPS point to convert
 * @param time Timestamp for the message
 * @return GpsInfo message
 * @throws std::invalid_argument if GPS coordinates are invalid
 */
inline ros_otter_custom_interfaces::msg::GpsInfo gpsPointToGpsInfo(const GpsPoint& gps_point, double time) {
    if (!gps_point.isValid()) {
        throw std::invalid_argument("Invalid GPS coordinates for GpsInfo conversion");
    }
    
    ros_otter_custom_interfaces::msg::GpsInfo gps_info;
    
    gps_info.time = time;
    gps_info.lat = std::abs(gps_point.lat);
    gps_info.lon = std::abs(gps_point.lon);
    gps_info.lat_dir = (gps_point.lat >= 0) ? "N" : "S";
    gps_info.lon_dir = (gps_point.lon >= 0) ? "E" : "W";
    gps_info.alt = 0.0;  // Default altitude
    gps_info.sog = 0.0;  // Default speed over ground
    gps_info.cog = 0.0;  // Default course over ground
    
    return gps_info;
}

/**
 * Convert Vertex message to GpsPoint
 * @param vertex The ROS2 Vertex message
 * @return GpsPoint with decimal degree coordinates
 * @throws std::invalid_argument if GPS coordinates are invalid
 */
inline GpsPoint vertexToGpsPoint(const ros_otter_custom_interfaces::msg::Vertex& vertex) {
    GpsPoint point(vertex.latitude, vertex.longitude);
    if (!point.isValid()) {
        throw std::invalid_argument("Invalid GPS coordinates in Vertex message");
    }
    return point;
}

/**
 * Convert vector of Vertex messages to vector of GpsPoints
 * @param vertices Vector of Vertex messages
 * @return Vector of GpsPoints
 * @throws std::invalid_argument if any GPS coordinates are invalid
 */
inline std::vector<GpsPoint> vertexArrayToGpsPoints(const std::vector<ros_otter_custom_interfaces::msg::Vertex>& vertices) {
    std::vector<GpsPoint> gps_points;
    gps_points.reserve(vertices.size());
    
    for (const auto& vertex : vertices) {
        gps_points.push_back(vertexToGpsPoint(vertex));
    }
    
    if (!validatePolygonVertices(gps_points)) {
        throw std::invalid_argument("Invalid polygon vertices");
    }
    
    return gps_points;
}

/**
 * Calculate the distance between two GPS points in meters
 * Uses Haversine formula for accuracy over larger distances
 * @param point1 First GPS point
 * @param point2 Second GPS point
 * @return Distance in meters
 */
inline double calculateGpsDistance(const GpsPoint& point1, const GpsPoint& point2) {
    double lat1_rad = point1.lat * config::DEG_TO_RAD;
    double lon1_rad = point1.lon * config::DEG_TO_RAD;
    double lat2_rad = point2.lat * config::DEG_TO_RAD;
    double lon2_rad = point2.lon * config::DEG_TO_RAD;
    
    double dlat = lat2_rad - lat1_rad;
    double dlon = lon2_rad - lon1_rad;
    
    double a = std::sin(dlat/2) * std::sin(dlat/2) + 
               std::cos(lat1_rad) * std::cos(lat2_rad) * 
               std::sin(dlon/2) * std::sin(dlon/2);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1-a));
    
    return config::EARTH_RADIUS * c;
}

/**
 * Calculate the distance between two Cartesian points
 * @param point1 First Cartesian point
 * @param point2 Second Cartesian point
 * @return Distance in meters
 */
inline double calculateCartesianDistance(const CartesianPoint& point1, const CartesianPoint& point2) {
    double dx = point2.x - point1.x;
    double dy = point2.y - point1.y;
    return std::sqrt(dx*dx + dy*dy);
}

/**
 * Convert a vector of GPS points to Cartesian coordinates
 * @param gps_points Vector of GPS points
 * @param ref_lat Reference latitude
 * @param ref_lon Reference longitude
 * @return Vector of Cartesian points
 * @throws std::invalid_argument if any GPS coordinates are invalid
 */
inline std::vector<CartesianPoint> gpsPolygonToCartesian(const std::vector<GpsPoint>& gps_points, 
                                                         double ref_lat, double ref_lon) {
    if (!validatePolygonVertices(gps_points)) {
        throw std::invalid_argument("Invalid GPS polygon vertices");
    }
    
    std::vector<CartesianPoint> cart_points;
    cart_points.reserve(gps_points.size());
    
    for (const auto& gps_point : gps_points) {
        cart_points.push_back(gpsToCartesian(gps_point, ref_lat, ref_lon));
    }
    
    return cart_points;
}

/**
 * Convert a vector of Cartesian points to GPS coordinates
 * @param cart_points Vector of Cartesian points
 * @param ref_lat Reference latitude
 * @param ref_lon Reference longitude
 * @return Vector of GPS points
 * @throws std::invalid_argument if reference coordinates are invalid
 */
inline std::vector<GpsPoint> cartesianPolygonToGps(const std::vector<CartesianPoint>& cart_points,
                                                   double ref_lat, double ref_lon) {
    if (!validateGpsCoordinates(ref_lat, ref_lon)) {
        throw std::invalid_argument("Invalid reference GPS coordinates");
    }
    
    std::vector<GpsPoint> gps_points;
    gps_points.reserve(cart_points.size());
    
    for (const auto& cart_point : cart_points) {
        gps_points.push_back(cartesianToGps(cart_point, ref_lat, ref_lon));
    }
    
    return gps_points;
}

/**
 * Convert decimal degrees to NMEA format (ddmm.mmm)
 * @param decimal_degrees The coordinate in decimal degrees
 * @return String in NMEA ddmm.mmm format
 */
inline std::string decimalDegreesToNMEA(double decimal_degrees) {
    double abs_degrees = std::abs(decimal_degrees);
    int degrees = static_cast<int>(abs_degrees);
    double minutes = (abs_degrees - degrees) * 60.0;
    
    char buffer[16];
    std::snprintf(buffer, sizeof(buffer), "%02d%06.3f", degrees, minutes);
    return std::string(buffer);
}

/**
 * Calculate NMEA checksum
 * @param sentence The NMEA sentence without $ and * characters
 * @return Checksum as uppercase hex string
 */
inline std::string calculateNMEAChecksum(const std::string& sentence) {
    uint8_t checksum = 0;
    for (char c : sentence) {
        checksum ^= static_cast<uint8_t>(c);
    }
    
    char buffer[3];
    std::snprintf(buffer, sizeof(buffer), "%02X", checksum);
    return std::string(buffer);
}

/**
 * Create PMARLEG NMEA sentence for leg mode
 * @param point0 Starting point of the leg
 * @param point1 Ending point of the leg  
 * @param speed Speed in m/s
 * @return Complete PMARLEG NMEA sentence with checksum
 * @throws std::invalid_argument if GPS coordinates are invalid
 */
inline std::string createPMARLEGMessage(const GpsPoint& point0, const GpsPoint& point1, double speed) {
    if (!point0.isValid() || !point1.isValid()) {
        throw std::invalid_argument("Invalid GPS coordinates for PMARLEG message");
    }
    
    // Convert coordinates to NMEA format
    std::string lat0_nmea = decimalDegreesToNMEA(point0.lat);
    std::string lon0_nmea = decimalDegreesToNMEA(point0.lon);
    std::string lat1_nmea = decimalDegreesToNMEA(point1.lat);
    std::string lon1_nmea = decimalDegreesToNMEA(point1.lon);
    
    // Determine directions
    std::string lat0_dir = (point0.lat >= 0) ? "N" : "S";
    std::string lon0_dir = (point0.lon >= 0) ? "E" : "W";
    std::string lat1_dir = (point1.lat >= 0) ? "N" : "S";
    std::string lon1_dir = (point1.lon >= 0) ? "E" : "W";
    
    // Build the sentence content (without $ and checksum)
    char sentence_buffer[256];
    std::snprintf(sentence_buffer, sizeof(sentence_buffer),
                  "PMARLEG,%s,%s,%s,%s,%s,%s,%s,%s,%.2f",
                  lat0_nmea.c_str(), lat0_dir.c_str(),
                  lon0_nmea.c_str(), lon0_dir.c_str(),
                  lat1_nmea.c_str(), lat1_dir.c_str(),
                  lon1_nmea.c_str(), lon1_dir.c_str(),
                  speed);
    
    std::string sentence_content(sentence_buffer);
    std::string checksum = calculateNMEAChecksum(sentence_content);
    
    // Build complete NMEA sentence
    return "$" + sentence_content + "*" + checksum + "\r\n";
}

} // namespace ppnode_utils

#endif // PPNODE_UTILS_HPP
