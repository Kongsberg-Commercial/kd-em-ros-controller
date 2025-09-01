cmake_minimum_required(VERSION 3.8)
project(ros2_kctrl_custom_interfaces)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(std_msgs REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/KctrlVersion.msg"
  "msg/KctrlDevice.msg"
  "msg/KctrlDetectedDevices.msg"
  "msg/KctrlParameter.msg"
  "msg/KctrlParameters.msg"
  "msg/KctrlMulticast.msg"
  "msg/KctrlStatusEntry.msg"
  "msg/KctrlStatus.msg"
  "msg/KctrlInfoWarnError.msg"
  "msg/KctrlDeviceDisconnected.msg"
  "msg/KctrlPUParams.msg"
  "srv/SounderCommand.srv"
  "srv/SetPUParameters.srv"
  "srv/RequestPUParameters.srv"
  "srv/RecordingControl.srv"
  "srv/UpdateRecordingPath.srv"
  "srv/PUParameters.srv"
  "srv/RequestDetectedSounders.srv"
  DEPENDENCIES std_msgs
)
ament_package()