const express = require("express");
const cors = require("cors");
const fs = require("fs");
const rclnodejs = require("rclnodejs");

const app = express();
const port = 3000;

app.use(cors());
app.use(express.json());

let polygonPublisher = null;
let latestPath = null;

// Initialize ROS2 node and publisher/subscriber
rclnodejs.init().then(() => {
  const node = rclnodejs.createNode('polygon_sender');
  polygonPublisher = node.createPublisher('std_msgs/msg/String', 'polygon_data');
  node.createSubscription('std_msgs/msg/String', '/path', (msg) => {
    latestPath = msg.data;
    console.log("Received path from ROS2:", latestPath);
  });
  rclnodejs.spin(node);
  console.log("ROS2 node started.");
});

// POST endpoint to receive polygon and start point
app.post("/api/polygon", (req, res) => {
  const { vertices, start } = req.body;

  // Basic validation
  if (!Array.isArray(vertices) || vertices.length < 3) {
    return res.status(400).json({ error: "Invalid vertices: must be an array of at least 3 points." });
  }
  if (!Array.isArray(start) || start.length !== 2) {
    return res.status(400).json({ error: "Invalid start position: must be [lat, lng]." });
  }
  for (const v of vertices) {
    if (!Array.isArray(v) || v.length !== 2 || !v.every(Number.isFinite)) {
      return res.status(400).json({ error: "Each vertex must be [lat, lng] as numbers." });
    }
  }

  console.log("Received vertices:", vertices);
  console.log("Received start position:", start);

  // Save to file
  const dataToSave = {
    vertices,
    start,
    timestamp: new Date().toISOString(),
  };
  fs.appendFile("polygons.txt", JSON.stringify(dataToSave) + "\n", (err) => {
    if (err) console.error("Could not save polygon:", err);
  });

  // Publish to ROS2 if publisher is ready
  if (polygonPublisher) {
    polygonPublisher.publish(JSON.stringify({ vertices, start }));
    console.log("Published polygon to ROS2 topic.");
  } else {
    console.warn("ROS2 publisher not ready, skipping publish.");
  }

  // Respond with status and latest path from ROS2 (if available)
  res.json({
    status: "ok",
    message: "Polygon data received!",
    path: latestPath || null
  });
});

// Start server
app.listen(port, () => {
  console.log(`Backend listening at http://localhost:${port}`);
});
