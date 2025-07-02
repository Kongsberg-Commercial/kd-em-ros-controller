// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from ros_kctrl_custom_interfaces:srv/RecordingControl.idl
// generated code does not contain a copyright notice

#include "ros_kctrl_custom_interfaces/srv/detail/recording_control__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_ros_kctrl_custom_interfaces
const rosidl_type_hash_t *
ros_kctrl_custom_interfaces__srv__RecordingControl__get_type_hash(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xf8, 0x63, 0x40, 0x3b, 0xac, 0x76, 0xd5, 0x2e,
      0xac, 0x0d, 0x32, 0x62, 0x59, 0x60, 0x2f, 0xb0,
      0x0e, 0xc0, 0x3c, 0x0a, 0xb7, 0x36, 0x1b, 0xb1,
      0x67, 0x00, 0x01, 0xf8, 0x13, 0xe7, 0xe6, 0x53,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_ros_kctrl_custom_interfaces
const rosidl_type_hash_t *
ros_kctrl_custom_interfaces__srv__RecordingControl_Request__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x92, 0x63, 0x0f, 0xed, 0x17, 0x77, 0xac, 0x64,
      0x83, 0xeb, 0xf5, 0x16, 0xe6, 0xa6, 0x0f, 0x38,
      0xaa, 0x71, 0x79, 0xef, 0xf9, 0x27, 0x2d, 0xaa,
      0xce, 0x74, 0x74, 0xc1, 0x1d, 0x36, 0x66, 0x49,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_ros_kctrl_custom_interfaces
const rosidl_type_hash_t *
ros_kctrl_custom_interfaces__srv__RecordingControl_Response__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x72, 0x5f, 0xee, 0xb5, 0x25, 0x6d, 0xd1, 0x15,
      0x4b, 0x2d, 0x88, 0x86, 0x6a, 0x17, 0xf9, 0xb5,
      0x79, 0xa5, 0xe7, 0x5d, 0xf0, 0xd5, 0xb6, 0x45,
      0x13, 0xd9, 0xb9, 0xa4, 0x26, 0x0e, 0xb5, 0x10,
    }};
  return &hash;
}

ROSIDL_GENERATOR_C_PUBLIC_ros_kctrl_custom_interfaces
const rosidl_type_hash_t *
ros_kctrl_custom_interfaces__srv__RecordingControl_Event__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x99, 0x6a, 0xe3, 0x36, 0x40, 0xde, 0xc7, 0xf8,
      0xa1, 0x1b, 0xd9, 0x4d, 0xe6, 0x30, 0xdf, 0x81,
      0xa6, 0xac, 0x12, 0x26, 0x51, 0x81, 0x04, 0x9c,
      0x7b, 0x39, 0x01, 0x2b, 0x67, 0x6e, 0x42, 0x52,
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

static char ros_kctrl_custom_interfaces__srv__RecordingControl__TYPE_NAME[] = "ros_kctrl_custom_interfaces/srv/RecordingControl";
static char builtin_interfaces__msg__Time__TYPE_NAME[] = "builtin_interfaces/msg/Time";
static char ros_kctrl_custom_interfaces__srv__RecordingControl_Event__TYPE_NAME[] = "ros_kctrl_custom_interfaces/srv/RecordingControl_Event";
static char ros_kctrl_custom_interfaces__srv__RecordingControl_Request__TYPE_NAME[] = "ros_kctrl_custom_interfaces/srv/RecordingControl_Request";
static char ros_kctrl_custom_interfaces__srv__RecordingControl_Response__TYPE_NAME[] = "ros_kctrl_custom_interfaces/srv/RecordingControl_Response";
static char service_msgs__msg__ServiceEventInfo__TYPE_NAME[] = "service_msgs/msg/ServiceEventInfo";

// Define type names, field names, and default values
static char ros_kctrl_custom_interfaces__srv__RecordingControl__FIELD_NAME__request_message[] = "request_message";
static char ros_kctrl_custom_interfaces__srv__RecordingControl__FIELD_NAME__response_message[] = "response_message";
static char ros_kctrl_custom_interfaces__srv__RecordingControl__FIELD_NAME__event_message[] = "event_message";

static rosidl_runtime_c__type_description__Field ros_kctrl_custom_interfaces__srv__RecordingControl__FIELDS[] = {
  {
    {ros_kctrl_custom_interfaces__srv__RecordingControl__FIELD_NAME__request_message, 15, 15},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {ros_kctrl_custom_interfaces__srv__RecordingControl_Request__TYPE_NAME, 56, 56},
    },
    {NULL, 0, 0},
  },
  {
    {ros_kctrl_custom_interfaces__srv__RecordingControl__FIELD_NAME__response_message, 16, 16},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {ros_kctrl_custom_interfaces__srv__RecordingControl_Response__TYPE_NAME, 57, 57},
    },
    {NULL, 0, 0},
  },
  {
    {ros_kctrl_custom_interfaces__srv__RecordingControl__FIELD_NAME__event_message, 13, 13},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {ros_kctrl_custom_interfaces__srv__RecordingControl_Event__TYPE_NAME, 54, 54},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription ros_kctrl_custom_interfaces__srv__RecordingControl__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    {NULL, 0, 0},
  },
  {
    {ros_kctrl_custom_interfaces__srv__RecordingControl_Event__TYPE_NAME, 54, 54},
    {NULL, 0, 0},
  },
  {
    {ros_kctrl_custom_interfaces__srv__RecordingControl_Request__TYPE_NAME, 56, 56},
    {NULL, 0, 0},
  },
  {
    {ros_kctrl_custom_interfaces__srv__RecordingControl_Response__TYPE_NAME, 57, 57},
    {NULL, 0, 0},
  },
  {
    {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
ros_kctrl_custom_interfaces__srv__RecordingControl__get_type_description(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {ros_kctrl_custom_interfaces__srv__RecordingControl__TYPE_NAME, 48, 48},
      {ros_kctrl_custom_interfaces__srv__RecordingControl__FIELDS, 3, 3},
    },
    {ros_kctrl_custom_interfaces__srv__RecordingControl__REFERENCED_TYPE_DESCRIPTIONS, 5, 5},
  };
  if (!constructed) {
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[1].fields = ros_kctrl_custom_interfaces__srv__RecordingControl_Event__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[2].fields = ros_kctrl_custom_interfaces__srv__RecordingControl_Request__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[3].fields = ros_kctrl_custom_interfaces__srv__RecordingControl_Response__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&service_msgs__msg__ServiceEventInfo__EXPECTED_HASH, service_msgs__msg__ServiceEventInfo__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[4].fields = service_msgs__msg__ServiceEventInfo__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char ros_kctrl_custom_interfaces__srv__RecordingControl_Request__FIELD_NAME__sounder_name[] = "sounder_name";
static char ros_kctrl_custom_interfaces__srv__RecordingControl_Request__FIELD_NAME__turn_on[] = "turn_on";

static rosidl_runtime_c__type_description__Field ros_kctrl_custom_interfaces__srv__RecordingControl_Request__FIELDS[] = {
  {
    {ros_kctrl_custom_interfaces__srv__RecordingControl_Request__FIELD_NAME__sounder_name, 12, 12},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {ros_kctrl_custom_interfaces__srv__RecordingControl_Request__FIELD_NAME__turn_on, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_BOOLEAN,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
ros_kctrl_custom_interfaces__srv__RecordingControl_Request__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {ros_kctrl_custom_interfaces__srv__RecordingControl_Request__TYPE_NAME, 56, 56},
      {ros_kctrl_custom_interfaces__srv__RecordingControl_Request__FIELDS, 2, 2},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char ros_kctrl_custom_interfaces__srv__RecordingControl_Response__FIELD_NAME__success[] = "success";
static char ros_kctrl_custom_interfaces__srv__RecordingControl_Response__FIELD_NAME__message[] = "message";

static rosidl_runtime_c__type_description__Field ros_kctrl_custom_interfaces__srv__RecordingControl_Response__FIELDS[] = {
  {
    {ros_kctrl_custom_interfaces__srv__RecordingControl_Response__FIELD_NAME__success, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_BOOLEAN,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {ros_kctrl_custom_interfaces__srv__RecordingControl_Response__FIELD_NAME__message, 7, 7},
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
ros_kctrl_custom_interfaces__srv__RecordingControl_Response__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {ros_kctrl_custom_interfaces__srv__RecordingControl_Response__TYPE_NAME, 57, 57},
      {ros_kctrl_custom_interfaces__srv__RecordingControl_Response__FIELDS, 2, 2},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}
// Define type names, field names, and default values
static char ros_kctrl_custom_interfaces__srv__RecordingControl_Event__FIELD_NAME__info[] = "info";
static char ros_kctrl_custom_interfaces__srv__RecordingControl_Event__FIELD_NAME__request[] = "request";
static char ros_kctrl_custom_interfaces__srv__RecordingControl_Event__FIELD_NAME__response[] = "response";

static rosidl_runtime_c__type_description__Field ros_kctrl_custom_interfaces__srv__RecordingControl_Event__FIELDS[] = {
  {
    {ros_kctrl_custom_interfaces__srv__RecordingControl_Event__FIELD_NAME__info, 4, 4},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    },
    {NULL, 0, 0},
  },
  {
    {ros_kctrl_custom_interfaces__srv__RecordingControl_Event__FIELD_NAME__request, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_BOUNDED_SEQUENCE,
      1,
      0,
      {ros_kctrl_custom_interfaces__srv__RecordingControl_Request__TYPE_NAME, 56, 56},
    },
    {NULL, 0, 0},
  },
  {
    {ros_kctrl_custom_interfaces__srv__RecordingControl_Event__FIELD_NAME__response, 8, 8},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_BOUNDED_SEQUENCE,
      1,
      0,
      {ros_kctrl_custom_interfaces__srv__RecordingControl_Response__TYPE_NAME, 57, 57},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription ros_kctrl_custom_interfaces__srv__RecordingControl_Event__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    {NULL, 0, 0},
  },
  {
    {ros_kctrl_custom_interfaces__srv__RecordingControl_Request__TYPE_NAME, 56, 56},
    {NULL, 0, 0},
  },
  {
    {ros_kctrl_custom_interfaces__srv__RecordingControl_Response__TYPE_NAME, 57, 57},
    {NULL, 0, 0},
  },
  {
    {service_msgs__msg__ServiceEventInfo__TYPE_NAME, 33, 33},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
ros_kctrl_custom_interfaces__srv__RecordingControl_Event__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {ros_kctrl_custom_interfaces__srv__RecordingControl_Event__TYPE_NAME, 54, 54},
      {ros_kctrl_custom_interfaces__srv__RecordingControl_Event__FIELDS, 3, 3},
    },
    {ros_kctrl_custom_interfaces__srv__RecordingControl_Event__REFERENCED_TYPE_DESCRIPTIONS, 4, 4},
  };
  if (!constructed) {
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[1].fields = ros_kctrl_custom_interfaces__srv__RecordingControl_Request__get_type_description(NULL)->type_description.fields;
    description.referenced_type_descriptions.data[2].fields = ros_kctrl_custom_interfaces__srv__RecordingControl_Response__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&service_msgs__msg__ServiceEventInfo__EXPECTED_HASH, service_msgs__msg__ServiceEventInfo__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[3].fields = service_msgs__msg__ServiceEventInfo__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "string sounder_name\n"
  "bool turn_on\n"
  "---\n"
  "bool success\n"
  "string message";

static char srv_encoding[] = "srv";
static char implicit_encoding[] = "implicit";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
ros_kctrl_custom_interfaces__srv__RecordingControl__get_individual_type_description_source(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {ros_kctrl_custom_interfaces__srv__RecordingControl__TYPE_NAME, 48, 48},
    {srv_encoding, 3, 3},
    {toplevel_type_raw_source, 64, 64},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
ros_kctrl_custom_interfaces__srv__RecordingControl_Request__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {ros_kctrl_custom_interfaces__srv__RecordingControl_Request__TYPE_NAME, 56, 56},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
ros_kctrl_custom_interfaces__srv__RecordingControl_Response__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {ros_kctrl_custom_interfaces__srv__RecordingControl_Response__TYPE_NAME, 57, 57},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource *
ros_kctrl_custom_interfaces__srv__RecordingControl_Event__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {ros_kctrl_custom_interfaces__srv__RecordingControl_Event__TYPE_NAME, 54, 54},
    {implicit_encoding, 8, 8},
    {NULL, 0, 0},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
ros_kctrl_custom_interfaces__srv__RecordingControl__get_type_description_sources(
  const rosidl_service_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[6];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 6, 6};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *ros_kctrl_custom_interfaces__srv__RecordingControl__get_individual_type_description_source(NULL),
    sources[1] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    sources[2] = *ros_kctrl_custom_interfaces__srv__RecordingControl_Event__get_individual_type_description_source(NULL);
    sources[3] = *ros_kctrl_custom_interfaces__srv__RecordingControl_Request__get_individual_type_description_source(NULL);
    sources[4] = *ros_kctrl_custom_interfaces__srv__RecordingControl_Response__get_individual_type_description_source(NULL);
    sources[5] = *service_msgs__msg__ServiceEventInfo__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
ros_kctrl_custom_interfaces__srv__RecordingControl_Request__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *ros_kctrl_custom_interfaces__srv__RecordingControl_Request__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
ros_kctrl_custom_interfaces__srv__RecordingControl_Response__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *ros_kctrl_custom_interfaces__srv__RecordingControl_Response__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
ros_kctrl_custom_interfaces__srv__RecordingControl_Event__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[5];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 5, 5};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *ros_kctrl_custom_interfaces__srv__RecordingControl_Event__get_individual_type_description_source(NULL),
    sources[1] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    sources[2] = *ros_kctrl_custom_interfaces__srv__RecordingControl_Request__get_individual_type_description_source(NULL);
    sources[3] = *ros_kctrl_custom_interfaces__srv__RecordingControl_Response__get_individual_type_description_source(NULL);
    sources[4] = *service_msgs__msg__ServiceEventInfo__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}
