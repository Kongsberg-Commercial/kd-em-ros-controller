#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from ros_otter_custom_interfaces.msg import GpsInfo
from visualization_msgs.msg import Marker, MarkerArray
from geometry_msgs.msg import Point
from std_msgs.msg import ColorRGBA
import time
import math

class DummyGpsPublisher(Node):
    def __init__(self):
        super().__init__('dummy_gps_publisher')
        self.publisher_ = self.create_publisher(GpsInfo, 'gps_info', 10)
        self.marker_publisher_ = self.create_publisher(MarkerArray, 'gps_visualization', 10)
        self.timer_ = self.create_timer(0.5, self.publish_gps)
        self.start_time = time.time()
        # Start at given location
        self.base_lat = 59.42790389312871
        self.base_lon = 10.472103926627671
        self.alt = 2.0  # meters
        self.speed = 1.0  # knots
        self.cog = 0.0   # course over ground (east)
        # Speed conversion: 1 knot ≈ 0.514444 m/s
        # At this latitude, 1 degree lat ≈ 111,320 m, 1 degree lon ≈ 111,320 * cos(lat) m
        self.lat_deg_per_meter = 1.0 / 111320.0
        self.lon_deg_per_meter = 1.0 / (111320.0 * math.cos(math.radians(self.base_lat)))
        
        # For visualization
        self.path_points = []
        self.max_path_points = 200  # Keep last 200 points for path visualization
        
    def gps_to_local_coords(self, lat, lon):
        """Convert GPS coordinates to local ENU coordinates for visualization"""
        # Convert to meters relative to base position
        x = (lon - self.base_lon) / self.lon_deg_per_meter
        y = (lat - self.base_lat) / self.lat_deg_per_meter
        z = self.alt
        return x, y, z
    
    def create_vessel_marker(self, lat, lon, marker_id=0):
        """Create a marker representing the vessel"""
        marker = Marker()
        marker.header.frame_id = "map"
        marker.header.stamp = self.get_clock().now().to_msg()
        marker.ns = "vessel"
        marker.id = marker_id
        marker.type = Marker.ARROW
        marker.action = Marker.ADD
        
        # Position
        x, y, z = self.gps_to_local_coords(lat, lon)
        marker.pose.position.x = x
        marker.pose.position.y = y
        marker.pose.position.z = z
        
        # Orientation (pointing east)
        marker.pose.orientation.x = 0.0
        marker.pose.orientation.y = 0.0
        marker.pose.orientation.z = 0.0
        marker.pose.orientation.w = 1.0
        
        # Scale
        marker.scale.x = 5.0  # length
        marker.scale.y = 1.0  # width
        marker.scale.z = 1.0  # height
        
        # Color (red)
        marker.color.r = 1.0
        marker.color.g = 0.0
        marker.color.b = 0.0
        marker.color.a = 1.0
        
        return marker
    
    def create_path_marker(self, marker_id=1):
        """Create a line strip marker showing the vessel's path"""
        marker = Marker()
        marker.header.frame_id = "map"
        marker.header.stamp = self.get_clock().now().to_msg()
        marker.ns = "path"
        marker.id = marker_id
        marker.type = Marker.LINE_STRIP
        marker.action = Marker.ADD
        
        # Scale
        marker.scale.x = 0.2  # line width
        
        # Color (blue)
        marker.color.r = 0.0
        marker.color.g = 0.0
        marker.color.b = 1.0
        marker.color.a = 0.8
        
        # Add all path points
        for lat, lon in self.path_points:
            point = Point()
            x, y, z = self.gps_to_local_coords(lat, lon)
            point.x = x
            point.y = y
            point.z = z
            marker.points.append(point)
        
        return marker
    
    def publish_gps(self):
        t = time.time() - self.start_time
        # Move in a straight line east at 1 knot
        # Convert knots to m/s: 1 knot = 0.514444 m/s
        speed_ms = self.speed * 0.514444
        distance_traveled = speed_ms * t  # meters
        
        # Calculate new position
        lat = self.base_lat
        lon = self.base_lon + (distance_traveled * self.lon_deg_per_meter)
        
        # Publish GPS message
        msg = GpsInfo()
        msg.time = time.time()
        msg.lat = lat
        msg.lat_dir = 'N' if lat >= 0 else 'S'
        msg.lon = lon
        msg.lon_dir = 'E' if lon >= 0 else 'W'
        msg.alt = self.alt
        msg.sog = self.speed
        msg.cog = self.cog
        
        self.publisher_.publish(msg)
        
        # Update path for visualization
        self.path_points.append((lat, lon))
        if len(self.path_points) > self.max_path_points:
            self.path_points.pop(0)  # Remove oldest point
        
        # Create and publish visualization markers
        marker_array = MarkerArray()
        
        # Add vessel marker
        vessel_marker = self.create_vessel_marker(lat, lon, 0)
        marker_array.markers.append(vessel_marker)
        
        # Add path marker (only if we have at least 2 points)
        if len(self.path_points) >= 2:
            path_marker = self.create_path_marker(1)
            marker_array.markers.append(path_marker)
        
        self.marker_publisher_.publish(marker_array)
        
        # self.get_logger().info(
        #    f"Publishing dummy GPS: lat={msg.lat:.7f}, lon={msg.lon:.7f}, alt={msg.alt}, sog={msg.sog}, cog={msg.cog}"
        #)

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