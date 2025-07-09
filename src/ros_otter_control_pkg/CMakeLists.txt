cmake_minimum_required(VERSION 3.8)
project(ros_otter_control_pkg)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(ros_otter_custom_interfaces REQUIRED)

add_executable(otter_tcp_node src/Otter_TCP_node.cpp)
ament_target_dependencies(otter_tcp_node rclcpp std_msgs ros_otter_custom_interfaces)

install(TARGETS otter_tcp_node DESTINATION lib/${PROJECT_NAME})

ament_package()