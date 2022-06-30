#include "Debugger.h"
#include "Types.h"
#include "imgui_memory_editor.h"

unsigned WIDTH	= 1000;
unsigned HEIGHT = 1000;

Debugger::Debugger(Cpu* cpu, Ppu* ppu, MemoryController* memoryController) {
	this->cpu = cpu;
	this->ppu = ppu;
	this->memoryController = memoryController;

	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "Could not initialize SDL2: " << SDL_GetError() << std::endl;
		return;
	}

	window = SDL_CreateWindow("JimBoy - Debugger", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);
	if (window == NULL) {
		std::cout << "Could not create window: " << SDL_GetError() << std::endl;
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		std::cout << "Could not create renderer: " << SDL_GetError() << std::endl;
		return;
	}

	ImGui::CreateContext();
	ImGuiSDL::Initialize(renderer, WIDTH, HEIGHT);
}

Debugger::~Debugger() {
	ImGuiSDL::Deinitialize();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	ImGui::DestroyContext();
}

void Debugger::Update(unsigned cycles) {
	if (mode == IDLE || mode == STEP) {
		Idle(cycles);
	} else {
		if (std::find(breakpoints.begin(), breakpoints.end(), cpu->pc) != breakpoints.end()) {
			mode = IDLE;
			Idle(cycles);
		}

		if (animate)
			RenderWindows(cycles);
	}
}

void Debugger::Idle(unsigned cycles) {
	std::chrono::time_point<std::chrono::high_resolution_clock> current, previous;
	previous = std::chrono::high_resolution_clock::now();

	mode = IDLE;
	while (mode == IDLE && !quit) {
		current = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::duration<float, std::milli>> (current - previous);
		previous = current;

		RenderWindows(cycles);

		static float DELAY_TIME = 1000.0f / 59.9f;
		if (elapsed.count() < DELAY_TIME) {
			std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(DELAY_TIME - elapsed.count()));
		}
	}
}

void Debugger::HandleInput() {
	SDL_Event event;
	ImGuiIO& io = ImGui::GetIO();
	while(SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				ExitDebugger();
			}
			io.KeysDown[event.key.keysym.scancode] = true;

		}

		if (event.type == SDL_KEYUP) {
			io.KeysDown[event.key.keysym.scancode] = false;
		}

		if (SDL_TEXTINPUT) {
            io.AddInputCharactersUTF8(event.text.text);
        }

		if (event.type == SDL_QUIT) {
			ExitDebugger();
		} else if (event.type == SDL_WINDOWEVENT) {
			if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
				io.DisplaySize.x = static_cast<float>(event.window.data1);
				io.DisplaySize.y = static_cast<float>(event.window.data2);
			}
		}
	}
	
	int mouseX, mouseY;
	const int buttons = SDL_GetMouseState(&mouseX, &mouseY);

	io.DeltaTime = 1.0f / 60.0f;
	io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
	io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
	io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);

	io.KeyMap[ImGuiKey_Enter] = SDL_SCANCODE_RETURN;
	io.KeyMap[ImGuiKey_Backspace] = SDL_SCANCODE_BACKSPACE;
}

void Debugger::RenderWindows(unsigned cycles) {
	HandleInput();

	float frame_rate = ImGui::GetIO().Framerate;
	float frame_time = 1000.0f / frame_rate;
	char title_buffer[128];
	snprintf(title_buffer, sizeof(title_buffer), "JimBoy - %.3f ms/frame (%.1f FPS)", frame_rate, frame_time);
	SDL_SetWindowTitle(window, title_buffer);

	ImGui::NewFrame();
	SDL_RenderClear(renderer);

	ImGui::SetNextWindowPos({20, 50});
	ImGui::SetNextWindowSize({450, 530});
	CreateCpuWindow(cycles);

	ImGui::SetNextWindowPos({500, 50});
	ImGui::SetNextWindowSize({500, 220});
	CreateCartridgeWindow();

	ImGui::SetNextWindowPos({500, 300});
	ImGui::SetNextWindowSize({500, 220});
	CreatePpuWindow();

	//ImGui::SetNextWindowPos({1100, 50});
	//CreateMemoryWindow();

	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	SDL_RenderPresent(renderer);
}

void Debugger::CreateCpuWindow(unsigned cycles) {
	ImGui::Begin("CPU");

	if (ImGui::Button("Start")) {
		mode = RUNNING;
	}
	ImGui::SameLine();
	if (ImGui::Button("Pause")) {
		mode = IDLE;
	}
	ImGui::SameLine();
	if (ImGui::Button("Step")) {
		mode = STEP;
	}
	ImGui::SameLine();
	ImGui::Checkbox("Animate", &animate);
	
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Breakpoint");
	ImGui::SameLine();
	char input_breakpoint[64];
	memset(input_breakpoint, 0, sizeof(char) * 64);
	if (ImGui::InputText("##hidelabel", input_breakpoint, 64, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_EnterReturnsTrue)) {
		int addr;
		if (sscanf_s(input_breakpoint, "%X", &addr)) {
			uint16_t address = addr & 0xFFFF;
			breakpoints.push_back(address);
		}
	}
	ImGui::Separator();

	ImGui::Columns(2, "instruction");
	uint8_t opcode = cpu->read(cpu->pc);
	bool prefix = false;
	if (opcode == 0xCB) {
		opcode = cpu->read(cpu->pc + 1);
		prefix = true;
		ImGui::Text("Opcode: 0xCB%02X", opcode);
	} else {
		ImGui::Text("Opcode: 0x%02X", opcode);
	}
	ImGui::Text("Instruction: %s", getInstruction(opcode, prefix).c_str());
	ImGui::NextColumn();
	ImGui::Text("Cycles: %d", cycles);
	ImGui::NextColumn();
	ImGui::Columns(1);
	ImGui::Separator();
	ImGui::Text("Registers:");
	ImGui::Separator();
	ImGui::Columns(2, "registers");
	ImGui::Text("AF: 0x%04X", cpu->registers.getAF());
	ImGui::Text("DE: 0x%04X", cpu->registers.getDE());
	ImGui::Text("PC: 0x%04X", cpu->pc);
	ImGui::NextColumn();
	ImGui::Text("BC: 0x%04X", cpu->registers.getBC());
	ImGui::Text("HL: 0x%04X", cpu->registers.getHL());
	ImGui::Text("SP: 0x%04X", cpu->sp);
	ImGui::NextColumn();
	ImGui::Columns(1);
	ImGui::Separator();

	ImGui::Text("Flags:");
	ImGui::Separator();
	ImGui::Columns(4, "flags");
	ImGui::Text("Zero");
	ImGui::Text(cpu->registers.getFlag(ZERO) ? "1" : "0");
	ImGui::NextColumn();
	ImGui::Text("Sub");
	ImGui::Text(cpu->registers.getFlag(SUB) ? "1" : "0");
	ImGui::NextColumn();
	ImGui::Text("Half Carry");
	ImGui::Text(cpu->registers.getFlag(HALF) ? "1" : "0");
	ImGui::NextColumn();
	ImGui::Text("Carry");
	ImGui::Text(cpu->registers.getFlag(CARRY) ? "1" : "0");
	ImGui::NextColumn();
	ImGui::Columns(1);
	ImGui::Separator();

	ImGui::Text("Breakpoints:");
	ImGui::Separator();

	ImGui::Columns(4, "breakpoints");
	ImGui::Text("Address");
	ImGui::NextColumn();
	ImGui::Text("Mnemonic");
	ImGui::NextColumn();
	ImGui::Text("Opcode");
	ImGui::NextColumn();
	ImGui::Text("Cycles");
	ImGui::NextColumn();
	ImGui::Separator();

	std::vector<uint16_t> currentBreakpoints = breakpoints;
	for (uint16_t breakpoint : currentBreakpoints) {
		uint8_t opcode = cpu->read(breakpoint);

		ImGui::Text("0x%04X", breakpoint);
		ImGui::NextColumn();
		ImGui::Text(getInstruction(opcode, false).c_str());
		ImGui::NextColumn();
		ImGui::Text("0x%04X", opcode);
		ImGui::NextColumn();
		ImGui::Text("%d", getCycle(opcode, false, false));
		ImGui::NextColumn();
	}
	ImGui::Columns(1);
	ImGui::Separator();

	ImGui::AlignTextToFramePadding();
	ImGui::Text("Remove breakpoint");
	ImGui::SameLine();
	char input_remove[64];
	memset(input_remove, 0, sizeof(char) * 64);
	if (ImGui::InputText("##hidelabel1", input_remove, 64, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_EnterReturnsTrue)) {
		int addr;
		if (sscanf_s(input_remove, "%X", &addr)) {
			uint16_t address = addr & 0xFFFF;
			breakpoints.erase(std::remove(breakpoints.begin(), breakpoints.end(), address), breakpoints.end());
		}
	}
	ImGui::Separator();

	ImGui::Text("Interrupts:");
	ImGui::Separator();
	ImGui::Text("IME: %s", cpu->ime ? "True" : "False");
	ImGui::Separator();
	// IE
	ImGui::Text("Interrupts Enable:");
	ImGui::Separator();
	ImGui::Columns(5, "interrupts_ie");
	ImGui::Text("V-Blank:"); 
	ImGui::Text("%d", memoryController->interrupts.getIe(VBLANK));
	ImGui::NextColumn();
	ImGui::Text("LCD:");
	ImGui::Text("%d", memoryController->interrupts.getIe(LCD));
	ImGui::NextColumn();
	ImGui::Text("Timer:");
	ImGui::Text("%d", memoryController->interrupts.getIe(TIMER));
	ImGui::NextColumn();
	ImGui::Text("Serial:");
	ImGui::Text("%d", memoryController->interrupts.getIe(SERIAL));
	ImGui::NextColumn();
	ImGui::Text("Joypad:");
	ImGui::Text("%d", memoryController->interrupts.getIe(JOYPAD));
	ImGui::NextColumn();
	ImGui::Columns(1);
	ImGui::Separator();

	ImGui::Text("Interrupts Request Flag:");
	ImGui::Separator();
	ImGui::Columns(5, "interrupts_if");
	ImGui::Text("V-Blank:");
	ImGui::Text("%d", memoryController->interrupts.getIf(VBLANK));
	ImGui::NextColumn();
	ImGui::Text("LCD:");
	ImGui::Text("%d", memoryController->interrupts.getIf(LCD));
	ImGui::NextColumn();
	ImGui::Text("Timer:");
	ImGui::Text("%d", memoryController->interrupts.getIf(TIMER));
	ImGui::NextColumn();
	ImGui::Text("Serial:");
	ImGui::Text("%d", memoryController->interrupts.getIf(SERIAL));
	ImGui::NextColumn();
	ImGui::Text("Joypad:");
	ImGui::Text("%d", memoryController->interrupts.getIf(JOYPAD));
	ImGui::NextColumn();
	ImGui::Columns(1);
	ImGui::Separator();

	ImGui::Text("Application info: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::End();
}

void Debugger::CreatePpuWindow() {
	ImGui::Begin("PPU");
	ImGui::Columns(2, "ppu_main");
	// LCD Control
	ImGui::Text("LCD Control:");
	ImGui::Text("0x%04X", ppu->getRegister(LCDC));
	ImGui::NextColumn();
	// LCD Status
	ImGui::Text("LCD Status:");
	ImGui::Text("0x%04X", ppu->getRegister(STAT));
	ImGui::NextColumn();
	ImGui::Columns(1);
	ImGui::Separator();

	ImGui::Columns(2, "ppu_d");
	// SCY - SCX
	ImGui::Text("SCY - SCX:");
	ImGui::Text("0x%04X - 0x%04X", ppu->getRegister(SCY), ppu->getRegister(SCX));
	ImGui::NextColumn();
	// WY - WX
	ImGui::Text("WY - WX:");
	ImGui::Text("0x%04X - 0x%04X", ppu->getRegister(WY), ppu->getRegister(WX));
	ImGui::NextColumn();
	ImGui::NextColumn();
	ImGui::Columns(1);
	ImGui::Separator();

	ImGui::Columns(3, "ppu_misc");
	// LY
	ImGui::Text("LY:");
	ImGui::Text("0x%04X", ppu->getRegister(LY));
	ImGui::NextColumn();
	// LYC
	ImGui::Text("LYC:");
	ImGui::Text("0x%04X", ppu->getRegister(LYC));
	ImGui::NextColumn();
	// BGP
	ImGui::Text("BGP");
	ImGui::Text("0x%04X", ppu->getRegister(BGP));
	ImGui::NextColumn();
	ImGui::Columns(1);
	ImGui::Separator();

	ImGui::Columns(2, "ppu_obp");
	// OBP0
	ImGui::Text("OBP0");
	ImGui::Text("0x%04X", ppu->getRegister(OBP0));
	ImGui::NextColumn();
	// OBP1
	ImGui::Text("OBP1");
	ImGui::Text("0x%04X", ppu->getRegister(OBP1));

	ImGui::NextColumn();
	ImGui::Columns(1);
	ImGui::Separator();
	ImGui::End();
}

void Debugger::CreateCartridgeWindow() {
	CartridgeInfo* cartInfo = &memoryController->cartridge->cartInfo;

	ImGui::Begin("Cartridge");

	ImGui::Text("Information:");
	ImGui::Separator();
	ImGui::Columns(2, "basic_info");
	ImGui::Text("Title: %s", cartInfo->title);
	ImGui::Text("Supports SGB:");
	ImGui::SameLine();
	ImGui::Text(cartInfo->sgbFlag ? "True" : "False");
	ImGui::Text("Version Number: %d", cartInfo->versionNumber);
	ImGui::Text("Destination Code:");
	ImGui::SameLine();
	ImGui::Text(cartInfo->destinationCode == 0 ? "Non-Japanese" : "Japanese");
	if (cartInfo->sgbFlag) {
		ImGui::Text("Licensee Code: %s", cartridgeNewLicencee.at((int)cartInfo->newLicenseeCode).c_str());
	} else {
		ImGui::Text("Licensee Code: %s", cartridgeOldLicencee.at(cartInfo->oldLicenseeCode).c_str());
	}
	ImGui::NextColumn();
	ImGui::Text("File Size: %dKB", cartInfo->fileSize / 1000);
	ImGui::Text("ROM Type: %s", cartridgeType.at(cartInfo->romType).c_str());
	ImGui::Text("ROM Size: %dKB", cartridgeRomSize.at(cartInfo->romSize)[0]);
	ImGui::Text("ROM Banks: %d", cartridgeRomSize.at(cartInfo->romSize)[1]);
	ImGui::Text("RAM Size: %dKB", cartridgeRamSize[cartInfo->ramSize]);
	ImGui::NextColumn();
	ImGui::Columns(1);
	ImGui::Separator();

	ImGui::Text("Checksums:");
	ImGui::Separator();
	ImGui::Columns(2, "checksums");
	ImGui::Text("Checksum: %d", cartInfo->checksum);
	ImGui::Text("Calculated Checksum: %d", cartInfo->calculatedChecksum);
	ImGui::Text("Checksum checks out:");
	ImGui::SameLine();
	ImGui::Text((cartInfo->checksum == cartInfo->calculatedChecksum) ? "True" : "False");
	ImGui::NextColumn();
	ImGui::Text("Global Checksum: %d", cartInfo->globalChecksum);
	ImGui::Text("Calculated Global Checksum: %d", cartInfo->calculatedGlobalChecksum);
	ImGui::Text("Gloabl Checksum checks out:");
	ImGui::SameLine();
	ImGui::Text((cartInfo->globalChecksum == cartInfo->calculatedGlobalChecksum) ? "True" : "False");
	ImGui::NextColumn();
	ImGui::Columns(1);

	ImGui::End();
}

void Debugger::CreateMemoryWindow() {
	static MemoryEditor mem_edit;
	ImGui::Begin("Memory");
	//mem_edit.DrawContents(memoryController->cartridge->rom, 0x8000, 0x0);
	ImGui::End();
}

void Debugger::ExitDebugger() {
	//delete this;
	quit = true;
}
