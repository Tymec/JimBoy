#pragma once
#include <iostream>
#include <fstream>

const uint16_t ROM_BANK_SIZE = 32768; // Size of ROM Bank
const uint16_t EXTERNAL_RAM_SIZE = 8192; // Size of External RAM
const uint16_t ROM_BANK_1_OFFSET = 0x4000; // Offset for ROM Bank 1

struct CartridgeInfo {
	char title[16];
	long fileSize;
	uint8_t romType;
	uint8_t romSize;
	uint8_t ramSize;
	char newLicenseeCode[2];
	uint8_t oldLicenseeCode;
	uint8_t destinationCode;
	uint8_t versionNumber;
	uint8_t sgbFlag;
	uint8_t checksum;
	uint8_t calculatedChecksum;
	uint16_t globalChecksum;
	uint16_t calculatedGlobalChecksum;

	void outputInformation() {
		std::cout
			<< "Title: " << title << std::endl
			<< "File Size: " << fileSize << std::endl
			<< "ROM Type: " << std::hex << (unsigned)romType << std::dec << std::endl
			<< "ROM Size: " << std::hex << (unsigned)romSize << std::dec << std::endl
			<< "RAM Size: " << std::hex << (unsigned)ramSize << std::dec << std::endl
			<< "New Licensee Code: " << newLicenseeCode << std::endl
			<< "Old Licensee Code: " << std::hex << (unsigned)oldLicenseeCode << std::dec << std::endl
			<< "Destination Code: " << std::hex << (unsigned)destinationCode << std::dec << std::endl
			<< "Version Number: " << std::hex << (unsigned)versionNumber << std::dec << std::endl
			<< "SGB Flag: " << std::hex << (unsigned)sgbFlag << std::dec << std::endl
			<< "Checksum: " << (unsigned)checksum << std::endl
			<< "Calculated Checksum: " << (unsigned)calculatedChecksum << std::endl
			<< "Checksum checks out: " << std::boolalpha << (checksum == calculatedChecksum) << std::noboolalpha << std::endl
			<< "Global Checksum: " << (unsigned)globalChecksum << std::endl
			<< "Calculated Global Checksum: " << (unsigned)calculatedGlobalChecksum << std::endl
			<< "Global Checksum checks out: " << std::boolalpha << (globalChecksum == calculatedGlobalChecksum) << std::noboolalpha << std::endl;
	};
};

class MemoryController;
class Cartridge {
public:
	Cartridge(MemoryController *memoryController, char const* filePath);
	~Cartridge();

	// Read value from ROM Bank 0 at address
	uint8_t readRom(uint16_t address);
	// Write value to ROM Bank 0 at address
	void writeRom(uint16_t address, uint8_t value);
	// Read value from External RAM at address
	uint8_t readERam(uint16_t address);
	// Write value to External RAM at address
	void writeERam(uint16_t address, uint8_t value);
    
    bool is_bootrom_enabled = true; // Is Boot ROM Enabled
private:
	// Load ROM into ROM Bank
	void LoadRom(char const* filePath);
	// Get Header From Rom
	void GetHeader(char* buffer);

	MemoryController *memoryController; // Pointer to Bus
	CartridgeInfo cartInfo{}; // CartridgeInfo

	uint8_t rom[0x8000]{}; // 32KB ROM Bank
    uint8_t bootrom[0x4000]{}; // 16KB Boot ROM
	uint8_t eram[EXTERNAL_RAM_SIZE]{}; // 8KB External RAM
};

