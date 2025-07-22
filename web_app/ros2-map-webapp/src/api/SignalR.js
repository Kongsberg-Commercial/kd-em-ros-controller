import * as signalR from '@microsoft/signalr';

let connectionState = {
    isConnected: false,
    reconnectAttempts: 0,
    maxReconnectAttempts: 50
};

// Create connection factory function so we can import it elsewhere
export function createConnection() {
    const connection = new signalR.HubConnectionBuilder()
        .withUrl('http://localhost:5050/chatHub') // Match your server URL
        .configureLogging(signalR.LogLevel.Information)
        .withAutomaticReconnect([0, 2000, 5000, 10000, 15000, 30000])
        .build();

    connection.onclose(error => {
        connectionState.isConnected = false;
        console.log('SignalR - Connection closed. Attempting to reconnect...', error);

        // If withAutomaticReconnect fails after its retry attempts
        if (connectionState.reconnectAttempts < connectionState.maxReconnectAttempts) {
            setTimeout(() => attemptReconnect(connection), 5000);
        }
    });

    connection.onreconnecting(error => {
        connectionState.isConnected = false;
        console.log('SignalR - Attempting to reconnect...', error);
    });

    connection.onreconnected(connectionId => {
        connectionState.isConnected = true;
        connectionState.reconnectAttempts = 0;
        console.log('SignalR - Reconnected with ID:', connectionId);
    });

    // Start initial connection
    startConnection(connection);

    return connection;
}


// Helper function for starting the connection
function startConnection(connection) {
    console.log('SignalR - Starting SignalR connection');
    connection.start()
        .then(() => {
            connectionState.isConnected = true;
            connectionState.reconnectAttempts = 0;
            console.log('SignalR - Connected to SignalR hub');
        })
        .catch(err => {
            console.error('SignalR - Connection error:', err);
            connectionState.isConnected = false;

            // If initial connection fails, try to reconnect
            if (connectionState.reconnectAttempts < connectionState.maxReconnectAttempts) {
                setTimeout(() => attemptReconnect(connection), 2000);
            }
        });
}

// Manual reconnection function with backoff
function attemptReconnect(connection) {
    connectionState.reconnectAttempts++;

    console.log(`SignalR - Reconnection attempt ${connectionState.reconnectAttempts}/${connectionState.maxReconnectAttempts}`);

    startConnection(connection);
}


// Export the connection for direct use
export const connection = createConnection();

// Public method to manually attempt reconnection
export function reconnect() {
    if (!connectionState.isConnected) {
        connectionState.reconnectAttempts = 0;
        startConnection(connection);
        return true;
    }
    return false; // Already connected
}

// Get connection status
export function getConnectionStatus() {
    return connectionState;
}
export function sendPolygonData(polygonData) {
    if (connectionState.isConnected) {
        connection.invoke('SendPolygonData', polygonData)
            .catch(err => console.error('SignalR - Error sending polygon data:', err));
    } else {
        console.warn('SignalR - Cannot send polygon data, not connected');
    }
}
export function sendStartPoint(startPoint) {
    if (connectionState.isConnected) {
        connection.invoke('SendStartPoint', startPoint)
            .catch(err => console.error('SignalR - Error sending start point:', err));
    } else {
        console.warn('SignalR - Cannot send start point, not connected');
    }
}

// Event listeners for online/offline status, to handle reconnection if the network state changes
window.addEventListener('online', () => {
    console.log('SignalR - Network connection restored');
    if (!connectionState.isConnected) {
        reconnect();
    }
});

window.addEventListener('offline', () => {
    console.log('SignalR - Network connection lost');
    connectionState.isConnected = false;
});