#pragma once
#include <iostream>

enum InterruptTypes {
	VBLANK = 0x01,
	LCD = 0x02,
	TIMER = 0x04,
	SERIAL = 0x08,
	JOYPAD = 0x10
};

class MemoryController;
class Interrupts {
public:
	Interrupts(MemoryController* memoryController);
	~Interrupts();

	bool getIeRegister(InterruptTypes i); // Interrupt Enable
	bool getIfRegister(InterruptTypes i); // Interrupt Request Flag
private:
	MemoryController* memoryController;
};

