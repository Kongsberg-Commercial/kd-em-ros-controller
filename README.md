# ROS2 K-Controller Wrapper

A ROS 2 package to interface with the Kongsberg K-Controller via UDP, providing services and topics for controlling and monitoring sounder devices.  
This project is designed to make integration with K-Controller hardware easy for robotics and automation applications using ROS 2.

---

## Table of Contents

- [Features](#features)
- [System Requirements](#system-requirements)
- [Prerequisites](#prerequisites)
- [Getting Started](#getting-started)
  - [1. Clone the Repository](#1-clone-the-repository)
  - [2. Install Dependencies](#2-install-dependencies)
  - [3. Build the Workspace](#3-build-the-workspace)
  - [4. Source the Workspace](#4-source-the-workspace)
- [Usage](#usage)
  - [Launching Nodes](#launching-nodes)
  - [Bash Aliases Overview](#bash-aliases-overview)
  - [Interacting with the K-Controller](#interacting-with-the-k-controller)
- [WSL Networking Note](#wsl-networking-note)
- [Package Structure](#package-structure)
- [Custom Messages and Services](#custom-messages-and-services)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)

---

## Features

- **UDP communication** with a K-Controller device.
- **ROS 2 services** for sending commands and receiving parameters.
- **ROS 2 topics** for real-time status and feedback.
- **Custom message and service definitions** for sounder operations.
- **Quick setup scripts** and bash aliases for convenience.

---

## System Requirements

- **Operating System:** Ubuntu 24.04 (tested under WSL2)
- **ROS 2 Distribution:** Jazzy
- **Tested Environment:** Developed and tested exclusively under WSL2 on Windows with Ubuntu 24.04 and ROS 2 Jazzy. Functionality on other platforms may require adaptation.
- **License:** Apache License 2.0

---

## Prerequisites

- **C++ compiler:** g++ (installed by default on Ubuntu)
- **git:** for cloning the repository

---

## Getting Started

> **New to WSL and ROS 2 Jazzy on Ubuntu 24.04?**  
> See the [WSL Ubuntu 24.04](https://documentation.ubuntu.com/wsl/latest/howto/install-ubuntu-wsl2/) & [ROS 2 Jazzy Setup Tutorial](https://docs.ros.org/en/jazzy/Installation/Ubuntu-Install-Debians.html#wsl-support) for a full guide to installing WSL, Ubuntu 24.04, and ROS 2 Jazzy.

### 1. Clone the Repository

Open a terminal and run:
```bash
git clone https://github.com/Kongsberg-Commercial/kd-ROS2-KController-wrapper.git
cd kd-ROS2-KController-wrapper
```

### 2. Install Dependencies

Install ROS2 if you haven't already.  
Then, install package dependencies (replace `<ros-distro>` with your ROS2 distribution, e.g., `jazzy`):

```bash
sudo apt update
sudo apt install ros-<ros-distro>-ros-base python3-colcon-common-extensions
```

If your package uses additional dependencies (update this list as needed):

```bash
sudo apt install ros-<ros-distro>-rclcpp ros-<ros-distro>-std-msgs ros-<ros-distro>-example-interfaces
```

### 3. Build the Workspace

From the workspace root:
```bash
colcon build
```

### 4. Source the Workspace

After building, source the setup script to overlay this workspace on your ROS2 environment:

```bash
source /opt/ros/<ros-distro>/setup.bash
```

```bash
source install/setup.bash
```
To make this automatic in every new terminal, add the line above to your `~/.bashrc` or use the provided bash alias script.

---

## Usage

### Launching Nodes

To start the K-Controller wrapper and UDP receiver nodes:

```bash
ros2 launch ros_kctrl_interface_pkg ros2_ctrl_launch.py
```

This will launch both the service-wrapper and UDP receiver nodes.

---

### Bash Aliases Overview

A bash aliases script is provided for convenience in `.ros2_kctrl_aliases.sh`.  
To use it:
```bash
source .ros2_kctrl_aliases.sh
```
Or add this line to your `~/.bashrc` for automatic loading.

#### Common Aliases

| Alias                                | Description                             |
|--------------------------------------|-----------------------------------------|
| `kctrl_status`                       | Echo K-Controller status                |
| `kctrl_version`                      | Echo K-Controller version               |
| `ros2_start_sounder SOUNDER`         | Send start sounder command              |
| ...                                  | ...                                     |

**Tip:** See [ros2_kctrl_bash_command.md](Lists/ros2_kctrl_bash_commands.md) for the full list and details.

---

### Interacting with the K-Controller

- **Service calls:**  
  Use `ros2 service call` with your custom service types (see [Custom Messages and Services](#custom-messages-and-services)).
- **Topic echo:**  
  Use `ros2 topic echo` to monitor status and feedback topics.

---

## WSL Networking Note

If you are running this package under WSL and need to receive UDP packets from Windows applications, you may need to forward UDP traffic from Windows localhost to your WSL IP.

You can use [ncat](https://nmap.org/ncat/) for this purpose:

```powershell
ncat.exe -ulk 127.0.0.1 4002 --sh-exec "ncat.exe --udp 192.168.48.211 4002"
```

- Replace `192.168.48.211` with your current WSL IP address (find it with `ip addr` inside WSL).
- Port `4002` is an example; use the port your application requires.
- [Download ncat](https://nmap.org/ncat/) if you do not already have it.

---

## Package Structure

```
kd-ROS2-KController-wrapper/
├── ros_kctrl_interface_pkg/           # Main ROS2 package
│   ├── src/
|   |   ├── ros2_udp_client.cpp         # UDP client for wrapper node
│   │   ├── ros2_ctrl_wrapper_node.cpp  # Wrapper node source
│   │   └── ros2_udp_receiver.cpp       # UDP receiver node source
│   ├── include/
|   |   ├── nlohmann/
|   |   |   └── json.hpp                # json header file
|   |   ├── ros_kctrl_interface_pkg/
|   |   |     ├── ros2ctrl_udp_client.hpp   #UDP client header
|   |   |     └── ros2ctrl_udp_receiver.hpp #UDP receiver header
|   ├── launch/
|   |   └── ros2_kctrl_launch.py       # launch file
│   ├── CMakeLists.txt
│   ├── package.xml
│   └── ...
├── ros_kctrl_custom_interfaces/       # Custom messages/services
│   ├── msg/
│   ├── srv/
│   ├── CMakeLists.txt
│   ├── package.xml
│   └── ...
├── .ros2_kctrl_aliases.sh             # Bash aliases for convenience
├── README.md
└── LICENSE
```

---

## Custom Messages and Services

This repository defines custom messages and services for communication with the K-Controller.

**Example:**  
- `SounderCommand.srv`
- `RequestPUParameters.srv`
- `SetPUParameters.srv`

Find these in `ros_kctrl_custom_interfaces/msg` and `ros_kctrl_custom_interfaces/srv`.

Use them with `ros2 service call` and `ros2 topic pub/echo` as needed.

---

## Troubleshooting

**Common Issues:**

### Build errors:  
- Make sure you sourced the correct ROS2 environment (`source /opt/ros/<ros-distro>/setup.bash`) before building.
- Make sure you deleted all old build artifacts:
  ```bash
  rm -rf build/ install/ log/
  ```

### "Executable not found" when launching:
- Ensure `colcon build` completed successfully.
- Double-check that you are sourcing `install/setup.bash` in your current terminal.

### "Service/Topic not found":
- Confirm that the nodes are running (`ros2 node list`).
- Use `ros2 service list` and `ros2 topic list` to check available interfaces.

### Can't communicate with K-Controller hardware:
- Check your network settings and firewall.
- Make sure you have the correct IP address and port configured in the launch file or parameters.
- If running in WSL, ensure UDP forwarding is set up as described above.

---

## Contributing

1. Fork the repository on GitHub
2. Create a new feature branch (`git checkout -b my-feature`)
3. Commit your changes (`git commit -am 'Add new feature'`)
4. Push to the branch (`git push origin my-feature`)
5. Open a Pull Request

---

## License

This project is licensed under the Apache License 2.0. See [LICENSE](LICENSE) for details.

---

## Contact

For questions or support, open a GitHub issue or contact the maintainers at:
- [Kongsberg Commercial GitHub](https://github.com/Kongsberg-Commercial)

---
