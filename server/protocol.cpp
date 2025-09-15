#include "protocol.hpp"

std::vector<uint8_t> build_ok212(uint8_t version) {
    std::vector<uint8_t> out;
    out.reserve(1 + 2 + 2);  // version + status + name_len

    out.push_back(version);

    uint16_t st = ST_GENERIC_OK;
    out.push_back(static_cast<uint8_t>(st & 0xFF));
    out.push_back(static_cast<uint8_t>((st >> 8) & 0xFF));

    out.push_back(0); // name_len low byte
    out.push_back(0); // name_len high byte

    return out;
}
