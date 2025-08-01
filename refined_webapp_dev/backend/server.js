const express = require("express");
const cors = require("cors");
const rclnodejs = require("rclnodejs");

const app = express();
const port = 3000;

app.use(cors());
app.use(express.json());

let polygonPublisher = null;
let surveyPaths = { gps: [], cartesian: [] }; // Store latest multi-leg paths

// Initialize ROS2 node and publisher for SurveyInfo message
rclnodejs.init().then(() => {
  const node = rclnodejs.createNode("polygon_sender");

  // Publisher for SurveyInfo message on 'polygon_vertices' topic
  polygonPublisher = node.createPublisher(
    "ros_otter_custom_interfaces/msg/SurveyInfo",
    "polygon_vertices"
  );

  // Subscribe to multi-leg paths from ppnode
  const gpsPathSub = node.createSubscription(
    "ros_otter_custom_interfaces/msg/Paths",
    "path_gps",
    (msg) => {
      // Process the complete path with multiple legs
      surveyPaths.gps = [];
      
      for (let i = 0; i < msg.paths.length; i++) {
        const leg = msg.paths[i];
        const gpsLeg = {
          legNumber: i + 1,
          start: {
            latitude: leg.start.latitude,
            longitude: leg.start.longitude
          },
          end: {
            latitude: leg.end.latitude,
            longitude: leg.end.longitude
          }
        };
        surveyPaths.gps.push(gpsLeg);
      }
      
      console.log(`Received GPS path with ${surveyPaths.gps.length} legs`);
      surveyPaths.gps.forEach((leg, index) => {
        console.log(`  Leg ${leg.legNumber}: [${leg.start.latitude.toFixed(6)}, ${leg.start.longitude.toFixed(6)}] -> [${leg.end.latitude.toFixed(6)}, ${leg.end.longitude.toFixed(6)}]`);
      });
    }
  );

  const cartesianPathSub = node.createSubscription(
    "ros_otter_custom_interfaces/msg/Paths",
    "path_cartesian",
    (msg) => {
      // Process the complete path with multiple legs (Cartesian coordinates)
      surveyPaths.cartesian = [];
      
      for (let i = 0; i < msg.paths.length; i++) {
        const leg = msg.paths[i];
        const cartLeg = {
          legNumber: i + 1,
          start: {
            x: leg.start.latitude,  // Using latitude field for x coordinate
            y: leg.start.longitude  // Using longitude field for y coordinate
          },
          end: {
            x: leg.end.latitude,    // Using latitude field for x coordinate
            y: leg.end.longitude    // Using longitude field for y coordinate
          }
        };
        surveyPaths.cartesian.push(cartLeg);
      }
      
      console.log(`Received Cartesian path with ${surveyPaths.cartesian.length} legs`);
    }
  );

  rclnodejs.spin(node);
  console.log("ROS2 node started, publisher and subscribers ready.");
});


// POST endpoint to receive polygon and start point
app.post("/api/polygon", (req, res) => {
  const { vertices, start } = req.body;

  // Clear previous paths when new polygon is received
  surveyPaths.gps = [];
  surveyPaths.cartesian = [];

  // Validate input
  if (!Array.isArray(vertices) || vertices.length < 3) {
    return res.status(400).json({ error: "Invalid vertices: must be an array of at least 3 points." });
  }
  if (!Array.isArray(start) || start.length !== 2) {
    return res.status(400).json({ error: "Invalid start position: must be [lat, lon]." });
  }
  for (const v of vertices) {
    if (!Array.isArray(v) || v.length !== 2 || !v.every(Number.isFinite)) {
      return res.status(400).json({ error: "Each vertex must be [lat, lon] as numbers." });
    }
  }

  // Create proper ROS message objects
  const surveyMsg = rclnodejs.createMessageObject('ros_otter_custom_interfaces/msg/SurveyInfo');
  
  // Create vertex message objects for the vertices array
  surveyMsg.vertices = vertices.map(([lat, lon]) => {
    const vertex = rclnodejs.createMessageObject('ros_otter_custom_interfaces/msg/Vertex');
    vertex.latitude = lat;
    vertex.longitude = lon;
    return vertex;
  });
  
  // Create vertex message object for the start point
  surveyMsg.start = rclnodejs.createMessageObject('ros_otter_custom_interfaces/msg/Vertex');
  surveyMsg.start.latitude = start[0];
  surveyMsg.start.longitude = start[1];

  // Publish to ROS2
  if (polygonPublisher) {
    polygonPublisher.publish(surveyMsg);
    console.log("=== PUBLISHED SurveyInfo to ROS2 topic 'polygon_vertices' ===");
    console.log(`Vertices (${surveyMsg.vertices.length}):`, 
      surveyMsg.vertices.map(v => `[${v.latitude.toFixed(6)}, ${v.longitude.toFixed(6)}]`).join(", "));
    console.log(`Start point: [${surveyMsg.start.latitude.toFixed(6)}, ${surveyMsg.start.longitude.toFixed(6)}]`);
    console.log("========================================================");
  } else {
    console.warn("ROS2 publisher not ready, skipping publish.");
  }

  // Respond to frontend
  res.json({
    status: "ok",
    message: "Polygon data received!"
  });
});

// GET endpoint to retrieve latest survey paths
app.get("/api/survey-paths", (req, res) => {
  res.json({
    gps: surveyPaths.gps,
    cartesian: surveyPaths.cartesian,
    totalLegs: surveyPaths.gps.length
  });
});

// GET endpoint to retrieve specific leg information
app.get("/api/leg/:legNumber", (req, res) => {
  const legNumber = parseInt(req.params.legNumber);
  
  if (isNaN(legNumber) || legNumber < 1) {
    return res.status(400).json({ error: "Invalid leg number. Must be a positive integer." });
  }
  
  if (legNumber > surveyPaths.gps.length) {
    return res.status(404).json({ 
      error: `Leg ${legNumber} not found. Available legs: 1-${surveyPaths.gps.length}` 
    });
  }
  
  const gpsLeg = surveyPaths.gps[legNumber - 1];  // Convert to 0-based indexing
  const cartesianLeg = surveyPaths.cartesian[legNumber - 1];
  
  res.json({
    legNumber: legNumber,
    gps: gpsLeg,
    cartesian: cartesianLeg
  });
});



// Start the server
app.listen(port, () => {
  console.log(`Backend listening at http://localhost:${port}`);
});


// npx generate-ros-messages --package ros_otter_custom_interfaces --message SurveyInfo,Vertex