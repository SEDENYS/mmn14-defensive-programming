
from .protocol import (
    build_backup_request,
    build_list_request,
    build_delete_request,
    build_retrieve_request,
)
from .connection import send_request

def send_backup_file(ip, port, user_id, filename):
    message = build_backup_request(user_id, filename)
    return send_request(ip, port, message)

def request_file_list(ip, port, user_id):
    message = build_list_request(user_id)
    return send_request(ip, port, message)

def delete_file(ip, port, user_id, filename):
    message = build_delete_request(user_id, filename)
    return send_request(ip, port, message)

def retrieve_file(ip, port, user_id, filename):
    message = build_retrieve_request(user_id, filename)
    return send_request(ip, port, message)
