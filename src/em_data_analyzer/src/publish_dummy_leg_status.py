#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from std_msgs.msg import Bool
import time

class DummyLegStatusPublisher(Node):
    def __init__(self):
        super().__init__('dummy_leg_status_publisher')
        self.publisher_ = self.create_publisher(Bool, '/leg_status', 10)
        self.timer_ = self.create_timer(1.0, self.publish_leg_status)
        self.start_time = time.time()
        self.leg_active = False
        self.last_toggle_time = time.time()
        
    def publish_leg_status(self):
        current_time = time.time()
        
        # Toggle leg status every 30 seconds (30s on, 30s off)
        if current_time - self.last_toggle_time > 60.0:
            self.leg_active = not self.leg_active
            self.last_toggle_time = current_time
            
        msg = Bool()
        msg.data = self.leg_active
        self.publisher_.publish(msg)
        
        status = "ACTIVE" if self.leg_active else "INACTIVE"
        # self.get_logger().info(f'Leg status: {status}')

def main(args=None):
    rclpy.init(args=args)
    node = DummyLegStatusPublisher()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
