#include "Interrupts.h"
#include "MemoryController.h"

Interrupts::Interrupts(MemoryController* memoryController) {
	this->memoryController = memoryController;
}

Interrupts::~Interrupts() {
}

void Interrupts::setInterrupt(InterruptTypes i, bool v) {
	uint8_t reg = memoryController->cpuRead(0xFF0F);
	uint8_t val = v ? (reg | i) : (reg & ~i);
	memoryController->cpuWrite(0xFF0F, val);
}

uint8_t Interrupts::handleInterrupts() {
	uint8_t fired = getInterruptFlag() & getInterruptEnable();

	if (fired != 0) {
		if (testInterrupt(VBLANK, fired))
			return VBLANK_VECTOR;
		else if (testInterrupt(LCD, fired))
			return LCD_VECTOR;
		else if (testInterrupt(TIMER, fired))
			return TIMER_VECTOR;
		else if (testInterrupt(SERIAL, fired))
			return SERIAL_VECTOR;
		else if (testInterrupt(JOYPAD, fired))
			return JOYPAD_VECTOR;
		else {
			std::cout << "Interrupts broke" << std::endl;
		}

	}
	return 0;
}

void Interrupts::requestInterrupt(InterruptTypes i) {
	uint8_t reg = memoryController->cpuRead(0xFF0F);
	reg |= i;
	memoryController->cpuWrite(0xFF0F, reg);
}

bool Interrupts::testInterrupt(InterruptTypes i, uint8_t fired) {
	bool handled = false;

	if ((fired & i) != 0) {
		handled = true;
		setInterrupt(i, false);
	}

	return handled;
}

uint8_t Interrupts::getInterruptEnable() {
	return memoryController->cpuRead(0xFFFF);
}

uint8_t Interrupts::getInterruptFlag() {
	return memoryController->cpuRead(0xFF0F);
}

bool Interrupts::getIe(InterruptTypes i) {
	uint8_t reg = getInterruptEnable();
	return (reg & i) != 0;
}

bool Interrupts::getIf(InterruptTypes i) {
	uint8_t reg = getInterruptFlag();
	return (reg & i) != 0;
}