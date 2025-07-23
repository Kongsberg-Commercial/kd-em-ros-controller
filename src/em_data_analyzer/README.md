# EM Data Analyzer

A ROS2 package for analyzing multibeam sonar pointcloud data, providing real-time swath statistics and shadow/gap detection for survey quality assessment.

## Features

- **Real-time Swath Analysis**: Computes swath width and depth statistics for each sonar ping
- **GPS Synchronization**: Matches pointcloud data with GPS positions using timestamp correlation
- **Leg-based Processing**: Collects statistics only during active survey legs
- **Shadow Detection**: Identifies areas with low point density that may need resurvey
- **Configurable Parameters**: All settings managed through YAML configuration files

## Architecture

### Class Structure
- `PointCloudAnalyzer`: Main analyzer class handling data processing
- `GeoPoint`: Structure for storing point data with both ENU and geographic coordinates
- `AnalyzerConfig`: Configuration structure with all tunable parameters

### Key Components
1. **GPS Buffer Management**: Maintains a buffer of recent GPS messages for timestamp matching
2. **Coordinate Transformations**: Uses GeographicLib for ENU ↔ Lat/Lon conversions
3. **Grid-based Shadow Detection**: Creates spatial grids to identify low-density areas
4. **Statistics Computation**: Calculates min/max/mean values for swath and depth data

## Configuration

All parameters are configurable via `config/analyzer_config.yaml`:

```yaml
pointcloud_analyzer:    topics:
      pointcloud: "/PointcloudFromKMALL"     # Input pointcloud topic
      gps: "gps_info"                       # GPS data topic
      leg_status: "/leg_status"             # Survey leg status topic
      stats_output: "/swath_line_stats"     # Output statistics topic
      shadow_output: "/shadow_analysis"     # Output shadow analysis topic
  
  shadow_detection:
    grid_cell_size: 10.0                  # Grid resolution in meters
    density_threshold: 0.1                # Shadow threshold (10% of mean density)
  
  gps:
    buffer_size: 100                      # Number of GPS messages to buffer
```

## Usage

### Basic Launch
```bash
ros2 launch em_data_analyzer analyzer.launch.py
```

### With Custom Configuration
```bash
ros2 launch em_data_analyzer analyzer.launch.py config_file:=/path/to/custom_config.yaml
```

### Direct Node Execution
```bash
ros2 run em_data_analyzer pointcloud_analyzer_node --ros-args --params-file /path/to/config.yaml
```

### Subscribe to Shadow Analysis
```bash
# Monitor shadow analysis in terminal
ros2 topic echo /shadow_analysis

# Run the example shadow subscriber
ros2 run em_data_analyzer shadow_subscriber_example.py
```

## Topics

### Subscribed Topics
- `/PointcloudFromKMALL` (sensor_msgs/PointCloud2): Multibeam pointcloud data
- `gps_info` (ros_otter_custom_interfaces/GpsInfo): GPS position data
- `/leg_status` (std_msgs/Bool): Survey leg active/inactive status

### Published Topics
- `/swath_line_stats` (ros_otter_custom_interfaces/SwathLineStats): Computed swath statistics
- `/shadow_analysis` (ros_otter_custom_interfaces/ShadowAnalysis): Detected shadow/gap areas

## Output

### Swath Statistics
For each completed survey leg:
- Minimum, maximum, and average swath width
- Minimum, maximum, and average depth
- Published as `SwathLineStats` message

### Shadow Detection
Published as `ShadowAnalysis` message containing:
- **Individual shadow cells**: ENU and geographic coordinates for each detected gap
- **Analysis metadata**: Grid parameters, detection thresholds, total cells analyzed
- **Leg information**: Unique leg ID and timestamp for tracking
- **Summary statistics**: Total shadows detected and mean point density

Each shadow cell includes:
- ENU coordinates (x, y) in meters
- Geographic coordinates (lat, lon) in degrees
- Cell size and detection parameters
- Unique leg identifier for tracking

## Dependencies

- ROS2 (Humble or later)
- GeographicLib
- sensor_msgs
- geometry_msgs
- ros_otter_custom_interfaces

## Building

```bash
cd /path/to/ros2_ws
colcon build --packages-select em_data_analyzer
source install/setup.bash
```

## File Structure

```
em_data_analyzer/
├── CMakeLists.txt
├── package.xml
├── README.md
├── config/
│   └── analyzer_config.yaml
├── include/em_data_analyzer/
│   └── pointcloud_analyzer.hpp
├── launch/
│   └── analyzer.launch.py
└── src/
    ├── pointcloud_analyzer.cpp
    ├── pointcloud_analyzer_main.cpp
    ├── dummy_pointcloud_publisher.py
    └── accumulate_swaths_node.py
```
