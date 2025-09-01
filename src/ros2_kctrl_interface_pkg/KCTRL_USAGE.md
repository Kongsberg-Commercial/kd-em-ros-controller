# Simple K-Controller Configuration Guide
# =====================================

# There is ONE launch file that handles everything: ros2_ctrl.launch.py

## Method 1: Default Settings
ros2 launch ros2_kctrl_interface_pkg ros2_ctrl.launch.py

## Method 2: Custom IP/Ports via Command Line
ros2 launch ros2_kctrl_interface_pkg ros2_ctrl.launch.py kctrl_ip:=10.0.0.100

ros2 launch ros2_kctrl_interface_pkg ros2_ctrl.launch.py \
    kctrl_ip:=192.168.1.50 \
    kctrl_port:=5000 \
    listen_port:=5001

## Method 3: YAML Configuration (Alternative approach)
# For YAML configs, run the nodes individually:

# Start receiver:
ros2 run ros2_kctrl_interface_pkg ros2ctrl_udp_receiver \
    --ros-args --params-file /path/to/your/config.yaml

# Start wrapper (in another terminal):
ros2 run ros2_kctrl_interface_pkg ros2ctrl_wrapper_node \
    --ros-args --params-file /path/to/your/config.yaml

## Easy Helper Script
# Use the helper script for guided setup:
./run_kctrl.sh
