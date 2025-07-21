cmake_minimum_required(VERSION 3.8)
project(path_publisher)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(ros_otter_custom_interfaces REQUIRED)

add_executable(leg_publisher src/leg_publisher.cpp)

ament_target_dependencies(leg_publisher
  rclcpp
  std_msgs
  geometry_msgs
  ros_otter_custom_interfaces
)

target_include_directories(leg_publisher PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

install(TARGETS leg_publisher
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()