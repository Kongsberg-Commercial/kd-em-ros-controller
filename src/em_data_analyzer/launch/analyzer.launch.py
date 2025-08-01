from launch import LaunchDescription
from launch_ros.actions import Node
from launch.substitutions import LaunchConfiguration
from launch.actions import DeclareLaunchArgument
import os
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    # Get the path to the config file
    config_file = os.path.join(
        get_package_share_directory('em_data_analyzer'),
        'config',
        'analyzer_config.yaml'
    )
    
    # Declare launch arguments
    config_file_arg = DeclareLaunchArgument(
        'config_file',
        default_value=config_file,
        description='Path to the analyzer configuration file'
    )
    
    log_level_arg = DeclareLaunchArgument(
        'log_level',
        default_value='info',
        description='Log level for the analyzer node'
    )
    
    # Create the analyzer node
    analyzer_node = Node(
        package='em_data_analyzer',
        executable='pointcloud_analyzer_node',
        name='pointcloud_analyzer',
        parameters=[LaunchConfiguration('config_file')],
        arguments=['--ros-args', '--log-level', LaunchConfiguration('log_level')],
        output='screen'
    )
    
    return LaunchDescription([
        config_file_arg,
        log_level_arg,
        analyzer_node
    ])
