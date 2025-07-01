// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from ros_kctrl_custom_interfaces:srv/SetPUParameters.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "ros_kctrl_custom_interfaces/srv/set_pu_parameters.h"


#ifndef ROS_KCTRL_CUSTOM_INTERFACES__SRV__DETAIL__SET_PU_PARAMETERS__STRUCT_H_
#define ROS_KCTRL_CUSTOM_INTERFACES__SRV__DETAIL__SET_PU_PARAMETERS__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'sounder_name'
#include "rosidl_runtime_c/string.h"

/// Struct defined in srv/SetPUParameters in the package ros_kctrl_custom_interfaces.
typedef struct ros_kctrl_custom_interfaces__srv__SetPUParameters_Request
{
  /// Request fields here
  rosidl_runtime_c__String sounder_name;
} ros_kctrl_custom_interfaces__srv__SetPUParameters_Request;

// Struct for a sequence of ros_kctrl_custom_interfaces__srv__SetPUParameters_Request.
typedef struct ros_kctrl_custom_interfaces__srv__SetPUParameters_Request__Sequence
{
  ros_kctrl_custom_interfaces__srv__SetPUParameters_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} ros_kctrl_custom_interfaces__srv__SetPUParameters_Request__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'message'
// already included above
// #include "rosidl_runtime_c/string.h"

/// Struct defined in srv/SetPUParameters in the package ros_kctrl_custom_interfaces.
typedef struct ros_kctrl_custom_interfaces__srv__SetPUParameters_Response
{
  bool success;
  rosidl_runtime_c__String message;
} ros_kctrl_custom_interfaces__srv__SetPUParameters_Response;

// Struct for a sequence of ros_kctrl_custom_interfaces__srv__SetPUParameters_Response.
typedef struct ros_kctrl_custom_interfaces__srv__SetPUParameters_Response__Sequence
{
  ros_kctrl_custom_interfaces__srv__SetPUParameters_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} ros_kctrl_custom_interfaces__srv__SetPUParameters_Response__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'info'
#include "service_msgs/msg/detail/service_event_info__struct.h"

// constants for array fields with an upper bound
// request
enum
{
  ros_kctrl_custom_interfaces__srv__SetPUParameters_Event__request__MAX_SIZE = 1
};
// response
enum
{
  ros_kctrl_custom_interfaces__srv__SetPUParameters_Event__response__MAX_SIZE = 1
};

/// Struct defined in srv/SetPUParameters in the package ros_kctrl_custom_interfaces.
typedef struct ros_kctrl_custom_interfaces__srv__SetPUParameters_Event
{
  service_msgs__msg__ServiceEventInfo info;
  ros_kctrl_custom_interfaces__srv__SetPUParameters_Request__Sequence request;
  ros_kctrl_custom_interfaces__srv__SetPUParameters_Response__Sequence response;
} ros_kctrl_custom_interfaces__srv__SetPUParameters_Event;

// Struct for a sequence of ros_kctrl_custom_interfaces__srv__SetPUParameters_Event.
typedef struct ros_kctrl_custom_interfaces__srv__SetPUParameters_Event__Sequence
{
  ros_kctrl_custom_interfaces__srv__SetPUParameters_Event * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} ros_kctrl_custom_interfaces__srv__SetPUParameters_Event__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // ROS_KCTRL_CUSTOM_INTERFACES__SRV__DETAIL__SET_PU_PARAMETERS__STRUCT_H_
