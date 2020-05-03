#include "Interrupts.h"
#include "MemoryController.h"

Interrupts::Interrupts(MemoryController* memoryController) {
	this->memoryController = memoryController;
}

Interrupts::~Interrupts() {
}

bool Interrupts::getIeRegister(InterruptTypes i) {
	uint8_t reg = memoryController->cpuRead(0xFFFF);
	return (reg & i) != 0;
}

bool Interrupts::getIfRegister(InterruptTypes i) {
	uint8_t reg = memoryController->cpuRead(0xFF0F);
	return (reg & i) != 0;
}