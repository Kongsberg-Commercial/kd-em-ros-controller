# $PMAROTR,<rpm_port>,<rpm_stb>,<temp_port>,<temp_stb>,<power_port>,<power_stb>,<power_total>,<batteries_count>*<checksum>
float32 rpm_port             # RPM port (always positive)
float32 rpm_stb              # RPM starboard
float32 temp_port            # Temperature port (Celsius)
float32 temp_stb             # Temperature starboard
float32 power_port           # Power port (Watts)
float32 power_stb            # Power starboard (Watts)
float32 power_total          # Total power consumption (Watts)
uint8 batteries_count        # Number of batteries connected