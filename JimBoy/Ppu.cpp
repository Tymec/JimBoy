#include "Ppu.h"
#include "MemoryController.h"

Ppu::Ppu(MemoryController *memoryController) {
	this->memoryController = memoryController;
}

Ppu::~Ppu() {
}

uint8_t Ppu::read(uint16_t address) {
	return memoryController->ppuRead(address);
}

void Ppu::write(uint16_t address, uint8_t value) {
	memoryController->ppuWrite(address, value);
}

void Ppu::Update(unsigned cycles) {
	switch(Mode) {
		case H_BLANK:
			break;
		case V_BLANK:
			break;
		case OAM_SEARCH:
			break;
		case TRANSFER:
			break;
	}
}

uint8_t Ppu::getRegister(ppuRegister reg) {
	return memoryController->ppuRead(reg);
}


