// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from ros_kctrl_custom_interfaces:srv/RequestPUParameters.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "ros_kctrl_custom_interfaces/srv/request_pu_parameters.hpp"


#ifndef ROS_KCTRL_CUSTOM_INTERFACES__SRV__DETAIL__REQUEST_PU_PARAMETERS__STRUCT_HPP_
#define ROS_KCTRL_CUSTOM_INTERFACES__SRV__DETAIL__REQUEST_PU_PARAMETERS__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__ros_kctrl_custom_interfaces__srv__RequestPUParameters_Request __attribute__((deprecated))
#else
# define DEPRECATED__ros_kctrl_custom_interfaces__srv__RequestPUParameters_Request __declspec(deprecated)
#endif

namespace ros_kctrl_custom_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct RequestPUParameters_Request_
{
  using Type = RequestPUParameters_Request_<ContainerAllocator>;

  explicit RequestPUParameters_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->sounder_name = "";
    }
  }

  explicit RequestPUParameters_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : sounder_name(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->sounder_name = "";
    }
  }

  // field types and members
  using _sounder_name_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _sounder_name_type sounder_name;
  using _param_names_type =
    std::vector<std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>>>;
  _param_names_type param_names;

  // setters for named parameter idiom
  Type & set__sounder_name(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->sounder_name = _arg;
    return *this;
  }
  Type & set__param_names(
    const std::vector<std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>>> & _arg)
  {
    this->param_names = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    ros_kctrl_custom_interfaces::srv::RequestPUParameters_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const ros_kctrl_custom_interfaces::srv::RequestPUParameters_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      ros_kctrl_custom_interfaces::srv::RequestPUParameters_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      ros_kctrl_custom_interfaces::srv::RequestPUParameters_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__ros_kctrl_custom_interfaces__srv__RequestPUParameters_Request
    std::shared_ptr<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__ros_kctrl_custom_interfaces__srv__RequestPUParameters_Request
    std::shared_ptr<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const RequestPUParameters_Request_ & other) const
  {
    if (this->sounder_name != other.sounder_name) {
      return false;
    }
    if (this->param_names != other.param_names) {
      return false;
    }
    return true;
  }
  bool operator!=(const RequestPUParameters_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct RequestPUParameters_Request_

// alias to use template instance with default allocator
using RequestPUParameters_Request =
  ros_kctrl_custom_interfaces::srv::RequestPUParameters_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace ros_kctrl_custom_interfaces


#ifndef _WIN32
# define DEPRECATED__ros_kctrl_custom_interfaces__srv__RequestPUParameters_Response __attribute__((deprecated))
#else
# define DEPRECATED__ros_kctrl_custom_interfaces__srv__RequestPUParameters_Response __declspec(deprecated)
#endif

namespace ros_kctrl_custom_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct RequestPUParameters_Response_
{
  using Type = RequestPUParameters_Response_<ContainerAllocator>;

  explicit RequestPUParameters_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->success = false;
      this->message = "";
    }
  }

  explicit RequestPUParameters_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : message(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->success = false;
      this->message = "";
    }
  }

  // field types and members
  using _success_type =
    bool;
  _success_type success;
  using _message_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _message_type message;

  // setters for named parameter idiom
  Type & set__success(
    const bool & _arg)
  {
    this->success = _arg;
    return *this;
  }
  Type & set__message(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->message = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    ros_kctrl_custom_interfaces::srv::RequestPUParameters_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const ros_kctrl_custom_interfaces::srv::RequestPUParameters_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      ros_kctrl_custom_interfaces::srv::RequestPUParameters_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      ros_kctrl_custom_interfaces::srv::RequestPUParameters_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__ros_kctrl_custom_interfaces__srv__RequestPUParameters_Response
    std::shared_ptr<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__ros_kctrl_custom_interfaces__srv__RequestPUParameters_Response
    std::shared_ptr<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const RequestPUParameters_Response_ & other) const
  {
    if (this->success != other.success) {
      return false;
    }
    if (this->message != other.message) {
      return false;
    }
    return true;
  }
  bool operator!=(const RequestPUParameters_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct RequestPUParameters_Response_

// alias to use template instance with default allocator
using RequestPUParameters_Response =
  ros_kctrl_custom_interfaces::srv::RequestPUParameters_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace ros_kctrl_custom_interfaces


// Include directives for member types
// Member 'info'
#include "service_msgs/msg/detail/service_event_info__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__ros_kctrl_custom_interfaces__srv__RequestPUParameters_Event __attribute__((deprecated))
#else
# define DEPRECATED__ros_kctrl_custom_interfaces__srv__RequestPUParameters_Event __declspec(deprecated)
#endif

namespace ros_kctrl_custom_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct RequestPUParameters_Event_
{
  using Type = RequestPUParameters_Event_<ContainerAllocator>;

  explicit RequestPUParameters_Event_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : info(_init)
  {
    (void)_init;
  }

  explicit RequestPUParameters_Event_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : info(_alloc, _init)
  {
    (void)_init;
  }

  // field types and members
  using _info_type =
    service_msgs::msg::ServiceEventInfo_<ContainerAllocator>;
  _info_type info;
  using _request_type =
    rosidl_runtime_cpp::BoundedVector<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Request_<ContainerAllocator>, 1, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Request_<ContainerAllocator>>>;
  _request_type request;
  using _response_type =
    rosidl_runtime_cpp::BoundedVector<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Response_<ContainerAllocator>, 1, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Response_<ContainerAllocator>>>;
  _response_type response;

  // setters for named parameter idiom
  Type & set__info(
    const service_msgs::msg::ServiceEventInfo_<ContainerAllocator> & _arg)
  {
    this->info = _arg;
    return *this;
  }
  Type & set__request(
    const rosidl_runtime_cpp::BoundedVector<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Request_<ContainerAllocator>, 1, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Request_<ContainerAllocator>>> & _arg)
  {
    this->request = _arg;
    return *this;
  }
  Type & set__response(
    const rosidl_runtime_cpp::BoundedVector<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Response_<ContainerAllocator>, 1, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Response_<ContainerAllocator>>> & _arg)
  {
    this->response = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    ros_kctrl_custom_interfaces::srv::RequestPUParameters_Event_<ContainerAllocator> *;
  using ConstRawPtr =
    const ros_kctrl_custom_interfaces::srv::RequestPUParameters_Event_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Event_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Event_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      ros_kctrl_custom_interfaces::srv::RequestPUParameters_Event_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Event_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      ros_kctrl_custom_interfaces::srv::RequestPUParameters_Event_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Event_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Event_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Event_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__ros_kctrl_custom_interfaces__srv__RequestPUParameters_Event
    std::shared_ptr<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Event_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__ros_kctrl_custom_interfaces__srv__RequestPUParameters_Event
    std::shared_ptr<ros_kctrl_custom_interfaces::srv::RequestPUParameters_Event_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const RequestPUParameters_Event_ & other) const
  {
    if (this->info != other.info) {
      return false;
    }
    if (this->request != other.request) {
      return false;
    }
    if (this->response != other.response) {
      return false;
    }
    return true;
  }
  bool operator!=(const RequestPUParameters_Event_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct RequestPUParameters_Event_

// alias to use template instance with default allocator
using RequestPUParameters_Event =
  ros_kctrl_custom_interfaces::srv::RequestPUParameters_Event_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace ros_kctrl_custom_interfaces

namespace ros_kctrl_custom_interfaces
{

namespace srv
{

struct RequestPUParameters
{
  using Request = ros_kctrl_custom_interfaces::srv::RequestPUParameters_Request;
  using Response = ros_kctrl_custom_interfaces::srv::RequestPUParameters_Response;
  using Event = ros_kctrl_custom_interfaces::srv::RequestPUParameters_Event;
};

}  // namespace srv

}  // namespace ros_kctrl_custom_interfaces

#endif  // ROS_KCTRL_CUSTOM_INTERFACES__SRV__DETAIL__REQUEST_PU_PARAMETERS__STRUCT_HPP_
