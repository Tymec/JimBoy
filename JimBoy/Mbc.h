#pragma once
#include <iostream>
#include <vector>

class MBC {
public:
	virtual ~MBC(){};

	virtual uint8_t readRom(uint16_t address) = 0;
	virtual void writeRom(uint16_t address, uint8_t value) = 0;

	virtual uint8_t readRam(uint16_t address) = 0;
	virtual void writeRam(uint16_t address, uint8_t value) = 0;
};

class NoMBC : public MBC {
public:
	NoMBC(uint8_t *rom, uint8_t *ram, size_t romSize, size_t ramSize, bool hasBattery = false) : 
		rom(rom), ram(ram), romSize(romSize), ramSize(ramSize), hasBattery(hasBattery) {};

	uint8_t readRom(uint16_t address);
	void writeRom(uint16_t address, uint8_t value);

	uint8_t readRam(uint16_t address);
	void writeRam(uint16_t address, uint8_t value);
private:
	bool hasBattery;

	size_t romSize, ramSize;
	uint8_t *rom, *ram;
};

class MBC1 : public MBC {
public:
	MBC1(uint8_t *rom, uint8_t *ram, size_t romSize, size_t ramSize, bool hasBattery = false) : 
		rom(rom), ram(ram), romSize(romSize), ramSize(ramSize), hasBattery(hasBattery) {};

	uint8_t readRom(uint16_t address);
	void writeRom(uint16_t address, uint8_t value);

	uint8_t readRam(uint16_t address);
	void writeRam(uint16_t address, uint8_t value);
private:
	bool hasBattery;

	size_t romSize, ramSize;
	uint8_t *rom, *ram;

	bool ramEnable = false;
	bool romBankingMode = true;
	uint8_t romBank = 1;
	uint8_t ramBank = 0;
};

class MBC2 : public MBC {
public:
	MBC2(uint8_t *rom, size_t romSize, bool hasBattery = false) : 
		rom(rom), romSize(romSize), hasBattery(hasBattery) {};

	uint8_t readRom(uint16_t address);
	void writeRom(uint16_t address, uint8_t value);

	uint8_t readRam(uint16_t address);
	void writeRam(uint16_t address, uint8_t value);
private:
	bool hasBattery;

	size_t romSize;
	uint8_t *rom;
	uint8_t ram[256]{};	// 512 * 4bits

	bool ramEnable = false;
	uint8_t romBank = 1;
};

class MBC3 : public MBC {
public:
	MBC3(uint8_t *rom, uint8_t *ram, size_t romSize, size_t ramSize, bool hasBattery = false, bool hasTimer = false) :
		rom(rom), ram(ram), romSize(romSize), ramSize(ramSize), hasBattery(hasBattery), hasTimer(hasTimer) {};

	uint8_t readRom(uint16_t address);
	void writeRom(uint16_t address, uint8_t value);

	uint8_t readRam(uint16_t address);
	void writeRam(uint16_t address, uint8_t value);
private:
	bool hasBattery;
	bool hasTimer;

	size_t romSize, ramSize;
	uint8_t *rom, *ram;
	//uint8_t rtcRegister{};

	bool ramEnable = false;
	uint8_t romBank = 1;
	uint8_t ramBank = 0;
};

// NOT IMPLEMENTED
class MBC5 : public MBC {
public:
	MBC5(uint8_t *rom, uint8_t *ram, size_t romSize, size_t ramSize, bool hasBattery = false, bool hasRumble = false) :
		rom(rom), ram(ram), romSize(romSize), ramSize(ramSize), hasBattery(hasBattery), hasRumble(hasRumble) {};

	uint8_t readRom(uint16_t address);
	void writeRom(uint16_t address, uint8_t value);

	uint8_t readRam(uint16_t address);
	void writeRam(uint16_t address, uint8_t value);
private:
	bool hasBattery;
	bool hasRumble;

	size_t romSize, ramSize;
	uint8_t *rom, *ram;

	bool ramEnable = false;
	uint16_t romBank = 0;
	uint8_t ramBank = 0;
};

// NOT IMPLEMENTED
class MBC6 : public MBC {
public:
	MBC6(uint8_t *rom, uint8_t *ram, size_t romSize, size_t ramSize) :
		rom(rom), ram(ram), romSize(romSize), ramSize(ramSize) {};

	uint8_t readRom(uint16_t address);
	void writeRom(uint16_t address, uint8_t value);

	uint8_t readRam(uint16_t address);
	void writeRam(uint16_t address, uint8_t value);
private:
	size_t romSize, ramSize;
	uint8_t *rom, *ram;
	std::vector<uint8_t> sram;
	std::vector<uint8_t> flash;

	bool ramEnable = false;
	bool flashEnable = false;
	bool flashWriteEnable = false;
	bool romSelectA = true;
	bool romSelectB = true;
	uint8_t romBankA = 1;
	uint8_t romBankB = 1;
	uint8_t ramBankA = 0;
	uint8_t ramBankB = 0;
};

// NOT IMPLEMENTED
class MBC7 : public MBC {
public:
	MBC7(uint8_t *rom, uint8_t *ram, size_t romSize, size_t ramSize) :
		rom(rom), ram(ram), romSize(romSize), ramSize(ramSize) {};

	uint8_t readRom(uint16_t address);
	void writeRom(uint16_t address, uint8_t value);

	uint8_t readRam(uint16_t address);
	void writeRam(uint16_t address, uint8_t value);
private:
	size_t romSize, ramSize;
	uint8_t *rom, *ram;
	std::vector<uint8_t> eeprom;

	//Accelerometer accelerometer;

	bool ramEnable = false;
	bool ramRegistersEnable = false;
	uint8_t romBank = 0;
};

// NOT IMPLEMENTED
class HuC1 : public MBC {
public:
	HuC1(uint8_t *rom, uint8_t *ram, size_t romSize, size_t ramSize) :
		rom(rom), ram(ram), romSize(romSize), ramSize(ramSize) {};

	uint8_t readRom(uint16_t address);
	void writeRom(uint16_t address, uint8_t value);

	uint8_t readRam(uint16_t address);
	void writeRam(uint16_t address, uint8_t value);
private:
	size_t romSize, ramSize;
	uint8_t *rom, *ram;
	uint8_t irRegister{};

	bool ramMode = false;
	uint8_t romBank = 1;
	uint8_t ramBank = 0;
	uint8_t irSelect = 0;
};

// NOT IMPLEMENTED
class HuC3 : public MBC {
public:
	HuC3(uint8_t *rom, uint8_t *ram, size_t romSize, size_t ramSize) :
		rom(rom), ram(ram), romSize(romSize), ramSize(ramSize) {};

	uint8_t readRom(uint16_t address);
	void writeRom(uint16_t address, uint8_t value);

	uint8_t readRam(uint16_t address);
	void writeRam(uint16_t address, uint8_t value);
private:
	size_t romSize, ramSize;
	uint8_t *rom, *ram;
};

// NOT IMPLEMENTED
class MMM01 : public MBC {
public:
	MMM01(uint8_t *rom, uint8_t *ram, size_t romSize, size_t ramSize, bool hasBattery = false) :
		rom(rom), ram(ram), romSize(romSize), ramSize(ramSize), hasBattery(hasBattery) {};

	uint8_t readRom(uint16_t address);
	void writeRom(uint16_t address, uint8_t value);

	uint8_t readRam(uint16_t address);
	void writeRam(uint16_t address, uint8_t value);
private:
	bool hasBattery;
	size_t romSize, ramSize;
	uint8_t *rom, *ram;
	
	bool ramEnable = false;
	bool romBankingMode = true;
	uint8_t romBank = 1;
	uint8_t ramBank = 0;
};
