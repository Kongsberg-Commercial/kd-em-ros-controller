// generated from rosidl_typesupport_c/resource/idl__type_support.cpp.em
// with input from ros_kctrl_custom_interfaces:srv/PUParameters.idl
// generated code does not contain a copyright notice

#include "cstddef"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "ros_kctrl_custom_interfaces/srv/detail/pu_parameters__struct.h"
#include "ros_kctrl_custom_interfaces/srv/detail/pu_parameters__type_support.h"
#include "ros_kctrl_custom_interfaces/srv/detail/pu_parameters__functions.h"
#include "rosidl_typesupport_c/identifier.h"
#include "rosidl_typesupport_c/message_type_support_dispatch.h"
#include "rosidl_typesupport_c/type_support_map.h"
#include "rosidl_typesupport_c/visibility_control.h"
#include "rosidl_typesupport_interface/macros.h"

namespace ros_kctrl_custom_interfaces
{

namespace srv
{

namespace rosidl_typesupport_c
{

typedef struct _PUParameters_Request_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _PUParameters_Request_type_support_ids_t;

static const _PUParameters_Request_type_support_ids_t _PUParameters_Request_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_c",  // ::rosidl_typesupport_fastrtps_c::typesupport_identifier,
    "rosidl_typesupport_introspection_c",  // ::rosidl_typesupport_introspection_c::typesupport_identifier,
  }
};

typedef struct _PUParameters_Request_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _PUParameters_Request_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _PUParameters_Request_type_support_symbol_names_t _PUParameters_Request_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, ros_kctrl_custom_interfaces, srv, PUParameters_Request)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros_kctrl_custom_interfaces, srv, PUParameters_Request)),
  }
};

typedef struct _PUParameters_Request_type_support_data_t
{
  void * data[2];
} _PUParameters_Request_type_support_data_t;

static _PUParameters_Request_type_support_data_t _PUParameters_Request_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _PUParameters_Request_message_typesupport_map = {
  2,
  "ros_kctrl_custom_interfaces",
  &_PUParameters_Request_message_typesupport_ids.typesupport_identifier[0],
  &_PUParameters_Request_message_typesupport_symbol_names.symbol_name[0],
  &_PUParameters_Request_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t PUParameters_Request_message_type_support_handle = {
  rosidl_typesupport_c__typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_PUParameters_Request_message_typesupport_map),
  rosidl_typesupport_c__get_message_typesupport_handle_function,
  &ros_kctrl_custom_interfaces__srv__PUParameters_Request__get_type_hash,
  &ros_kctrl_custom_interfaces__srv__PUParameters_Request__get_type_description,
  &ros_kctrl_custom_interfaces__srv__PUParameters_Request__get_type_description_sources,
};

}  // namespace rosidl_typesupport_c

}  // namespace srv

}  // namespace ros_kctrl_custom_interfaces

#ifdef __cplusplus
extern "C"
{
#endif

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_c, ros_kctrl_custom_interfaces, srv, PUParameters_Request)() {
  return &::ros_kctrl_custom_interfaces::srv::rosidl_typesupport_c::PUParameters_Request_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif

// already included above
// #include "cstddef"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "ros_kctrl_custom_interfaces/srv/detail/pu_parameters__struct.h"
// already included above
// #include "ros_kctrl_custom_interfaces/srv/detail/pu_parameters__type_support.h"
// already included above
// #include "ros_kctrl_custom_interfaces/srv/detail/pu_parameters__functions.h"
// already included above
// #include "rosidl_typesupport_c/identifier.h"
// already included above
// #include "rosidl_typesupport_c/message_type_support_dispatch.h"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
// already included above
// #include "rosidl_typesupport_c/visibility_control.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"

namespace ros_kctrl_custom_interfaces
{

namespace srv
{

namespace rosidl_typesupport_c
{

typedef struct _PUParameters_Response_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _PUParameters_Response_type_support_ids_t;

static const _PUParameters_Response_type_support_ids_t _PUParameters_Response_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_c",  // ::rosidl_typesupport_fastrtps_c::typesupport_identifier,
    "rosidl_typesupport_introspection_c",  // ::rosidl_typesupport_introspection_c::typesupport_identifier,
  }
};

typedef struct _PUParameters_Response_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _PUParameters_Response_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _PUParameters_Response_type_support_symbol_names_t _PUParameters_Response_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, ros_kctrl_custom_interfaces, srv, PUParameters_Response)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros_kctrl_custom_interfaces, srv, PUParameters_Response)),
  }
};

typedef struct _PUParameters_Response_type_support_data_t
{
  void * data[2];
} _PUParameters_Response_type_support_data_t;

static _PUParameters_Response_type_support_data_t _PUParameters_Response_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _PUParameters_Response_message_typesupport_map = {
  2,
  "ros_kctrl_custom_interfaces",
  &_PUParameters_Response_message_typesupport_ids.typesupport_identifier[0],
  &_PUParameters_Response_message_typesupport_symbol_names.symbol_name[0],
  &_PUParameters_Response_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t PUParameters_Response_message_type_support_handle = {
  rosidl_typesupport_c__typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_PUParameters_Response_message_typesupport_map),
  rosidl_typesupport_c__get_message_typesupport_handle_function,
  &ros_kctrl_custom_interfaces__srv__PUParameters_Response__get_type_hash,
  &ros_kctrl_custom_interfaces__srv__PUParameters_Response__get_type_description,
  &ros_kctrl_custom_interfaces__srv__PUParameters_Response__get_type_description_sources,
};

}  // namespace rosidl_typesupport_c

}  // namespace srv

}  // namespace ros_kctrl_custom_interfaces

#ifdef __cplusplus
extern "C"
{
#endif

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_c, ros_kctrl_custom_interfaces, srv, PUParameters_Response)() {
  return &::ros_kctrl_custom_interfaces::srv::rosidl_typesupport_c::PUParameters_Response_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif

// already included above
// #include "cstddef"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "ros_kctrl_custom_interfaces/srv/detail/pu_parameters__struct.h"
// already included above
// #include "ros_kctrl_custom_interfaces/srv/detail/pu_parameters__type_support.h"
// already included above
// #include "ros_kctrl_custom_interfaces/srv/detail/pu_parameters__functions.h"
// already included above
// #include "rosidl_typesupport_c/identifier.h"
// already included above
// #include "rosidl_typesupport_c/message_type_support_dispatch.h"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
// already included above
// #include "rosidl_typesupport_c/visibility_control.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"

namespace ros_kctrl_custom_interfaces
{

namespace srv
{

namespace rosidl_typesupport_c
{

typedef struct _PUParameters_Event_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _PUParameters_Event_type_support_ids_t;

static const _PUParameters_Event_type_support_ids_t _PUParameters_Event_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_c",  // ::rosidl_typesupport_fastrtps_c::typesupport_identifier,
    "rosidl_typesupport_introspection_c",  // ::rosidl_typesupport_introspection_c::typesupport_identifier,
  }
};

typedef struct _PUParameters_Event_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _PUParameters_Event_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _PUParameters_Event_type_support_symbol_names_t _PUParameters_Event_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, ros_kctrl_custom_interfaces, srv, PUParameters_Event)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros_kctrl_custom_interfaces, srv, PUParameters_Event)),
  }
};

typedef struct _PUParameters_Event_type_support_data_t
{
  void * data[2];
} _PUParameters_Event_type_support_data_t;

static _PUParameters_Event_type_support_data_t _PUParameters_Event_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _PUParameters_Event_message_typesupport_map = {
  2,
  "ros_kctrl_custom_interfaces",
  &_PUParameters_Event_message_typesupport_ids.typesupport_identifier[0],
  &_PUParameters_Event_message_typesupport_symbol_names.symbol_name[0],
  &_PUParameters_Event_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t PUParameters_Event_message_type_support_handle = {
  rosidl_typesupport_c__typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_PUParameters_Event_message_typesupport_map),
  rosidl_typesupport_c__get_message_typesupport_handle_function,
  &ros_kctrl_custom_interfaces__srv__PUParameters_Event__get_type_hash,
  &ros_kctrl_custom_interfaces__srv__PUParameters_Event__get_type_description,
  &ros_kctrl_custom_interfaces__srv__PUParameters_Event__get_type_description_sources,
};

}  // namespace rosidl_typesupport_c

}  // namespace srv

}  // namespace ros_kctrl_custom_interfaces

#ifdef __cplusplus
extern "C"
{
#endif

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_c, ros_kctrl_custom_interfaces, srv, PUParameters_Event)() {
  return &::ros_kctrl_custom_interfaces::srv::rosidl_typesupport_c::PUParameters_Event_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif

// already included above
// #include "cstddef"
#include "rosidl_runtime_c/service_type_support_struct.h"
// already included above
// #include "ros_kctrl_custom_interfaces/srv/detail/pu_parameters__type_support.h"
// already included above
// #include "rosidl_typesupport_c/identifier.h"
#include "rosidl_typesupport_c/service_type_support_dispatch.h"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"
#include "service_msgs/msg/service_event_info.h"
#include "builtin_interfaces/msg/time.h"

namespace ros_kctrl_custom_interfaces
{

namespace srv
{

namespace rosidl_typesupport_c
{
typedef struct _PUParameters_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _PUParameters_type_support_ids_t;

static const _PUParameters_type_support_ids_t _PUParameters_service_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_c",  // ::rosidl_typesupport_fastrtps_c::typesupport_identifier,
    "rosidl_typesupport_introspection_c",  // ::rosidl_typesupport_introspection_c::typesupport_identifier,
  }
};

typedef struct _PUParameters_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _PUParameters_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _PUParameters_type_support_symbol_names_t _PUParameters_service_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, ros_kctrl_custom_interfaces, srv, PUParameters)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros_kctrl_custom_interfaces, srv, PUParameters)),
  }
};

typedef struct _PUParameters_type_support_data_t
{
  void * data[2];
} _PUParameters_type_support_data_t;

static _PUParameters_type_support_data_t _PUParameters_service_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _PUParameters_service_typesupport_map = {
  2,
  "ros_kctrl_custom_interfaces",
  &_PUParameters_service_typesupport_ids.typesupport_identifier[0],
  &_PUParameters_service_typesupport_symbol_names.symbol_name[0],
  &_PUParameters_service_typesupport_data.data[0],
};

static const rosidl_service_type_support_t PUParameters_service_type_support_handle = {
  rosidl_typesupport_c__typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_PUParameters_service_typesupport_map),
  rosidl_typesupport_c__get_service_typesupport_handle_function,
  &PUParameters_Request_message_type_support_handle,
  &PUParameters_Response_message_type_support_handle,
  &PUParameters_Event_message_type_support_handle,
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_CREATE_EVENT_MESSAGE_SYMBOL_NAME(
    rosidl_typesupport_c,
    ros_kctrl_custom_interfaces,
    srv,
    PUParameters
  ),
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_DESTROY_EVENT_MESSAGE_SYMBOL_NAME(
    rosidl_typesupport_c,
    ros_kctrl_custom_interfaces,
    srv,
    PUParameters
  ),
  &ros_kctrl_custom_interfaces__srv__PUParameters__get_type_hash,
  &ros_kctrl_custom_interfaces__srv__PUParameters__get_type_description,
  &ros_kctrl_custom_interfaces__srv__PUParameters__get_type_description_sources,
};

}  // namespace rosidl_typesupport_c

}  // namespace srv

}  // namespace ros_kctrl_custom_interfaces

#ifdef __cplusplus
extern "C"
{
#endif

const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_c, ros_kctrl_custom_interfaces, srv, PUParameters)() {
  return &::ros_kctrl_custom_interfaces::srv::rosidl_typesupport_c::PUParameters_service_type_support_handle;
}

#ifdef __cplusplus
}
#endif
