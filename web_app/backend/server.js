const express = require("express");
const cors = require("cors");
const fs = require("fs");
const rclnodejs = require("rclnodejs");
const WebSocket = require('ws');

const app = express();
const port = 3000;

app.use(cors());
app.use(express.json());

let polygonPublisher = null;
let latestPath = null;
let debugDataPublisher = null;

// WebSocket server for real-time updates
const wss = new WebSocket.Server({ port: 8080 });
let connectedClients = [];

wss.on('connection', (ws) => {
  console.log('New WebSocket client connected');
  connectedClients.push(ws);
  
  ws.on('close', () => {
    console.log('WebSocket client disconnected');
    connectedClients = connectedClients.filter(client => client !== ws);
  });
});

// Broadcast to all connected clients
function broadcastToClients(data) {
  connectedClients.forEach(client => {
    if (client.readyState === WebSocket.OPEN) {
      client.send(JSON.stringify(data));
    }
  });
}

// Initialize ROS2 node and publisher/subscriber
rclnodejs.init().then(() => {
  const node = rclnodejs.createNode('polygon_sender');
  polygonPublisher = node.createPublisher('std_msgs/msg/String', 'polygon_data');
  
  // Subscribe to path updates
  node.createSubscription('std_msgs/msg/String', '/path', (msg) => {
    latestPath = msg.data;
    console.log("Received path from ROS2:", latestPath);
    
    // Broadcast path to WebSocket clients
    broadcastToClients({
      type: 'path_update',
      data: latestPath
    });
  });
  
  // Subscribe to debug data
  node.createSubscription('std_msgs/msg/String', '/debug_data', (msg) => {
    try {
      const debugData = JSON.parse(msg.data);
      console.log("Received debug data from ROS2:", debugData);
      
      // Broadcast debug data to WebSocket clients
      broadcastToClients({
        type: 'debug_data',
        data: debugData
      });
    } catch (e) {
      console.error("Failed to parse debug data:", e);
    }
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
