#include <iostream>
#include <chrono>
#include <string>
#include "MemoryController.h"
#include "Cpu.h"

int main(int argc, char* argv[]) {
	/*
	if (argc != 4) {
		std::cerr << "Usage: " << argv[0] << " <Delay> <ROM>\n";
		std::exit(EXIT_FAILURE);
	}
	
	int cycleDelay = std::stoi(argv[1]);
	char const* romFileName = argv[2];
	*/
	int cycleDelay = 4;

	MemoryController memoryController;
	Cpu cpu(&memoryController);

	memoryController.InsertCartridge("..//Roms//Tetris.gb");
	//memoryController.InsertCartridge(romFileName);

	auto lastCycleTime = std::chrono::high_resolution_clock::now();

	bool quit = false;
	while (!quit) {
		quit = false;

		// Get current time and calculate difference
		auto currentTime = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();

		// If difference is bigger than delay execute code
		if (dt > cycleDelay) {
			lastCycleTime = currentTime;
			cpu.cycle();
		}

	}
	return 0;
}