#pragma once
#include <iostream>
#include <fstream>
#include "Mbc.h"

struct CartridgeInfo {
	//std::string title;
	char title[16];
	size_t fileSize;
	uint8_t romType;
	uint8_t romSize;
	uint8_t ramSize;
	char newLicenseeCode[2];
	uint8_t oldLicenseeCode;
	uint8_t destinationCode;
	uint8_t versionNumber;
	bool sgbFlag;
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

	// Read & write from/to ROM Bank 00~N
	uint8_t readRom(uint16_t address);
	void writeRom(uint16_t address, uint8_t value);
	// Read & write from/to External RAM
	uint8_t readERam(uint16_t address);
	void writeERam(uint16_t address, uint8_t value);

	bool isLoaded();

	CartridgeInfo cartInfo{}; // CartridgeInfo
private:
	// Load ROM into ROM Bank
	void LoadRom(char const* filePath);
	// Get Header From Rom
	void GetHeader(char* buffer);

	friend class Debugger; // NOT SMART!!!
	MemoryController *memoryController; // Pointer to Bus
	std::unique_ptr<MBC> mbc;
	bool loaded = false;
};

