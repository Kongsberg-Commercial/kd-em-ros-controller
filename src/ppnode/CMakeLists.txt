cmake_minimum_required(VERSION 3.8)
project(ppnode)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(ros_otter_custom_interfaces REQUIRED)

add_executable(pp_node 
  src/ppnode.cpp
  src/ppnode_impl.cpp
  src/path_planner.cpp
)

ament_target_dependencies(pp_node
  rclcpp
  std_msgs
  geometry_msgs
  ros_otter_custom_interfaces
)

target_include_directories(pp_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

install(TARGETS pp_node 
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()