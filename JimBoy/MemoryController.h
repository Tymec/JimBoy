#pragma once
#include <iostream>

#include "Cpu.h"
#include "Memory.h"
#include "Cartridge.h"
#include "Interrupts.h"

class MemoryController {
public:
	MemoryController();
	~MemoryController();

	// Initializes the cartridge object
	void InsertCartridge(char const* file_path);
	// CPU Read & Write
	uint8_t cpuRead(uint16_t address);
	void cpuWrite(uint16_t address, uint8_t value);
	// PPU Read & Write
	uint8_t ppuRead(uint16_t address);
	void ppuWrite(uint16_t address, uint8_t value);
	// Direct Write
	void directWrite(uint16_t address, uint8_t value);

	void requestInterrupt(InterruptTypes i);
	uint8_t handleInterrupts();

	void insertBootrom(uint8_t* bootrom);
	bool isBootromEnabled();
	bool isCartridgeLoaded();
private:
	friend class Debugger; // NOT SMART!!!
	Memory memory; // Memory
	Interrupts interrupts; // Interrupts
	Cartridge *cartridge = nullptr; // Cartridge
	uint8_t *bootrom = nullptr; // 256B Boot ROM
};