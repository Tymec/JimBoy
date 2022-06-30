#pragma once
#include <iostream>

enum InterruptTypes {
	VBLANK = 0x01,
	LCD = 0x02,
	TIMER = 0x04,
	SERIAL = 0x08,
	JOYPAD = 0x10
};

enum InterruptVector {
	VBLANK_VECTOR = 0x40,
	LCD_VECTOR = 0x48,
	TIMER_VECTOR = 0x50,
	SERIAL_VECTOR = 0x58,
	JOYPAD_VECTOR = 0x60
};

class MemoryController;
class Interrupts {
public:
	Interrupts(MemoryController* memoryController);
	~Interrupts();
	
	uint8_t handleInterrupts();
	void requestInterrupt(InterruptTypes i);
private:
	MemoryController* memoryController;
	friend class Debugger;	// VERY BAD!!!

	void setInterrupt(InterruptTypes i, bool v);
	bool testInterrupt(InterruptTypes i, uint8_t fired);
	uint8_t getInterruptFlag();
	uint8_t getInterruptEnable();

	bool getIe(InterruptTypes i); // Interrupt Enable
	bool getIf(InterruptTypes i); // Interrupt Flags
};

