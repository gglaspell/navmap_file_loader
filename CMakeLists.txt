cmake_minimum_required(VERSION 3.8)
project(navmap_file_loader)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(pluginlib REQUIRED)
find_package(rclcpp REQUIRED)
find_package(easynav_core REQUIRED)
find_package(navmap_core REQUIRED)
find_package(navmap_ros REQUIRED)
find_package(navmap_ros_interfaces REQUIRED)

add_library(${PROJECT_NAME} SHARED
  src/NavmapFileLoader.cpp
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

ament_target_dependencies(${PROJECT_NAME}
  pluginlib
  rclcpp
  easynav_core
  navmap_core
  navmap_ros
  navmap_ros_interfaces
)

pluginlib_export_plugin_description_file(easynav_core navmap_file_loader_plugins.xml)

install(
  TARGETS ${PROJECT_NAME}
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

ament_export_targets(export_${PROJECT_NAME})

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

ament_export_include_directories(include)
ament_export_libraries(${PROJECT_NAME})
ament_export_dependencies(
  pluginlib
  rclcpp
  easynav_core
  navmap_core
  navmap_ros
  navmap_ros_interfaces
)

ament_package()
