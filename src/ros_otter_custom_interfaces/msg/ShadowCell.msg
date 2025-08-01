# Shadow cell detected during survey leg analysis
# Represents an area with low point density that may need resurvey

# Cell center coordinates in ENU (East-North-Up) frame
float64 enu_x
float64 enu_y

# Cell center coordinates in geographic frame  
float64 latitude
float64 longitude

# Grid cell properties
float64 cell_size          # Size of the grid cell in meters
int32 point_count          # Number of points found in this cell
float64 density_ratio      # Ratio of actual density to expected mean density

# Metadata
builtin_interfaces/Time detection_time    # When this shadow was detected
string leg_id                            # Identifier for the survey leg
