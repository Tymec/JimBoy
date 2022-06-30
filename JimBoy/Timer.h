#pragma once
#include <iostream>
#include "MemoryController.h"
#include "Interrupts.h"
#include "Utils.h"
#include "Types.h"

enum TimerRegisters {
	DIV = 0xFF04,	// Divider Register
	TIMA = 0xFF05,	// Timer counter
	TMA = 0xFF06,	// Timer Modulo
	TAC = 0xFF07	// Timer Control
};
const int TimerClocks[] = {1024, 16, 64, 256};

class MemoryController;
class Timer {
public:
	Timer(MemoryController* memoryController);
	~Timer();

	void Update(unsigned cycles);
private:
	MemoryController* memoryController;

	uint16_t timerCounter = TimerClocks[0];
	uint8_t timerClock = 0;
	uint16_t divCounter = 0;

	void updateDivider(unsigned cycles);
	void checkTimerClock();
	bool isClockEnabled();
	uint8_t getTimerClock();
	void setTimerFreq();
	uint8_t getRegister(TimerRegisters reg);
};

