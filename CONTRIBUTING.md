# Contributing to ros2_kctrl_interface_pkg
Thanks for your interest in contributing! This project wraps Kongsberg EM Multibeam EchoSounder devices into a ROS2 interface, and contributions help make it more robust, flexible, and user-friendly.

## Before You Start
This is a student-developed project, so contributions are especially welcome to improve usability, documentation, and compatibility.

The wrapper is tested with ROS2 Jazzy on WSL2. If you're using a different setup, please note any platform-specific issues.

## How to Contribute

### 1. Fork & Clone

```bash
git clone https://github.com/<your-username>/ros2_kctrl_interface_pkg.git
cd ros2_kctrl_interface_pkg
```

### 2. Set Up Your Workspace
Follow the setup instructions in the README to build and source your workspace.

### 3. Make Your Changes
Follow the coding conventions used in the repo.

If you're modifying ROS2 services or messages, update the ros2_kctrl_custom_interfaces package accordingly.

For shell aliases, edit ros_kctrl_aliases.sh and document your changes.

### 4. Test Thoroughly
Use ros2 run and ros2 launch to verify your changes.

If possible, test with a real K-Controller device or simulated data.

### 5. Submit a Pull Request
Include a clear description of your changes.

Reference any related issues or feature requests.

If your PR adds new functionality, consider updating the README and alias guide.

## Testing Guidelines
Unit tests are encouraged for new logic.

If you add a new service or topic, include a minimal example of how to call or subscribe to it.

## Communication
For questions or discussions, feel free to open an issue.

If you're unsure where to start, check the Troubleshooting or Advanced Usage sections.
