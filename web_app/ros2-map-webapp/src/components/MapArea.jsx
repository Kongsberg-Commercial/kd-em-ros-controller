import React, { useState, useEffect } from "react";
import { MapContainer, TileLayer, Polygon, Marker, Polyline, useMapEvents } from "react-leaflet";
import "leaflet/dist/leaflet.css";
import { redDotIcon, greenMarkerIcon, yellowMarkerIcon, purpleMarkerIcon } from "../icons/LeafletIcons";
import {postPolygon} from "../api/api";

const DEFAULT_CENTER = [59.4303437, 10.4726724]; // Horten

function ClickHandler({ addVertex, setStart, enableVertexPlacement }) {
  useMapEvents({
    click(e) {
      if (enableVertexPlacement) {
        addVertex([e.latlng.lat, e.latlng.lng]);
      }
    },
    contextmenu(e) {
      setStart([e.latlng.lat, e.latlng.lng]);
    },
  });
  return null;
}

const MapArea = () => {
  const [vertices, setVertices] = useState([]);
  const [start, setStart] = useState(null);
  const [enableVertexPlacement, setEnableVertexPlacement] = useState(true);
  const [debugData, setDebugData] = useState(null);
  const [pathData, setPathData] = useState([]);
  
  // WebSocket connection for real-time updates
  useEffect(() => {
    const ws = new WebSocket('ws://localhost:8080');
    
    ws.onopen = () => {
      console.log('WebSocket connected');
    };
    
    ws.onmessage = (event) => {
      try {
        const message = JSON.parse(event.data);
        console.log('WebSocket message received:', message);
        
        if (message.type === 'debug_data') {
          setDebugData(message.data);
        } else if (message.type === 'path_update') {
          setPathData(prev => [...prev, message.data]);
        }
      } catch (e) {
        console.error('Failed to parse WebSocket message:', e);
      }
    };
    
    ws.onclose = () => {
      console.log('WebSocket disconnected');
    };
    
    return () => {
      ws.close();
    };
  }, []);

  // Handler for dragging vertices
  const moveVertex = (index, newLatLng) => {
    const newVertices = [...vertices];
    newVertices[index] = [newLatLng.lat, newLatLng.lng];
    setVertices(newVertices);
  };

  // Undo last vertex
  const undoVertex = () => {
    setVertices(vertices.slice(0, -1));
  };

  // Erase all vertices
  const eraseAllVertices = () => {
    setVertices([]);
  };
  
  // Clear debug data
  const clearDebugData = () => {
    setDebugData(null);
    setPathData([]);
  };
  
  // Calculate ray end point for visualization
  const calculateRayEndPoint = (start, bearing, distance = 0.01) => { // Doubled the distance
    const R = 6371000; // Earth's radius in meters
    const lat1 = start[0] * Math.PI / 180;
    const lon1 = start[1] * Math.PI / 180;
    const bearingRad = bearing * Math.PI / 180;
    
    const lat2 = Math.asin(Math.sin(lat1) * Math.cos(distance / R) +
                          Math.cos(lat1) * Math.sin(distance / R) * Math.cos(bearingRad));
    const lon2 = lon1 + Math.atan2(Math.sin(bearingRad) * Math.sin(distance / R) * Math.cos(lat1),
                                  Math.cos(distance / R) - Math.sin(lat1) * Math.sin(lat2));
    
    return [lat2 * 180 / Math.PI, lon2 * 180 / Math.PI];
  };

  return (
    <div style={{ display: "flex", flexDirection: "column", alignItems: "center" }}>
      <div style={{ marginBottom: "10px" }}>
        <button
          onClick={() => setEnableVertexPlacement(!enableVertexPlacement)}
          style={{
            backgroundColor: enableVertexPlacement ? "#e74c3c" : "#2ecc40",
            color: "#fff",
            marginRight: "10px",
            padding: "8px 12px",
            borderRadius: "4px",
            border: "none",
            fontWeight: "bold"
          }}
        >
          {enableVertexPlacement ? "Vertex Placement: ON" : "Vertex Placement: OFF"}
        </button>
        <button
          onClick={undoVertex}
          disabled={vertices.length === 0}
          style={{
            marginRight: "10px",
            padding: "8px 12px",
            borderRadius: "4px",
            border: "none",
            fontWeight: "bold"
          }}
        >
          Undo
        </button>
        <button
          onClick={eraseAllVertices}
          disabled={vertices.length === 0}
          style={{
            marginRight: "10px",
            padding: "8px 12px",
            borderRadius: "4px",
            border: "none",
            fontWeight: "bold"
          }}
        >
          Erase All
        </button>
        <button
          onClick={clearDebugData}
          style={{
            padding: "8px 12px",
            borderRadius: "4px",
            border: "none",
            fontWeight: "bold",
            backgroundColor: "#f39c12",
            color: "white"
          }}
        >
          Clear Debug
        </button>
      </div>
      <MapContainer
        center={DEFAULT_CENTER}
        zoom={13}
        scrollWheelZoom
        style={{ height: "725px", width: "1400px" }}
      >
        <TileLayer
          attribution="&copy; OpenStreetMap contributors"
          url="https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png"
        />
        <ClickHandler
          addVertex={(latlng) => setVertices([...vertices, latlng])}
          setStart={setStart}
          enableVertexPlacement={enableVertexPlacement}
        />
        {vertices.length > 2 && <Polygon positions={vertices} color="blue" />}
        {vertices.map((vertex, idx) => (
          <Marker
            key={idx}
            position={vertex}
            icon={redDotIcon}
            draggable={!enableVertexPlacement}
            eventHandlers={{
              dragend: (e) => {
                const latlng = e.target.getLatLng();
                moveVertex(idx, latlng);
              },
            }}
          />
        ))}
        {start && (
          <Marker
            position={start}
            icon={greenMarkerIcon}
          />
        )}
        
        {/* Debug visualization */}
        {debugData && (
          <>
            {/* Debug start point (yellow marker) */}
            <Marker
              position={debugData.start_point}
              icon={yellowMarkerIcon}
            />
            
            {/* Debug end point (purple marker) if intersection found */}
            {(debugData.intersection_found === "true" || debugData.intersection_found === true) && (
              <Marker
                position={debugData.end_point}
                icon={purpleMarkerIcon}
              />
            )}
            
            {/* Ray line (cyan solid line) showing survey bearing */}
            {debugData.survey_bearing !== undefined && (
              <Polyline
                positions={[
                  debugData.start_point,
                  calculateRayEndPoint(debugData.start_point, debugData.survey_bearing)
                ]}
                color="cyan"
                weight={3}
                opacity={0.9}
              />
            )}
            
            {/* Survey leg line (red dashed line) */}
            {(debugData.intersection_found === "true" || debugData.intersection_found === true) && (
              <Polyline
                positions={[debugData.start_point, debugData.end_point]}
                color="red"
                weight={3}
                opacity={0.8}
                dashArray="8, 8"
              />
            )}
          </>
        )}
      </MapContainer>
      <button
        onClick={() => {
        console.log("Vertices:", vertices);
        console.log("Start position:", start);
        postPolygon({vertices, start}).then(
          response => console.log("Polygon sent successfully:", response),
          error => console.error("Error sending polygon:", error)
        );
        }}
        style={{ marginTop: "10px", padding: "8px 12px", borderRadius: "4px", border: "none", fontWeight: "bold" }}
      >
        Send Area to Backend
      </button>
      <div style={{ fontSize: "0.95em", marginTop: "6px", color: "#aaa", textAlign: "center", lineHeight: "1.3" }}>
        Left click to add vertex (when ON) &nbsp;|&nbsp; Right click to set start position &nbsp;|&nbsp; Undo/Erase buttons &nbsp;|&nbsp; Drag vertices when placement is OFF
        <br />
        <span style={{ fontSize: "0.85em", color: "#888" }}>
          🟢 Start Position &nbsp;|&nbsp; 🔴 Vertices &nbsp;|&nbsp; 🟡 Leg Start &nbsp;|&nbsp; 🟣 Leg End &nbsp;|&nbsp; 🔵 Ray Direction
        </span>
      </div>
      
      {/* Debug info display */}
      {debugData && (
        <div style={{ 
          marginTop: "10px", 
          padding: "10px", 
          backgroundColor: "#333", 
          borderRadius: "4px", 
          fontSize: "0.9em",
          color: "#fff",
          textAlign: "left",
          maxWidth: "600px"
        }}>
          <strong>Debug Info (Leg {debugData.leg_number}):</strong><br/>
          Start: [{debugData.start_point[0].toFixed(6)}, {debugData.start_point[1].toFixed(6)}]<br/>
          End: [{debugData.end_point[0].toFixed(6)}, {debugData.end_point[1].toFixed(6)}]<br/>
          Survey Bearing: {debugData.survey_bearing.toFixed(2)}°<br/>
          Leg Distance: {debugData.leg_distance.toFixed(2)}m<br/>
          Intersection Found: {String(debugData.intersection_found)} (type: {typeof debugData.intersection_found})
        </div>
      )}
    </div>
  );
}

export default MapArea;