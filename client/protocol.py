# protocol.py
import struct

def build_backup_request(user_id, filename):
    """
    יוצר בקשה בינארית לגיבוי קובץ לפי הפרוטוקול
    """
    version = 1
    op_code = 100

    with open(filename, "rb") as f:
        file_data = f.read()

    file_bytes = filename.encode("ascii")
    name_len = len(file_bytes)
    file_size = len(file_data)

    # header: user_id(4), version(1), op_code(1), name_len(2), filename
    header = struct.pack("<IBBH", user_id, version, op_code, name_len)
    header += file_bytes

    # payload: size(4), file_data
    payload = struct.pack("<I", file_size) + file_data

    return header + payload

def build_list_request(user_id, version=1):
    """
    בונה בקשה בינארית לבקשת רשימת קבצים מהשרת
    """
    op_code = 202
    name_len = 0  # אין קובץ
    header = struct.pack("<IBBH", user_id, version, op_code, name_len)
    return header  # אין filename, ואין payload

def build_delete_request(user_id, filename, version=1):
    """
    בונה בקשה למחיקת קובץ מהשרת
    """
    op_code = 201
    file_bytes = filename.encode("ascii")
    name_len = len(file_bytes)

    header = struct.pack("<IBBH", user_id, version, op_code, name_len)
    header += file_bytes

    return header  # אין size ואין תוכן

def build_retrieve_request(user_id, filename, version=1):
    """
    בונה בקשה לאחזור קובץ מהשרת
    """
    op_code = 200
    file_bytes = filename.encode("ascii")
    name_len = len(file_bytes)

    header = struct.pack("<IBBH", user_id, version, op_code, name_len)
    header += file_bytes

    return header
