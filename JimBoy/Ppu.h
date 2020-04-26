#pragma once
#include <iostream>

class Ppu {
public:
private:
	// PPU Registers
	uint8_t lcdc{}; // LCD Control
	uint8_t stat{}; // LCDC Status
	uint8_t scy{}; // Scroll Y
	uint8_t scx{}; // Scroll X
	uint8_t ly{}; // LCDC Y-Coordinate
	uint8_t lyc{}; // LY Compare
	uint8_t wy{}; // Window Y Position
	uint8_t wx{}; // Window X Position
	uint8_t bgp{}; // BG Palette Data
	uint8_t obp0{}; // Object Palette 0 Data
	uint8_t obp1{}; // Object Palette 1 Data
};

