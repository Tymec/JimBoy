#pragma once
#include <iostream>

inline bool halfCarryAdd(uint8_t a, uint8_t b) {
	return (((a & 0xF) + (b & 0xF)) & 0x10) != 0;
}

inline bool halfCarryAdd16(uint16_t a, uint16_t b) {
	return (((a & 0xFF) + (b & 0xFF)) & 0x100) != 0;
}

inline bool halfCarrySub(uint8_t a, uint8_t b) {
	return ((a & 0xF) - (b & 0xF)) < 0;
}

inline bool halfCarrySub16(uint16_t a, uint16_t b) {
	return ((a & 0xFF) - (b & 0xFF)) < 0;
}

inline bool halfCheck16(const uint16_t a, const uint16_t b, const uint16_t res) {
    return ((a ^ b ^ res) & 0x1000);
}

inline bool halfCheck(const uint8_t a, const uint8_t b, const uint8_t res) {
    return ((a ^ b ^ res) & 0x10);
}