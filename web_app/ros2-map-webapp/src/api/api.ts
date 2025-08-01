
export const API_BASE_URL = "http://localhost:8000/api"

interface Polygon {
    vertices: number[][];
    start: number[];
}

export const postPolygon = async (polygon: Polygon): Promise<void> => {
    await fetch(`${API_BASE_URL}/polygon`, {
        method: "POST",
        headers: {
            "Content-Type": "application/json",
        },
        body: JSON.stringify( polygon ),
    });
}