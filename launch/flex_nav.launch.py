import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node

def generate_launch_description():
    package_dir = get_package_share_directory('sobits_nav')

    declare_robot_name_cmd = DeclareLaunchArgument(
        'robot_name',
        # default_value="sobit_pro",
        # default_value="sobit_edu",
        # default_value="sobit_mini",
        # default_value="sobit_light",
        default_value="hsr_sim",
        # default_value="hsrb_robot",
        description='Choose your used robot name')

    robot_name_config = LaunchConfiguration('robot_name')
    param_file_path = PathJoinSubstitution([
        package_dir, 'param', robot_name_config, 'navigation_config.yaml'
    ])

    return LaunchDescription([
        declare_robot_name_cmd,
        
        Node(
            package='flex_nav',
            executable='head_controller_node',
            name='head_controller_node',
            output='screen',
            parameters=[param_file_path]
        ),
        Node(
            package='flex_nav',
            executable='head_angle_publisher_node',
            name='head_angle_publisher_node', 
            output='screen',
            parameters=[param_file_path]
        )
    ])