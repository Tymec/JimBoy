#pragma once
#include <iostream>
#include <fstream>

const uint8_t ZERO_FLAG_POSITION = 0x80;
const uint8_t SUB_FLAG_POSITION = 0x40;
const uint8_t HALF_FLAG_POSITION = 0x20;
const uint8_t CARRY_FLAG_POSITION = 0x10;

struct Registers {
	uint8_t a{}; // Accumulator Register
	uint8_t b{}; // B Register
	uint8_t c{}; // C Register
	uint8_t d{}; // D Register
	uint8_t e{}; // E Register
	uint8_t f{}; // Flags Register
	uint8_t h{}; // H Register
	uint8_t l{}; // L Register

	// Returns AF Register
	uint16_t getAF() {
		return (a << 8u) | f;
	};

	// Sets AF Register
	void setAF(uint16_t value) {
		a = (value & 0xFF00u) >> 8u;
		f = value & 0x00F0u;
	};
	
	// Returns BC Register
	uint16_t getBC() {
		return (b << 8u) | c;
	};

	// Sets BC Register
	void setBC(uint16_t value) {
		b = (value & 0xFF00u) >> 8u;
		c = value & 0x00FFu;
	};
	
	// Returns DE Register
	uint16_t getDE() {
		return (d << 8u) | e;
	};

	// Sets DE Register
	void setDE(uint16_t value) {
		d = (value & 0xFF00u) >> 8u;
		e = value & 0x00FFu;
	};
	
	// Returns HL Register
	uint16_t getHL() {
		return (h << 8u) | l;
	};

	// Sets HL Register
	void setHL(uint16_t value) {
		h = (value & 0xFF00u) >> 8u;
		l = value & 0x00FFu;
	};

	// Get ZERO Flag | 0x80
	bool getFlagZ() {
		return getFlag(ZERO_FLAG_POSITION);
	}

	// Set ZERO Flag | 0x80
	void setFlagZ(uint16_t value) {
		bool v = (value & 0xFFFF);
		setFlag(ZERO_FLAG_POSITION, !v);
	}

	// Get SUB Flag | 0x40
	bool getFlagN() {
		return getFlag(SUB_FLAG_POSITION);
	}

	// Set SUB Flag | 0x40
	void setFlagN(bool v) {
		setFlag(SUB_FLAG_POSITION, v);
	}

	// Get HALF CARRY Flag | 0x20
	bool getFlagH() {
		return getFlag(HALF_FLAG_POSITION);
	}

	// Set HALF CARRY Flag | 0x20
	void setFlagH(uint16_t value, uint16_t old, uint16_t diff) {
		bool v = (old ^ diff ^ value) & 0x10;
		setFlag(HALF_FLAG_POSITION, v);
	}

	// Get CARRY Flag | 0x10
	bool getFlagC() {
		return getFlag(CARRY_FLAG_POSITION);
	}

	// Set CARRY Flag | 0x10
	void setFlagC(uint16_t value) {
		if (value & 0xFF00u) {
			setFlag(CARRY_FLAG_POSITION, value > 0xFFFFu);
		} else {
			setFlag(CARRY_FLAG_POSITION, value > 0xFFu);
		}
	}

private:
	bool getFlag(uint8_t pos) {
		return (f & pos) != 0;
	}

	void setFlag(uint8_t pos, bool val) {
		f = val ? (f | pos) : (f & ~pos);
	}
};

class MemoryController;
class Cpu {
public:
	Cpu(MemoryController *memoryController);
	~Cpu();

	// Fetch, Decode & Execute
	void cycle();
private:
	MemoryController *memoryController; // Pointer to MemoryController
	uint8_t opcode{}; // Current opcode
	uint16_t sp{}; // Stack Pointer
	uint16_t pc{}; // Program Counter
	Registers registers; // Registers
	uint8_t cycles{}; // Cycles

	bool lowPowerMode{}; // Low Power Mode

	// Execute current instruction
	void executeInstruction();

	// Read & Write the Bus
	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t value);

	// Read from memory
	uint8_t read8();
	uint16_t read16();

	/// Opcodes
	void OP_ld();
	void OP_ld16();
	void OP_push();
	void OP_pop();
	void OP_add();
	void OP_adc();
	void OP_sub();
	void OP_sbc();
	void OP_and();
	void OP_xor();
	void OP_or();
	void OP_cp();
	void OP_inc();
	void OP_dec();
	void OP_daa();
	void OP_cpl();
	void OP_add16();
	void OP_inc16();
	void OP_dec16();
	//void OP_ld16();
	void OP_rlca();
	void OP_rla();
	void OP_rrca();
	void OP_rra();
	void OP_ccf();
	void OP_scf();
	void OP_nop();
	void OP_halt();
	void OP_stop();
	void OP_di();
	void OP_ei();
	void OP_jp();
	void OP_jr(bool cc);
	void OP_call();
	void OP_ret();
	void OP_reti();
	void OP_rst();
	// CB-Prefix
	void OP_rlc();
	void OP_rl();
	void OP_rrc();
	void OP_rr();
	void OP_sla();
	void OP_swap();
	void OP_sra();
	void OP_srl();
	void OP_bit(uint8_t v, uint8_t reg);
	void OP_set();
	void OP_res();
};