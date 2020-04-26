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

uint8_t Memory::readOam(uint8_t address) {
	return oam[address];
}

void Memory::writeOam(uint8_t address, uint8_t value) {
	oam[address] = value;
}

uint8_t Memory::readIo(uint8_t address) {
	return io[address];
}

void Memory::writeIo(uint8_t address, uint8_t value) {
	io[address] = value;
}

uint8_t Memory::readHRam(uint8_t address) {
	return hram[address];
}

void Memory::writeHRam(uint8_t address, uint8_t value) {
	hram[address] = value;
}

uint8_t Memory::getIe() {
	return hram[0x7F];
}

void Memory::setIe(uint8_t value) {
	hram[0x7F] = value;
}

uint8_t Memory::getIf() {
	return io[0x0F];
}

void Memory::setIf(uint8_t value) {
	io[0x0F] = value;
}

uint8_t Memory::getDiv() {
	return io[0x04];
}

void Memory::setDiv(uint8_t value) {
	io[0x04] = value;
}

uint8_t Memory::getTima() {
	return io[0x05];
}

void Memory::setTima(uint8_t value) {
	io[0x05] = value;
}

uint8_t Memory::getTma() {
	return io[0x06];
}

void Memory::setTma(uint8_t value) {
	io[0x06] = value;
}

uint8_t Memory::getTac() {
	return io[0x07];
}

void Memory::setTac(uint8_t value) {
	io[0x07] = value;
}

bool Memory::isTimerEnabled() {
	uint8_t tac = this->getTac();
	return (tac & 0x04) != 0;
}

uint8_t Memory::getTimerFreq() {
	uint8_t tac = this->getTac();
	return tac & 0x03;
}
