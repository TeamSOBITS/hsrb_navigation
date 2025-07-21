from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='nav2_head_controller',
            executable='head_controller_node',
            name='nav2_head_controller',
            output='screen'
        ),
        Node(
            package='nav2_head_controller',
            executable='head_angle_publisher_node',
            name='head_angle_publisher',
            output='screen'
        )
    ])
