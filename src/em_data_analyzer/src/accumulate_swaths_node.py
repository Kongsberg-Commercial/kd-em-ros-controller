#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import PointCloud2
import numpy as np
import struct
import copy

class SwathAccumulator(Node):
    def __init__(self):
        super().__init__('swath_accumulator')
        self.subscription = self.create_subscription(
            PointCloud2,
            '/PointcloudFromKMALL',
            self.pointcloud_callback,
            10)
        self.publisher = self.create_publisher(
            PointCloud2,
            '/accumulated_swath_cloud',
            10)
        self.accumulated_cloud = None
        self.timer = self.create_timer(1.0, self.publish_accumulated_cloud)  # publish every second

    def pointcloud_callback(self, msg):
        if self.accumulated_cloud is None:
            # Deep copy first swath as the start
            self.accumulated_cloud = copy.deepcopy(msg)
            self.accumulated_cloud.data = bytearray(msg.data)
            self.accumulated_cloud.width = msg.width * msg.height
            self.accumulated_cloud.height = 1
        else:
            # Append new points to accumulated_cloud
            # Assume PointCloud2 is unorganized (height=1), and the fields are consistent
            new_points = msg.data
            self.accumulated_cloud.data += new_points
            self.accumulated_cloud.width += msg.width * msg.height
            self.accumulated_cloud.height = 1
            self.accumulated_cloud.row_step = self.accumulated_cloud.width * self.accumulated_cloud.point_step

    def publish_accumulated_cloud(self):
        if self.accumulated_cloud is not None:
            self.publisher.publish(self.accumulated_cloud)
            self.get_logger().info(f"Published accumulated cloud with width: {self.accumulated_cloud.width}")

def main(args=None):
    rclpy.init(args=args)
    node = SwathAccumulator()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()