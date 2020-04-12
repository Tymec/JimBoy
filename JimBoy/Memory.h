#pragma once
#include <iostream>

const uint16_t VIDEO_RAM_SIZE = 8192; // Size of Video RAM
const uint8_t OAM_SIZE = 160; // Size of OAM
const uint8_t IO_REGISTERS_SIZE = 128; // Size of I/O Registers
const uint8_t HIGH_RAM_SIZE = 127; // Size of High RAM
const uint16_t WORK_RAM_SIZE = 8192; // Size of Work RAM

class MemoryController;
class Memory {
public:
	Memory(MemoryController *memoryController);
	~Memory();

	// Read value from VRAM at address
	uint8_t readVRam(uint16_t address);
	// Write value to VRAM at address
	void writeVRam(uint16_t address, uint8_t value);
	// Read value from OAM at address
	uint8_t readOam(uint16_t address);
	// Write value to OAM at address
	void writeOam(uint16_t address, uint8_t value);
	// Read value from IO at address
	uint8_t readIo(uint16_t address);
	// Write value to IO at address
	void writeIo(uint16_t address, uint8_t value);
	// Read value from HRAM at address
	uint8_t readHRam(uint16_t address);
	// Write value to HRAM at address
	void writeHRam(uint16_t address, uint8_t value);
	// Read value from WRAM at address
	uint8_t readWRam(uint16_t address);
	// Write value to WRAM at address
	void writeWRam(uint16_t address, uint8_t value);
	// Read IE value
	uint8_t readIe();
	// Write value to IE
	void writeIe(uint8_t value);
private:
	MemoryController *memoryController; // Pointer to Bus
	
	uint8_t vram[VIDEO_RAM_SIZE]{}; // 8KB Video RAM
	uint8_t oam[OAM_SIZE]{}; // 160B Sprite Attribute Table
	uint8_t io[IO_REGISTERS_SIZE]{}; // 128B I/O Registers
	uint8_t hram[HIGH_RAM_SIZE]{}; // 127B High RAM
	uint8_t wram[WORK_RAM_SIZE]{}; // 4KB+4KB Work RAM
	uint8_t ie{}; // Interrupts Enable Register
};

