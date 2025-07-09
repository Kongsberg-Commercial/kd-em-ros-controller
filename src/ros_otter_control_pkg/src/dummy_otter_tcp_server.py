import socket
import threading

HOST = '0.0.0.0'      # Listen on all interfaces
PORT = 2009           # The port your node tries to connect to

def handle_client(conn, addr):
    print(f"[Dummy Otter] Connected by {addr}")
    try:
        while True:
            data = conn.recv(1024)
            if not data:
                break
            # Echo received data, or print it
            print(f"[Dummy Otter] Received: {data.decode(errors='replace').strip()}")
            # Respond with a fake NMEA sentence for testing, if you want:
            conn.sendall(b"$PMARFAKE,1,2,3*42\r\n")
    except Exception as e:
        print(f"[Dummy Otter] Exception: {e}")
    finally:
        print(f"[Dummy Otter] Connection closed: {addr}")
        conn.close()

def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.bind((HOST, PORT))
        s.listen()
        print(f"[Dummy Otter] Listening on {HOST}:{PORT}")
        while True:
            conn, addr = s.accept()
            threading.Thread(target=handle_client, args=(conn, addr), daemon=True).start()

if __name__ == "__main__":
    main()