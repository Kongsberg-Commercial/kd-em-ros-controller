#!/bin/bash

# ROS2 Otter Control Package Launch Script
set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}=== ROS2 Otter Control Package Launch Script ===${NC}"

# Get script directory
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
PACKAGE_DIR="$(dirname "$SCRIPT_DIR")"
WS_DIR="$(dirname "$(dirname "$PACKAGE_DIR")")"

echo -e "${BLUE}Package directory: ${PACKAGE_DIR}${NC}"
echo -e "${BLUE}Workspace directory: ${WS_DIR}${NC}"

# Check if we're in a ROS2 workspace
if [ ! -f "$WS_DIR/src/ros_otter_control_pkg/package.xml" ]; then
    echo -e "${RED}Error: Not in a valid ROS2 workspace or package not found${NC}"
    exit 1
fi

# Source ROS2 environment
echo -e "${YELLOW}Sourcing ROS2 environment...${NC}"
if [ -f "/opt/ros/humble/setup.bash" ]; then
    source /opt/ros/humble/setup.bash
elif [ -f "/opt/ros/iron/setup.bash" ]; then
    source /opt/ros/iron/setup.bash
elif [ -f "/opt/ros/jazzy/setup.bash" ]; then
    source /opt/ros/jazzy/setup.bash
else
    echo -e "${RED}Error: No ROS2 installation found${NC}"
    exit 1
fi

# Navigate to workspace
cd "$WS_DIR"

# Build the package if requested or if executable doesn't exist
BUILD_PACKAGE=false
if [ "$1" = "--build" ] || [ "$1" = "-b" ]; then
    BUILD_PACKAGE=true
elif [ ! -f "install/ros_otter_control_pkg/lib/ros_otter_control_pkg/otter_tcp_node" ]; then
    echo -e "${YELLOW}Executable not found, building package...${NC}"
    BUILD_PACKAGE=true
fi

if [ "$BUILD_PACKAGE" = true ]; then
    echo -e "${YELLOW}Building ros_otter_control_pkg...${NC}"
    colcon build --packages-select ros_otter_control_pkg --cmake-args -DCMAKE_BUILD_TYPE=Release
    if [ $? -ne 0 ]; then
        echo -e "${RED}Build failed!${NC}"
        exit 1
    fi
    echo -e "${GREEN}Build completed successfully!${NC}"
fi

# Source the workspace
echo -e "${YELLOW}Sourcing workspace...${NC}"
source install/setup.bash

# Check if config file exists
CONFIG_FILE="$PACKAGE_DIR/config/otter_config.yaml"
if [ ! -f "$CONFIG_FILE" ]; then
    echo -e "${RED}Error: Config file not found at $CONFIG_FILE${NC}"
    exit 1
fi

# Parse command line arguments
EXTRA_ARGS=""
while [[ $# -gt 0 ]]; do
    case $1 in
        --build|-b)
            # Already handled above
            shift
            ;;
        --config|-c)
            CONFIG_FILE="$2"
            shift 2
            ;;
        --ip)
            EXTRA_ARGS="$EXTRA_ARGS --ros-args -p device_ip:=$2"
            shift 2
            ;;
        --port)
            EXTRA_ARGS="$EXTRA_ARGS --ros-args -p device_port:=$2"
            shift 2
            ;;
        --help|-h)
            echo "Usage: $0 [OPTIONS]"
            echo ""
            echo "Options:"
            echo "  --build, -b           Build the package before running"
            echo "  --config FILE, -c     Use specific config file"
            echo "  --ip IP               Override device IP address"
            echo "  --port PORT           Override device port"
            echo "  --help, -h            Show this help message"
            echo ""
            echo "Examples:"
            echo "  $0                    Run with default config"
            echo "  $0 --build            Build and run"
            echo "  $0 --ip 192.168.1.100 --port 2010"
            echo "  $0 --config /path/to/custom_config.yaml"
            exit 0
            ;;
        *)
            echo -e "${RED}Unknown option: $1${NC}"
            echo "Use --help for usage information"
            exit 1
            ;;
    esac
done

echo -e "${YELLOW}Using config file: $CONFIG_FILE${NC}"

# Launch the node
echo -e "${GREEN}Launching otter_tcp_node...${NC}"
echo -e "${BLUE}Press Ctrl+C to stop${NC}"
echo ""

ros2 run ros_otter_control_pkg otter_tcp_node \
    --ros-args --params-file "$CONFIG_FILE" \
    $EXTRA_ARGS

echo -e "${YELLOW}Node stopped.${NC}"
