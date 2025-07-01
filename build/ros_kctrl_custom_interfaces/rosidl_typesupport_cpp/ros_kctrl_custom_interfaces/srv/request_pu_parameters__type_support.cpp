// generated from rosidl_typesupport_cpp/resource/idl__type_support.cpp.em
// with input from ros_kctrl_custom_interfaces:srv/RequestPUParameters.idl
// generated code does not contain a copyright notice

#include "cstddef"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "ros_kctrl_custom_interfaces/srv/detail/request_pu_parameters__functions.h"
#include "ros_kctrl_custom_interfaces/srv/detail/request_pu_parameters__struct.hpp"
#include "rosidl_typesupport_cpp/identifier.hpp"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_c/type_support_map.h"
#include "rosidl_typesupport_cpp/message_type_support_dispatch.hpp"
#include "rosidl_typesupport_cpp/visibility_control.h"
#include "rosidl_typesupport_interface/macros.h"

namespace ros_kctrl_custom_interfaces
{

namespace srv
{

namespace rosidl_typesupport_cpp
{

typedef struct _RequestPUParameters_Request_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _RequestPUParameters_Request_type_support_ids_t;

static const _RequestPUParameters_Request_type_support_ids_t _RequestPUParameters_Request_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _RequestPUParameters_Request_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _RequestPUParameters_Request_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _RequestPUParameters_Request_type_support_symbol_names_t _RequestPUParameters_Request_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, ros_kctrl_custom_interfaces, srv, RequestPUParameters_Request)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, ros_kctrl_custom_interfaces, srv, RequestPUParameters_Request)),
  }
};

typedef struct _RequestPUParameters_Request_type_support_data_t
{
  void * data[2];
} _RequestPUParameters_Request_type_support_data_t;

static _RequestPUParameters_Request_type_support_data_t _RequestPUParameters_Request_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _RequestPUParameters_Request_message_typesupport_map = {
  2,
  "ros_kctrl_custom_interfaces",
  &_RequestPUParameters_Request_message_typesupport_ids.typesupport_identifier[0],
  &_RequestPUParameters_Request_message_typesupport_symbol_names.symbol_name[0],
  &_RequestPUParameters_Request_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t RequestPUParameters_Request_message_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_RequestPUParameters_Request_message_typesupport_map),
  ::rosidl_typesupport_cpp::get_message_typesupport_handle_function,
  &ros_kctrl_custom_interfaces__srv__RequestPUParameters_Request__get_type_hash,
  &ros_kctrl_custom_interfaces__srv__RequestPUParameters_Request__get_type_description,
  &ros_kctrl_custom_interfaces__srv__RequestPUParameters_Request__get_type_description_sources,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace srv

}  // namespace ros_kctrl_custom_interfaces

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Request>()
{
  return &::ros_kctrl_custom_interfaces::srv::rosidl_typesupport_cpp::RequestPUParameters_Request_message_type_support_handle;
}

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_cpp, ros_kctrl_custom_interfaces, srv, RequestPUParameters_Request)() {
  return get_message_type_support_handle<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Request>();
}

#ifdef __cplusplus
}
#endif
}  // namespace rosidl_typesupport_cpp

// already included above
// #include "cstddef"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "ros_kctrl_custom_interfaces/srv/detail/request_pu_parameters__functions.h"
// already included above
// #include "ros_kctrl_custom_interfaces/srv/detail/request_pu_parameters__struct.hpp"
// already included above
// #include "rosidl_typesupport_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support_dispatch.hpp"
// already included above
// #include "rosidl_typesupport_cpp/visibility_control.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"

namespace ros_kctrl_custom_interfaces
{

namespace srv
{

namespace rosidl_typesupport_cpp
{

typedef struct _RequestPUParameters_Response_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _RequestPUParameters_Response_type_support_ids_t;

static const _RequestPUParameters_Response_type_support_ids_t _RequestPUParameters_Response_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _RequestPUParameters_Response_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _RequestPUParameters_Response_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _RequestPUParameters_Response_type_support_symbol_names_t _RequestPUParameters_Response_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, ros_kctrl_custom_interfaces, srv, RequestPUParameters_Response)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, ros_kctrl_custom_interfaces, srv, RequestPUParameters_Response)),
  }
};

typedef struct _RequestPUParameters_Response_type_support_data_t
{
  void * data[2];
} _RequestPUParameters_Response_type_support_data_t;

static _RequestPUParameters_Response_type_support_data_t _RequestPUParameters_Response_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _RequestPUParameters_Response_message_typesupport_map = {
  2,
  "ros_kctrl_custom_interfaces",
  &_RequestPUParameters_Response_message_typesupport_ids.typesupport_identifier[0],
  &_RequestPUParameters_Response_message_typesupport_symbol_names.symbol_name[0],
  &_RequestPUParameters_Response_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t RequestPUParameters_Response_message_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_RequestPUParameters_Response_message_typesupport_map),
  ::rosidl_typesupport_cpp::get_message_typesupport_handle_function,
  &ros_kctrl_custom_interfaces__srv__RequestPUParameters_Response__get_type_hash,
  &ros_kctrl_custom_interfaces__srv__RequestPUParameters_Response__get_type_description,
  &ros_kctrl_custom_interfaces__srv__RequestPUParameters_Response__get_type_description_sources,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace srv

}  // namespace ros_kctrl_custom_interfaces

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Response>()
{
  return &::ros_kctrl_custom_interfaces::srv::rosidl_typesupport_cpp::RequestPUParameters_Response_message_type_support_handle;
}

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_cpp, ros_kctrl_custom_interfaces, srv, RequestPUParameters_Response)() {
  return get_message_type_support_handle<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Response>();
}

#ifdef __cplusplus
}
#endif
}  // namespace rosidl_typesupport_cpp

// already included above
// #include "cstddef"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "ros_kctrl_custom_interfaces/srv/detail/request_pu_parameters__functions.h"
// already included above
// #include "ros_kctrl_custom_interfaces/srv/detail/request_pu_parameters__struct.hpp"
// already included above
// #include "rosidl_typesupport_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support.hpp"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
// already included above
// #include "rosidl_typesupport_cpp/message_type_support_dispatch.hpp"
// already included above
// #include "rosidl_typesupport_cpp/visibility_control.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"

namespace ros_kctrl_custom_interfaces
{

namespace srv
{

namespace rosidl_typesupport_cpp
{

typedef struct _RequestPUParameters_Event_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _RequestPUParameters_Event_type_support_ids_t;

static const _RequestPUParameters_Event_type_support_ids_t _RequestPUParameters_Event_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _RequestPUParameters_Event_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _RequestPUParameters_Event_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _RequestPUParameters_Event_type_support_symbol_names_t _RequestPUParameters_Event_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, ros_kctrl_custom_interfaces, srv, RequestPUParameters_Event)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, ros_kctrl_custom_interfaces, srv, RequestPUParameters_Event)),
  }
};

typedef struct _RequestPUParameters_Event_type_support_data_t
{
  void * data[2];
} _RequestPUParameters_Event_type_support_data_t;

static _RequestPUParameters_Event_type_support_data_t _RequestPUParameters_Event_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _RequestPUParameters_Event_message_typesupport_map = {
  2,
  "ros_kctrl_custom_interfaces",
  &_RequestPUParameters_Event_message_typesupport_ids.typesupport_identifier[0],
  &_RequestPUParameters_Event_message_typesupport_symbol_names.symbol_name[0],
  &_RequestPUParameters_Event_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t RequestPUParameters_Event_message_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_RequestPUParameters_Event_message_typesupport_map),
  ::rosidl_typesupport_cpp::get_message_typesupport_handle_function,
  &ros_kctrl_custom_interfaces__srv__RequestPUParameters_Event__get_type_hash,
  &ros_kctrl_custom_interfaces__srv__RequestPUParameters_Event__get_type_description,
  &ros_kctrl_custom_interfaces__srv__RequestPUParameters_Event__get_type_description_sources,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace srv

}  // namespace ros_kctrl_custom_interfaces

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Event>()
{
  return &::ros_kctrl_custom_interfaces::srv::rosidl_typesupport_cpp::RequestPUParameters_Event_message_type_support_handle;
}

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_cpp, ros_kctrl_custom_interfaces, srv, RequestPUParameters_Event)() {
  return get_message_type_support_handle<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Event>();
}

#ifdef __cplusplus
}
#endif
}  // namespace rosidl_typesupport_cpp

// already included above
// #include "cstddef"
#include "rosidl_runtime_c/service_type_support_struct.h"
#include "rosidl_typesupport_cpp/service_type_support.hpp"
// already included above
// #include "ros_kctrl_custom_interfaces/srv/detail/request_pu_parameters__struct.hpp"
// already included above
// #include "rosidl_typesupport_cpp/identifier.hpp"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
#include "rosidl_typesupport_cpp/service_type_support_dispatch.hpp"
// already included above
// #include "rosidl_typesupport_cpp/visibility_control.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"

namespace ros_kctrl_custom_interfaces
{

namespace srv
{

namespace rosidl_typesupport_cpp
{

typedef struct _RequestPUParameters_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _RequestPUParameters_type_support_ids_t;

static const _RequestPUParameters_type_support_ids_t _RequestPUParameters_service_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_cpp",  // ::rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
    "rosidl_typesupport_introspection_cpp",  // ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  }
};

typedef struct _RequestPUParameters_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _RequestPUParameters_type_support_symbol_names_t;
#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _RequestPUParameters_type_support_symbol_names_t _RequestPUParameters_service_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, ros_kctrl_custom_interfaces, srv, RequestPUParameters)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, ros_kctrl_custom_interfaces, srv, RequestPUParameters)),
  }
};

typedef struct _RequestPUParameters_type_support_data_t
{
  void * data[2];
} _RequestPUParameters_type_support_data_t;

static _RequestPUParameters_type_support_data_t _RequestPUParameters_service_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _RequestPUParameters_service_typesupport_map = {
  2,
  "ros_kctrl_custom_interfaces",
  &_RequestPUParameters_service_typesupport_ids.typesupport_identifier[0],
  &_RequestPUParameters_service_typesupport_symbol_names.symbol_name[0],
  &_RequestPUParameters_service_typesupport_data.data[0],
};

static const rosidl_service_type_support_t RequestPUParameters_service_type_support_handle = {
  ::rosidl_typesupport_cpp::typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_RequestPUParameters_service_typesupport_map),
  ::rosidl_typesupport_cpp::get_service_typesupport_handle_function,
  ::rosidl_typesupport_cpp::get_message_type_support_handle<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Request>(),
  ::rosidl_typesupport_cpp::get_message_type_support_handle<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Response>(),
  ::rosidl_typesupport_cpp::get_message_type_support_handle<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Event>(),
  &::rosidl_typesupport_cpp::service_create_event_message<ros_kctrl_custom_interfaces::srv::RequestPUParameters>,
  &::rosidl_typesupport_cpp::service_destroy_event_message<ros_kctrl_custom_interfaces::srv::RequestPUParameters>,
  &ros_kctrl_custom_interfaces__srv__RequestPUParameters__get_type_hash,
  &ros_kctrl_custom_interfaces__srv__RequestPUParameters__get_type_description,
  &ros_kctrl_custom_interfaces__srv__RequestPUParameters__get_type_description_sources,
};

}  // namespace rosidl_typesupport_cpp

}  // namespace srv

}  // namespace ros_kctrl_custom_interfaces

namespace rosidl_typesupport_cpp
{

template<>
ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_service_type_support_t *
get_service_type_support_handle<ros_kctrl_custom_interfaces::srv::RequestPUParameters>()
{
  return &::ros_kctrl_custom_interfaces::srv::rosidl_typesupport_cpp::RequestPUParameters_service_type_support_handle;
}

}  // namespace rosidl_typesupport_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_CPP_PUBLIC
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_cpp, ros_kctrl_custom_interfaces, srv, RequestPUParameters)() {
  return ::rosidl_typesupport_cpp::get_service_type_support_handle<ros_kctrl_custom_interfaces::srv::RequestPUParameters>();
}

#ifdef __cplusplus
}
#endif
