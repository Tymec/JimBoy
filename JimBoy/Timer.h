#pragma once
#include <iostream>

class Timer {
public:
private:
	// Timer and Divider Registers
	uint8_t div{}; // Divider Register
	uint8_t tima{}; // Timer counter
	uint8_t tma{}; // Timer Modulo
	uint8_t tac{}; // Timer Control
};

