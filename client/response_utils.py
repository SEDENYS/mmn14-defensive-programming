import struct

def parse_header(data):
    version = data[0]
    status = int.from_bytes(data[1:3], "little")
    name_len = int.from_bytes(data[3:5], "little")
    size = int.from_bytes(data[5:9], "little")
    filename = data[9:9+name_len].decode() if name_len > 0 else ""
    payload = data[9+name_len:9+name_len+size] if size > 0 else b""

    if version != 1:
        # check the version
        print(f" Warning: unexpected version {version}, expected 1")

    return version, status, filename, size, payload


#  BACKUP
def parse_backup_response(data: bytes) -> str:
    version, status, filename, size, payload = parse_header(data)
    if status == 210:
        return f"Backup OK ({filename})"
    elif status == 1001:
        return f"Backup failed ({filename})"
    else:
        return f"Unexpected status {status} for BACKUP"

# DELETE
def parse_delete_response(data: bytes) -> str:
    version, status, filename, size, payload = parse_header(data)
    if status == 211:
        return f"Delete OK ({filename})"
    elif status == 1002:
        return f"Delete failed: {filename} not found"
    else:
        return f"Unexpected status {status} for DELETE"

#  LIST
def parse_list_response(data: bytes):
    version, status, filename, size, payload = parse_header(data)
    if status != 212:
        return f"List failed (status={status})"
    files = []
    offset = 0
    count = struct.unpack("<I", payload[offset:offset+4])[0]
    offset += 4
    for _ in range(count):
        name_len = struct.unpack("<H", payload[offset:offset+2])[0]
        offset += 2
        name = payload[offset:offset+name_len].decode()
        offset += name_len
        files.append(name)
    return files

# RETRIEVE
def parse_retrieve_response(data: bytes, filename: str) -> str:
    version, status, fname, size, payload = parse_header(data)
    if status == 212:
        if size > 0:
            outname = f"retrieved_{filename}"
            with open(outname, "wb") as f:
                f.write(payload)
            return f"Retrieve OK: saved {outname}"
        else:
            return f"Retrieve OK but empty file ({filename})"
    elif status == 1003:
        return f"{filename} not found on server"
    else:
        return f"Unexpected status {status} for RETRIEVE"
