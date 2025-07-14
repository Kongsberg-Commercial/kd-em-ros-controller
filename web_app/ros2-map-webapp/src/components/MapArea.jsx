import React, { useState } from "react";
import { MapContainer, TileLayer, Polygon, Marker, useMapEvents } from "react-leaflet";
import L from "leaflet";
import "leaflet/dist/leaflet.css";

// Small red dot icon for vertices
const redDotIcon = new L.Icon({
  iconUrl: "https://raw.githubusercontent.com/pointhi/leaflet-color-markers/master/img/marker-icon-red.png",
  iconSize: [16, 16],
  iconAnchor: [8, 8],
  popupAnchor: [0, -8],
  shadowUrl: null,
  shadowSize: null,
  shadowAnchor: null,
});

// Green marker for start position
const greenMarkerIcon = new L.Icon({
  iconUrl: "https://raw.githubusercontent.com/pointhi/leaflet-color-markers/master/img/marker-icon-green.png",
  iconSize: [22, 22],
  iconAnchor: [11, 22],
  popupAnchor: [0, -22],
  shadowUrl: null,
  shadowSize: null,
  shadowAnchor: null,
});

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

export default function MapArea() {
  const [vertices, setVertices] = useState([]);
  const [start, setStart] = useState(null);
  const [enableVertexPlacement, setEnableVertexPlacement] = useState(true);

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
            padding: "8px 12px",
            borderRadius: "4px",
            border: "none",
            fontWeight: "bold"
          }}
        >
          Erase All
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
      </MapContainer>
      <button
        onClick={() => {
        console.log("Vertices:", vertices);
        console.log("Start position:", start);
          fetch("http://localhost:3000/api/polygon", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({ vertices, start }),
          });
        }}
        style={{ marginTop: "10px", padding: "8px 12px", borderRadius: "4px", border: "none", fontWeight: "bold" }}
      >
        Send Area to Backend
      </button>
      <div style={{ fontSize: "0.95em", marginTop: "6px", color: "#aaa", textAlign: "center", lineHeight: "1.3" }}>
        Left click to add vertex (when ON) &nbsp;|&nbsp; Right click to set start position &nbsp;|&nbsp; Undo/Erase buttons &nbsp;|&nbsp; Drag vertices when placement is OFF
      </div>
    </div>
  );
}