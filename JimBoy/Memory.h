#pragma once
#include <iostream>

const uint16_t VIDEO_RAM_SIZE = 0x2000; // Size of Video RAM
const uint8_t OAM_SIZE = 0xA0; // Size of OAM
const uint8_t IO_REGISTERS_SIZE = 0x80; // Size of I/O Registers
const uint8_t HIGH_RAM_SIZE = 0x80; // Size of High RAM
const uint16_t WORK_RAM_SIZE = 0x2000; // Size of Work RAM

class MemoryController;
class Memory {
public:
	Memory(MemoryController *memoryController);
	~Memory();

	// Read & Write value from/to VRAM
	uint8_t readVRam(uint16_t address);
	void writeVRam(uint16_t address, uint8_t value);
	// Read & Write value from/to OAM
	uint8_t readOam(uint8_t address);
	void writeOam(uint8_t address, uint8_t value);
	// Read & Write value from/to IO
	uint8_t readIo(uint8_t address);
	void writeIo(uint8_t address, uint8_t value);
	// Read & Write value from/to HRAM
	uint8_t readHRam(uint8_t address);
	void writeHRam(uint8_t address, uint8_t value);
	// Read & Write value from/to WRAM
	uint8_t readWRam(uint16_t address);
	void writeWRam(uint16_t address, uint8_t value);
private:
	MemoryController *memoryController; // Pointer to Bus
	
	uint8_t vram[VIDEO_RAM_SIZE]{};	// 8KB Video RAM
	uint8_t oam[OAM_SIZE]{}; // 160B Sprite Attribute Table
	uint8_t io[IO_REGISTERS_SIZE]{}; // 128B I/O Registers
	uint8_t hram[HIGH_RAM_SIZE]{}; // 127B High RAM
	uint8_t wram[WORK_RAM_SIZE]{}; // 4KB+4KB Work RAM
};

