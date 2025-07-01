// generated from rosidl_generator_c/resource/idl__type_support.h.em
// with input from ros_kctrl_custom_interfaces:srv/StartPinging.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "ros_kctrl_custom_interfaces/srv/start_pinging.h"


#ifndef ROS_KCTRL_CUSTOM_INTERFACES__SRV__DETAIL__START_PINGING__TYPE_SUPPORT_H_
#define ROS_KCTRL_CUSTOM_INTERFACES__SRV__DETAIL__START_PINGING__TYPE_SUPPORT_H_

#include "rosidl_typesupport_interface/macros.h"

#include "ros_kctrl_custom_interfaces/msg/rosidl_generator_c__visibility_control.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "rosidl_runtime_c/message_type_support_struct.h"

// Forward declare the get type support functions for this type.
ROSIDL_GENERATOR_C_PUBLIC_ros_kctrl_custom_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(
  rosidl_typesupport_c,
  ros_kctrl_custom_interfaces,
  srv,
  StartPinging_Request
)(void);

// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"

// Forward declare the get type support functions for this type.
ROSIDL_GENERATOR_C_PUBLIC_ros_kctrl_custom_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(
  rosidl_typesupport_c,
  ros_kctrl_custom_interfaces,
  srv,
  StartPinging_Response
)(void);

// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"

// Forward declare the get type support functions for this type.
ROSIDL_GENERATOR_C_PUBLIC_ros_kctrl_custom_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(
  rosidl_typesupport_c,
  ros_kctrl_custom_interfaces,
  srv,
  StartPinging_Event
)(void);

#include "rosidl_runtime_c/service_type_support_struct.h"

// Forward declare the get type support functions for this type.
ROSIDL_GENERATOR_C_PUBLIC_ros_kctrl_custom_interfaces
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(
  rosidl_typesupport_c,
  ros_kctrl_custom_interfaces,
  srv,
  StartPinging
)(void);

// Forward declare the function to create a service event message for this type.
ROSIDL_GENERATOR_C_PUBLIC_ros_kctrl_custom_interfaces
void *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_CREATE_EVENT_MESSAGE_SYMBOL_NAME(
  rosidl_typesupport_c,
  ros_kctrl_custom_interfaces,
  srv,
  StartPinging
)(
  const rosidl_service_introspection_info_t * info,
  rcutils_allocator_t * allocator,
  const void * request_message,
  const void * response_message);

// Forward declare the function to destroy a service event message for this type.
ROSIDL_GENERATOR_C_PUBLIC_ros_kctrl_custom_interfaces
bool
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_DESTROY_EVENT_MESSAGE_SYMBOL_NAME(
  rosidl_typesupport_c,
  ros_kctrl_custom_interfaces,
  srv,
  StartPinging
)(
  void * event_msg,
  rcutils_allocator_t * allocator);

#ifdef __cplusplus
}
#endif

#endif  // ROS_KCTRL_CUSTOM_INTERFACES__SRV__DETAIL__START_PINGING__TYPE_SUPPORT_H_
