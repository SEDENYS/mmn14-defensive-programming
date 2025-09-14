import random

def generate_user_id():
    """יוצר מספר אקראי בגודל 4 בתים (uint32)"""
    return random.randint(1, 2**32 - 1)

def read_server_info(filename="server.info"):
    """קורא את כתובת השרת והפורט מתוך הקובץ"""
    with open(filename, "r") as f:
        content = f.read().strip()
        ip, port = content.split(":")
        return ip, int(port)

def read_backup_files(filename="backup.info"):
    """קורא את שמות הקבצים לגיבוי"""
    with open(filename, "r") as f:
        return [line.strip() for line in f if line.strip()]
