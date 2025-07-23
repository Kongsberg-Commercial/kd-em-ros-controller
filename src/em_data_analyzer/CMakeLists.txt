cmake_minimum_required(VERSION 3.8)
project(em_data_analyzer)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(ros_otter_custom_interfaces REQUIRED)

# Include directories
include_directories(include)

# Create library for the analyzer
add_library(pointcloud_analyzer_lib src/pointcloud_analyzer.cpp)
ament_target_dependencies(pointcloud_analyzer_lib rclcpp sensor_msgs geometry_msgs ros_otter_custom_interfaces)
target_link_libraries(pointcloud_analyzer_lib GeographicLib)

# Create executable
add_executable(pointcloud_analyzer_node src/pointcloud_analyzer_main.cpp)
target_link_libraries(pointcloud_analyzer_node pointcloud_analyzer_lib)
ament_target_dependencies(pointcloud_analyzer_node rclcpp)

install(TARGETS
  pointcloud_analyzer_lib
  pointcloud_analyzer_node
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY
  config/
  DESTINATION share/${PROJECT_NAME}/config
)

install(DIRECTORY
  launch/
  DESTINATION share/${PROJECT_NAME}/launch
)

install(DIRECTORY
  include/em_data_analyzer
  DESTINATION include
)

install(PROGRAMS
  src/publish_dummy_gps.py
  DESTINATION lib/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()