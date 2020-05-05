#include "MBC.h"

/// NoMBC

uint8_t NoMBC::readRom(uint16_t address) {
	return rom[address];
}

void NoMBC::writeRom(uint16_t address, uint8_t value) {
	std::cout << "Game tried to write value: 0x" << std::hex << (unsigned)value << std::dec << " to ROM at address: 0x" << std::hex << (unsigned)address << std::dec << std::endl;
	// Do nothing
}

uint8_t NoMBC::readRam(uint16_t address) {
	if (address > ramSize) {
		std::cout << "Invalid address: 0x" << std::hex << (unsigned)address << std::dec << std::endl;
		return 0x00;
	}
	return ram[address];
}

void NoMBC::writeRam(uint16_t address, uint8_t value) {
	if (address > ramSize) {
		std::cout << "Invalid address: 0x" << std::hex << (unsigned)address << std::dec << std::endl;
		return;
	}
	ram[address] = value;
}

/// MBC1

uint8_t MBC1::readRom(uint16_t address) {
	// ROM Bank 0
	if (address <= 0x3FFF)
		return rom[address];
	
	// ROM Bank 01-7F
	return rom[address + (0x4000 * romBank)];
}

void MBC1::writeRom(uint16_t address, uint8_t value) {
	if (address <= 0x1FFF) {
		ramEnable = (value & 0x0F) == 0xA;
	} else if (address <= 0x3FFF) {
		switch (value) {
			case 0x00:
				romBank = 1;
				break;
			case 0x20:
				romBank = 21;
				break;
			case 0x40:
				romBank = 41;
				break;
			case 0x60:
				romBank = 61;
				break;
			default:
				romBank = value & 0x1F;
		}
	} else if (address <= 0x5FFF) {
		if (romBankingMode) {
			romBank |= value << 5;
		} else {
			ramBank = value;
		}
	} else if (address <= 0x7FFF) {
		romBankingMode = value == 0x00;
	}
}

uint8_t MBC1::readRam(uint16_t address) {
	if (address > ramSize) {
		std::cout << "Invalid address: 0x" << std::hex << (unsigned)address << std::dec << std::endl;
		return 0x00;
	}
	return ram[address];
}

void MBC1::writeRam(uint16_t address, uint8_t value) {
	if (address > ramSize) {
		std::cout << "Invalid address: 0x" << std::hex << (unsigned)address << std::dec << std::endl;
		return;
	}
	ram[address] = value;
}

/// MBC2

uint8_t MBC2::readRom(uint16_t address) {
	return uint8_t();
}

void MBC2::writeRom(uint16_t address, uint8_t value) {
}

uint8_t MBC2::readRam(uint16_t address) {
	return uint8_t();
}

void MBC2::writeRam(uint16_t address, uint8_t value) {
}

/// MBC3

uint8_t MBC3::readRom(uint16_t address) {
	return uint8_t();
}

void MBC3::writeRom(uint16_t address, uint8_t value) {
}

uint8_t MBC3::readRam(uint16_t address) {
	return uint8_t();
}

void MBC3::writeRam(uint16_t address, uint8_t value) {
}

/// MBC5

uint8_t MBC5::readRom(uint16_t address) {
	return uint8_t();
}

void MBC5::writeRom(uint16_t address, uint8_t value) {
}

uint8_t MBC5::readRam(uint16_t address) {
	return uint8_t();
}

void MBC5::writeRam(uint16_t address, uint8_t value) {
}
