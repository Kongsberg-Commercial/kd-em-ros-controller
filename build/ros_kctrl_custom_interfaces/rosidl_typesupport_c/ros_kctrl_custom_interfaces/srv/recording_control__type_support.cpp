// generated from rosidl_typesupport_c/resource/idl__type_support.cpp.em
// with input from ros_kctrl_custom_interfaces:srv/RecordingControl.idl
// generated code does not contain a copyright notice

#include "cstddef"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "ros_kctrl_custom_interfaces/srv/detail/recording_control__struct.h"
#include "ros_kctrl_custom_interfaces/srv/detail/recording_control__type_support.h"
#include "ros_kctrl_custom_interfaces/srv/detail/recording_control__functions.h"
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

typedef struct _RecordingControl_Request_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _RecordingControl_Request_type_support_ids_t;

static const _RecordingControl_Request_type_support_ids_t _RecordingControl_Request_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_c",  // ::rosidl_typesupport_fastrtps_c::typesupport_identifier,
    "rosidl_typesupport_introspection_c",  // ::rosidl_typesupport_introspection_c::typesupport_identifier,
  }
};

typedef struct _RecordingControl_Request_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _RecordingControl_Request_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _RecordingControl_Request_type_support_symbol_names_t _RecordingControl_Request_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, ros_kctrl_custom_interfaces, srv, RecordingControl_Request)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros_kctrl_custom_interfaces, srv, RecordingControl_Request)),
  }
};

typedef struct _RecordingControl_Request_type_support_data_t
{
  void * data[2];
} _RecordingControl_Request_type_support_data_t;

static _RecordingControl_Request_type_support_data_t _RecordingControl_Request_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _RecordingControl_Request_message_typesupport_map = {
  2,
  "ros_kctrl_custom_interfaces",
  &_RecordingControl_Request_message_typesupport_ids.typesupport_identifier[0],
  &_RecordingControl_Request_message_typesupport_symbol_names.symbol_name[0],
  &_RecordingControl_Request_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t RecordingControl_Request_message_type_support_handle = {
  rosidl_typesupport_c__typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_RecordingControl_Request_message_typesupport_map),
  rosidl_typesupport_c__get_message_typesupport_handle_function,
  &ros_kctrl_custom_interfaces__srv__RecordingControl_Request__get_type_hash,
  &ros_kctrl_custom_interfaces__srv__RecordingControl_Request__get_type_description,
  &ros_kctrl_custom_interfaces__srv__RecordingControl_Request__get_type_description_sources,
};

}  // namespace rosidl_typesupport_c

}  // namespace srv

}  // namespace ros_kctrl_custom_interfaces

#ifdef __cplusplus
extern "C"
{
#endif

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_c, ros_kctrl_custom_interfaces, srv, RecordingControl_Request)() {
  return &::ros_kctrl_custom_interfaces::srv::rosidl_typesupport_c::RecordingControl_Request_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif

// already included above
// #include "cstddef"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "ros_kctrl_custom_interfaces/srv/detail/recording_control__struct.h"
// already included above
// #include "ros_kctrl_custom_interfaces/srv/detail/recording_control__type_support.h"
// already included above
// #include "ros_kctrl_custom_interfaces/srv/detail/recording_control__functions.h"
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

typedef struct _RecordingControl_Response_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _RecordingControl_Response_type_support_ids_t;

static const _RecordingControl_Response_type_support_ids_t _RecordingControl_Response_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_c",  // ::rosidl_typesupport_fastrtps_c::typesupport_identifier,
    "rosidl_typesupport_introspection_c",  // ::rosidl_typesupport_introspection_c::typesupport_identifier,
  }
};

typedef struct _RecordingControl_Response_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _RecordingControl_Response_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _RecordingControl_Response_type_support_symbol_names_t _RecordingControl_Response_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, ros_kctrl_custom_interfaces, srv, RecordingControl_Response)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros_kctrl_custom_interfaces, srv, RecordingControl_Response)),
  }
};

typedef struct _RecordingControl_Response_type_support_data_t
{
  void * data[2];
} _RecordingControl_Response_type_support_data_t;

static _RecordingControl_Response_type_support_data_t _RecordingControl_Response_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _RecordingControl_Response_message_typesupport_map = {
  2,
  "ros_kctrl_custom_interfaces",
  &_RecordingControl_Response_message_typesupport_ids.typesupport_identifier[0],
  &_RecordingControl_Response_message_typesupport_symbol_names.symbol_name[0],
  &_RecordingControl_Response_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t RecordingControl_Response_message_type_support_handle = {
  rosidl_typesupport_c__typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_RecordingControl_Response_message_typesupport_map),
  rosidl_typesupport_c__get_message_typesupport_handle_function,
  &ros_kctrl_custom_interfaces__srv__RecordingControl_Response__get_type_hash,
  &ros_kctrl_custom_interfaces__srv__RecordingControl_Response__get_type_description,
  &ros_kctrl_custom_interfaces__srv__RecordingControl_Response__get_type_description_sources,
};

}  // namespace rosidl_typesupport_c

}  // namespace srv

}  // namespace ros_kctrl_custom_interfaces

#ifdef __cplusplus
extern "C"
{
#endif

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_c, ros_kctrl_custom_interfaces, srv, RecordingControl_Response)() {
  return &::ros_kctrl_custom_interfaces::srv::rosidl_typesupport_c::RecordingControl_Response_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif

// already included above
// #include "cstddef"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "ros_kctrl_custom_interfaces/srv/detail/recording_control__struct.h"
// already included above
// #include "ros_kctrl_custom_interfaces/srv/detail/recording_control__type_support.h"
// already included above
// #include "ros_kctrl_custom_interfaces/srv/detail/recording_control__functions.h"
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

typedef struct _RecordingControl_Event_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _RecordingControl_Event_type_support_ids_t;

static const _RecordingControl_Event_type_support_ids_t _RecordingControl_Event_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_c",  // ::rosidl_typesupport_fastrtps_c::typesupport_identifier,
    "rosidl_typesupport_introspection_c",  // ::rosidl_typesupport_introspection_c::typesupport_identifier,
  }
};

typedef struct _RecordingControl_Event_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _RecordingControl_Event_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _RecordingControl_Event_type_support_symbol_names_t _RecordingControl_Event_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, ros_kctrl_custom_interfaces, srv, RecordingControl_Event)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros_kctrl_custom_interfaces, srv, RecordingControl_Event)),
  }
};

typedef struct _RecordingControl_Event_type_support_data_t
{
  void * data[2];
} _RecordingControl_Event_type_support_data_t;

static _RecordingControl_Event_type_support_data_t _RecordingControl_Event_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _RecordingControl_Event_message_typesupport_map = {
  2,
  "ros_kctrl_custom_interfaces",
  &_RecordingControl_Event_message_typesupport_ids.typesupport_identifier[0],
  &_RecordingControl_Event_message_typesupport_symbol_names.symbol_name[0],
  &_RecordingControl_Event_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t RecordingControl_Event_message_type_support_handle = {
  rosidl_typesupport_c__typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_RecordingControl_Event_message_typesupport_map),
  rosidl_typesupport_c__get_message_typesupport_handle_function,
  &ros_kctrl_custom_interfaces__srv__RecordingControl_Event__get_type_hash,
  &ros_kctrl_custom_interfaces__srv__RecordingControl_Event__get_type_description,
  &ros_kctrl_custom_interfaces__srv__RecordingControl_Event__get_type_description_sources,
};

}  // namespace rosidl_typesupport_c

}  // namespace srv

}  // namespace ros_kctrl_custom_interfaces

#ifdef __cplusplus
extern "C"
{
#endif

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_c, ros_kctrl_custom_interfaces, srv, RecordingControl_Event)() {
  return &::ros_kctrl_custom_interfaces::srv::rosidl_typesupport_c::RecordingControl_Event_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif

// already included above
// #include "cstddef"
#include "rosidl_runtime_c/service_type_support_struct.h"
// already included above
// #include "ros_kctrl_custom_interfaces/srv/detail/recording_control__type_support.h"
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
typedef struct _RecordingControl_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _RecordingControl_type_support_ids_t;

static const _RecordingControl_type_support_ids_t _RecordingControl_service_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_c",  // ::rosidl_typesupport_fastrtps_c::typesupport_identifier,
    "rosidl_typesupport_introspection_c",  // ::rosidl_typesupport_introspection_c::typesupport_identifier,
  }
};

typedef struct _RecordingControl_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _RecordingControl_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _RecordingControl_type_support_symbol_names_t _RecordingControl_service_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, ros_kctrl_custom_interfaces, srv, RecordingControl)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_c, ros_kctrl_custom_interfaces, srv, RecordingControl)),
  }
};

typedef struct _RecordingControl_type_support_data_t
{
  void * data[2];
} _RecordingControl_type_support_data_t;

static _RecordingControl_type_support_data_t _RecordingControl_service_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _RecordingControl_service_typesupport_map = {
  2,
  "ros_kctrl_custom_interfaces",
  &_RecordingControl_service_typesupport_ids.typesupport_identifier[0],
  &_RecordingControl_service_typesupport_symbol_names.symbol_name[0],
  &_RecordingControl_service_typesupport_data.data[0],
};

static const rosidl_service_type_support_t RecordingControl_service_type_support_handle = {
  rosidl_typesupport_c__typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_RecordingControl_service_typesupport_map),
  rosidl_typesupport_c__get_service_typesupport_handle_function,
  &RecordingControl_Request_message_type_support_handle,
  &RecordingControl_Response_message_type_support_handle,
  &RecordingControl_Event_message_type_support_handle,
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_CREATE_EVENT_MESSAGE_SYMBOL_NAME(
    rosidl_typesupport_c,
    ros_kctrl_custom_interfaces,
    srv,
    RecordingControl
  ),
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_DESTROY_EVENT_MESSAGE_SYMBOL_NAME(
    rosidl_typesupport_c,
    ros_kctrl_custom_interfaces,
    srv,
    RecordingControl
  ),
  &ros_kctrl_custom_interfaces__srv__RecordingControl__get_type_hash,
  &ros_kctrl_custom_interfaces__srv__RecordingControl__get_type_description,
  &ros_kctrl_custom_interfaces__srv__RecordingControl__get_type_description_sources,
};

}  // namespace rosidl_typesupport_c

}  // namespace srv

}  // namespace ros_kctrl_custom_interfaces

#ifdef __cplusplus
extern "C"
{
#endif

const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_c, ros_kctrl_custom_interfaces, srv, RecordingControl)() {
  return &::ros_kctrl_custom_interfaces::srv::rosidl_typesupport_c::RecordingControl_service_type_support_handle;
}

#ifdef __cplusplus
}
#endif
