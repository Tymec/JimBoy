#pragma once
#include <iostream>

enum lcdMode {
	H_BLANK,
	V_BLANK,
	OAM_SEARCH,
	TRANSFER
};

enum ppuRegister {
	LCDC = 0xFF40,	// LCD Control
	STAT = 0xFF41,	// LCDC Status
	SCY = 0xFF42,	// Scroll Y
	SCX = 0xFF43,	// Scroll X
	LY = 0xFF44,	// LCDC Y-Coordinate
	LYC = 0xFF45,	// LY Compare
	WY = 0xFF4A,	// Window Y Position
	WX = 0xFF4B,	// Window X Position
	BGP = 0xFF47,	// BG Palette Data
	OBP0 = 0xFF48,	// Object Palette 0 Data
	OBP1 = 0xFF49,	// Object Palette 1 Data
	DMA = 0xFF46	// DMA Transfer & Start Address
};

class MemoryController;
class Ppu {
public:
	Ppu(MemoryController *memoryController);
	~Ppu();
	
	uint32_t framebuffer[160 * 144]{};
	void Update(unsigned cycles);
	uint8_t getRegister(ppuRegister reg);
private:
	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t value);
	lcdMode Mode = OAM_SEARCH;

	MemoryController *memoryController; // Pointer to MemoryController
};

