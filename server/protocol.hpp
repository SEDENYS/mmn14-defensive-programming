#pragma once
#include <cstdint>
#include <vector>
#include <string>

// פרוטוקול בקשה (Client->Server)
struct RequestHeader {
    uint32_t user_id;   // 4
    uint8_t  version;   // 1
    uint8_t  op;        // 1
    uint16_t name_len;  // 2 (little-endian)
    // אחריו filename (name_len בייטים)
    // ואז (ב־op=100) size (4) + payload (size בייטים)
};

// פרוטוקול תשובה (Server->Client)
struct ResponseHeader {
    uint8_t  version;   // 1
    uint16_t status;    // 2 (little-endian)
    uint16_t name_len;  // 2
    // אחריו filename (name_len בייטים, ללא \0)
    // ואז אופציונלי: size (4) + payload (size בייטים)
};

// קודי פעולות
enum Op : uint8_t {
    OP_BACKUP   = 100,
    OP_RETRIEVE = 200,
    OP_DELETE   = 201,
    OP_LIST     = 202
};

// קודי סטטוס
enum Status : uint16_t {
    ST_RETRIEVE_OK = 210,
    ST_LIST_OK     = 211,
    ST_GENERIC_OK  = 212,
    ST_NOT_FOUND   = 1001,
    ST_NO_FILES    = 1002,
    ST_SERVER_ERR  = 1003
};

// בניית תשובת "הצלחה כללית" 212 ללא payload וללא filename
inline std::vector<uint8_t> build_ok212(uint8_t version = 1) {
    std::vector<uint8_t> out;
    out.reserve(1 + 2 + 2);            // version + status + name_len
    out.push_back(version);
    // status=212 (little-endian)
    uint16_t st = ST_GENERIC_OK;
    out.push_back(static_cast<uint8_t>(st & 0xFF));
    out.push_back(static_cast<uint8_t>((st >> 8) & 0xFF));
    // name_len = 0
    out.push_back(0);
    out.push_back(0);
    return out;
}
