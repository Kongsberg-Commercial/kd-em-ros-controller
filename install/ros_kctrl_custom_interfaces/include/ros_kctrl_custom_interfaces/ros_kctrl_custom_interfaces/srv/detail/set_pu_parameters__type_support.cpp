// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from ros_kctrl_custom_interfaces:srv/SetPUParameters.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "ros_kctrl_custom_interfaces/srv/detail/set_pu_parameters__functions.h"
#include "ros_kctrl_custom_interfaces/srv/detail/set_pu_parameters__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace ros_kctrl_custom_interfaces
{

namespace srv
{

namespace rosidl_typesupport_introspection_cpp
{

void SetPUParameters_Request_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) ros_kctrl_custom_interfaces::srv::SetPUParameters_Request(_init);
}

void SetPUParameters_Request_fini_function(void * message_memory)
{
  auto typed_message = static_cast<ros_kctrl_custom_interfaces::srv::SetPUParameters_Request *>(message_memory);
  typed_message->~SetPUParameters_Request();
}

size_t size_function__SetPUParameters_Request__param_names(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<std::string> *>(untyped_member);
  return member->size();
}

const void * get_const_function__SetPUParameters_Request__param_names(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<std::string> *>(untyped_member);
  return &member[index];
}

void * get_function__SetPUParameters_Request__param_names(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<std::string> *>(untyped_member);
  return &member[index];
}

void fetch_function__SetPUParameters_Request__param_names(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const std::string *>(
    get_const_function__SetPUParameters_Request__param_names(untyped_member, index));
  auto & value = *reinterpret_cast<std::string *>(untyped_value);
  value = item;
}

void assign_function__SetPUParameters_Request__param_names(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<std::string *>(
    get_function__SetPUParameters_Request__param_names(untyped_member, index));
  const auto & value = *reinterpret_cast<const std::string *>(untyped_value);
  item = value;
}

void resize_function__SetPUParameters_Request__param_names(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<std::string> *>(untyped_member);
  member->resize(size);
}

size_t size_function__SetPUParameters_Request__param_values(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<std::string> *>(untyped_member);
  return member->size();
}

const void * get_const_function__SetPUParameters_Request__param_values(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<std::string> *>(untyped_member);
  return &member[index];
}

void * get_function__SetPUParameters_Request__param_values(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<std::string> *>(untyped_member);
  return &member[index];
}

void fetch_function__SetPUParameters_Request__param_values(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const std::string *>(
    get_const_function__SetPUParameters_Request__param_values(untyped_member, index));
  auto & value = *reinterpret_cast<std::string *>(untyped_value);
  value = item;
}

void assign_function__SetPUParameters_Request__param_values(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<std::string *>(
    get_function__SetPUParameters_Request__param_values(untyped_member, index));
  const auto & value = *reinterpret_cast<const std::string *>(untyped_value);
  item = value;
}

void resize_function__SetPUParameters_Request__param_values(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<std::string> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember SetPUParameters_Request_message_member_array[3] = {
  {
    "sounder_name",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros_kctrl_custom_interfaces::srv::SetPUParameters_Request, sounder_name),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "param_names",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros_kctrl_custom_interfaces::srv::SetPUParameters_Request, param_names),  // bytes offset in struct
    nullptr,  // default value
    size_function__SetPUParameters_Request__param_names,  // size() function pointer
    get_const_function__SetPUParameters_Request__param_names,  // get_const(index) function pointer
    get_function__SetPUParameters_Request__param_names,  // get(index) function pointer
    fetch_function__SetPUParameters_Request__param_names,  // fetch(index, &value) function pointer
    assign_function__SetPUParameters_Request__param_names,  // assign(index, value) function pointer
    resize_function__SetPUParameters_Request__param_names  // resize(index) function pointer
  },
  {
    "param_values",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros_kctrl_custom_interfaces::srv::SetPUParameters_Request, param_values),  // bytes offset in struct
    nullptr,  // default value
    size_function__SetPUParameters_Request__param_values,  // size() function pointer
    get_const_function__SetPUParameters_Request__param_values,  // get_const(index) function pointer
    get_function__SetPUParameters_Request__param_values,  // get(index) function pointer
    fetch_function__SetPUParameters_Request__param_values,  // fetch(index, &value) function pointer
    assign_function__SetPUParameters_Request__param_values,  // assign(index, value) function pointer
    resize_function__SetPUParameters_Request__param_values  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers SetPUParameters_Request_message_members = {
  "ros_kctrl_custom_interfaces::srv",  // message namespace
  "SetPUParameters_Request",  // message name
  3,  // number of fields
  sizeof(ros_kctrl_custom_interfaces::srv::SetPUParameters_Request),
  false,  // has_any_key_member_
  SetPUParameters_Request_message_member_array,  // message members
  SetPUParameters_Request_init_function,  // function to initialize message memory (memory has to be allocated)
  SetPUParameters_Request_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t SetPUParameters_Request_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &SetPUParameters_Request_message_members,
  get_message_typesupport_handle_function,
  &ros_kctrl_custom_interfaces__srv__SetPUParameters_Request__get_type_hash,
  &ros_kctrl_custom_interfaces__srv__SetPUParameters_Request__get_type_description,
  &ros_kctrl_custom_interfaces__srv__SetPUParameters_Request__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace srv

}  // namespace ros_kctrl_custom_interfaces


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<ros_kctrl_custom_interfaces::srv::SetPUParameters_Request>()
{
  return &::ros_kctrl_custom_interfaces::srv::rosidl_typesupport_introspection_cpp::SetPUParameters_Request_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, ros_kctrl_custom_interfaces, srv, SetPUParameters_Request)() {
  return &::ros_kctrl_custom_interfaces::srv::rosidl_typesupport_introspection_cpp::SetPUParameters_Request_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif

// already included above
// #include "array"
// already included above
// #include "cstddef"
// already included above
// #include "string"
// already included above
// #include "vector"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
// already included above
// #include "rosidl_typesupport_interface/macros.h"
// already included above
// #include "ros_kctrl_custom_interfaces/srv/detail/set_pu_parameters__functions.h"
// already included above
// #include "ros_kctrl_custom_interfaces/srv/detail/set_pu_parameters__struct.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/field_types.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace ros_kctrl_custom_interfaces
{

namespace srv
{

namespace rosidl_typesupport_introspection_cpp
{

void SetPUParameters_Response_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) ros_kctrl_custom_interfaces::srv::SetPUParameters_Response(_init);
}

void SetPUParameters_Response_fini_function(void * message_memory)
{
  auto typed_message = static_cast<ros_kctrl_custom_interfaces::srv::SetPUParameters_Response *>(message_memory);
  typed_message->~SetPUParameters_Response();
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember SetPUParameters_Response_message_member_array[2] = {
  {
    "success",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros_kctrl_custom_interfaces::srv::SetPUParameters_Response, success),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "message",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros_kctrl_custom_interfaces::srv::SetPUParameters_Response, message),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers SetPUParameters_Response_message_members = {
  "ros_kctrl_custom_interfaces::srv",  // message namespace
  "SetPUParameters_Response",  // message name
  2,  // number of fields
  sizeof(ros_kctrl_custom_interfaces::srv::SetPUParameters_Response),
  false,  // has_any_key_member_
  SetPUParameters_Response_message_member_array,  // message members
  SetPUParameters_Response_init_function,  // function to initialize message memory (memory has to be allocated)
  SetPUParameters_Response_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t SetPUParameters_Response_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &SetPUParameters_Response_message_members,
  get_message_typesupport_handle_function,
  &ros_kctrl_custom_interfaces__srv__SetPUParameters_Response__get_type_hash,
  &ros_kctrl_custom_interfaces__srv__SetPUParameters_Response__get_type_description,
  &ros_kctrl_custom_interfaces__srv__SetPUParameters_Response__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace srv

}  // namespace ros_kctrl_custom_interfaces


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<ros_kctrl_custom_interfaces::srv::SetPUParameters_Response>()
{
  return &::ros_kctrl_custom_interfaces::srv::rosidl_typesupport_introspection_cpp::SetPUParameters_Response_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, ros_kctrl_custom_interfaces, srv, SetPUParameters_Response)() {
  return &::ros_kctrl_custom_interfaces::srv::rosidl_typesupport_introspection_cpp::SetPUParameters_Response_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif

// already included above
// #include "array"
// already included above
// #include "cstddef"
// already included above
// #include "string"
// already included above
// #include "vector"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
// already included above
// #include "rosidl_typesupport_interface/macros.h"
// already included above
// #include "ros_kctrl_custom_interfaces/srv/detail/set_pu_parameters__functions.h"
// already included above
// #include "ros_kctrl_custom_interfaces/srv/detail/set_pu_parameters__struct.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/field_types.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace ros_kctrl_custom_interfaces
{

namespace srv
{

namespace rosidl_typesupport_introspection_cpp
{

void SetPUParameters_Event_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) ros_kctrl_custom_interfaces::srv::SetPUParameters_Event(_init);
}

void SetPUParameters_Event_fini_function(void * message_memory)
{
  auto typed_message = static_cast<ros_kctrl_custom_interfaces::srv::SetPUParameters_Event *>(message_memory);
  typed_message->~SetPUParameters_Event();
}

size_t size_function__SetPUParameters_Event__request(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<ros_kctrl_custom_interfaces::srv::SetPUParameters_Request> *>(untyped_member);
  return member->size();
}

const void * get_const_function__SetPUParameters_Event__request(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<ros_kctrl_custom_interfaces::srv::SetPUParameters_Request> *>(untyped_member);
  return &member[index];
}

void * get_function__SetPUParameters_Event__request(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<ros_kctrl_custom_interfaces::srv::SetPUParameters_Request> *>(untyped_member);
  return &member[index];
}

void fetch_function__SetPUParameters_Event__request(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const ros_kctrl_custom_interfaces::srv::SetPUParameters_Request *>(
    get_const_function__SetPUParameters_Event__request(untyped_member, index));
  auto & value = *reinterpret_cast<ros_kctrl_custom_interfaces::srv::SetPUParameters_Request *>(untyped_value);
  value = item;
}

void assign_function__SetPUParameters_Event__request(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<ros_kctrl_custom_interfaces::srv::SetPUParameters_Request *>(
    get_function__SetPUParameters_Event__request(untyped_member, index));
  const auto & value = *reinterpret_cast<const ros_kctrl_custom_interfaces::srv::SetPUParameters_Request *>(untyped_value);
  item = value;
}

void resize_function__SetPUParameters_Event__request(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<ros_kctrl_custom_interfaces::srv::SetPUParameters_Request> *>(untyped_member);
  member->resize(size);
}

size_t size_function__SetPUParameters_Event__response(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<ros_kctrl_custom_interfaces::srv::SetPUParameters_Response> *>(untyped_member);
  return member->size();
}

const void * get_const_function__SetPUParameters_Event__response(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<ros_kctrl_custom_interfaces::srv::SetPUParameters_Response> *>(untyped_member);
  return &member[index];
}

void * get_function__SetPUParameters_Event__response(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<ros_kctrl_custom_interfaces::srv::SetPUParameters_Response> *>(untyped_member);
  return &member[index];
}

void fetch_function__SetPUParameters_Event__response(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const ros_kctrl_custom_interfaces::srv::SetPUParameters_Response *>(
    get_const_function__SetPUParameters_Event__response(untyped_member, index));
  auto & value = *reinterpret_cast<ros_kctrl_custom_interfaces::srv::SetPUParameters_Response *>(untyped_value);
  value = item;
}

void assign_function__SetPUParameters_Event__response(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<ros_kctrl_custom_interfaces::srv::SetPUParameters_Response *>(
    get_function__SetPUParameters_Event__response(untyped_member, index));
  const auto & value = *reinterpret_cast<const ros_kctrl_custom_interfaces::srv::SetPUParameters_Response *>(untyped_value);
  item = value;
}

void resize_function__SetPUParameters_Event__response(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<ros_kctrl_custom_interfaces::srv::SetPUParameters_Response> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember SetPUParameters_Event_message_member_array[3] = {
  {
    "info",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<service_msgs::msg::ServiceEventInfo>(),  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(ros_kctrl_custom_interfaces::srv::SetPUParameters_Event, info),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "request",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<ros_kctrl_custom_interfaces::srv::SetPUParameters_Request>(),  // members of sub message
    false,  // is key
    true,  // is array
    1,  // array size
    true,  // is upper bound
    offsetof(ros_kctrl_custom_interfaces::srv::SetPUParameters_Event, request),  // bytes offset in struct
    nullptr,  // default value
    size_function__SetPUParameters_Event__request,  // size() function pointer
    get_const_function__SetPUParameters_Event__request,  // get_const(index) function pointer
    get_function__SetPUParameters_Event__request,  // get(index) function pointer
    fetch_function__SetPUParameters_Event__request,  // fetch(index, &value) function pointer
    assign_function__SetPUParameters_Event__request,  // assign(index, value) function pointer
    resize_function__SetPUParameters_Event__request  // resize(index) function pointer
  },
  {
    "response",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<ros_kctrl_custom_interfaces::srv::SetPUParameters_Response>(),  // members of sub message
    false,  // is key
    true,  // is array
    1,  // array size
    true,  // is upper bound
    offsetof(ros_kctrl_custom_interfaces::srv::SetPUParameters_Event, response),  // bytes offset in struct
    nullptr,  // default value
    size_function__SetPUParameters_Event__response,  // size() function pointer
    get_const_function__SetPUParameters_Event__response,  // get_const(index) function pointer
    get_function__SetPUParameters_Event__response,  // get(index) function pointer
    fetch_function__SetPUParameters_Event__response,  // fetch(index, &value) function pointer
    assign_function__SetPUParameters_Event__response,  // assign(index, value) function pointer
    resize_function__SetPUParameters_Event__response  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers SetPUParameters_Event_message_members = {
  "ros_kctrl_custom_interfaces::srv",  // message namespace
  "SetPUParameters_Event",  // message name
  3,  // number of fields
  sizeof(ros_kctrl_custom_interfaces::srv::SetPUParameters_Event),
  false,  // has_any_key_member_
  SetPUParameters_Event_message_member_array,  // message members
  SetPUParameters_Event_init_function,  // function to initialize message memory (memory has to be allocated)
  SetPUParameters_Event_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t SetPUParameters_Event_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &SetPUParameters_Event_message_members,
  get_message_typesupport_handle_function,
  &ros_kctrl_custom_interfaces__srv__SetPUParameters_Event__get_type_hash,
  &ros_kctrl_custom_interfaces__srv__SetPUParameters_Event__get_type_description,
  &ros_kctrl_custom_interfaces__srv__SetPUParameters_Event__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace srv

}  // namespace ros_kctrl_custom_interfaces


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<ros_kctrl_custom_interfaces::srv::SetPUParameters_Event>()
{
  return &::ros_kctrl_custom_interfaces::srv::rosidl_typesupport_introspection_cpp::SetPUParameters_Event_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, ros_kctrl_custom_interfaces, srv, SetPUParameters_Event)() {
  return &::ros_kctrl_custom_interfaces::srv::rosidl_typesupport_introspection_cpp::SetPUParameters_Event_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif

// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_cpp/service_type_support.hpp"
// already included above
// #include "rosidl_typesupport_interface/macros.h"
// already included above
// #include "rosidl_typesupport_introspection_cpp/visibility_control.h"
// already included above
// #include "ros_kctrl_custom_interfaces/srv/detail/set_pu_parameters__functions.h"
// already included above
// #include "ros_kctrl_custom_interfaces/srv/detail/set_pu_parameters__struct.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/service_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/service_type_support_decl.hpp"

namespace ros_kctrl_custom_interfaces
{

namespace srv
{

namespace rosidl_typesupport_introspection_cpp
{

// this is intentionally not const to allow initialization later to prevent an initialization race
static ::rosidl_typesupport_introspection_cpp::ServiceMembers SetPUParameters_service_members = {
  "ros_kctrl_custom_interfaces::srv",  // service namespace
  "SetPUParameters",  // service name
  // the following fields are initialized below on first access
  // see get_service_type_support_handle<ros_kctrl_custom_interfaces::srv::SetPUParameters>()
  nullptr,  // request message
  nullptr,  // response message
  nullptr,  // event message
};

static const rosidl_service_type_support_t SetPUParameters_service_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &SetPUParameters_service_members,
  get_service_typesupport_handle_function,
  ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<ros_kctrl_custom_interfaces::srv::SetPUParameters_Request>(),
  ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<ros_kctrl_custom_interfaces::srv::SetPUParameters_Response>(),
  ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<ros_kctrl_custom_interfaces::srv::SetPUParameters_Event>(),
  &::rosidl_typesupport_cpp::service_create_event_message<ros_kctrl_custom_interfaces::srv::SetPUParameters>,
  &::rosidl_typesupport_cpp::service_destroy_event_message<ros_kctrl_custom_interfaces::srv::SetPUParameters>,
  &ros_kctrl_custom_interfaces__srv__SetPUParameters__get_type_hash,
  &ros_kctrl_custom_interfaces__srv__SetPUParameters__get_type_description,
  &ros_kctrl_custom_interfaces__srv__SetPUParameters__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace srv

}  // namespace ros_kctrl_custom_interfaces


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_service_type_support_t *
get_service_type_support_handle<ros_kctrl_custom_interfaces::srv::SetPUParameters>()
{
  // get a handle to the value to be returned
  auto service_type_support =
    &::ros_kctrl_custom_interfaces::srv::rosidl_typesupport_introspection_cpp::SetPUParameters_service_type_support_handle;
  // get a non-const and properly typed version of the data void *
  auto service_members = const_cast<::rosidl_typesupport_introspection_cpp::ServiceMembers *>(
    static_cast<const ::rosidl_typesupport_introspection_cpp::ServiceMembers *>(
      service_type_support->data));
  // make sure all of the service_members are initialized
  // if they are not, initialize them
  if (
    service_members->request_members_ == nullptr ||
    service_members->response_members_ == nullptr ||
    service_members->event_members_ == nullptr)
  {
    // initialize the request_members_ with the static function from the external library
    service_members->request_members_ = static_cast<
      const ::rosidl_typesupport_introspection_cpp::MessageMembers *
      >(
      ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<
        ::ros_kctrl_custom_interfaces::srv::SetPUParameters_Request
      >()->data
      );
    // initialize the response_members_ with the static function from the external library
    service_members->response_members_ = static_cast<
      const ::rosidl_typesupport_introspection_cpp::MessageMembers *
      >(
      ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<
        ::ros_kctrl_custom_interfaces::srv::SetPUParameters_Response
      >()->data
      );
    // initialize the event_members_ with the static function from the external library
    service_members->event_members_ = static_cast<
      const ::rosidl_typesupport_introspection_cpp::MessageMembers *
      >(
      ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<
        ::ros_kctrl_custom_interfaces::srv::SetPUParameters_Event
      >()->data
      );
  }
  // finally return the properly initialized service_type_support handle
  return service_type_support;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, ros_kctrl_custom_interfaces, srv, SetPUParameters)() {
  return ::rosidl_typesupport_introspection_cpp::get_service_type_support_handle<ros_kctrl_custom_interfaces::srv::SetPUParameters>();
}

#ifdef __cplusplus
}
#endif
