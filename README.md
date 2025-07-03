# ROS2 K-Controller Wrapper

A ROS2 package for controlling and monitoring K-Controller devices via UDP.

## Features

- ROS2 services for device control
- UDP receiver node to publish device status and feedback

## How to build

```bash
git clone https://github.com/yourusername/ros2-kctrl-wrapper.git
cd ros2-kctrl-wrapper
colcon build
source install/setup.bash
```

## Usage

Launch both nodes:
```bash
ros2 launch ros_kctrl_interface_pkg ros2_ctrl_launch.py
```

## License

Apache 2.0
