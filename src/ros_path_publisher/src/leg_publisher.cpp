#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include "path_publisher/utils.hpp"

class LegPublisher : public rclcpp::Node
{
public:
    LegPublisher()
    : Node("leg_publisher")
    {
        // Publisher on /path topic
        pub_ = this->create_publisher<std_msgs::msg::String>("/path", 10);

        // Timer to publish every 1 second
        timer_ = this->create_wall_timer(
            std::chrono::seconds(1),
            std::bind(&LegPublisher::publish_leg, this)
        );
    }

private:
    void publish_leg()
    {
        // Example coordinates (decimal degrees)
        double lat0 = 37.7749;    // Start point latitude
        double lon0 = -122.4194;  // Start point longitude
        double lat1 = 37.7849;    // End point latitude
        double lon1 = -122.4094;  // End point longitude
        double speed = 2.5;       // Speed in meters/second

        // Format each field
        std::string lat0_nmea = decDegToNmeaLat(lat0);   // e.g. 3746.494,N
        std::string lon0_nmea = decDegToNmeaLon(lon0);   // e.g. 12225.164,W
        std::string lat1_nmea = decDegToNmeaLat(lat1);
        std::string lon1_nmea = decDegToNmeaLon(lon1);

        // Compose message body (excluding $ and *checksum)
        std::ostringstream nmea_body;
        nmea_body << "PMARLEG,"
                  << lat0_nmea << ","
                  << lon0_nmea << ","
                  << lat1_nmea << ","
                  << lon1_nmea << ","
                  << std::fixed << std::setprecision(2) << speed;

        std::string nmea_payload = nmea_body.str();

        // Compute checksum
        std::string checksum = nmeaChecksum(nmea_payload);

        // Compose full message: $ + body + * + checksum + <CR><LF>
        std::string nmea_msg = "$" + nmea_payload + "*" + checksum + "\r\n";

        // Publish
        std_msgs::msg::String msg;
        msg.data = nmea_msg;
        pub_->publish(msg);

        RCLCPP_INFO(this->get_logger(), "Published: %s", nmea_msg.c_str());
    }

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<LegPublisher>());
    rclcpp::shutdown();
    return 0;
}