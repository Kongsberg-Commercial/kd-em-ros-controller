# ROS2 K-Controller Bash Commands & Usage Examples

## Sounder control (SounderCommand-based services)

| Command                                   | Example usage                                       |
|--------------------------------------------|-----------------------------------------------------|
| `ros2_start_sounder SOUNDER`               | `ros2_start_sounder MySounder`                      |
| `ros2_start_pinging SOUNDER`               | `ros2_start_pinging MySounder`                      |
| `ros2_stop_pinging SOUNDER`                | `ros2_stop_pinging MySounder`                       |
| `ros2_start_water_column SOUNDER`          | `ros2_start_water_column MySounder`                 |
| `ros2_stop_water_column SOUNDER`           | `ros2_stop_water_column MySounder`                  |
| `ros2_start_stave SOUNDER`                 | `ros2_start_stave MySounder`                        |
| `ros2_stop_stave SOUNDER`                  | `ros2_stop_stave MySounder`                         |
| `ros2_shutdown`                            | `ros2_shutdown`                                     |
| `ros2_request_install_runtime_parameters SOUNDER` | `ros2_request_install_runtime_parameters MySounder` |
| `ros2_request_multicast_address SOUNDER`   | `ros2_request_multicast_address MySounder`          |
| `ros2_disconnect_sounder SOUNDER`          | `ros2_disconnect_sounder MySounder`                 |

---

## PU Parameter Services

| Command                                   | Example usage                                                  |
|--------------------------------------------|----------------------------------------------------------------|
| `ros2_request_pu_parameters SOUNDER PARAM1 [PARAM2 ...]` | `ros2_request_pu_parameters MySounder GAIN FREQ`              |
| `ros2_set_pu_parameters SOUNDER NAME1 VALUE1 [NAME2 VALUE2 ...]` | `ros2_set_pu_parameters MySounder GAIN 5 FREQ 300`            |

---

## Recording and Path Control

| Command                                   | Example usage                                      |
|--------------------------------------------|----------------------------------------------------|
| `ros2_recording_control SOUNDER true|false` | `ros2_recording_control MySounder true`           |
| `ros2_update_recording_path SOUNDER PATH`  | `ros2_update_recording_path MySounder /tmp/rec`    |

---

## PU Parameter Import/Export

| Command                                   | Example usage                                              |
|--------------------------------------------|------------------------------------------------------------|
| `ros2_export_pu_parameters SOUNDER FILE`   | `ros2_export_pu_parameters MySounder /tmp/params.xml`      |
| `ros2_import_pu_parameters SOUNDER FILE`   | `ros2_import_pu_parameters MySounder /tmp/params.xml`      |

---

## Detected Sounders

| Command                                   | Example usage                      |
|--------------------------------------------|------------------------------------|
| `ros2_request_detected_sounders`           | `ros2_request_detected_sounders`   |

---

# Notes

- Replace `MySounder` with your actual sounder name.
- For boolean values (`true`/`false`), use lowercase.
- For parameter lists, see the example formats above.
- All commands must be run in a terminal with your ROS2 workspace sourced (e.g., `source ~/ros2_ws/install/setup.bash`).
