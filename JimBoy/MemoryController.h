#pragma once
#include <iostream>

#include "Cpu.h"
#include "Memory.h"
#include "Cartridge.h"

class MemoryController {
public:
	MemoryController();
	~MemoryController();

	// Initializes the cartridge object
	void InsertCartridge(char const* file_path);
	// Read
	uint8_t read(uint16_t address);
	// Write
	void write(uint16_t address, uint8_t value);
private:
	Memory memory; // Memory
	Cartridge* cartridge = nullptr; // Cartridge
};