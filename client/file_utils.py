# file_utils.py
import random

def generate_user_id():
    # Generates a random 4-byte (uint32) number
    return random.randint(1, 2**32 - 1)

def read_server_info(filename="server.info"):
    # Reads the server address and port from the file
    with open(filename, "r") as f:
        content = f.read().strip()
        ip, port = content.split(":")
        return ip, int(port)

def read_backup_files(filename="backup.info"):
    # Reads the filenames to back up
    with open(filename, "r") as f:
        return [line.strip() for line in f if line.strip()]
