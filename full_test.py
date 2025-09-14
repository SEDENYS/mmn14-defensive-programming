import os
from client.file_utils import generate_user_id, read_server_info
from client.actions import (
    send_backup_file,
    request_file_list,
    delete_file,
    retrieve_file,
)
from client.response_utils import (
    parse_backup_response,
    parse_list_response,
    parse_delete_response,
    parse_retrieve_response,
    parse_header,
)

BACKUPSVR_DIR = "server/backupsvr"
FILENAMES = ["mmn14.pdf", "terminator2.avi"]

def file_exists(user_id, filename):
    path = os.path.join(BACKUPSVR_DIR, str(user_id), filename)
    return os.path.isfile(path)

def print_fs_check(user_id):
    dir_path = os.path.join(BACKUPSVR_DIR, str(user_id))
    if os.path.isdir(dir_path):
        print(f"📂 {dir_path} -> {os.listdir(dir_path)}")
    else:
        print(f"❌ Directory {dir_path} does not exist.")

if __name__ == "__main__":
    ip, port = read_server_info()
    user_id = generate_user_id()
    print(f"User ID: {user_id}")

    # === BACKUP ===
    print("\n=== BACKUP ===")
    for fname in FILENAMES:
        resp = send_backup_file(ip, port, user_id, fname)
        print(parse_backup_response(resp))
    print_fs_check(user_id)
    assert file_exists(user_id, FILENAMES[0]), "❌ BACKUP failed, file not found on disk"

    # === LIST ===
    print("\n=== LIST ===")
    resp = request_file_list(ip, port, user_id)
    files = parse_list_response(resp)
    print("Files on server:", files)
    for fname in FILENAMES:
        assert fname in files, f"❌ LIST failed, {fname} missing"

    # === DELETE ===
    print("\n=== DELETE ===")
    resp = delete_file(ip, port, user_id, FILENAMES[0])
    print(parse_delete_response(resp))
    print_fs_check(user_id)
    assert not file_exists(user_id, FILENAMES[0]), "❌ DELETE failed, file still exists"

    # === RETRIEVE ===
    print("\n=== RETRIEVE ===")
    resp = retrieve_file(ip, port, user_id, FILENAMES[0])
    print(parse_retrieve_response(resp, FILENAMES[0]))
    print_fs_check(user_id)

    print("✅ Full test finished")
