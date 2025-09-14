#pragma once
#include <cstdint>

uint16_t read_le16(const uint8_t* p);
uint32_t read_le32(const uint8_t* p);
void write_le16(uint8_t* p, uint16_t v);
void write_le32(uint8_t* p, uint32_t v);
