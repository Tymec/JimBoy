#include "Memory.h"
#include "MemoryController.h"

Memory::Memory(MemoryController *memoryController) {
	this->memoryController = memoryController;
}

Memory::~Memory() {
}

uint8_t Memory::readVRam(uint16_t address) {
	return vram[address];
}

void Memory::writeVRam(uint16_t address, uint8_t value) {
	vram[address] = value;
}

uint8_t Memory::readWRam(uint16_t address) {
	return wram[address];
}

void Memory::writeWRam(uint16_t address, uint8_t value) {
	wram[address] = value;
}

uint8_t Memory::readOam(uint16_t address) {
	return oam[address];
}

void Memory::writeOam(uint16_t address, uint8_t value) {
	oam[address] = value;
}

uint8_t Memory::readIo(uint16_t address) {
	return io[address];
}

void Memory::writeIo(uint16_t address, uint8_t value) {
	io[address] = value;
}

uint8_t Memory::readHRam(uint16_t address) {
	return hram[address];
}

void Memory::writeHRam(uint16_t address, uint8_t value) {
	hram[address] = value;
}

uint8_t Memory::readIe() {
	return ie;
}

void Memory::writeIe(uint8_t value) {
	ie = value;
}