#pragma once
#include <iostream>
#include "MemoryController.h"
#include "Cpu.h"
#include "Ppu.h"
#include "Display.h"
#include "Timer.h"
#include "Debugger.h"
#include "Types.h"

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
	Timer timer;
	Display display;
	Debugger debugger;
	uint8_t bootrom[0x100]{};
	unsigned MAX_CYCLES = CLOCKSPEED / 60;
};

