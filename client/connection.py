# connection.py
import socket

def send_request(ip, port, message):
    """פותח חיבור לשרת, שולח הודעה, מחזיר תשובה"""
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            print(f"[CLIENT] Connecting to {ip}:{port}...")
            s.connect((ip, port))
            print("[CLIENT] Connected!")

            s.sendall(message)
            print("[CLIENT] Message sent!")

            response = s.recv(1024)
            print("[CLIENT] Response received!")
            return response

    except Exception as e:
        print(f"[CLIENT][ERROR] {e}")
        return b''
