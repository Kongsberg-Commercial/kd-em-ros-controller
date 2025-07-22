#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from ros_otter_custom_interfaces.msg import GpsInfo
import time
import math

class DummyGpsPublisher(Node):
    def __init__(self):
        super().__init__('dummy_gps_publisher')
        self.publisher_ = self.create_publisher(GpsInfo, 'gps_info', 10)
        self.timer_ = self.create_timer(0.5, self.publish_gps)
        self.start_time = time.time()
        # Start at given location
        self.base_lat = 59.42790389312871
        self.base_lon = 10.472103926627671
        self.alt = 2.0  # meters
        self.radius = 0.0002  # ~22 meters
        self.speed = 2.0  # knots
        self.cog = 90.0   # course over ground (east)
        
    def publish_gps(self):
        t = time.time() - self.start_time
        # Move in a circle
        angle = (t / 30.0) * 2 * math.pi
        lat = self.base_lat + self.radius * math.cos(angle)
        lon = self.base_lon + self.radius * math.sin(angle)
        
        msg = GpsInfo()
        msg.time = time.time()
        msg.lat = lat
        msg.lat_dir = 'N' if lat >= 0 else 'S'
        msg.lon = lon
        msg.lon_dir = 'E' if lon >= 0 else 'W'
        msg.alt = self.alt
        msg.sog = self.speed
        msg.cog = (math.degrees(angle) + 90.0) % 360  # simple course
        
        self.publisher_.publish(msg)
        self.get_logger().info(
            f"Publishing dummy GPS: lat={msg.lat:.7f}, lon={msg.lon:.7f}, alt={msg.alt}, sog={msg.sog}, cog={msg.cog}"
        )

def main(args=None):
    rclpy.init(args=args)
    node = DummyGpsPublisher()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()