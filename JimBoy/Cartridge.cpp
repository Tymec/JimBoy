#include "Cartridge.h"
#include "MemoryController.h"

Cartridge::Cartridge(MemoryController *memoryController, char const* filePath) {
	this->memoryController = memoryController;
	LoadRom(filePath);
}

Cartridge::~Cartridge() {
}

void Cartridge::LoadRom(char const* filePath) {
	// Open the file as binary and put cursor at the end
	std::ifstream file(filePath, std::ios::binary | std::ios::ate);

	if (file.is_open()) {
		// Get size of the file and allocate buffer with size
		cartInfo.fileSize = file.tellg();
		char* buffer = new char[cartInfo.fileSize];

		// Reset cursor position and fill file stream into buffer
		file.seekg(0, std::ios::beg);
		file.read(buffer, cartInfo.fileSize);
		file.close();

		// Get Header Information
		GetHeader(buffer);

		// Load the ROM into memory
		for (uint16_t i = 0; i < cartInfo.fileSize; i++) {
			writeRom(i, buffer[i]); // Implement MBC
		}

		// Clear the buffer
		delete[] buffer;

		return;
	}
	std::cout << "Failed to open file." << std::endl;
}

void Cartridge::GetHeader(char* buffer) {
	// Entry Point **
	uint8_t headerEntryPoint[4];
	for (uint8_t i = 0; i < 4; i++) {
		headerEntryPoint[i] = buffer[0x100 + i];
	}

	// Nintendo Logo
	/*
	uint8_t n_logo[30];
	for (uint8_t i = 0; i < 30; i++) {
		n_logo[i] = buffer[0x104 + i];
	}
	std::cout << "Nintendo Logo: " << std::hex << (unsigned)n_logo << std::dec << std::endl;
	*/

	//Title
	for (uint8_t i = 0; i < 16; i++) {
		cartInfo.title[i] = (char)buffer[0x134 + i];
	}

	// New Licensee Code
	cartInfo.newLicenseeCode[0] = buffer[0x144];
	cartInfo.newLicenseeCode[1] = buffer[0x145];

	// SGB Flag
	cartInfo.sgbFlag = buffer[0x146];

	// Cartridge Type
	cartInfo.romType = buffer[0x147];

	// ROM Size
	cartInfo.romSize = buffer[0x148];

	// RAM Size
	cartInfo.ramSize = buffer[0x149];

	// Destination Code
	cartInfo.destinationCode = buffer[0x014A];

	// Old Licensee Code
	cartInfo.oldLicenseeCode = buffer[0x014B];

	// Mask ROM Version Number
	cartInfo.versionNumber = buffer[0x014C];

	// Header Checksum
	cartInfo.checksum = buffer[0x014D];
	cartInfo.calculatedChecksum = 0;
	for (int i = 0x0134; i < 0x014C + 1; i++) {
		cartInfo.calculatedChecksum = (cartInfo.calculatedChecksum - (uint8_t)buffer[i]) - 1;
	}

	// Global Checksum
	cartInfo.globalChecksum = ((uint8_t)buffer[0x014E] << 8) | (uint8_t)buffer[0x014F];
	cartInfo.calculatedGlobalChecksum = 0;
	for (int i = 0; i < cartInfo.fileSize; i++) {
		if (i == 0x014E) {
			continue;
		} else if (i == 0x014F) {
			continue;
		}
		cartInfo.calculatedGlobalChecksum = (cartInfo.calculatedGlobalChecksum + (uint8_t)buffer[i]);
	}

	// DEBUG
	//cartInfo.outputInformation();
}

uint8_t Cartridge::readRom(uint16_t address) {
	return rom[address];
}

void Cartridge::writeRom(uint16_t address, uint8_t value) {
	rom[address] = value;
}

uint8_t Cartridge::readERam(uint16_t address) {
	return eram[address];
}

void Cartridge::writeERam(uint16_t address, uint8_t value) {
	eram[address] = value;
}