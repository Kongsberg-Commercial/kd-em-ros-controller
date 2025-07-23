import React, { useState, useRef, useMemo, useEffect, useCallback } from "react";
import { MapContainer, TileLayer, Polygon, Marker, Polyline, useMapEvents } from "react-leaflet";
import L from "leaflet";
import "leaflet/dist/leaflet.css";
import { verticesLatLonToCartesian } from "./geoUtils";

// --- ICONS (as before; insert your icon definitions here) ---
const redDotIcon = new L.Icon({
  iconUrl: "https://raw.githubusercontent.com/pointhi/leaflet-color-markers/master/img/marker-icon-red.png",
  iconSize: [16, 16],
  iconAnchor: [8, 8],
  popupAnchor: [0, -8],
  shadowUrl: null,
  shadowSize: null,
  shadowAnchor: null,
});
const greenMarkerIcon = new L.Icon({
  iconUrl: "https://raw.githubusercontent.com/pointhi/leaflet-color-markers/master/img/marker-icon-green.png",
  iconSize: [22, 22],
  iconAnchor: [11, 22],
  popupAnchor: [0, -22],
  shadowUrl: null,
  shadowSize: null,
  shadowAnchor: null,
});

const yellowMarkerIcon = new L.Icon({
  iconUrl: "https://raw.githubusercontent.com/pointhi/leaflet-color-markers/master/img/marker-icon-yellow.png",
  iconSize: [22, 22],
  iconAnchor: [11, 22],
  popupAnchor: [0, -22],
  shadowUrl: null,
  shadowSize: null,
  shadowAnchor: null,
});

const violetMarkerIcon = new L.Icon({
  iconUrl: "https://raw.githubusercontent.com/pointhi/leaflet-color-markers/master/img/marker-icon-violet.png",
  iconSize: [22, 22],
  iconAnchor: [11, 22],
  popupAnchor: [0, -22],
  shadowUrl: null,
  shadowSize: null,
  shadowAnchor: null,
});
// ...Add other icon definitions if needed

function ClickHandler({ addVertex, setStart, enableVertexPlacement }) {
  useMapEvents({
    click(e) {
      if (enableVertexPlacement) addVertex([e.latlng.lat, e.latlng.lng]);
    },
    contextmenu(e) {
      setStart([e.latlng.lat, e.latlng.lng]);
    }
  });
  return null;
}

const DEFAULT_CENTER = [59.4303437, 10.4726724];

// --- PAN/ZOOMABLE CARTESIAN PLOT (SVG) ---
function CartesianPlot({ cartesianVertices = [], cartesianPath = [], width = 700, height = 690 }) {
  try {
    // cartesianVertices: [[x,y], ...] from geoUtils conversion
    // cartesianPath: [[x,y], ...] from backend cartesian data
    const safeVertices = Array.isArray(cartesianVertices) ? cartesianVertices : [];
    const safePath = Array.isArray(cartesianPath) ? cartesianPath : [];
    
    console.log("CartesianPlot - Safe vertices:", safeVertices);
    console.log("CartesianPlot - Safe path:", safePath);

    // All points for bounds calculation
    const allPoints = [...safeVertices, ...safePath];
    const xs = allPoints.map(([x, y]) => x);
    const ys = allPoints.map(([x, y]) => y);

  // Default bounds if empty
  let minX = xs.length ? Math.min(...xs) : -50, maxX = xs.length ? Math.max(...xs) : 50;
  let minY = ys.length ? Math.min(...ys) : -50, maxY = ys.length ? Math.max(...ys) : 50;
  const padding = 20;
  minX -= padding; maxX += padding; minY -= padding; maxY += padding;

  // --- PAN/ZOOM STATE ---
  const [viewBox, setViewBox] = useState({
    x: minX,
    y: minY,
    w: maxX - minX,
    h: maxY - minY,
  });

  // Reset viewBox if vertices change
  React.useEffect(() => {
    setViewBox({
      x: minX,
      y: minY,
      w: maxX - minX,
      h: maxY - minY,
    });
    // eslint-disable-next-line
  }, [cartesianVertices.length]); // only reset when points change

  // Mouse drag/pan logic
  const drag = useRef({ active: false, startX: 0, startY: 0, origX: 0, origY: 0 });
  function onMouseDown(e) {
    drag.current.active = true;
    drag.current.startX = e.clientX;
    drag.current.startY = e.clientY;
    drag.current.origX = viewBox.x;
    drag.current.origY = viewBox.y;
  }
  function onMouseMove(e) {
    if (!drag.current.active) return;
    const dx = (e.clientX - drag.current.startX) * viewBox.w / width;
    const dy = (e.clientY - drag.current.startY) * viewBox.h / height;
    setViewBox(v => ({ ...v, x: drag.current.origX - dx, y: drag.current.origY + dy }));
  }
  function onMouseUp() {
    drag.current.active = false;
  }
  // Zoom logic (mouse wheel)
  function onWheel(e) {
    e.preventDefault();
    const zoom = e.deltaY < 0 ? 0.9 : 1.1;
    const mx = e.nativeEvent.offsetX / width;
    const my = e.nativeEvent.offsetY / height;
    setViewBox(v => {
      const newW = v.w * zoom;
      const newH = v.h * zoom;
      return {
        w: newW,
        h: newH,
        x: v.x + (v.w - newW) * mx,
        y: v.y + (v.h - newH) * (1 - my),
      };
    });
  }
  // Button zoom (for accessibility)
  function zoomBy(factor) {
    setViewBox(v => {
      const newW = v.w * factor;
      const newH = v.h * factor;
      return {
        w: newW,
        h: newH,
        x: v.x + (v.w - newW) / 2,
        y: v.y + (v.h - newH) / 2,
      };
    });
  }

  // Mapping (cartesian to SVG)
  const scale = (val, min, max, size) =>
    max - min === 0 ? size / 2 : ((val - min) / (max - min)) * size;
  const mapPoint = ([x, y]) => [
    ((x - viewBox.x) / viewBox.w) * width,
    height - ((y - viewBox.y) / viewBox.h) * height
  ];

  // Grid step in meters
  const gridStep = 20;
  const gridMinX = Math.floor(viewBox.x / gridStep) * gridStep;
  const gridMaxX = Math.ceil((viewBox.x + viewBox.w) / gridStep) * gridStep;
  const gridMinY = Math.floor(viewBox.y / gridStep) * gridStep;
  const gridMaxY = Math.ceil((viewBox.y + viewBox.h) / gridStep) * gridStep;

  // Grid lines and axis numbers
  const gridLines = [];
  const axisNumbers = [];
  for (let gx = gridMinX; gx <= gridMaxX; gx += gridStep) {
    const [x1, y1] = mapPoint([gx, gridMinY]);
    const [x2, y2] = mapPoint([gx, gridMaxY]);
    gridLines.push(<line key={`vgrid${gx}`} x1={x1} y1={y1} x2={x2} y2={y2} stroke="#e0e0e0" strokeWidth={1} />);
    if (Math.abs(gx) < 1e-6) continue;
    axisNumbers.push(
      <text key={`xnum${gx}`} x={x1} y={height - 5} fontSize="18" fill="#888" textAnchor="middle">{gx}</text>
    );
  }
  for (let gy = gridMinY; gy <= gridMaxY; gy += gridStep) {
    const [x1, y1] = mapPoint([gridMinX, gy]);
    const [x2, y2] = mapPoint([gridMaxX, gy]);
    gridLines.push(<line key={`hgrid${gy}`} x1={x1} y1={y1} x2={x2} y2={y2} stroke="#e0e0e0" strokeWidth={1} />);
    if (Math.abs(gy) < 1e-6) continue;
    axisNumbers.push(
      <text key={`ynum${gy}`} x={10} y={y1 + 6} fontSize="18" fill="#888" textAnchor="start">{gy}</text>
    );
  }

  // Axes (0,0), labels, and origin
  const zeroX = mapPoint([0, viewBox.y])[0];
  const zeroY = mapPoint([viewBox.x, 0])[1];
  axisNumbers.push(
    <text key="xlabel" x={width / 2} y={height - 25} fontSize="20" fill="#444" textAnchor="middle" fontWeight="bold">+X (East, meters)</text>
  );
  axisNumbers.push(
    <text key="ylabel" x={32} y={40} fontSize="20" fill="#444" textAnchor="start" fontWeight="bold" transform={`rotate(-90 32,40)`}>+Y (North, meters)</text>
  );
  axisNumbers.push(
    <text key="origin" x={zeroX + 8} y={zeroY - 8} fontSize="16" fill="#444" textAnchor="start">0,0</text>
  );

  // --- SVG handlers: add event listeners for panning
  // (handle mousemove/mouseup on window for smoother pan)
  React.useEffect(() => {
    function handleUp() { drag.current.active = false; }
    function handleMove(e) { onMouseMove(e); }
    window.addEventListener("mousemove", handleMove);
    window.addEventListener("mouseup", handleUp);
    return () => {
      window.removeEventListener("mousemove", handleMove);
      window.removeEventListener("mouseup", handleUp);
    };
    // eslint-disable-next-line
  }, [viewBox, width, height]);

  return (
    <div style={{ width, height, background: "#f8f8f8", position: "relative", userSelect: "none" }}>
      {/* Leaflet-like Zoom controls */}
      <div
        className="leaflet-control leaflet-bar leaflet-control-zoom"
        style={{
          position: "absolute",
          top: 12,
          right: 12,
          zIndex: 10,
          boxShadow: "0 1px 5px rgba(0,0,0,0.65)",
          borderRadius: 4,
        }}
      >
        <button
          type="button"
          className="leaflet-control-zoom-in"
          title="Zoom in"
          style={{
            display: "block",
            width: 26,
            height: 26,
            lineHeight: "26px",
            padding: 0,
            margin: 0,
            textAlign: "center",
            background: "#fff",
            border: "none",
            borderBottom: "1px solid #ccc",
            borderRadius: "4px 4px 0 0",
            cursor: "pointer",
            fontSize: 18,
            fontWeight: "bold",
            color: "#000",
            outline: "none",
            userSelect: "none"
          }}
          onClick={() => zoomBy(0.9)}
          aria-label="Zoom in"
        >+</button>
        <button
          type="button"
          className="leaflet-control-zoom-out"
          title="Zoom out"
          style={{
            display: "block",
            width: 26,
            height: 26,
            lineHeight: "26px",
            padding: 0,
            margin: 0,
            textAlign: "center",
            background: "#fff",
            border: "none",
            borderTop: "1px solid #ccc",
            borderRadius: "0 0 4px 4px",
            cursor: "pointer",
            fontSize: 18,
            fontWeight: "bold",
            color: "#000",
            outline: "none",
            userSelect: "none"
          }}
          onClick={() => zoomBy(1.1)}
          aria-label="Zoom out"
        >−</button>
      </div>
      <svg
        width={width}
        height={height}
        style={{ background: "#f8f8f8", cursor: drag.current.active ? "grabbing" : "grab", border: "none" }}
        onMouseDown={onMouseDown}
        onWheel={onWheel}
      >
        {/* Grid */}
        {gridLines}
        {/* Axes */}
        <line x1={zeroX} y1={0} x2={zeroX} y2={height} stroke="#999" strokeWidth={2} />
        <line x1={0} y1={zeroY} x2={width} y2={zeroY} stroke="#999" strokeWidth={2} />
        {/* Axis numbers and labels */}
        {axisNumbers}
        {/* Polygon */}
        {safeVertices.length > 2 && (
          <polygon
            points={safeVertices.map(mapPoint).map(([x, y]) => `${x},${y}`).join(" ")}
            fill="none"
            stroke="#2a6eea"
            strokeWidth={2}
          />
        )}
        {/* Path */}
        {safePath.length > 1 && (
          <polyline
            points={safePath.map(mapPoint).map(([x, y]) => `${x},${y}`).join(" ")}
            fill="none"
            stroke="#e74c3c"
            strokeWidth={3}
          />
        )}
        {/* Vertices */}
        {safeVertices.map((p, i) => {
          const [x, y] = mapPoint(p);
          return (
            <circle key={i} cx={x} cy={y} r={8} fill="#e74c3c" stroke="#fff" strokeWidth={2} />
          );
        })}
        {/* Path points with proper colors */}
        {safePath.map((p, i) => {
          const [x, y] = mapPoint(p);
          const color = i === 0 ? "#ffff00" : "#8b00ff"; // Yellow for start, Violet for end
          return (
            <circle key={"path-" + i} cx={x} cy={y} r={8} fill={color} stroke="#fff" strokeWidth={2} />
          );
        })}
      </svg>
    </div>
  );
  } catch (error) {
    console.error("Error in CartesianPlot:", error);
    return (
      <div style={{ width, height, background: "#f8f8f8", display: "flex", alignItems: "center", justifyContent: "center" }}>
        <div style={{ color: "#666" }}>Error rendering plot</div>
      </div>
    );
  }
}

export default function MapArea() {
  // LatLon state and rest as before...
  const [vertices, setVertices] = useState([]);
  const [start, setStart] = useState(null);
  const [enableVertexPlacement, setEnableVertexPlacement] = useState(true);
  const [pathData, setPathData] = useState([]);
  const [resultPoints, setResultPoints] = useState({ gps: [], cartesian: [] });

  // Reference point: start position (boat), or fallback
  const refLat = start ? start[0] : (vertices[0]?.[0] ?? DEFAULT_CENTER[0]);
  const refLon = start ? start[1] : (vertices[0]?.[1] ?? DEFAULT_CENTER[1]);

  // Memoized cartesian conversion for efficiency (live as user edits)
  const cartesianVertices = useMemo(
    () => verticesLatLonToCartesian(vertices, refLat, refLon),
    [vertices, refLat, refLon]
  );

  const cartesianPath = useMemo(() => {
    try {
      if (!resultPoints.cartesian || !Array.isArray(resultPoints.cartesian)) {
        return [];
      }
      // Backend sends cartesian data as objects: {x: number, y: number}
      return resultPoints.cartesian.map(p => [p.x, p.y]);
    } catch (error) {
      console.error("Error processing cartesian path:", error);
      return [];
    }
  }, [resultPoints.cartesian]);

  // Function to fetch result points from backend
  const fetchResultPoints = useCallback(async () => {
    try {
      const response = await fetch("http://localhost:3000/api/result-points");
      if (!response.ok) {
        throw new Error(`HTTP error! status: ${response.status}`);
      }
      const data = await response.json();
      console.log("Raw fetched data:", data);
      
      // Backend sends: { gps: [{latitude, longitude, timestamp}, ...], cartesian: [{x, y}, ...] }
      if (data && typeof data === 'object') {
        const safeData = {
          gps: Array.isArray(data.gps) ? data.gps : [],
          cartesian: Array.isArray(data.cartesian) ? data.cartesian : []
        };
        
        console.log("Setting result points:", safeData);
        setResultPoints(safeData);
      } else {
        console.warn("Invalid data structure received:", data);
      }
    } catch (error) {
      console.error("Error fetching result points:", error);
    }
  }, []);

  // Auto-fetch result points every 2 seconds when we have vertices
  useEffect(() => {
    let interval;
    if (vertices.length > 0) {
      interval = setInterval(fetchResultPoints, 2000);
    }
    return () => {
      if (interval) {
        clearInterval(interval);
      }
    };
  }, [vertices.length, fetchResultPoints]);

  // Handlers
  const moveVertex = (index, newLatLng) => {
    const newVertices = [...vertices];
    newVertices[index] = [newLatLng.lat, newLatLng.lng];
    setVertices(newVertices);
  };
  const undoVertex = () => setVertices(vertices.slice(0, -1));
  const eraseAllVertices = () => {
    setVertices([]);
    setResultPoints({ gps: [], cartesian: [] }); // Clear result points too
  };
  const clearDebugData = () => setPathData([]);

  return (
    <div style={{ display: "flex", flexDirection: "column", alignItems: "center", width: "100%" }}>
      {/* Top Button Bar */}
      <div style={{ marginBottom: "12px", marginTop: "10px" }}>
        <button
          onClick={() => setEnableVertexPlacement(!enableVertexPlacement)}
          style={{
            backgroundColor: enableVertexPlacement ? "#e74c3c" : "#2ecc40",
            color: "#fff",
            marginRight: "10px",
            padding: "8px 16px",
            fontWeight: "bold",
            border: "none",
            borderRadius: "4px",
            fontSize: "1.1em"
          }}
        >
          {enableVertexPlacement ? "Vertex Placement: ON" : "Vertex Placement: OFF"}
        </button>
        <button
          onClick={undoVertex}
          disabled={vertices.length === 0}
          style={{
            marginRight: "10px",
            padding: "8px 16px",
            borderRadius: "4px",
            border: "none",
            fontWeight: "bold",
            fontSize: "1.1em",
            background: "#363636",
            color: "#fff"
          }}
        >
          Undo
        </button>
        <button
          onClick={eraseAllVertices}
          disabled={vertices.length === 0}
          style={{
            marginRight: "10px",
            padding: "8px 16px",
            borderRadius: "4px",
            border: "none",
            fontWeight: "bold",
            fontSize: "1.1em",
            background: "#363636",
            color: "#fff"
          }}
        >
          Erase All
        </button>
        <button
          onClick={clearDebugData}
          style={{
            padding: "8px 16px",
            borderRadius: "4px",
            border: "none",
            fontWeight: "bold",
            fontSize: "1.1em",
            backgroundColor: "#f39c12",
            color: "white"
          }}
        >
          Clear Debug
        </button>
      </div>
      {/* Map+Plot side-by-side */}
      <div
        style={{
          display: "flex",
          flexDirection: "row",
          borderRadius: "7px",
          width: "1400px",
          height: "690px",
          boxSizing: "content-box",
          marginBottom: "17px"
        }}
      >
        {/* Left: Pan/Zoom Cartesian Plot */}
        <div
          style={{
            width: "50%",
            height: "100%",
            borderRight: "4px solid #2c1887",
            boxSizing: "border-box",
            background: "#f8f8f8"
          }}
        >
          <CartesianPlot cartesianVertices={cartesianVertices} cartesianPath={cartesianPath} width={700} height={690} />
        </div>
        {/* Right: Leaflet Map */}
        <div style={{ width: "50%", height: "100%" }}>
          <MapContainer
            center={DEFAULT_CENTER}
            zoom={13}
            scrollWheelZoom
            style={{ width: "100%", height: "100%" }}
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
                  }
                }}
              />
            ))}
            {start && (
              <Marker position={start} icon={greenMarkerIcon} />
            )}
            {/* Result Points - Start (Yellow) and End (Violet) */}
            {resultPoints.gps && resultPoints.gps.length > 0 && (
              <Marker 
                position={[resultPoints.gps[0].latitude, resultPoints.gps[0].longitude]} 
                icon={yellowMarkerIcon} 
              />
            )}
            {resultPoints.gps && resultPoints.gps.length > 1 && (
              <Marker 
                position={[resultPoints.gps[1].latitude, resultPoints.gps[1].longitude]} 
                icon={violetMarkerIcon} 
              />
            )}
            {/* Path Line between result points */}
            {resultPoints.gps && resultPoints.gps.length === 2 && (
              <Polyline 
                positions={[
                  [resultPoints.gps[0].latitude, resultPoints.gps[0].longitude], 
                  [resultPoints.gps[1].latitude, resultPoints.gps[1].longitude]
                ]} 
                color="red" 
                weight={3}
              />
            )}
            {/* Add Polyline/Path rendering here as needed */}
          </MapContainer>
        </div>
      </div>
      {/* Send to Backend Button */}
      <button
        onClick={async () => {
          try {
            const response = await fetch("http://localhost:3000/api/polygon", {
              method: "POST",
              headers: { "Content-Type": "application/json" },
              body: JSON.stringify({ vertices, start }),
            });
            
            if (!response.ok) {
              throw new Error(`HTTP error! status: ${response.status}`);
            }
            
            const result = await response.json();
            console.log("Survey sent:", result);
            
            // Start polling for results after sending
            setTimeout(() => {
              fetchResultPoints();
            }, 1000);
          } catch (error) {
            console.error("Error sending survey:", error);
            // Optionally show user-friendly error message
          }
        }}
        style={{
          marginTop: "0px",
          marginBottom: "8px",
          padding: "10px 24px",
          borderRadius: "5px",
          border: "none",
          fontWeight: "bold",
          background: "#222",
          color: "#fff",
          fontSize: "1.1em",
          letterSpacing: ".05em",
          boxShadow: "0 2px 10px #0002"
        }}
      >
        Send Survey Area
      </button>
      {/* Help Text */}
      <div style={{
        fontSize: "1em",
        marginTop: "2px",
        color: "#aaa",
        textAlign: "center",
        lineHeight: "1.3",
        width: "1200px",
        background: "#222",
        borderRadius: "0 0 7px 7px",
        paddingBottom: "6px"
      }}>
        Left click to add vertex (when ON) &nbsp;|&nbsp; Right click to set start position &nbsp;|&nbsp; Undo/Erase buttons &nbsp;|&nbsp; Drag vertices when placement is OFF
      </div>
    </div>
  );
}