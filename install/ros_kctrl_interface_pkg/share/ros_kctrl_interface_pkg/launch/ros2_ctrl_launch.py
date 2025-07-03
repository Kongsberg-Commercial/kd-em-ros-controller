from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='ros_kctrl_interface_pkg',
            executable='ros2ctrl_wrapper_node',
            name='ros2_ctrl_wrapper_node'
        ),
        Node(
            package='ros_kctrl_interface_pkg',
            executable='ros2ctrl_udp_receiver',
            name='ros2_udp_receiver'
        ),
    ])