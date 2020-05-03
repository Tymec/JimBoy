#pragma once
#include <iostream>
#include <fstream>

const uint8_t ZERO_FLAG_POSITION = 0x80;
const uint8_t SUB_FLAG_POSITION = 0x40;
const uint8_t HALF_FLAG_POSITION = 0x20;
const uint8_t CARRY_FLAG_POSITION = 0x10;

const uint16_t IF_REGISTER_ADDRESS = 0xFF0F;
const uint16_t IE_REGISTER_ADDRESS = 0xFFFF;

enum FLAGS {
	ZERO = 0x80,
	SUB = 0x40,
	HALF = 0x20,
	CARRY = 0x10
};

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

	// Get Flag
	bool getFlag(FLAGS flag) {
		return (f & flag) != 0;
	}

	// Set Flag
	void setFlag(FLAGS flag, bool value) {
		f = value ? (f | flag) : (f & ~flag);
	}
};

class MemoryController;
class Cpu {
public:
	Cpu(MemoryController *memoryController);
	~Cpu();

	// Fetch, Decode & Execute
	unsigned Cycle();
	Registers registers; // Registers
	uint16_t sp{}; // Stack Pointer
	uint8_t opcode{}; // Current opcode
	uint16_t pc{}; // Program Counter
private:
	friend class Debugger; // NOT SMART!!!
	MemoryController *memoryController; // Pointer to MemoryController

	bool branch{}; // If jump is branching
	uint16_t prevPc{}; // PC when cycle began
	bool prefix{}; // Instructions 0xCB
	bool ime{}; // Interrupt Master Enable Flag 
	bool halted{}; // Is CPU halted
	bool ime_scheduled{}; // IME should be enabled after intruction

	// Execute current instruction
	void ExecuteInstruction();

	// Read & Write the Bus
	uint8_t read(uint16_t address);
	void write(uint16_t address, uint8_t value);

	// Read from memory
	uint8_t read8();
	uint16_t read16();

	/// Opcodes
	uint8_t OP_ld(uint8_t v);
	void OP_ld16(uint16_t v);
	void OP_push(uint16_t v);
	uint16_t OP_pop();
	void OP_add(uint8_t v);
	void OP_adc(uint8_t v);
	void OP_sub(uint8_t v);
	void OP_sbc(uint8_t v);
	void OP_and(uint8_t v);
	void OP_xor(uint8_t v);
	void OP_or(uint8_t v);
	void OP_cp(uint8_t v);
	uint8_t OP_inc(uint8_t v);
	uint8_t OP_dec(uint8_t v);
	void OP_daa();
	void OP_cpl();
	uint16_t OP_add16(uint16_t reg, uint16_t v, bool s=false);
	uint16_t OP_inc16(uint16_t v);
	uint16_t OP_dec16(uint16_t v);
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
	void OP_jp(bool cc);
	void OP_jr(bool cc);
	void OP_call(bool cc);
	void OP_ret(bool cc);
	void OP_reti();
	void OP_rst(uint8_t v);
	// CB-Prefix
	uint8_t OP_rlc(uint8_t reg);
	uint8_t OP_rl(uint8_t reg);
	uint8_t OP_rrc(uint8_t reg);
	uint8_t OP_rr(uint8_t reg);
	uint8_t OP_sla(uint8_t reg);
	uint8_t OP_swap(uint8_t reg);
	uint8_t OP_sra(uint8_t reg);
	uint8_t OP_srl(uint8_t reg);
	void OP_bit(uint8_t v, uint8_t reg);
	uint8_t OP_set(uint8_t v, uint8_t reg);
	uint8_t OP_res(uint8_t v, uint8_t reg);
};