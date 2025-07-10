#pragma once
#include <string>
#include <iomanip>
#include <sstream>
#include <cmath>

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