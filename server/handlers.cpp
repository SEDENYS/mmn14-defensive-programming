// ===== handlers.cpp =====
#include "handlers.hpp"
#include "protocol_utils.hpp"
#include "file_utils.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <mutex>
#include <filesystem>

std::mutex log_mutex;
// בדיקת גרסה - חלק מ-Defensive Programming
void check_version(uint8_t version) {
    if (version != 1) {
        std::lock_guard<std::mutex> lock(log_mutex);
        std::cerr << "[SERVER] Warning: unexpected protocol version "
                  << (int)version << ", expected 1\n";
    }
}

// פונקציה אחידה לשליחת תשובה ללקוח
void send_response(int client_fd, uint16_t status, const std::string &filename,
                   const std::vector<uint8_t> &payload = {})
{
    uint8_t version = 1;
    uint16_t name_len = filename.size();
    uint32_t size = payload.size();

    std::vector<uint8_t> reply;

    // version
    reply.push_back(version);

    // status (2B LE)
    reply.resize(reply.size() + 2);
    write_le16(reply.data() + 1, status);

    // name_len (2B LE)
    reply.resize(reply.size() + 2);
    write_le16(reply.data() + 3, name_len);

    // size (4B LE)
    reply.resize(reply.size() + 4);
    write_le32(reply.data() + 5, size);

    // filename
    if (name_len > 0)
    {
        size_t old = reply.size();
        reply.resize(old + name_len);
        memcpy(reply.data() + old, filename.data(), name_len);
    }

    // payload
    if (size > 0)
    {
        size_t old = reply.size();
        reply.resize(old + size);
        memcpy(reply.data() + old, payload.data(), size);
    }

    send(client_fd, reply.data(), reply.size(), 0);
}

void handle_backup(int client_fd, uint32_t user_id, const std::string &filename)
{
    uint8_t szbuf[4];
    if (recv(client_fd, szbuf, 4, 0) != 4)
    {
        send_response(client_fd, 1001, filename); // error
        return;
    }
    uint32_t size = read_le32(szbuf);

    std::vector<uint8_t> filedata(size);
    if (size > 0 && recv(client_fd, filedata.data(), size, 0) != (int)size)
    {
        send_response(client_fd, 1001, filename);
        return;
    }

    std::string path = get_backup_path(user_id, filename);
    std::ofstream out(path, std::ios::binary);
    if (!out)
    {
        send_response(client_fd, 1001, filename); // open error
        return;
    }

    if (size == 0)
    {
        // מטלה יכולה להחליט אם זה נחשב כשלון או הצלחה. כאן נגדיר ככשלון.
        send_response(client_fd, 1001, filename);
        return;
    }

    out.write((char *)filedata.data(), size);
    send_response(client_fd, 210, filename); // Backup OK
}

void handle_delete(int client_fd, uint32_t user_id, const std::string &filename)
{
    std::string path = get_backup_path(user_id, filename);
    uint16_t status = (remove(path.c_str()) == 0) ? 211 : 1002;
    send_response(client_fd, status, filename);
}

void handle_retrieve(int client_fd, uint32_t user_id, const std::string &filename)
{
    std::string path = get_backup_path(user_id, filename);
    std::ifstream in(path, std::ios::binary);
    if (!in)
    {
        send_response(client_fd, 1003, filename); // file not found
        return;
    }

    in.seekg(0, std::ios::end);
    uint32_t size = in.tellg();
    in.seekg(0, std::ios::beg);

    std::vector<uint8_t> buf(size);
    in.read((char *)buf.data(), size);

    std::vector<uint8_t> payload(4 + size);
    write_le32(payload.data(), size);
    if (size > 0)
    {
        memcpy(payload.data() + 4, buf.data(), size);
    }

    send_response(client_fd, 212, filename, payload);
}

void handle_list(int client_fd, uint32_t user_id)
{
    std::string dir = "backupsvr/" + std::to_string(user_id);
    std::vector<std::string> files;

    if (std::filesystem::exists(dir))
    {
        for (const auto &entry : std::filesystem::directory_iterator(dir))
        {
            if (entry.is_regular_file())
            {
                files.push_back(entry.path().filename().string());
            }
        }
    }

    uint32_t count = files.size();
    std::vector<uint8_t> payload(4);
    write_le32(payload.data(), count);

    for (const auto &f : files)
    {
        uint16_t len = f.size();
        size_t old = payload.size();
        payload.resize(old + 2 + len);
        write_le16(payload.data() + old, len);
        memcpy(payload.data() + old + 2, f.data(), len);
    }

    send_response(client_fd, 212, "", payload);
}

void handle_client(int client_fd)
{
    uint8_t header[8];
    if (recv(client_fd, header, 8, 0) != 8)
    {
        close(client_fd);
        return;
    }
    uint32_t user_id = read_le32(header);
    uint8_t version = header[4];
    check_version(version);
    uint8_t op = header[5];
    uint16_t name_len = read_le16(header + 6);

    std::string filename;
    if (name_len > 0)
    {
        std::vector<char> namebuf(name_len);
        recv(client_fd, namebuf.data(), name_len, 0);
        filename.assign(namebuf.data(), name_len);
    }

    std::lock_guard<std::mutex> lock(log_mutex);
    std::cout << "[SERVER] user=" << user_id << " op=" << (int)op << " file=" << filename << "\n";

    switch (op)
    {
    case 100:
        handle_backup(client_fd, user_id, filename);
        break;
    case 200:
        handle_retrieve(client_fd, user_id, filename);
        break;
    case 201:
        handle_delete(client_fd, user_id, filename);
        break;
    case 202:
        handle_list(client_fd, user_id);
        break;
    default:
    {
        std::lock_guard<std::mutex> lock(log_mutex);
        std::cerr << "Unknown op: " << (int)op << std::endl;
    }
        send_response(client_fd, 9999, filename);
        break;
    }

    close(client_fd);
}
