# ROS2 K-Controller Wrapper Service Shortcuts

# --- SounderCommand-based services ---
ros2_start_sounder() {
  ros2 service call /kctrl/start_sounder ros2_kctrl_custom_interfaces/srv/SounderCommand "{sounder_name: '$1'}"
}
ros2_start_pinging() {
  ros2 service call /kctrl/start_pinging ros2_kctrl_custom_interfaces/srv/SounderCommand "{sounder_name: '$1'}"
}
ros2_stop_pinging() {
  ros2 service call /kctrl/stop_pinging ros_kctrl_custom_interfaces/srv/SounderCommand "{sounder_name: '$1'}"
}
ros2_start_water_column() {
  ros2 service call /kctrl/start_water_column ros2_kctrl_custom_interfaces/srv/SounderCommand "{sounder_name: '$1'}"
}
ros2_stop_water_column() {
  ros2 service call /kctrl/stop_water_column ros2_kctrl_custom_interfaces/srv/SounderCommand "{sounder_name: '$1'}"
}
ros2_start_stave() {
  ros2 service call /kctrl/start_stave ros2_kctrl_custom_interfaces/srv/SounderCommand "{sounder_name: '$1'}"
}
ros2_stop_stave() {
  ros2 service call /kctrl/stop_stave ros2_kctrl_custom_interfaces/srv/SounderCommand "{sounder_name: '$1'}"
}
ros2_shutdown() {
  ros2 service call /kctrl/shutdown ros2_kctrl_custom_interfaces/srv/SounderCommand "{}"
}
ros2_request_install_runtime_parameters() {
  ros2 service call /kctrl/request_install_runtime_parameters ros2_kctrl_custom_interfaces/srv/SounderCommand "{sounder_name: '$1'}"
}
ros2_request_multicast_address() {
  ros2 service call /kctrl/request_multicast_address ros2_kctrl_custom_interfaces/srv/SounderCommand "{sounder_name: '$1'}"
}
ros2_disconnect_sounder() {
  ros2 service call /kctrl/disconnect_sounder ros2_kctrl_custom_interfaces/srv/SounderCommand "{sounder_name: '$1'}"
}

# --- PU Parameters ---
ros2_request_pu_parameters() {
  local sounder="$1"; shift
  # Remaining args are parameter names
  local params="["
  for param in "$@"; do params="${params}'$param', "; done
  params="${params%, }]"
  ros2 service call /kctrl/request_pu_parameters ros2_kctrl_custom_interfaces/srv/RequestPUParameters "{sounder_name: '$sounder', param_names: $params}"
}
ros2_set_pu_parameters() {
  local sounder="$1"; shift
  local names="["
  local values="["
  while [[ $# -gt 1 ]]; do
    names="${names}'$1', "
    values="${values}'$2', "
    shift 2
  done
  names="${names%, }]"
  values="${values%, }]"
  ros2 service call /kctrl/set_pu_parameters ros2_kctrl_custom_interfaces/srv/SetPUParameters "{sounder_name: '$sounder', param_names: $names, param_values: $values}"
}

# --- Recording Control ---
ros2_recording_control() {
  ros2 service call /kctrl/recording_control ros2_kctrl_custom_interfaces/srv/RecordingControl "{sounder_name: '$1', turn_on: $2}"
}

# --- Update Recording Path ---
ros2_update_recording_path() {
  ros2 service call /kctrl/update_recording_path ros2_kctrl_custom_interfaces/srv/UpdateRecordingPath "{sounder_name: '$1', folder_path: '$2'}"
}

# --- Export/Import PU Parameters ---
ros2_export_pu_parameters() {
  ros2 service call /kctrl/export_pu_parameters ros2_kctrl_custom_interfaces/srv/PUParameters "{sounder_name: '$1', file_name: '$2'}"
}
ros2_import_pu_parameters() {
  ros2 service call /kctrl/import_pu_parameters ros2_kctrl_custom_interfaces/srv/PUParameters "{sounder_name: '$1', file_name: '$2'}"
}

# --- Request Detected Sounders ---
ros2_request_detected_sounders() {
  ros2 service call /kctrl/request_detected_sounders ros2_kctrl_custom_interfaces/srv/RequestDetectedSounders "{}"
}

alias kctrl_version="ros2 topic echo /kctrl/version"

alias kctrl_detected_devices="ros2 topic echo --field data /kctrl/detected_devices | python3 -c 'import sys; [print(line.encode().decode(\"unicode_escape\")) for line in sys.stdin]'"

alias kctrl_parameters="ros2 topic echo --field data /kctrl/parameters | python3 -c 'import sys; [print(line.encode().decode(\"unicode_escape\")) for line in sys.stdin]'"

alias kctrl_multicast="ros2 topic echo /kctrl/multicast"

alias kctrl_info_warn_error="ros2 topic echo --field data /kctrl/info_warn_error | python3 -c 'import sys; [print(line.encode().decode(\"unicode_escape\")) for line in sys.stdin]'"

alias kctrl_status="ros2 topic echo --field data /kctrl/status | python3 -c 'import sys; [print(line.encode().decode(\"unicode_escape\")) for line in sys.stdin]'"

alias kctrl_pu_params="ros2 topic echo --full-length /kctrl/pu_params | python3 -c 'import sys; [print(line.encode().decode(\"unicode_escape\")) for line in sys.stdin]'"

alias kctrl_device_disconnected="ros2 topic echo --field data /kctrl/device_disconnected | python3 -c 'import sys; [print(line.encode().decode(\"unicode_escape\")) for line in sys.stdin]'"
