#pragma once
#include <SDL.h>

class Display {
public:
	Display();
	~Display();
private:
	uint16_t display[160 * 144]{}; // Display
};

