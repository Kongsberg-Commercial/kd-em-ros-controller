# EM Data Analyzer

A ROS2 package for analyzing multibeam sonar pointcloud data, providing real-time swath statistics, shadow/gap detection, and RViz visualization for survey quality assessment.

## Features

- **Real-time Swath Analysis**: Computes swath width and depth statistics for each sonar ping
- **GPS Synchronization**: Matches pointcloud data with GPS positions using timestamp correlation
- **Leg-based Processing**: Collects statistics only during active survey legs
- **Shadow Detection**: Identifies areas with low point density that may need resurvey with adaptive edge exclusion
- **RViz Visualization**: Real-time display of accumulated pointcloud, GPS path, and detected shadows
- **Configurable Parameters**: All settings managed through YAML configuration files

## Architecture

### Class Structure
- `PointCloudAnalyzer`: Main analyzer class handling data processing and visualization
- `GeoPoint`: Structure for storing point data with both ENU and geographic coordinates
- `AnalyzerConfig`: Configuration structure with all tunable parameters

### Key Components
1. **GPS Buffer Management**: Maintains a buffer of recent GPS messages for timestamp matching
2. **Coordinate Transformations**: Uses GeographicLib for ENU ↔ Lat/Lon conversions
3. **Grid-based Shadow Detection**: Creates spatial grids to identify low-density areas with edge exclusion
4. **Statistics Computation**: Calculates min/max/mean values for swath and depth data
5. **Visualization**: Publishes accumulated pointcloud, GPS path, and shadow markers for RViz

## Configuration

All parameters are configurable via `config/analyzer_config.yaml`:

```yaml
pointcloud_analyzer:
  ros__parameters:
    topics:
      pointcloud: "/PointcloudFromKMALL"     # Input pointcloud topic
      gps: "gps_info"                       # GPS data topic
      leg_status: "/leg_status"             # Survey leg status topic
      stats_output: "/swath_line_stats"     # Output statistics topic
      shadow_output: "/shadow_analysis"     # Shadow analysis output
      viz_pointcloud: "/viz/accumulated_pointcloud"  # Visualization pointcloud
      viz_gps_path: "/viz/gps_path"         # GPS path markers
      viz_shadows: "/viz/shadows"           # Shadow markers
    
    shadow_detection:
      grid_cell_size: 5.0                   # Grid resolution in meters
      density_threshold: 0.3                # Shadow threshold (30% of mean density)
      edge_exclusion_factor: 0.05           # Exclude 5% of swath width from edges
      along_track_exclusion: 10.0           # Fixed exclusion along track (meters)
    
    visualization:
      max_accumulated_points: 100000        # Maximum points to keep in memory
      point_decimation_distance: 1.0        # Minimum distance between points
    
    gps:
      buffer_size: 100                      # Number of GPS messages to buffer
```

## Usage

### Basic Execution
```bash
# Start the analyzer with configuration file
ros2 run em_data_analyzer pointcloud_analyzer_node --ros-args --params-file src/em_data_analyzer/config/analyzer_config.yaml
```

### With RViz Visualization
```bash
# Terminal 1: Start the analyzer
ros2 run em_data_analyzer pointcloud_analyzer_node --ros-args --params-file src/em_data_analyzer/config/analyzer_config.yaml

# Terminal 2: Start RViz
rviz2

# Terminal 3: Play your recorded multibeam data
ros2 bag play your_multibeam_data.bag

# Terminal 4: If needed, provide leg status signals
ros2 topic pub /leg_status std_msgs/Bool "data: true"  # Start leg
ros2 topic pub /leg_status std_msgs/Bool "data: false" # End leg
```

### RViz Setup
Add the following displays in RViz:
- **PointCloud2**: Subscribe to `/viz/accumulated_pointcloud`
- **MarkerArray**: Subscribe to `/viz/gps_path` (GPS path and vessel position)
- **MarkerArray**: Subscribe to `/viz/shadows` (detected shadow areas)
- Set Fixed Frame to `map`

### Monitor Output
```bash
# Monitor swath statistics
ros2 topic echo /swath_line_stats

# Monitor shadow analysis
ros2 topic echo /shadow_analysis

```

## Topics

### Subscribed Topics
- `/PointcloudFromKMALL` (sensor_msgs/PointCloud2): Multibeam pointcloud data
- `gps_info` (ros_otter_custom_interfaces/GpsInfo): GPS position data
- `/leg_status` (std_msgs/Bool): Survey leg active/inactive status

### Published Topics
- `/swath_line_stats` (ros_otter_custom_interfaces/SwathLineStats): Computed swath statistics
- `/shadow_analysis` (ros_otter_custom_interfaces/ShadowAnalysis): Detected shadow/gap areas
- `/viz/accumulated_pointcloud` (sensor_msgs/PointCloud2): Accumulated and transformed pointcloud
- `/viz/gps_path` (visualization_msgs/MarkerArray): GPS path and vessel position
- `/viz/shadows` (visualization_msgs/MarkerArray): Detected shadow areas as markers

## Output Data

### Swath Statistics
Published for each completed survey leg:
- Minimum, maximum, and average swath width (meters)
- Minimum, maximum, and average depth (meters)
- Leg identifier and timestamp

### Shadow Detection
Published as `ShadowAnalysis` message containing:
- **Individual shadow cells**: ENU and geographic coordinates for each detected gap
- **Analysis metadata**: Grid parameters, detection thresholds, analyzed vs excluded cells
- **Leg information**: Unique leg ID and processing timestamp
- **Edge exclusion statistics**: Automatically excludes swath edges based on mean swath width

Each shadow cell includes:
- ENU coordinates (x, y) in meters relative to leg start position
- Geographic coordinates (lat, lon) in decimal degrees
- Cell size and detection parameters used
- Unique leg identifier for data association

### RViz Visualization
Real-time visualization components:
- **Accumulated Pointcloud**: All processed pointcloud data transformed to global coordinates, color-coded by depth
- **GPS Path**: Green line showing vessel track with red arrow indicating current position
- **Shadow Areas**: Semi-transparent magenta cubes showing detected low-density areas
- **Configurable Display**: All visualization parameters adjustable via RViz interface

## Shadow Detection Algorithm

The shadow detection uses an intelligent approach to minimize false positives:

1. **Adaptive Grid Creation**: Divides survey area into configurable grid cells
2. **Swath-Aware Edge Exclusion**: Excludes cells near swath edges based on calculated mean swath width
3. **Density Analysis**: Compares cell point density to mean density of analyzed (non-excluded) area
4. **Configurable Threshold Detection**: Flags cells below configurable density threshold as potential shadows

This approach effectively distinguishes between natural swath boundaries and actual data gaps requiring attention.

## Dependencies

- **ROS2** (Humble or later)
- **GeographicLib** (for coordinate transformations)
- **sensor_msgs** (pointcloud handling)
- **geometry_msgs** (spatial data types)
- **visualization_msgs** (RViz markers)
- **ros_otter_custom_interfaces** (custom message definitions)

## Building

```bash
cd /home/arveds/ros2_ws
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
└── src/
    ├── pointcloud_analyzer.cpp
    ├── pointcloud_analyzer_main.cpp
    └── publish_dummy_gps.py    # Optional GPS simulator for testing
```

## Testing

For testing a dummy GPS publisher is included:

```bash
# Optional: Start dummy GPS publisher for testing
ros2 run em_data_analyzer publish_dummy_gps.py
```

This publishes simulated GPS data moving east at 1 knot from a starting position, useful for testing the analyzer's GPS synchronization and coordinate transformation functionality.
