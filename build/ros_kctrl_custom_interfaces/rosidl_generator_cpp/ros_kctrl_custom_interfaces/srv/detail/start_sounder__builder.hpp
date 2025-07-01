// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from ros_kctrl_custom_interfaces:srv/StartSounder.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "ros_kctrl_custom_interfaces/srv/start_sounder.hpp"


#ifndef ROS_KCTRL_CUSTOM_INTERFACES__SRV__DETAIL__START_SOUNDER__BUILDER_HPP_
#define ROS_KCTRL_CUSTOM_INTERFACES__SRV__DETAIL__START_SOUNDER__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "ros_kctrl_custom_interfaces/srv/detail/start_sounder__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace ros_kctrl_custom_interfaces
{

namespace srv
{

namespace builder
{

class Init_StartSounder_Request_sounder_name
{
public:
  Init_StartSounder_Request_sounder_name()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::ros_kctrl_custom_interfaces::srv::StartSounder_Request sounder_name(::ros_kctrl_custom_interfaces::srv::StartSounder_Request::_sounder_name_type arg)
  {
    msg_.sounder_name = std::move(arg);
    return std::move(msg_);
  }

private:
  ::ros_kctrl_custom_interfaces::srv::StartSounder_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::ros_kctrl_custom_interfaces::srv::StartSounder_Request>()
{
  return ros_kctrl_custom_interfaces::srv::builder::Init_StartSounder_Request_sounder_name();
}

}  // namespace ros_kctrl_custom_interfaces


namespace ros_kctrl_custom_interfaces
{

namespace srv
{

namespace builder
{

class Init_StartSounder_Response_message
{
public:
  explicit Init_StartSounder_Response_message(::ros_kctrl_custom_interfaces::srv::StartSounder_Response & msg)
  : msg_(msg)
  {}
  ::ros_kctrl_custom_interfaces::srv::StartSounder_Response message(::ros_kctrl_custom_interfaces::srv::StartSounder_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return std::move(msg_);
  }

private:
  ::ros_kctrl_custom_interfaces::srv::StartSounder_Response msg_;
};

class Init_StartSounder_Response_success
{
public:
  Init_StartSounder_Response_success()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_StartSounder_Response_message success(::ros_kctrl_custom_interfaces::srv::StartSounder_Response::_success_type arg)
  {
    msg_.success = std::move(arg);
    return Init_StartSounder_Response_message(msg_);
  }

private:
  ::ros_kctrl_custom_interfaces::srv::StartSounder_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::ros_kctrl_custom_interfaces::srv::StartSounder_Response>()
{
  return ros_kctrl_custom_interfaces::srv::builder::Init_StartSounder_Response_success();
}

}  // namespace ros_kctrl_custom_interfaces


namespace ros_kctrl_custom_interfaces
{

namespace srv
{

namespace builder
{

class Init_StartSounder_Event_response
{
public:
  explicit Init_StartSounder_Event_response(::ros_kctrl_custom_interfaces::srv::StartSounder_Event & msg)
  : msg_(msg)
  {}
  ::ros_kctrl_custom_interfaces::srv::StartSounder_Event response(::ros_kctrl_custom_interfaces::srv::StartSounder_Event::_response_type arg)
  {
    msg_.response = std::move(arg);
    return std::move(msg_);
  }

private:
  ::ros_kctrl_custom_interfaces::srv::StartSounder_Event msg_;
};

class Init_StartSounder_Event_request
{
public:
  explicit Init_StartSounder_Event_request(::ros_kctrl_custom_interfaces::srv::StartSounder_Event & msg)
  : msg_(msg)
  {}
  Init_StartSounder_Event_response request(::ros_kctrl_custom_interfaces::srv::StartSounder_Event::_request_type arg)
  {
    msg_.request = std::move(arg);
    return Init_StartSounder_Event_response(msg_);
  }

private:
  ::ros_kctrl_custom_interfaces::srv::StartSounder_Event msg_;
};

class Init_StartSounder_Event_info
{
public:
  Init_StartSounder_Event_info()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_StartSounder_Event_request info(::ros_kctrl_custom_interfaces::srv::StartSounder_Event::_info_type arg)
  {
    msg_.info = std::move(arg);
    return Init_StartSounder_Event_request(msg_);
  }

private:
  ::ros_kctrl_custom_interfaces::srv::StartSounder_Event msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::ros_kctrl_custom_interfaces::srv::StartSounder_Event>()
{
  return ros_kctrl_custom_interfaces::srv::builder::Init_StartSounder_Event_info();
}

}  // namespace ros_kctrl_custom_interfaces

#endif  // ROS_KCTRL_CUSTOM_INTERFACES__SRV__DETAIL__START_SOUNDER__BUILDER_HPP_
