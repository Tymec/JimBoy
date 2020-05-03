#pragma once
#include <iostream>

enum timerRegisters {
	DIV = 0xFF04,	// Divider Register
	TIMA = 0xFF05,	// Timer counter
	TMA = 0xFF06,	// Timer Modulo
	TAC = 0xFF07	// Timer Control
};

class MemoryController;
class Timer {
public:
	Timer(MemoryController* memoryController);
	~Timer();
private:
	MemoryController* memoryController;

	uint8_t getRegister(timerRegisters reg);
};

