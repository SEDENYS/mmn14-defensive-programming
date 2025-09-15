#pragma once
#include <cstdint>
#include <vector>
#include <string>

struct RequestHeader {
    uint32_t user_id;
    uint8_t  version;
    uint8_t  op;
    uint16_t name_len;
};

struct ResponseHeader {
    uint8_t  version;
    uint16_t status;
    uint16_t name_len;
};

enum Op : uint8_t {
    OP_BACKUP   = 100,
    OP_RETRIEVE = 200,
    OP_DELETE   = 201,
    OP_LIST     = 202
};

enum Status : uint16_t {
    ST_RETRIEVE_OK = 210,
    ST_LIST_OK     = 211,
    ST_GENERIC_OK  = 212,
    ST_NOT_FOUND   = 1001,
    ST_NO_FILES    = 1002,
    ST_SERVER_ERR  = 1003
};

// Build a generic OK=212 response without payload or filename
std::vector<uint8_t> build_ok212(uint8_t version = 1);
