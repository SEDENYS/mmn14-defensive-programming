# mmn14client.py
from client.file_utils import generate_user_id, read_server_info, read_backup_files
from client.actions import send_backup_file, request_file_list, delete_file, retrieve_file
from client.response_utils import (
    parse_backup_response,
    parse_list_response,
    parse_delete_response,
    parse_retrieve_response,
)

if __name__ == "__main__":
    user_id = generate_user_id()
    print(f"User ID: {user_id}")

    ip, port = read_server_info()
    print(f"Connecting to server at {ip}:{port}")

    files = read_backup_files()
    print("Files to backup:", files)

    # === 1. LIST (initial) ===
    print("\n=== LIST (initial) ===")
    resp = request_file_list(ip, port, user_id)
    print("Files on server:", parse_list_response(resp))

    # === 2. BACKUP file 1 ===
    print("\n=== BACKUP file 1 ===")
    resp = send_backup_file(ip, port, user_id, files[0])
    print(parse_backup_response(resp), f"({files[0]})")

    # === 3. BACKUP file 2 ===
    print("\n=== BACKUP file 2 ===")
    if len(files) > 1:
        resp = send_backup_file(ip, port, user_id, files[1])
        print(parse_backup_response(resp), f"({files[1]})")

    # === 4. LIST (after backups) ===
    print("\n=== LIST (after backups) ===")
    resp = request_file_list(ip, port, user_id)
    print("Files on server:", parse_list_response(resp))

    # === 5. RETRIEVE file 1 ===
    print("\n=== RETRIEVE file 1 ===")
    resp = retrieve_file(ip, port, user_id, files[0])
    print(parse_retrieve_response(resp, files[0]))

    # === 6. DELETE file 1 ===
    print("\n=== DELETE file 1 ===")
    resp = delete_file(ip, port, user_id, files[0])
    print(parse_delete_response(resp, files[0]))

    # === 7. RETRIEVE file 1 again (should fail) ===
    print("\n=== RETRIEVE file 1 again ===")
    resp = retrieve_file(ip, port, user_id, files[0])
    print(parse_retrieve_response(resp, files[0]))
