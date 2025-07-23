const express = require("express");
const cors = require("cors");
const rclnodejs = require("rclnodejs");

const app = express();
const port = 3000;

app.use(cors());
app.use(express.json());

let polygonPublisher = null;
let resultPoints = { gps: [], cartesian: [] }; // Store latest result points

// Initialize ROS2 node and publisher for SurveyInfo message
rclnodejs.init().then(() => {
  const node = rclnodejs.createNode("polygon_sender");

  // Publisher for SurveyInfo message on 'polygon_vertices' topic
  polygonPublisher = node.createPublisher(
    "ros_otter_custom_interfaces/msg/SurveyInfo",
    "polygon_vertices"
  );

  // Subscribe to result points from ppnode
  const gpsResultSub = node.createSubscription(
    "ros_otter_custom_interfaces/msg/GpsInfo",
    "result_points_gps",
    (msg) => {
      // Reconstruct signed coordinates from absolute values + directions
      const latitude = msg.lat * (msg.lat_dir === "N" ? 1 : -1);
      const longitude = msg.lon * (msg.lon_dir === "E" ? 1 : -1);
      
      const gpsPoint = {
        latitude: latitude,
        longitude: longitude,
        timestamp: msg.time
      };
      resultPoints.gps.push(gpsPoint);
      console.log(`Received GPS result point: ${gpsPoint.latitude}, ${gpsPoint.longitude}`);
    }
  );

  const cartesianResultSub = node.createSubscription(
    "geometry_msgs/msg/Point",
    "result_points_cartesian",
    (msg) => {
      const cartPoint = {
        x: msg.x,
        y: msg.y,
        z: msg.z
      };
      resultPoints.cartesian.push(cartPoint);
      console.log(`Received Cartesian result point: ${cartPoint.x}, ${cartPoint.y}`);
    }
  );

  rclnodejs.spin(node);
  console.log("ROS2 node started, publisher and subscribers ready.");
});


// POST endpoint to receive polygon and start point
app.post("/api/polygon", (req, res) => {
  const { vertices, start } = req.body;

  // Clear previous result points when new polygon is received
  resultPoints.gps = [];
  resultPoints.cartesian = [];

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

// GET endpoint to retrieve latest result points
app.get("/api/result-points", (req, res) => {
  res.json({
    gps: resultPoints.gps,
    cartesian: resultPoints.cartesian
  });
});



// Start the server
app.listen(port, () => {
  console.log(`Backend listening at http://localhost:${port}`);
});


// npx generate-ros-messages --package ros_otter_custom_interfaces --message SurveyInfo,Vertex