#include "Cartridge.h"
#include "MemoryController.h"

Cartridge::Cartridge(MemoryController *memoryController, char const *filePath) {
	this->memoryController = memoryController;
	LoadRom(filePath);
}

Cartridge::~Cartridge() {
}

void Cartridge::LoadRom(char const *filePath) {
	// Open the file as binary and put cursor at the end
	std::ifstream file(filePath, std::ios::binary | std::ios::ate);

	if (file.is_open()) {
		// Get size of the file and allocate buffer with size
		cartInfo.fileSize = file.tellg();
		char *buffer = new char[cartInfo.fileSize];

		// Reset cursor position and fill file stream into buffer
		file.seekg(0, std::ios::beg);
		file.read(buffer, cartInfo.fileSize);
		file.close();

		// Get Header Information
		GetHeader(buffer);

		// Fill ROM with buffer
		uint8_t *rom = new uint8_t[cartInfo.fileSize];
		for (size_t i = 0; i < cartInfo.fileSize; i++) {
			rom[i] = buffer[i];
		}

		// Determine RAM size and create RAM array
		size_t _ramSize;
		switch (cartInfo.ramSize) {
			case 0x01:	_ramSize = 0x00800;	break;	// 2KB
			case 0x02:	_ramSize = 0x02000;	break;	// 8KB
			case 0x03:	_ramSize = 0x08000;	break;	// 32KB
			case 0x04:	_ramSize = 0x20000;	break;	// 128KB
			case 0x05:	_ramSize = 0x10000;	break;	// 64KB
			default:	_ramSize = 0x00001;	break;	// None
		}
		uint8_t *ram = new uint8_t[_ramSize];

		// Create MBC according to cartridge type
		switch (cartInfo.romType) {
			case 0x00:	mbc.reset(new NoMBC(rom, ram, cartInfo.fileSize, _ramSize));						break;	// NoMBC
			case 0x01:	mbc.reset(new MBC1(rom, ram, cartInfo.fileSize, _ramSize));							break;	// MBC1
			case 0x02:	mbc.reset(new MBC1(rom, ram, cartInfo.fileSize, _ramSize));							break;	// MBC1+RAM
			case 0x03:	mbc.reset(new MBC1(rom, ram, cartInfo.fileSize, _ramSize, true));					break;	// MBC1+RAM+BATTERY
			case 0x05:	mbc.reset(new MBC2(rom, cartInfo.fileSize));										break;	// MBC2
			case 0x06:	mbc.reset(new MBC2(rom, cartInfo.fileSize, true));									break;	// MBC2+BATTERY
			case 0x08:	mbc.reset(new NoMBC(rom, ram, cartInfo.fileSize, _ramSize));						break;	// ROM+RAM
			case 0x09:	mbc.reset(new NoMBC(rom, ram, cartInfo.fileSize, _ramSize, true));					break;	// ROM+RAM+BATTERY
			case 0x0B:	std::cout << "MBC Error: MMM01 has not been implemented yet" << std::endl;			break;	// MMM01
			case 0x0C:	std::cout << "MBC Error: MMM01 has not been implemented yet" << std::endl;			break;	// MMM01+RAM
			case 0x0D:	std::cout << "MBC Error: MMM01 has not been implemented yet" << std::endl;			break;	// MMM01+RAM+BATTERY
			case 0x0F:	mbc.reset(new MBC3(rom, ram, cartInfo.fileSize, _ramSize, true, true));				break;	// MBC3+TIMER+BATTERY
			case 0x10:	mbc.reset(new MBC3(rom, ram, cartInfo.fileSize, _ramSize, true, true));				break;	// MBC3+TIMER+RAM+BATTERY
			case 0x11:	mbc.reset(new MBC3(rom, ram, cartInfo.fileSize, _ramSize));							break;	// MBC3
			case 0x12:	mbc.reset(new MBC3(rom, ram, cartInfo.fileSize, _ramSize));							break;	// MBC3+RAM
			case 0x13:	mbc.reset(new MBC3(rom, ram, cartInfo.fileSize, _ramSize, true));					break;	// MBC3+RAM+BATTERY
			case 0x19:	mbc.reset(new MBC5(rom, ram, cartInfo.fileSize, _ramSize));							break;	// MBC5
			case 0x1A:	mbc.reset(new MBC5(rom, ram, cartInfo.fileSize, _ramSize));							break;	// MBC5+RAM
			case 0x1B:	mbc.reset(new MBC5(rom, ram, cartInfo.fileSize, _ramSize, true));					break;	// MBC5+RAM+BATTERY
			case 0x1C:	mbc.reset(new MBC5(rom, ram, cartInfo.fileSize, _ramSize, false, true));			break;	// MBC5+RUMBLE
			case 0x1D:	mbc.reset(new MBC5(rom, ram, cartInfo.fileSize, _ramSize, false, true));			break;	// MBC5+RUMBLE+RAM
			case 0x1E:	mbc.reset(new MBC5(rom, ram, cartInfo.fileSize, _ramSize, true, true));				break;	// MBC5+RUMBLE+RAM+BATTERY
			case 0x20:	std::cout << "MBC Error: MBC6 has not been implemented yet" << std::endl;			break;	// MBC6
			case 0x22:	std::cout << "MBC Error: MBC7 has not been implemented yet" << std::endl;			break;	// MBC7+SENSOR+RUMBLE+RAM+BATTERY
			case 0xFC:	std::cout << "MBC Error: Pocket Camera has not been implemented yet" << std::endl;	break;	// POCKET CAMERA
			case 0xFD:	std::cout << "MBC Error: Bandai Tama5 has not been implemented yet" << std::endl;	break;	// BANDAI TAMA5
			case 0xFE:	std::cout << "MBC Error: HuC3 has not been implemented yet" << std::endl;			break;	// HuC3
			case 0xFF:	std::cout << "MBC Error: HuC1 has not been implemented yet" << std::endl;			break;	// HuC1+RAM+BATTERY
			default:	mbc.reset(new NoMBC(rom, ram, cartInfo.fileSize, _ramSize));						break;	// NoMBC
		}
		// Clear the buffer
		delete[] buffer;

		loaded = true;
		return;
	}
	std::cout << "Failed to open file." << std::endl;
	loaded = false;
}

void Cartridge::GetHeader(char* buffer) {
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
	cartInfo.sgbFlag = buffer[0x146] != 0;

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
		if (i == 0x014E || i == 0x014F)
			continue;
		cartInfo.calculatedGlobalChecksum = (cartInfo.calculatedGlobalChecksum + (uint8_t)buffer[i]);
	}
}

uint8_t Cartridge::readRom(uint16_t address) {
	return mbc->readRom(address);
}

void Cartridge::writeRom(uint16_t address, uint8_t value) {
	mbc->writeRom(address, value);
}

uint8_t Cartridge::readERam(uint16_t address) {
	return mbc->readRam(address);
}

void Cartridge::writeERam(uint16_t address, uint8_t value) {
	mbc->writeRam(address, value);
}

bool Cartridge::isLoaded() {
	return loaded;
}
