import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    config_file = os.path.join(
        get_package_share_directory("navmap_file_loader"),
        "config",
        "navmap_file_loader.yaml",
    )

    system_node = Node(
        package="easynav_system",
        executable="system_main",
        name="easynav_system",
        output="screen",
        parameters=[config_file],
    )

    return LaunchDescription([
        system_node,
    ])
