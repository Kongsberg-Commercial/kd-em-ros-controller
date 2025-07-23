from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration

def generate_launch_description():
    """
    Unified launch file for K-Controller ROS2 Interface
    
    Usage examples:
    1. Default settings:
       ros2 launch ros_kctrl_interface_pkg ros2_ctrl.launch.py
    
    2. Custom IP/ports:
       ros2 launch ros_kctrl_interface_pkg ros2_ctrl.launch.py kctrl_ip:=10.0.0.100
    
    3. YAML config file:
       ros2 launch ros_kctrl_interface_pkg ros2_ctrl.launch.py config_file:=/path/to/config.yaml
    """
    
    # Declare launch arguments
    kctrl_ip_arg = DeclareLaunchArgument(
        'kctrl_ip',
        default_value='192.168.48.1',
        description='IP address of the K-Controller'
    )
    
    kctrl_port_arg = DeclareLaunchArgument(
        'kctrl_port', 
        default_value='4001',
        description='Port for K-Controller communication'
    )
    
    listen_port_arg = DeclareLaunchArgument(
        'listen_port',
        default_value='4002', 
        description='Port for receiving K-Controller messages'
    )
    
    config_file_arg = DeclareLaunchArgument(
        'config_file',
        default_value='',
        description='YAML config file path (overrides other parameters if specified)'
    )

    return LaunchDescription([
        # Arguments
        kctrl_ip_arg,
        kctrl_port_arg, 
        listen_port_arg,
        config_file_arg,
        
        # K-Controller command sender node
        Node(
            package='ros_kctrl_interface_pkg',
            executable='ros2ctrl_wrapper_node',
            name='ros2_ctrl_wrapper_node',
            parameters=[
                LaunchConfiguration('config_file'),  # YAML file (if provided)
                {  # Launch arguments (fallback if no YAML)
                    'kctrl_ip': LaunchConfiguration('kctrl_ip'),
                    'kctrl_port': LaunchConfiguration('kctrl_port')
                }
            ],
            output='screen'
        ),
        
        # K-Controller message receiver node  
        Node(
            package='ros_kctrl_interface_pkg',
            executable='ros2ctrl_udp_receiver',
            name='ros2_udp_receiver',
            parameters=[
                LaunchConfiguration('config_file'),  # YAML file (if provided)
                {  # Launch arguments (fallback if no YAML)
                    'kctrl_ip': LaunchConfiguration('kctrl_ip'),
                    'kctrl_port': LaunchConfiguration('kctrl_port'),
                    'listen_port': LaunchConfiguration('listen_port')
                }
            ],
            output='screen'
        ),
    ])
