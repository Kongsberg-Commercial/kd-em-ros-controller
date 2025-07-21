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

add_executable(pointcloud_analyzer_node src/pointcloud_analyzer_node.cpp)
ament_target_dependencies(pointcloud_analyzer_node rclcpp sensor_msgs geometry_msgs ros_otter_custom_interfaces)

install(TARGETS
  pointcloud_analyzer_node
  DESTINATION lib/${PROJECT_NAME}
)

# Install Python dummy publisher
install(PROGRAMS
  src/dummy_pointcloud_publisher.py
  src/accumulate_swaths_node.py
  DESTINATION lib/${PROJECT_NAME}
)

#install(DIRECTORY
#  launch
#  DESTINATION share/${PROJECT_NAME}/
#)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()