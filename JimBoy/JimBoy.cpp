#include "JimBoy.h"

JimBoy::JimBoy() : memoryController(), cpu(&memoryController), interrupts(&memoryController), ppu(&memoryController), debugger(&cpu, &ppu, &interrupts, &memoryController) {
}

JimBoy::~JimBoy() {
}

void JimBoy::Start(char const* rom_path) {
	memoryController.InsertCartridge(rom_path);

	std::chrono::time_point<std::chrono::high_resolution_clock> current, previous;
	previous = std::chrono::high_resolution_clock::now();

	bool quit = false;
	while (!quit) {
		previous = std::chrono::high_resolution_clock::now();

		unsigned cycles = 0;
		while (cycles < MAX_CYCLES && !quit) {
			cycles += cpu.Cycle();

			debugger.Update(cycles);
			if (debugger.quit)
				quit = true;
		}

		current = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::duration<float, std::milli>> (current - previous);

		float DELAY_TIME = 1000.0f / 59.9f;
		if (elapsed.count() < DELAY_TIME) {
			std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(DELAY_TIME - elapsed.count()));
		}
	}
	SDL_Quit();
}

void JimBoy::LoadBootRom(char const* bootrom_path) {
	if (bootrom_path == "none") {
		cpu.sp = 0xFFFE;
		cpu.pc = 0x100;
		cpu.registers.setAF(0x01B0);
		cpu.registers.setBC(0x0013);
		cpu.registers.setDE(0x00D8);
		cpu.registers.setHL(0x014D);
		memoryController.cpuWrite(0xFF05, 0x00);	// TIMA
		memoryController.cpuWrite(0xFF06, 0x00);	// TMA
		memoryController.cpuWrite(0xFF07, 0x00);	// TAC
		memoryController.cpuWrite(0xFF10, 0x80);	// NR10
		memoryController.cpuWrite(0xFF11, 0xBF);	// NR11
		memoryController.cpuWrite(0xFF12, 0xF3);	// NR12
		memoryController.cpuWrite(0xFF14, 0xBF);	// NR14
		memoryController.cpuWrite(0xFF16, 0x3F);	// NR21
		memoryController.cpuWrite(0xFF17, 0x00);	// NR22
		memoryController.cpuWrite(0xFF19, 0xBF);	// NR24
		memoryController.cpuWrite(0xFF1A, 0x7F);	// NR30
		memoryController.cpuWrite(0xFF1B, 0xFF);	// NR31
		memoryController.cpuWrite(0xFF1C, 0x9F);	// NR32
		memoryController.cpuWrite(0xFF1E, 0xBF);	// NR33
		memoryController.cpuWrite(0xFF20, 0xFF);	// NR41
		memoryController.cpuWrite(0xFF21, 0x00);	// NR42
		memoryController.cpuWrite(0xFF22, 0x00);	// NR43
		memoryController.cpuWrite(0xFF23, 0xBF);	// NR44
		memoryController.cpuWrite(0xFF24, 0x77);	// NR50
		memoryController.cpuWrite(0xFF25, 0xF3);	// NR51
		memoryController.cpuWrite(0xFF26, 0xF1);	// NR52 0xF1-GB, 0xF0-SGB
		memoryController.cpuWrite(0xFF40, 0x91);	// LCDC
		memoryController.cpuWrite(0xFF42, 0x00);	// SCY
		memoryController.cpuWrite(0xFF43, 0x00);	// SCX
		memoryController.cpuWrite(0xFF45, 0x00);	// LYC
		memoryController.cpuWrite(0xFF47, 0xFC);	// BGP
		memoryController.cpuWrite(0xFF48, 0xFF);	// OBP0
		memoryController.cpuWrite(0xFF49, 0xFF);	// OBP1
		memoryController.cpuWrite(0xFF50, 0x01);	// BOOT_OFF
		memoryController.cpuWrite(0xFF4A, 0x00);	// WY
		memoryController.cpuWrite(0xFF4B, 0x00);	// WX
		memoryController.cpuWrite(0xFFFF, 0x00);	// IE

		for (uint16_t i = 0; i <= 0x2000; i++) {
			memoryController.cpuWrite(0xCFFF + i, 0x00);
		}

		return;
	} 
	else {
		// Open the file as binary and put cursor at the end
		std::ifstream file(bootrom_path, std::ios::binary | std::ios::ate);

		if (file.is_open()) {
			// Get size of the file and allocate buffer with size
			std::streampos file_size = file.tellg();
			char* buffer = new char[file_size];

			// Reset cursor position and fill file stream into buffer
			file.seekg(0, std::ios::beg);
			file.read(buffer, file_size);
			file.close();

			// Load the ROM into memory
			for (uint16_t i = 0; i < file_size; i++) {
				bootrom[i] = buffer[i];
			}

			// Clear the buffer
			delete[] buffer;

			memoryController.insertBootrom(bootrom);
			return;
		}
		std::cout << "Failed to open file." << std::endl;
		LoadBootRom("none");
	}
}