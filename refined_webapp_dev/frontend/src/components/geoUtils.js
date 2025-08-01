// WGS84 constants
const R = 6378137; // Earth's radius in meters (mean radius for equirectangular)

// Helper: Convert lat/lon to local cartesian (ENU) given a reference point
export function latLonToLocalCartesian(lat, lon, refLat, refLon) {
  // Equirectangular approximation (good for small areas)
  const degToRad = Math.PI / 180;
  const dLat = (lat - refLat) * degToRad;
  const dLon = (lon - refLon) * degToRad;
  const meanLat = ((lat + refLat) / 2) * degToRad;
  const x = dLon * Math.cos(meanLat) * R;
  const y = dLat * R;
  return [x, y];
}

// Helper: Convert array of vertices [[lat,lon], ...] to cartesian
export function verticesLatLonToCartesian(vertices, refLat, refLon) {
  return vertices.map(([lat, lon]) => latLonToLocalCartesian(lat, lon, refLat, refLon));
}