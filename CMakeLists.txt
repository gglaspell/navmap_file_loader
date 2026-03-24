cmake_minimum_required(VERSION 3.8)
project(navmap_file_loader)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(pluginlib REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(lifecycle_msgs REQUIRED)
find_package(easynav_core REQUIRED)
find_package(easynav_maps_manager REQUIRED)
find_package(navmap_core REQUIRED)
find_package(navmap_ros REQUIRED)
find_package(navmap_ros_interfaces REQUIRED)

# Plugin library
add_library(${PROJECT_NAME} SHARED
  src/NavmapFileLoader.cpp
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME} PUBLIC
  pluginlib::pluginlib
  rclcpp::rclcpp
  easynav_core::easynav_core
  navmap_core::navmap_core
  navmap_ros::navmap_ros
  ${navmap_ros_interfaces_TARGETS}
)

pluginlib_export_plugin_description_file(easynav_core navmap_file_loader_plugins.xml)

# Standalone maps_manager_node executable
add_executable(maps_manager_node
  src/maps_manager_main.cpp
)

target_link_libraries(maps_manager_node PRIVATE
  rclcpp::rclcpp
  rclcpp_lifecycle::rclcpp_lifecycle
  ${lifecycle_msgs_TARGETS}
  easynav_common::easynav_common
  easynav_core::easynav_core
  easynav_maps_manager::easynav_maps_manager
)

install(
  TARGETS ${PROJECT_NAME} maps_manager_node
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)

install(
  DIRECTORY include/
  DESTINATION include
)

install(
  FILES navmap_file_loader_plugins.xml
  DESTINATION share/${PROJECT_NAME}
)

install(
  DIRECTORY launch config
  DESTINATION share/${PROJECT_NAME}
)

ament_export_targets(export_${PROJECT_NAME} HAS_LIBRARY_TARGET)
ament_export_dependencies(
  pluginlib
  rclcpp
  easynav_core
  navmap_core
  navmap_ros
  navmap_ros_interfaces
)

ament_package()
