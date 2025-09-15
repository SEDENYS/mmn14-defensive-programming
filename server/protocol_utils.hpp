#pragma once

#include <cstdint>

// Read 16-bit little-endian
uint16_t read_le16(const uint8_t* p);

// Read 32-bit little-endian
uint32_t read_le32(const uint8_t* p);

// Write 16-bit little-endian
void write_le16(uint8_t* p, uint16_t v);

// Write 32-bit little-endian
void write_le32(uint8_t* p, uint32_t v);
