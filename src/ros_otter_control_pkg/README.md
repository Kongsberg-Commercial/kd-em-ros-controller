# ROS2 Otter Control Package

This package provides ROS2 integration for controlling Maritime Robotics Otter USV via TCP communication.

## Quick Start

### Using the Launch Script (Recommended)

Make the script executable:
```bash
chmod +x scripts/run.sh
```

Run with default configuration:
```bash
./scripts/run.sh
```

Build and run:
```bash
./scripts/run.sh --build
```

Override IP and port:
```bash
./scripts/run.sh --ip 192.168.1.100 --port 2010
```

### Manual Launch

```bash
# Build the package
colcon build --packages-select ros_otter_control_pkg

# Source the workspace
source install/setup.bash

# Run with config file
ros2 run ros_otter_control_pkg otter_tcp_node --ros-args --params-file src/ros_otter_control_pkg/config/otter_config.yaml
```

## Configuration

Edit `config/otter_config.yaml` to customize:
- TCP connection settings
- Position thresholds
- Retry intervals
- Automatic leg progression settings

## Services

- `/set_drift_mode` - Set drift mode
- `/set_manual_mode` - Manual control
- `/set_course_mode` - Course following
- `/set_leg_mode` - Leg following
- `/set_station_mode` - Station keeping
- `/set_parameters` - Update control parameters

## Topics

### Published
- `/gps_info` - GPS position data
- `/imu_info` - IMU data
- `/mode_info` - Current mode information
- `/error_info` - Error messages
- `/otter_info` - Otter-specific data
- `/leg_status` - Leg completion status

## Dependencies

- ROS2 (Humble/Iron/Jazzy)
- ros_otter_custom_interfaces package
