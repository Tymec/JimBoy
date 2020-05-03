#include "Timer.h"
#include "MemoryController.h"

Timer::Timer(MemoryController* memoryController) {
	this->memoryController = memoryController;
}

Timer::~Timer() {
}

uint8_t Timer::getRegister(timerRegisters reg) {
	return memoryController->cpuRead(reg);
}
