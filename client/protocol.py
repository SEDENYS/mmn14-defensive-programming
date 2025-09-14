# protocol.py
import struct

def build_backup_request(user_id, filename):
    # Creates a binary request to back up a file according to the protocol
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
    # Builds a binary request to get the list of files from the server
    op_code = 202
    name_len = 0  # no file
    header = struct.pack("<IBBH", user_id, version, op_code, name_len)
    return header  # no filename, no payload

def build_delete_request(user_id, filename, version=1):
    # Builds a request to delete a file from the server
    op_code = 201
    file_bytes = filename.encode("ascii")
    name_len = len(file_bytes)

    header = struct.pack("<IBBH", user_id, version, op_code, name_len)
    header += file_bytes

    return header  # without size or content

def build_retrieve_request(user_id, filename, version=1):
    # Builds a request to retrieve a file from the server
    op_code = 200
    file_bytes = filename.encode("ascii")
    name_len = len(file_bytes)

    header = struct.pack("<IBBH", user_id, version, op_code, name_len)
    header += file_bytes

    return header
