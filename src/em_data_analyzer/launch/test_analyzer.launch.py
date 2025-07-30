from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import ExecuteProcess
import os
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    # Get the path to the config file
    config_dir = os.path.join(get_package_share_directory('em_data_analyzer'), 'config')
    config_file = os.path.join(config_dir, 'analyzer_config.yaml')
    
    return LaunchDescription([
        # Dummy GPS publisher (if you need GPS simulation)
        Node(
            package='em_data_analyzer',
            executable='publish_dummy_gps.py',
            name='dummy_gps_publisher',
            output='screen'
        ),
        
        # Dummy Leg Status publisher
        Node(
            package='em_data_analyzer',
            executable='publish_dummy_leg_status.py',
            name='dummy_leg_status_publisher',
            output='screen'
        ),
        
        # PointCloud Analyzer
        Node(
            package='em_data_analyzer',
            executable='pointcloud_analyzer',
            name='pointcloud_analyzer',
            parameters=[config_file],
            output='screen'
        ),
        
        # RViz with custom config
        Node(
            package='rviz2',
            executable='rviz2',
            name='rviz2',
            arguments=['-d', os.path.join(config_dir, 'analyzer_rviz.rviz')],
            output='screen'
        )
    ])
