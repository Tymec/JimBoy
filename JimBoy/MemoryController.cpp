#include "MemoryController.h"

MemoryController::MemoryController() : memory(this) {
}

MemoryController::~MemoryController() {
}

void MemoryController::InsertCartridge(char const* file_path) {
	cartridge = new Cartridge(this, file_path);
}

uint8_t MemoryController::cpuRead(uint16_t address) {
	if (bootrom != nullptr && address < 0x100) {
		return bootrom[address];
	}

    if (address <= 0x3FFF) {
	 	return cartridge->readRom(address);
	} else if (address <= 0x7FFF) {
		return cartridge->readRom(address);
	} else if (address <= 0x9FFF) {
		return memory.readVRam(address & 0x1FFF);
	} else if (address <= 0xBFFF) {
		return cartridge->readERam(address & 0x1FFF);
	} else if (address <= 0xCFFF) {
		return memory.readWRam(address & 0x1FFF); // Replace with WRam0
	} else if (address <= 0xDFFF) {
		return memory.readWRam(address & 0x1FFF); // Replace with WRam1
	} else if (address <= 0xFDFF) {
		return memory.readWRam(address & 0x1FFF); // Replace with Wram0 and Wram1
	} else if (address <= 0xFE9F) {
		return memory.readOam(address & 0x9F);
	} else if (address <= 0xFEFF) {
		return 0x00;
	} else if (address <= 0xFF7F) {
		return memory.readIo(address & 0x7F);
	} else if (address <= 0xFFFF) {
		return memory.readHRam(address & 0x7F);
	}
	return 0x00;
}

void MemoryController::cpuWrite(uint16_t address, uint8_t value) {
	if (address <= 0x3FFF) {
		//cartridge->writeRom(address, value); // Replace with Rom0
	} else if (address <= 0x7FFF) {
		//cartridge->writeRom(address, value); // Replace with Rom1
	} else if (address <= 0x9FFF) {
		memory.writeVRam(address & 0x1FFF, value);
	} else if (address <= 0xBFFF) {
		cartridge->writeERam(address & 0x1FFF, value);
	} else if (address <= 0xCFFF) {
		memory.writeWRam(address & 0x1FFF, value); // Replace with WRam0
	} else if (address <= 0xDFFF) {
		memory.writeWRam(address & 0x1FFF, value); // Replace with WRam1
	} else if (address <= 0xFDFF) {
		memory.writeWRam(address & 0x1FFF, value); // Replace with Wram0 and Wram1
	} else if (address <= 0xFE9F) {
		memory.writeOam(address & 0x9F, value);
	} else if (address <= 0xFEFF) {
		return;
	} else if (address <= 0xFF7F) {
		memory.writeIo(address & 0x7F, value);
	} else if (address <= 0xFFFF) {
		memory.writeHRam(address & 0x7F, value);
	}
}

uint8_t MemoryController::ppuRead(uint16_t address) {
	return 0x00;
}

void MemoryController::ppuWrite(uint16_t address, uint8_t value) {

}

void MemoryController::insertBootrom(uint8_t* bootrom) {
	this->bootrom = bootrom;
}

bool MemoryController::isBootromEnabled() {
	return (cpuRead(0xFF50) & 0x1) == 0;
}