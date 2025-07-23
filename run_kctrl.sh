#!/bin/bash

# K-Controller ROS2 Interface - Easy Configuration Script
# This script helps you run the system with different configuration methods

echo "K-Controller ROS2 Interface Configuration Helper"
echo "=================================================="

# Source ROS2 and workspace
cd /home/arveds/ros2_ws
source /opt/ros/jazzy/setup.bash
source install/setup.bash

echo ""
echo "Available configuration methods:"
echo "1. Use default settings (192.168.48.1:4001)"
echo "2. Use custom IP via command line"
echo "3. Use YAML config file (workspace root)"
echo "4. Use YAML config file (custom path)"
echo "5. Edit the workspace config file"
echo ""

read -p "Choose option (1-5): " choice

case $choice in
    1)
        echo "Starting with default settings..."
        ros2 launch ros_kctrl_interface_pkg ros2_ctrl.launch.py
        ;;
    2)
        read -p "Enter K-Controller IP address: " ip
        read -p "Enter K-Controller port (default 14002): " port
        port=${port:-14002}
        read -p "Enter listen port (default 4002): " listen_port
        listen_port=${listen_port:-4002}
        
        echo "Starting with IP: $ip:$port, listening on: $listen_port"
        ros2 launch ros_kctrl_interface_pkg ros2_ctrl.launch.py \
            kctrl_ip:=$ip \
            kctrl_port:=$port \
            listen_port:=$listen_port
        ;;
    3)
        echo "Using config file: /home/arveds/ros2_ws/kctrl_config.yaml"
        if [ -f "/home/arveds/ros2_ws/kctrl_config.yaml" ]; then
            ros2 launch ros_kctrl_interface_pkg ros2_ctrl.launch.py \
                config_file:=/home/arveds/ros2_ws/kctrl_config.yaml
        else
            echo "Config file not found! Please create it first."
            exit 1
        fi
        ;;
    4)
        read -p "Enter path to YAML config file: " config_path
        if [ -f "$config_path" ]; then
            echo "Using config file: $config_path"
            ros2 launch ros_kctrl_interface_pkg ros2_ctrl.launch.py \
                config_file:=$config_path
        else
            echo "Config file not found: $config_path"
            exit 1
        fi
        ;;
    5)
        echo "Opening config file for editing..."
        if [ -f "/home/arveds/ros2_ws/kctrl_config.yaml" ]; then
            ${EDITOR:-nano} /home/arveds/ros2_ws/kctrl_config.yaml
            echo "Config file updated. Run this script again to use the new settings."
        else
            echo "Config file not found. Creating a new one..."
            cp install/ros_kctrl_interface_pkg/share/ros_kctrl_interface_pkg/config/kctrl_config.yaml /home/arveds/ros2_ws/kctrl_config.yaml
            ${EDITOR:-nano} /home/arveds/ros2_ws/kctrl_config.yaml
        fi
        ;;
    *)
        echo "Invalid option. Exiting."
        exit 1
        ;;
esac
