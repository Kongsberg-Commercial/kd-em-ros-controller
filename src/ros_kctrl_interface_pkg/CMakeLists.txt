cmake_minimum_required(VERSION 3.8)
project(ros_kctrl_interface_pkg)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(Boost REQUIRED COMPONENTS system thread)
find_package(ros_kctrl_custom_interfaces REQUIRED)

include_directories(include)

add_library(ros2ctrl_udp_client src/ros2ctrl_udp_client.cpp)
target_include_directories(ros2ctrl_udp_client PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
ament_target_dependencies(ros2ctrl_udp_client rclcpp std_msgs Boost ros_kctrl_custom_interfaces)

add_executable(ros2ctrl_wrapper_node src/ros2ctrl_wrapper_node.cpp)
target_link_libraries(ros2ctrl_wrapper_node
  ros2ctrl_udp_client
  ${Boost_LIBRARIES}
)
ament_target_dependencies(ros2ctrl_wrapper_node rclcpp std_msgs Boost ros_kctrl_custom_interfaces)

add_executable(ros2ctrl_udp_receiver src/ros2ctrl_udp_receiver.cpp)
target_include_directories(ros2ctrl_udp_receiver PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
ament_target_dependencies(ros2ctrl_udp_receiver rclcpp std_msgs)

install(TARGETS
  ros2ctrl_wrapper_node
  ros2ctrl_udp_client
  ros2ctrl_udp_receiver
  DESTINATION lib/${PROJECT_NAME}
)
install(
  DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

install(DIRECTORY include/
  DESTINATION include/
)

ament_package()