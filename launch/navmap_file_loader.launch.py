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

    maps_manager_node = Node(
        package="navmap_file_loader",
        executable="maps_manager_node",
        name="maps_manager_node",
        output="screen",
        parameters=[config_file],
    )

    return LaunchDescription([
        maps_manager_node,
    ])
