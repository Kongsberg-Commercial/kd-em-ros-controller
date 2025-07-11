#pragma once
#include <string>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <limits>

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