# Array of shadow cells detected during a survey leg
# Published when a leg ends and shadow analysis is complete

# Header with timestamp and frame
std_msgs/Header header

# Survey leg information
string leg_id                    # Identifier for the survey leg
int32 total_cells_analyzed       # Total number of grid cells analyzed
int32 shadow_cells_detected      # Number of shadow cells found
float64 mean_point_density       # Mean points per cell for this leg

# Analysis parameters used
float64 grid_cell_size          # Grid cell size used in meters
float64 density_threshold       # Density threshold used (fraction of mean)

# Shadow cells found
ShadowCell[] shadow_cells
