// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from ros_kctrl_custom_interfaces:srv/RequestPUParameters.idl
// generated code does not contain a copyright notice

#include "ros_kctrl_custom_interfaces/srv/detail/request_pu_parameters__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_ros_kctrl_custom_interfaces
const rosidl_type_hash_t *
ros_kctrl_custom_interfaces__srv__RequestPUParameters__get_type_hash(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x45, 0x6f, 0x90, 0x9d, 0x80, 0xb7, 0xd8, 0x14,
      0xbd, 0x1a, 0x55, 0x53, 0x60, 0x85, 0xfd, 0xcb,
      0x4a, 0x8d, 0xe4, 0xb2, 0x73, 0x01, 0xe9, 0xcb,
      0x15, 0xd8, 0x71, 0x8e, 0x6c, 0x48, 0x5b, 0x8d,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_ros_kctrl_custom_interfaces
const rosidl_type_hash_t *
ros_kctrl_custom_interfaces__srv__RequestPUParameters_Request__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x18, 0xb7, 0x10, 0xb4, 0x62, 0xf2, 0xbc, 0x11,
      0xa5, 0x26, 0xc4, 0xb8, 0x58, 0xcb, 0x4b, 0xe2,
      0xc3, 0x5c, 0xbd, 0xa8, 0x8f, 0xfc, 0x35, 0x14,
      0x05, 0xb4, 0x95, 0x8c, 0x91, 0x5b, 0xb5, 0x22,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_ros_kctrl_custom_interfaces
const rosidl_type_hash_t *
ros_kctrl_custom_interfaces__srv__RequestPUParameters_Response__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x1c, 0x13, 0xf3, 0xd9, 0xb5, 0x40, 0xe2, 0x5b,
      0x33, 0x2b, 0x89, 0xe8, 0xfe, 0x7c, 0x46, 0x57,
      0x5b, 0x2f, 0xbb, 0x06, 0x9b, 0x49, 0x03, 0x0e,
      0xcd, 0xe2, 0xd0, 0x94, 0x2d, 0x58, 0xd1, 0x62,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_ros_kctrl_custom_interfaces
const rosidl_type_hash_t *
ros_kctrl_custom_interfaces__srv__RequestPUParameters_Event__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xf4, 0x6f, 0x5c, 0xb8, 0xa1, 0x1e, 0x8a, 0x54,
      0xc7, 0xe9, 0xfa, 0xe9, 0xa3, 0xe5, 0x36, 0x25,
      0x2a, 0x4e, 0xbf, 0x33, 0xf4, 0xa6, 0x77, 0x88,
      0x5e, 0x94, 0x94, 0x45, 0x58, 0x52, 0x2c, 0x92,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types
#include "builtin_interfaces/msg/detail/time__functions.h"
#include "service_msgs/msg/detail/service_event_info__functions.h"

// Hashes for external referenced types
#ifndef NDEBUG
static const rosidl_type_hash_t builtin_interfaces__msg__Time__EXPECTED_HASH = {1, {
    0xb1, 0x06, 0x23, 0x5e, 0x25, 0xa4, 0xc5, 0xed,
    0x35, 0x09, 0x8a, 0xa0, 0xa6, 0x1a, 0x3e, 0xe9,
    0xc9, 0xb1, 0x8d, 0x19, 0x7f, 0x39, 0x8b, 0x0e,
    0x42, 0x06, 0xce, 0xa9, 0xac, 0xf9, 0xc1, 0x97,
  }};
static const rosidl_type_hash_t service_msgs__msg__ServiceEventInfo__EXPECTED_HASH = {1, {
    0x41, 0xbc, 0xbb, 0xe0, 0x7a, 0x75, 0xc9, 0xb5,
    0x2b, 0xc9, 0x6b, 0xfd, 0x5c, 0x24, 0xd7, 0xf0,
    0xfc, 0x0a, 0x08, 0xc0, 0xcb, 0x79, 0x21, 0xb3,
    0x37, 0x3c, 0x57, 0x32, 0x34, 0x5a, 0x6f, 0x45,
  }};
#endif

static char ros_kctrl_custom_interfaces__srv__RequestPUParameters__TYPE_NAME[] = "ros_kctrl_custom_interfaces/srv/RequestPUParameters";
static char builtin_interfaces__msg__Time__TYPE_NAME[] = "builtin_interfaces/msg/Time";
static char ros_kctrl_custom_interfaces__srv__RequestPUParameters_Event__TYPE_NAME[] = "ros_kctrl_custom_interfaces/srv/RequestPUParameters_Event";
static char ros_kctrl_custom_interfaces__srv__RequestPUParameters_Request__TYPE_NAME[] = "ros_kctrl_custom_interfaces/srv/RequestPUParameters_Request";
static char ros_kctrl_custom_interfaces__srv__RequestPUParameters_Response__TYPE_NAME[] = "ros_kctrl_custom_interfaces/srv/RequestPUParameters_Response";
static char service_msgs__msg__ServiceEventInfo__TYPE_NAME[] = "service_msgs/msg/ServiceEventInfo";

// Define type names, field names, and default values
static char ros_kctrl_custom_interfaces__srv__RequestPUParameters__FIELD_NAME__request_message[] = "request_message";
static char ros_kctrl_custom_interfaces__srv__RequestPUParameters__FIELD_NAME__response_message[] = "response_message";
static char ros_kctrl_custom_interfaces__srv__RequestPUParameters__FIELD_NAME__event_message[] = "event_message";

static rosidl_runtime_c__type_description__Field ros_kctrl_custom_interfaces__srv__RequestPUParameters__FIELDS[] = {
  {
    {ros_kctrl_custom_interfaces__srv__RequestPUParameters__FIELD_NAME__request_message, 15, 15},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {ros_kctrl_custom_interfaces__srv__RequestPUParameters_Request__TYPE_NAME, 59, 59},
    },
    {NULL, 0, 0},
  },
  {
    {ros_kctrl_custom_interfaces__srv__RequestPUParameters__FIELD_NAME__response_message, 16, 16},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {ros_kctrl_custom_interfaces__srv__RequestPUParameters_Response__TYPE_NAME, 60, 60},
    },
    {NULL, 0, 0},
  },
  {
    {ros_kctrl_custom_interfaces__srv__RequestPUParameters__FIELD_NAME__event_message, 13, 13},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {ros_kctrl_custom_interfaces__srv__RequestPUParameters_Event__TYPE_NAME, 57, 57},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription ros_kctrl_custom_interfaces__srv__RequestPUParameters__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    {NULL, 0, 0},
  },
  {
    {ros_kctrl_custom_interfaces__srv__RequestPUParameters_Event__TYPE_NAME, 57, 57},
    {NULL, 0, 0},
  },
  {
    {ros_kctrl_custom_interfaces__srv__RequestPUParameters_Request__TYPE_NAME, 59, 59},
    {NULL, 0, 0},
  },
  {
    {ros_kctrl_custom_interfaces__srv__RequestPUParameters_Response__TYPE_NAME, 60, 60},
    {NULL, 0, 0},
  },
  {
    {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
ros_kctrl_custom_interfaces__srv__RequestPUParameters__get_type_description(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {ros_kctrl_custom_interfaces__srv__RequestPUParameters__TYPE_NAME, 51, 51},
      {ros_kctrl_custom_interfaces__srv__RequestPUParameters__FIELDS, 3, 3},
    },
    {ros_kctrl_custom_interfaces__srv__RequestPUParameters__REFERENCED_TYPE_DESCRIPTIONS, 5, 5},
  };
  if (!constructed) {
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[1].fields = ros_kctrl_custom_interfaces__srv__RequestPUParameters_Event__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[2].fields = ros_kctrl_custom_interfaces__srv__RequestPUParameters_Request__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[3].fields = ros_kctrl_custom_interfaces__srv__RequestPUParameters_Response__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&service_msgs__msg__ServiceEventInfo__EXPECTED_HASH, service_msgs__msg__ServiceEventInfo__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[4].fields = service_msgs__msg__ServiceEventInfo__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char ros_kctrl_custom_interfaces__srv__RequestPUParameters_Request__FIELD_NAME__sounder_name[] = "sounder_name";
static char ros_kctrl_custom_interfaces__srv__RequestPUParameters_Request__FIELD_NAME__param_names[] = "param_names";

static rosidl_runtime_c__type_description__Field ros_kctrl_custom_interfaces__srv__RequestPUParameters_Request__FIELDS[] = {
  {
    {ros_kctrl_custom_interfaces__srv__RequestPUParameters_Request__FIELD_NAME__sounder_name, 12, 12},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {ros_kctrl_custom_interfaces__srv__RequestPUParameters_Request__FIELD_NAME__param_names, 11, 11},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING_UNBOUNDED_SEQUENCE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
ros_kctrl_custom_interfaces__srv__RequestPUParameters_Request__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {ros_kctrl_custom_interfaces__srv__RequestPUParameters_Request__TYPE_NAME, 59, 59},
      {ros_kctrl_custom_interfaces__srv__RequestPUParameters_Request__FIELDS, 2, 2},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char ros_kctrl_custom_interfaces__srv__RequestPUParameters_Response__FIELD_NAME__success[] = "success";
static char ros_kctrl_custom_interfaces__srv__RequestPUParameters_Response__FIELD_NAME__message[] = "message";

static rosidl_runtime_c__type_description__Field ros_kctrl_custom_interfaces__srv__RequestPUParameters_Response__FIELDS[] = {
  {
    {ros_kctrl_custom_interfaces__srv__RequestPUParameters_Response__FIELD_NAME__success, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_BOOLEAN,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {ros_kctrl_custom_interfaces__srv__RequestPUParameters_Response__FIELD_NAME__message, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
ros_kctrl_custom_interfaces__srv__RequestPUParameters_Response__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {ros_kctrl_custom_interfaces__srv__RequestPUParameters_Response__TYPE_NAME, 60, 60},
      {ros_kctrl_custom_interfaces__srv__RequestPUParameters_Response__FIELDS, 2, 2},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char ros_kctrl_custom_interfaces__srv__RequestPUParameters_Event__FIELD_NAME__info[] = "info";
static char ros_kctrl_custom_interfaces__srv__RequestPUParameters_Event__FIELD_NAME__request[] = "request";
static char ros_kctrl_custom_interfaces__srv__RequestPUParameters_Event__FIELD_NAME__response[] = "response";

static rosidl_runtime_c__type_description__Field ros_kctrl_custom_interfaces__srv__RequestPUParameters_Event__FIELDS[] = {
  {
    {ros_kctrl_custom_interfaces__srv__RequestPUParameters_Event__FIELD_NAME__info, 4, 4},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    },
    {NULL, 0, 0},
  },
  {
    {ros_kctrl_custom_interfaces__srv__RequestPUParameters_Event__FIELD_NAME__request, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_BOUNDED_SEQUENCE,
      1,
      0,
      {ros_kctrl_custom_interfaces__srv__RequestPUParameters_Request__TYPE_NAME, 59, 59},
    },
    {NULL, 0, 0},
  },
  {
    {ros_kctrl_custom_interfaces__srv__RequestPUParameters_Event__FIELD_NAME__response, 8, 8},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_BOUNDED_SEQUENCE,
      1,
      0,
      {ros_kctrl_custom_interfaces__srv__RequestPUParameters_Response__TYPE_NAME, 60, 60},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription ros_kctrl_custom_interfaces__srv__RequestPUParameters_Event__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    {NULL, 0, 0},
  },
  {
    {ros_kctrl_custom_interfaces__srv__RequestPUParameters_Request__TYPE_NAME, 59, 59},
    {NULL, 0, 0},
  },
  {
    {ros_kctrl_custom_interfaces__srv__RequestPUParameters_Response__TYPE_NAME, 60, 60},
    {NULL, 0, 0},
  },
  {
    {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
ros_kctrl_custom_interfaces__srv__RequestPUParameters_Event__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {ros_kctrl_custom_interfaces__srv__RequestPUParameters_Event__TYPE_NAME, 57, 57},
      {ros_kctrl_custom_interfaces__srv__RequestPUParameters_Event__FIELDS, 3, 3},
    },
    {ros_kctrl_custom_interfaces__srv__RequestPUParameters_Event__REFERENCED_TYPE_DESCRIPTIONS, 4, 4},
  };
  if (!constructed) {
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[1].fields = ros_kctrl_custom_interfaces__srv__RequestPUParameters_Request__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[2].fields = ros_kctrl_custom_interfaces__srv__RequestPUParameters_Response__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&service_msgs__msg__ServiceEventInfo__EXPECTED_HASH, service_msgs__msg__ServiceEventInfo__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[3].fields = service_msgs__msg__ServiceEventInfo__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "string sounder_name\n"
  "string[] param_names\n"
  "---\n"
  "bool success\n"
  "string message";

static char srv_encoding[] = "srv";
static char implicit_encoding[] = "implicit";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
ros_kctrl_custom_interfaces__srv__RequestPUParameters__get_individual_type_description_source(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {ros_kctrl_custom_interfaces__srv__RequestPUParameters__TYPE_NAME, 51, 51},
    {srv_encoding, 3, 3},
    {toplevel_type_raw_source, 73, 73},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
ros_kctrl_custom_interfaces__srv__RequestPUParameters_Request__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {ros_kctrl_custom_interfaces__srv__RequestPUParameters_Request__TYPE_NAME, 59, 59},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
ros_kctrl_custom_interfaces__srv__RequestPUParameters_Response__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {ros_kctrl_custom_interfaces__srv__RequestPUParameters_Response__TYPE_NAME, 60, 60},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
ros_kctrl_custom_interfaces__srv__RequestPUParameters_Event__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {ros_kctrl_custom_interfaces__srv__RequestPUParameters_Event__TYPE_NAME, 57, 57},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
ros_kctrl_custom_interfaces__srv__RequestPUParameters__get_type_description_sources(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[6];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 6, 6};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *ros_kctrl_custom_interfaces__srv__RequestPUParameters__get_individual_type_description_source(NULL),
    sources[1] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    sources[2] = *ros_kctrl_custom_interfaces__srv__RequestPUParameters_Event__get_individual_type_description_source(NULL);
    sources[3] = *ros_kctrl_custom_interfaces__srv__RequestPUParameters_Request__get_individual_type_description_source(NULL);
    sources[4] = *ros_kctrl_custom_interfaces__srv__RequestPUParameters_Response__get_individual_type_description_source(NULL);
    sources[5] = *service_msgs__msg__ServiceEventInfo__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
ros_kctrl_custom_interfaces__srv__RequestPUParameters_Request__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *ros_kctrl_custom_interfaces__srv__RequestPUParameters_Request__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
ros_kctrl_custom_interfaces__srv__RequestPUParameters_Response__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *ros_kctrl_custom_interfaces__srv__RequestPUParameters_Response__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
ros_kctrl_custom_interfaces__srv__RequestPUParameters_Event__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[5];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 5, 5};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *ros_kctrl_custom_interfaces__srv__RequestPUParameters_Event__get_individual_type_description_source(NULL),
    sources[1] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    sources[2] = *ros_kctrl_custom_interfaces__srv__RequestPUParameters_Request__get_individual_type_description_source(NULL);
    sources[3] = *ros_kctrl_custom_interfaces__srv__RequestPUParameters_Response__get_individual_type_description_source(NULL);
    sources[4] = *service_msgs__msg__ServiceEventInfo__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}
