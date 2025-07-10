cmake_minimum_required(VERSION 3.8)
project(ros_otter_control_pkg)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(ros_otter_custom_interfaces REQUIRED)

add_executable(otter_tcp_node src/Otter_TCP_node.cpp)
ament_target_dependencies(otter_tcp_node
  rclcpp
  ros_otter_custom_interfaces
)

target_include_directories(otter_tcp_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

install(TARGETS otter_tcp_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()