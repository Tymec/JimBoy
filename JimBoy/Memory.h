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

	// Read value from VRAM at address
	uint8_t readVRam(uint16_t address);
	// Write value to VRAM at address
	void writeVRam(uint16_t address, uint8_t value);
	// Read value from OAM at address
	uint8_t readOam(uint8_t address);
	// Write value to OAM at address
	void writeOam(uint8_t address, uint8_t value);
	// Read value from IO at address
	uint8_t readIo(uint8_t address);
	// Write value to IO at address
	void writeIo(uint8_t address, uint8_t value);
	// Read value from HRAM at address
	uint8_t readHRam(uint8_t address);
	// Write value to HRAM at address
	void writeHRam(uint8_t address, uint8_t value);
	// Read value from WRAM at address
	uint8_t readWRam(uint16_t address);
	// Write value to WRAM at address
	void writeWRam(uint16_t address, uint8_t value);

	// Get IE
	uint8_t getIe();
	// Set IE
	void setIe(uint8_t value);
	// Get IF
	uint8_t getIf();
	// Set IF
	void setIf(uint8_t value);

	// Get DIV
	uint8_t getDiv();
	// Set DIV
	void setDiv(uint8_t value);
	// Get TIMA
	uint8_t getTima();
	// Set TIMA
	void setTima(uint8_t value);
	// Get TMA
	uint8_t getTma();
	// Set TMA
	void setTma(uint8_t value);
	// Get TAC
	uint8_t getTac();
	// Set TAC
	void setTac(uint8_t value);
	// Check if timer is enabled
	bool isTimerEnabled();
	// Get timer frequency
	uint8_t getTimerFreq();

	// Get JOYP
	uint8_t getJoyp();
	// Set JOYP
	void setJoyp(uint8_t value);

private:
	MemoryController *memoryController; // Pointer to Bus
	
	uint8_t vram[VIDEO_RAM_SIZE]{};	// 8KB Video RAM
	uint8_t oam[OAM_SIZE]{}; // 160B Sprite Attribute Table
	uint8_t io[IO_REGISTERS_SIZE]{}; // 128B I/O Registers
	uint8_t hram[HIGH_RAM_SIZE]{}; // 127B High RAM
	uint8_t wram[WORK_RAM_SIZE]{}; // 4KB+4KB Work RAM

	// Interrupt Registers
	uint8_t ie{}; // Interrupt Enable Register
	uint8_t ir{}; // Interrupt Request Flag Register

	uint8_t joyp{}; // Joypad Register
};

