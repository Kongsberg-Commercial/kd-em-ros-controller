// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from ros_kctrl_custom_interfaces:srv/StopPinging.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "ros_kctrl_custom_interfaces/srv/detail/stop_pinging__rosidl_typesupport_introspection_c.h"
#include "ros_kctrl_custom_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "ros_kctrl_custom_interfaces/srv/detail/stop_pinging__functions.h"
#include "ros_kctrl_custom_interfaces/srv/detail/stop_pinging__struct.h"


// Include directives for member types
// Member `sounder_name`
#include "rosidl_runtime_c/string_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void ros_kctrl_custom_interfaces__srv__StopPinging_Request__rosidl_typesupport_introspection_c__StopPinging_Request_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  ros_kctrl_custom_interfaces__srv__StopPinging_Request__init(message_memory);
}

void ros_kctrl_custom_interfaces__srv__StopPinging_Request__rosidl_typesupport_introspection_c__StopPinging_Request_fini_function(void * message_memory)
{
  ros_kctrl_custom_interfaces__srv__StopPinging_Request__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember ros_kctrl_custom_interfaces__srv__StopPinging_Request__rosidl_typesupport_introspection_c__StopPinging_Request_message_member_array[1] = {
  {
    "sounder_name",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros_kctrl_custom_interfaces__srv__StopPinging_Request, sounder_name),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers ros_kctrl_custom_interfaces__srv__StopPinging_Request__rosidl_typesupport_introspection_c__StopPinging_Request_message_members = {
  "ros_kctrl_custom_interfaces__srv",  // message namespace
  "StopPinging_Request",  // message name
  1,  // number of fields
  sizeof(ros_kctrl_custom_interfaces__srv__StopPinging_Request),
  false,  // has_any_key_member_
  ros_kctrl_custom_interfaces__srv__StopPinging_Request__rosidl_typesupport_introspection_c__StopPinging_Request_message_member_array,  // message members
  ros_kctrl_custom_interfaces__srv__StopPinging_Request__rosidl_typesupport_introspection_c__StopPinging_Request_init_function,  // function to initialize message memory (memory has to be allocated)
  ros_kctrl_custom_interfaces__srv__StopPinging_Request__rosidl_typesupport_introspection_c__StopPinging_Request_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t ros_kctrl_custom_interfaces__srv__StopPinging_Request__rosidl_typesupport_introspection_c__StopPinging_Request_message_type_support_handle = {
  0,
  &ros_kctrl_custom_interfaces__srv__StopPinging_Request__rosidl_typesupport_introspection_c__StopPinging_Request_message_members,
  get_message_typesupport_handle_function,
  &ros_kctrl_custom_interfaces__srv__StopPinging_Request__get_type_hash,
  &ros_kctrl_custom_interfaces__srv__StopPinging_Request__get_type_description,
  &ros_kctrl_custom_interfaces__srv__StopPinging_Request__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_ros_kctrl_custom_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros_kctrl_custom_interfaces, srv, StopPinging_Request)() {
  if (!ros_kctrl_custom_interfaces__srv__StopPinging_Request__rosidl_typesupport_introspection_c__StopPinging_Request_message_type_support_handle.typesupport_identifier) {
    ros_kctrl_custom_interfaces__srv__StopPinging_Request__rosidl_typesupport_introspection_c__StopPinging_Request_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &ros_kctrl_custom_interfaces__srv__StopPinging_Request__rosidl_typesupport_introspection_c__StopPinging_Request_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "ros_kctrl_custom_interfaces/srv/detail/stop_pinging__rosidl_typesupport_introspection_c.h"
// already included above
// #include "ros_kctrl_custom_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "ros_kctrl_custom_interfaces/srv/detail/stop_pinging__functions.h"
// already included above
// #include "ros_kctrl_custom_interfaces/srv/detail/stop_pinging__struct.h"


// Include directives for member types
// Member `message`
// already included above
// #include "rosidl_runtime_c/string_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void ros_kctrl_custom_interfaces__srv__StopPinging_Response__rosidl_typesupport_introspection_c__StopPinging_Response_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  ros_kctrl_custom_interfaces__srv__StopPinging_Response__init(message_memory);
}

void ros_kctrl_custom_interfaces__srv__StopPinging_Response__rosidl_typesupport_introspection_c__StopPinging_Response_fini_function(void * message_memory)
{
  ros_kctrl_custom_interfaces__srv__StopPinging_Response__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember ros_kctrl_custom_interfaces__srv__StopPinging_Response__rosidl_typesupport_introspection_c__StopPinging_Response_message_member_array[2] = {
  {
    "success",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros_kctrl_custom_interfaces__srv__StopPinging_Response, success),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "message",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros_kctrl_custom_interfaces__srv__StopPinging_Response, message),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers ros_kctrl_custom_interfaces__srv__StopPinging_Response__rosidl_typesupport_introspection_c__StopPinging_Response_message_members = {
  "ros_kctrl_custom_interfaces__srv",  // message namespace
  "StopPinging_Response",  // message name
  2,  // number of fields
  sizeof(ros_kctrl_custom_interfaces__srv__StopPinging_Response),
  false,  // has_any_key_member_
  ros_kctrl_custom_interfaces__srv__StopPinging_Response__rosidl_typesupport_introspection_c__StopPinging_Response_message_member_array,  // message members
  ros_kctrl_custom_interfaces__srv__StopPinging_Response__rosidl_typesupport_introspection_c__StopPinging_Response_init_function,  // function to initialize message memory (memory has to be allocated)
  ros_kctrl_custom_interfaces__srv__StopPinging_Response__rosidl_typesupport_introspection_c__StopPinging_Response_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t ros_kctrl_custom_interfaces__srv__StopPinging_Response__rosidl_typesupport_introspection_c__StopPinging_Response_message_type_support_handle = {
  0,
  &ros_kctrl_custom_interfaces__srv__StopPinging_Response__rosidl_typesupport_introspection_c__StopPinging_Response_message_members,
  get_message_typesupport_handle_function,
  &ros_kctrl_custom_interfaces__srv__StopPinging_Response__get_type_hash,
  &ros_kctrl_custom_interfaces__srv__StopPinging_Response__get_type_description,
  &ros_kctrl_custom_interfaces__srv__StopPinging_Response__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_ros_kctrl_custom_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros_kctrl_custom_interfaces, srv, StopPinging_Response)() {
  if (!ros_kctrl_custom_interfaces__srv__StopPinging_Response__rosidl_typesupport_introspection_c__StopPinging_Response_message_type_support_handle.typesupport_identifier) {
    ros_kctrl_custom_interfaces__srv__StopPinging_Response__rosidl_typesupport_introspection_c__StopPinging_Response_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &ros_kctrl_custom_interfaces__srv__StopPinging_Response__rosidl_typesupport_introspection_c__StopPinging_Response_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "ros_kctrl_custom_interfaces/srv/detail/stop_pinging__rosidl_typesupport_introspection_c.h"
// already included above
// #include "ros_kctrl_custom_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "ros_kctrl_custom_interfaces/srv/detail/stop_pinging__functions.h"
// already included above
// #include "ros_kctrl_custom_interfaces/srv/detail/stop_pinging__struct.h"


// Include directives for member types
// Member `info`
#include "service_msgs/msg/service_event_info.h"
// Member `info`
#include "service_msgs/msg/detail/service_event_info__rosidl_typesupport_introspection_c.h"
// Member `request`
// Member `response`
#include "ros_kctrl_custom_interfaces/srv/stop_pinging.h"
// Member `request`
// Member `response`
// already included above
// #include "ros_kctrl_custom_interfaces/srv/detail/stop_pinging__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__StopPinging_Event_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  ros_kctrl_custom_interfaces__srv__StopPinging_Event__init(message_memory);
}

void ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__StopPinging_Event_fini_function(void * message_memory)
{
  ros_kctrl_custom_interfaces__srv__StopPinging_Event__fini(message_memory);
}

size_t ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__size_function__StopPinging_Event__request(
  const void * untyped_member)
{
  const ros_kctrl_custom_interfaces__srv__StopPinging_Request__Sequence * member =
    (const ros_kctrl_custom_interfaces__srv__StopPinging_Request__Sequence *)(untyped_member);
  return member->size;
}

const void * ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__get_const_function__StopPinging_Event__request(
  const void * untyped_member, size_t index)
{
  const ros_kctrl_custom_interfaces__srv__StopPinging_Request__Sequence * member =
    (const ros_kctrl_custom_interfaces__srv__StopPinging_Request__Sequence *)(untyped_member);
  return &member->data[index];
}

void * ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__get_function__StopPinging_Event__request(
  void * untyped_member, size_t index)
{
  ros_kctrl_custom_interfaces__srv__StopPinging_Request__Sequence * member =
    (ros_kctrl_custom_interfaces__srv__StopPinging_Request__Sequence *)(untyped_member);
  return &member->data[index];
}

void ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__fetch_function__StopPinging_Event__request(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const ros_kctrl_custom_interfaces__srv__StopPinging_Request * item =
    ((const ros_kctrl_custom_interfaces__srv__StopPinging_Request *)
    ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__get_const_function__StopPinging_Event__request(untyped_member, index));
  ros_kctrl_custom_interfaces__srv__StopPinging_Request * value =
    (ros_kctrl_custom_interfaces__srv__StopPinging_Request *)(untyped_value);
  *value = *item;
}

void ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__assign_function__StopPinging_Event__request(
  void * untyped_member, size_t index, const void * untyped_value)
{
  ros_kctrl_custom_interfaces__srv__StopPinging_Request * item =
    ((ros_kctrl_custom_interfaces__srv__StopPinging_Request *)
    ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__get_function__StopPinging_Event__request(untyped_member, index));
  const ros_kctrl_custom_interfaces__srv__StopPinging_Request * value =
    (const ros_kctrl_custom_interfaces__srv__StopPinging_Request *)(untyped_value);
  *item = *value;
}

bool ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__resize_function__StopPinging_Event__request(
  void * untyped_member, size_t size)
{
  ros_kctrl_custom_interfaces__srv__StopPinging_Request__Sequence * member =
    (ros_kctrl_custom_interfaces__srv__StopPinging_Request__Sequence *)(untyped_member);
  ros_kctrl_custom_interfaces__srv__StopPinging_Request__Sequence__fini(member);
  return ros_kctrl_custom_interfaces__srv__StopPinging_Request__Sequence__init(member, size);
}

size_t ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__size_function__StopPinging_Event__response(
  const void * untyped_member)
{
  const ros_kctrl_custom_interfaces__srv__StopPinging_Response__Sequence * member =
    (const ros_kctrl_custom_interfaces__srv__StopPinging_Response__Sequence *)(untyped_member);
  return member->size;
}

const void * ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__get_const_function__StopPinging_Event__response(
  const void * untyped_member, size_t index)
{
  const ros_kctrl_custom_interfaces__srv__StopPinging_Response__Sequence * member =
    (const ros_kctrl_custom_interfaces__srv__StopPinging_Response__Sequence *)(untyped_member);
  return &member->data[index];
}

void * ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__get_function__StopPinging_Event__response(
  void * untyped_member, size_t index)
{
  ros_kctrl_custom_interfaces__srv__StopPinging_Response__Sequence * member =
    (ros_kctrl_custom_interfaces__srv__StopPinging_Response__Sequence *)(untyped_member);
  return &member->data[index];
}

void ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__fetch_function__StopPinging_Event__response(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const ros_kctrl_custom_interfaces__srv__StopPinging_Response * item =
    ((const ros_kctrl_custom_interfaces__srv__StopPinging_Response *)
    ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__get_const_function__StopPinging_Event__response(untyped_member, index));
  ros_kctrl_custom_interfaces__srv__StopPinging_Response * value =
    (ros_kctrl_custom_interfaces__srv__StopPinging_Response *)(untyped_value);
  *value = *item;
}

void ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__assign_function__StopPinging_Event__response(
  void * untyped_member, size_t index, const void * untyped_value)
{
  ros_kctrl_custom_interfaces__srv__StopPinging_Response * item =
    ((ros_kctrl_custom_interfaces__srv__StopPinging_Response *)
    ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__get_function__StopPinging_Event__response(untyped_member, index));
  const ros_kctrl_custom_interfaces__srv__StopPinging_Response * value =
    (const ros_kctrl_custom_interfaces__srv__StopPinging_Response *)(untyped_value);
  *item = *value;
}

bool ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__resize_function__StopPinging_Event__response(
  void * untyped_member, size_t size)
{
  ros_kctrl_custom_interfaces__srv__StopPinging_Response__Sequence * member =
    (ros_kctrl_custom_interfaces__srv__StopPinging_Response__Sequence *)(untyped_member);
  ros_kctrl_custom_interfaces__srv__StopPinging_Response__Sequence__fini(member);
  return ros_kctrl_custom_interfaces__srv__StopPinging_Response__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__StopPinging_Event_message_member_array[3] = {
  {
    "info",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros_kctrl_custom_interfaces__srv__StopPinging_Event, info),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "request",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    true,  // is array
    1,  // array size
    true,  // is upper bound
    offsetof(ros_kctrl_custom_interfaces__srv__StopPinging_Event, request),  // bytes offset in struct
    NULL,  // default value
    ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__size_function__StopPinging_Event__request,  // size() function pointer
    ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__get_const_function__StopPinging_Event__request,  // get_const(index) function pointer
    ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__get_function__StopPinging_Event__request,  // get(index) function pointer
    ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__fetch_function__StopPinging_Event__request,  // fetch(index, &value) function pointer
    ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__assign_function__StopPinging_Event__request,  // assign(index, value) function pointer
    ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__resize_function__StopPinging_Event__request  // resize(index) function pointer
  },
  {
    "response",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    true,  // is array
    1,  // array size
    true,  // is upper bound
    offsetof(ros_kctrl_custom_interfaces__srv__StopPinging_Event, response),  // bytes offset in struct
    NULL,  // default value
    ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__size_function__StopPinging_Event__response,  // size() function pointer
    ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__get_const_function__StopPinging_Event__response,  // get_const(index) function pointer
    ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__get_function__StopPinging_Event__response,  // get(index) function pointer
    ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__fetch_function__StopPinging_Event__response,  // fetch(index, &value) function pointer
    ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__assign_function__StopPinging_Event__response,  // assign(index, value) function pointer
    ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__resize_function__StopPinging_Event__response  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__StopPinging_Event_message_members = {
  "ros_kctrl_custom_interfaces__srv",  // message namespace
  "StopPinging_Event",  // message name
  3,  // number of fields
  sizeof(ros_kctrl_custom_interfaces__srv__StopPinging_Event),
  false,  // has_any_key_member_
  ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__StopPinging_Event_message_member_array,  // message members
  ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__StopPinging_Event_init_function,  // function to initialize message memory (memory has to be allocated)
  ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__StopPinging_Event_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__StopPinging_Event_message_type_support_handle = {
  0,
  &ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__StopPinging_Event_message_members,
  get_message_typesupport_handle_function,
  &ros_kctrl_custom_interfaces__srv__StopPinging_Event__get_type_hash,
  &ros_kctrl_custom_interfaces__srv__StopPinging_Event__get_type_description,
  &ros_kctrl_custom_interfaces__srv__StopPinging_Event__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_ros_kctrl_custom_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros_kctrl_custom_interfaces, srv, StopPinging_Event)() {
  ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__StopPinging_Event_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, service_msgs, msg, ServiceEventInfo)();
  ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__StopPinging_Event_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros_kctrl_custom_interfaces, srv, StopPinging_Request)();
  ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__StopPinging_Event_message_member_array[2].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros_kctrl_custom_interfaces, srv, StopPinging_Response)();
  if (!ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__StopPinging_Event_message_type_support_handle.typesupport_identifier) {
    ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__StopPinging_Event_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__StopPinging_Event_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

#include "rosidl_runtime_c/service_type_support_struct.h"
// already included above
// #include "ros_kctrl_custom_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "ros_kctrl_custom_interfaces/srv/detail/stop_pinging__rosidl_typesupport_introspection_c.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/service_introspection.h"

// this is intentionally not const to allow initialization later to prevent an initialization race
static rosidl_typesupport_introspection_c__ServiceMembers ros_kctrl_custom_interfaces__srv__detail__stop_pinging__rosidl_typesupport_introspection_c__StopPinging_service_members = {
  "ros_kctrl_custom_interfaces__srv",  // service namespace
  "StopPinging",  // service name
  // the following fields are initialized below on first access
  NULL,  // request message
  // ros_kctrl_custom_interfaces__srv__detail__stop_pinging__rosidl_typesupport_introspection_c__StopPinging_Request_message_type_support_handle,
  NULL,  // response message
  // ros_kctrl_custom_interfaces__srv__detail__stop_pinging__rosidl_typesupport_introspection_c__StopPinging_Response_message_type_support_handle
  NULL  // event_message
  // ros_kctrl_custom_interfaces__srv__detail__stop_pinging__rosidl_typesupport_introspection_c__StopPinging_Response_message_type_support_handle
};


static rosidl_service_type_support_t ros_kctrl_custom_interfaces__srv__detail__stop_pinging__rosidl_typesupport_introspection_c__StopPinging_service_type_support_handle = {
  0,
  &ros_kctrl_custom_interfaces__srv__detail__stop_pinging__rosidl_typesupport_introspection_c__StopPinging_service_members,
  get_service_typesupport_handle_function,
  &ros_kctrl_custom_interfaces__srv__StopPinging_Request__rosidl_typesupport_introspection_c__StopPinging_Request_message_type_support_handle,
  &ros_kctrl_custom_interfaces__srv__StopPinging_Response__rosidl_typesupport_introspection_c__StopPinging_Response_message_type_support_handle,
  &ros_kctrl_custom_interfaces__srv__StopPinging_Event__rosidl_typesupport_introspection_c__StopPinging_Event_message_type_support_handle,
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_CREATE_EVENT_MESSAGE_SYMBOL_NAME(
    rosidl_typesupport_c,
    ros_kctrl_custom_interfaces,
    srv,
    StopPinging
  ),
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_DESTROY_EVENT_MESSAGE_SYMBOL_NAME(
    rosidl_typesupport_c,
    ros_kctrl_custom_interfaces,
    srv,
    StopPinging
  ),
  &ros_kctrl_custom_interfaces__srv__StopPinging__get_type_hash,
  &ros_kctrl_custom_interfaces__srv__StopPinging__get_type_description,
  &ros_kctrl_custom_interfaces__srv__StopPinging__get_type_description_sources,
};

// Forward declaration of message type support functions for service members
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros_kctrl_custom_interfaces, srv, StopPinging_Request)(void);

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros_kctrl_custom_interfaces, srv, StopPinging_Response)(void);

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros_kctrl_custom_interfaces, srv, StopPinging_Event)(void);

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_ros_kctrl_custom_interfaces
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros_kctrl_custom_interfaces, srv, StopPinging)(void) {
  if (!ros_kctrl_custom_interfaces__srv__detail__stop_pinging__rosidl_typesupport_introspection_c__StopPinging_service_type_support_handle.typesupport_identifier) {
    ros_kctrl_custom_interfaces__srv__detail__stop_pinging__rosidl_typesupport_introspection_c__StopPinging_service_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  rosidl_typesupport_introspection_c__ServiceMembers * service_members =
    (rosidl_typesupport_introspection_c__ServiceMembers *)ros_kctrl_custom_interfaces__srv__detail__stop_pinging__rosidl_typesupport_introspection_c__StopPinging_service_type_support_handle.data;

  if (!service_members->request_members_) {
    service_members->request_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros_kctrl_custom_interfaces, srv, StopPinging_Request)()->data;
  }
  if (!service_members->response_members_) {
    service_members->response_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros_kctrl_custom_interfaces, srv, StopPinging_Response)()->data;
  }
  if (!service_members->event_members_) {
    service_members->event_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros_kctrl_custom_interfaces, srv, StopPinging_Event)()->data;
  }

  return &ros_kctrl_custom_interfaces__srv__detail__stop_pinging__rosidl_typesupport_introspection_c__StopPinging_service_type_support_handle;
}
