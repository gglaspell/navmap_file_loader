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
        package="easynav_system",        # <-- changed from easynav_maps_manager
        executable="maps_manager_node",
        name="maps_manager_node",
        output="screen",
        parameters=[config_file],
        # Uncomment if downstream EasyNav nodes expect a bare /navmap topic:
        # remappings=[
        #     ("maps_manager_node/navmap/map", "navmap"),
        # ],
    )

    return LaunchDescription([
        maps_manager_node,
    ])
