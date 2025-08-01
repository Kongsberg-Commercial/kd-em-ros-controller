# $PMARGPS,<time>,<lat>,<lat_dir>,<lon>,<lon_dir>,<alt>,<sog>,<cog>*<checksum>
float64 time                 # Timestamp (usually seconds)
float64 lat                  # Latitude (decimal degrees)
string lat_dir               # Latitude direction ('N' or 'S')
float64 lon                  # Longitude (decimal degrees)
string lon_dir               # Longitude direction ('E' or 'W')
float64 alt                  # Altitude (meters)
float64 sog                  # Speed over ground (knots or m/s as per your source)
float64 cog                  # Course over ground (degrees)