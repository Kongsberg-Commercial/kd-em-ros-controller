#!/bin/bash

# EM Data Analyzer Run Script
# Usage: ./run.sh [MODE] [OPTIONS]

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Script directory
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
WORKSPACE_ROOT="$(dirname "$(dirname "$SCRIPT_DIR")")"

# Configuration
CONFIG_FILE="$SCRIPT_DIR/config/analyzer_config.yaml"
RVIZ_CONFIG="$SCRIPT_DIR/config/analyzer_rviz.rviz"

# Function to print colored output
print_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Function to check if workspace is built
check_workspace() {
    if [ ! -f "$WORKSPACE_ROOT/install/setup.bash" ]; then
        print_error "Workspace not built. Please run 'colcon build' first."
        exit 1
    fi
}

# Function to source workspace
source_workspace() {
    print_info "Sourcing workspace..."
    source "$WORKSPACE_ROOT/install/setup.bash"
}

# Function to check if config file exists
check_config() {
    if [ ! -f "$CONFIG_FILE" ]; then
        print_error "Configuration file not found: $CONFIG_FILE"
        exit 1
    fi
}

# Function to show usage
show_usage() {
    echo "EM Data Analyzer - Run Script"
    echo ""
    echo "Usage: $0 [MODE] [OPTIONS]"
    echo ""
    echo "MODES:"
    echo "  analyzer              - Run only the analyzer node"
    echo "  test                  - Run analyzer with dummy GPS and leg status"
    echo "  full                  - Run analyzer + RViz + dummy data (default)"
    echo "  viz                   - Run analyzer + RViz (no dummy data)"
    echo "  launch                - Use launch file (recommended for production)"
    echo ""
    echo "OPTIONS:"
    echo "  --config FILE         - Use custom config file"
    echo "  --log-level LEVEL     - Set log level (debug, info, warn, error)"
    echo "  --help, -h            - Show this help message"
    echo ""
    echo "EXAMPLES:"
    echo "  $0                    - Run full setup with dummy data"
    echo "  $0 analyzer           - Run only analyzer node"
    echo "  $0 viz --log-level debug - Run analyzer + RViz with debug logging"
    echo "  $0 launch             - Use launch file for production"
    echo ""
}

# Function to kill background processes on exit
cleanup() {
    print_info "Cleaning up background processes..."
    jobs -p | xargs -r kill
    wait
    print_success "Cleanup complete"
}

# Set up cleanup trap
trap cleanup EXIT INT TERM

# Parse command line arguments
MODE="full"
LOG_LEVEL="info"
CUSTOM_CONFIG=""

while [[ $# -gt 0 ]]; do
    case $1 in
        analyzer|test|full|viz|launch)
            MODE="$1"
            shift
            ;;
        --config)
            CUSTOM_CONFIG="$2"
            shift 2
            ;;
        --log-level)
            LOG_LEVEL="$2"
            shift 2
            ;;
        --help|-h)
            show_usage
            exit 0
            ;;
        *)
            print_error "Unknown option: $1"
            show_usage
            exit 1
            ;;
    esac
done

# Use custom config if provided
if [ -n "$CUSTOM_CONFIG" ]; then
    CONFIG_FILE="$CUSTOM_CONFIG"
fi

# Main execution
print_info "Starting EM Data Analyzer in '$MODE' mode..."
print_info "Workspace: $WORKSPACE_ROOT"
print_info "Config: $CONFIG_FILE"
print_info "Log level: $LOG_LEVEL"

# Check prerequisites
check_workspace
source_workspace
check_config

case $MODE in
    "analyzer")
        print_info "Running analyzer node only..."
        ros2 run em_data_analyzer pointcloud_analyzer_node \
            --ros-args --params-file "$CONFIG_FILE" --log-level "$LOG_LEVEL"
        ;;
        
    "test")
        print_info "Running analyzer with dummy data..."
        
        # Start dummy GPS publisher
        print_info "Starting dummy GPS publisher..."
        ros2 run em_data_analyzer publish_dummy_gps.py &
        GPS_PID=$!
        
        # Start dummy leg status publisher
        print_info "Starting dummy leg status publisher..."
        ros2 run em_data_analyzer publish_dummy_leg_status.py &
        LEG_PID=$!
        
        # Give dummy publishers time to start
        sleep 2
        
        # Start analyzer
        print_info "Starting analyzer..."
        ros2 run em_data_analyzer pointcloud_analyzer_node \
            --ros-args --params-file "$CONFIG_FILE" --log-level "$LOG_LEVEL"
        ;;
        
    "viz")
        print_info "Running analyzer with RViz..."
        
        # Start analyzer in background
        print_info "Starting analyzer..."
        ros2 run em_data_analyzer pointcloud_analyzer_node \
            --ros-args --params-file "$CONFIG_FILE" --log-level "$LOG_LEVEL" &
        ANALYZER_PID=$!
        
        # Give analyzer time to start
        sleep 3
        
        # Start RViz
        print_info "Starting RViz..."
        if [ -f "$RVIZ_CONFIG" ]; then
            rviz2 -d "$RVIZ_CONFIG"
        else
            print_warning "RViz config not found, starting with default config"
            rviz2
        fi
        ;;
        
    "full")
        print_info "Running full setup with dummy data and RViz..."
        
        # Start dummy GPS publisher
        print_info "Starting dummy GPS publisher..."
        ros2 run em_data_analyzer publish_dummy_gps.py &
        GPS_PID=$!
        
        # Start dummy leg status publisher
        print_info "Starting dummy leg status publisher..."
        ros2 run em_data_analyzer publish_dummy_leg_status.py &
        LEG_PID=$!
        
        # Give dummy publishers time to start
        sleep 2
        
        # Start analyzer in background
        print_info "Starting analyzer..."
        ros2 run em_data_analyzer pointcloud_analyzer_node \
            --ros-args --params-file "$CONFIG_FILE" --log-level "$LOG_LEVEL" &
        ANALYZER_PID=$!
        
        # Give analyzer time to start
        sleep 3
        
        # Start RViz
        print_info "Starting RViz..."
        if [ -f "$RVIZ_CONFIG" ]; then
            rviz2 -d "$RVIZ_CONFIG"
        else
            print_warning "RViz config not found, starting with default config"
            rviz2
        fi
        ;;
        
    "launch")
        print_info "Using launch file..."
        if [ -f "$SCRIPT_DIR/launch/test_analyzer.launch.py" ]; then
            ros2 launch em_data_analyzer test_analyzer.launch.py
        else
            print_error "Launch file not found"
            exit 1
        fi
        ;;
        
    *)
        print_error "Unknown mode: $MODE"
        show_usage
        exit 1
        ;;
esac

print_success "EM Data Analyzer session completed"
