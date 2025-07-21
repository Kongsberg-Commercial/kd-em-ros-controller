#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import PointCloud2, PointField
from std_msgs.msg import Header, Bool
import numpy as np

class DummyPointcloudAndStatusPublisher(Node):
    def __init__(self):
        super().__init__('dummy_pointcloud_and_status_publisher')
        self.pointcloud_pub = self.create_publisher(PointCloud2, '/PointcloudFromKMALL', 10)
        self.leg_status_pub = self.create_publisher(Bool, '/leg_status', 10)

        self.leg_active = False
        self.swath_count = 0

        self.timer = self.create_timer(1.0, self.timer_callback)
        self.leg_status_timer = self.create_timer(12.0, self.toggle_leg_status)

    def create_dummy_pointcloud(self):
        # Create dummy points for one swath
        num_points = 10
        points = np.zeros((num_points, 3), dtype=np.float32)
        points[:, 0] = np.linspace(0, 1, num_points)     # x-coord
        points[:, 1] = np.linspace(-5, 5, num_points)    # y-coord (swath width)
        points[:, 2] = np.linspace(-10, -20, num_points) # z-coord (depth)

        # Optionally vary points for each swath
        points += self.swath_count

        data = points.tobytes()
        fields = [
            PointField(name='x', offset=0, datatype=PointField.FLOAT32, count=1),
            PointField(name='y', offset=4, datatype=PointField.FLOAT32, count=1),
            PointField(name='z', offset=8, datatype=PointField.FLOAT32, count=1)
        ]
        msg = PointCloud2(
            header=Header(frame_id='base_link'),
            height=1,
            width=num_points,
            is_dense=True,
            is_bigendian=False,
            fields=fields,
            point_step=12,
            row_step=12 * num_points,
            data=data
        )
        return msg

    def timer_callback(self):
        if self.leg_active:
            msg = self.create_dummy_pointcloud()
            msg.header.stamp = self.get_clock().now().to_msg()
            self.pointcloud_pub.publish(msg)
            self.get_logger().info(f'Published dummy PointCloud2 swath {self.swath_count}')
            self.swath_count += 1
        else:
            self.get_logger().info('Leg not active, not publishing PointCloud2')

    def toggle_leg_status(self):
        # Toggle leg status every 12 seconds for demonstration
        self.leg_active = not self.leg_active
        status_msg = Bool()
        status_msg.data = self.leg_active
        self.leg_status_pub.publish(status_msg)
        if self.leg_active:
            self.get_logger().info('Leg started, /leg_status set to True')
            self.swath_count = 0
        else:
            self.get_logger().info('Leg ended, /leg_status set to False')

def main(args=None):
    rclpy.init(args=args)
    node = DummyPointcloudAndStatusPublisher()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()