#pragma once
#include <iostream>
#include "MemoryController.h"
#include "Cpu.h"
#include "Ppu.h"
#include "Display.h"
#include "Interrupts.h"
#include "Debugger.h"

class JimBoy {
public:
	JimBoy();
	~JimBoy();

	void LoadBootRom(char const* bootrom_path);
	void Start(char const* rom_path);
private:

	MemoryController memoryController;
	Cpu cpu;
	Ppu ppu;
	Display display;
	Debugger debugger;
	uint8_t bootrom[0x100]{};
	unsigned MAX_CYCLES = 70021;
};

