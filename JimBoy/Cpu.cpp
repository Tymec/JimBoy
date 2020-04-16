#include "Cpu.h"
#include "MemoryController.h"
#include "Disassembler.h"

Cpu::Cpu(MemoryController *memoryController) {
	this->memoryController = memoryController;

	// DEBUG
	pc = 0x100;
}

Cpu::~Cpu() {
}

void Cpu::cycle() {
	if (cycles == 0) {
		opcode = read8();
		executeInstruction();
		//std::cout << "Instruction: " << "0x" << std::hex << (unsigned)opcode << " | " << (unsigned)read(pc) << std::dec << std::endl;
		//std::cout << Translation[opcode] << std::endl;
	}
}

void Cpu::executeInstruction() {
	switch(opcode) {
		case(0x00): OP_nop();	break;	// NOP					1b	4t
		case(0x01): OP_ld16();	break;	// LD BC, u16			3b	12t
		case(0x02): OP_ld();	break;	// LD (BC), A			1b	8t
		case(0x03): OP_inc16();	break;	// INC BC				1b	8t
		case(0x04): OP_inc();	break;	// INC B				1b	4t
		case(0x05): OP_dec();	break;	// DEC B				1b	4t
		case(0x06):	OP_ld();	break;	// LD B, u8				2b	8t
		case(0x07):	OP_rlca();	break;	// RLCA					1b	4t
		case(0x08):	OP_ld16();	break;	// LD (u16), SP			3b	20t
		case(0x09):	OP_add16();	break;	// ADD HL, BC			1b	8t
		case(0x0A):	OP_ld();	break;	// LD A, (BC)			1b	8t
		case(0x0B):	OP_dec16();	break;	// DEC BC				1b	8t
		case(0x0C):	OP_inc();	break;	// INC C				1b	4t
		case(0x0D):	OP_dec();	break;	// DEC C				1b	4t
		case(0x0E):	OP_ld();	break;	// LD C, u8				2b	8t
		case(0x0F):	OP_rrca();	break;	// RRCA					1b	4t
		case(0x10):	OP_stop();	break;	// STOP					2b	4t
		case(0x11):	OP_ld16();	break;	// LD DE, u16			3b	12t
		case(0x12):	OP_ld();	break;	// LD (DE), A			1b	8t
		case(0x13):	OP_inc16();	break;	// INC DE				1b	8t
		case(0x14):	OP_inc();	break;	// INC D				1b	4t
		case(0x15):	OP_dec();	break;	// DEC D				1b	4t
		case(0x16):	OP_ld();	break;	// LD D, u8				2b	8t
		case(0x17):	OP_rla();	break;	// RLA					1b	4t
		case(0x18):	OP_jr(true);	break;	// JR i8				2b	12t
		case(0x19):	OP_add16();	break;	// ADD HL, DE			1b	8t
		case(0x1A):	OP_ld();	break;	// LD A, (DE)			1b	8t
		case(0x1B): OP_dec16();	break;	// DEC DE				1b	8t
		case(0x1C): OP_inc();	break;	// INC E				1b	4t
		case(0x1D): OP_inc();	break;	// DEC E				1b	4t
		case(0x1E):	OP_ld();	break;	// LD E, u8				2b	8t
		case(0x1F): OP_rra();	break;	// RRA					1b	4t
		case(0x20): OP_jr(!registers.getFlagZ());	break;	// JR NZ, i8			2b	8t-12t
		case(0x21): OP_ld16();	break;	// LD HL, u16			3b	12t
		case(0x22): OP_ld();	break;	// LD (HL+), A			1b	8t
		case(0x23): OP_inc16();	break;	// INC HL				1b	8t
		case(0x24): OP_inc();	break;	// INC H				1b	4t
		case(0x25): OP_dec();	break;	// DEC H				1b	4t
		case(0x26): OP_ld();	break;	// LD H, u8				2b	8t
		case(0x27): OP_daa();	break;	// DAA					1b	4t
		case(0x28): OP_jr(registers.getFlagZ());	break;	// JR Z, i8				2b	8t-12t
		case(0x29): OP_add16();	break;	// ADD HL, HL			1b	8t
		case(0x2A): OP_ld();	break;	// LD A, (HL+)			1b	8t
		case(0x2B): OP_dec16();	break;	// DEC HL				1b	8t
		case(0x2C): OP_inc();	break;	// INC L				1b	4t
		case(0x2D): OP_dec();	break;	// DEC L				1b	4t
		case(0x2E): OP_ld();	break;	// LD L, u8				2b	8t
		case(0x2F): OP_cpl();	break;	// CPL					1b	4t
		case(0x30): OP_jr(!registers.getFlagC());	break;	// JR NC, i8			2b	8t-12t
		case(0x31):	OP_ld16();	break;	// LD SP, u16			3b	12t
		case(0x32): OP_ld();	break;	// LD (HL-), A			1b	8t
		case(0x33): OP_inc16();	break;	// INC SP				1b	8t
		case(0x34): OP_inc();	break;	// INC (HL)				1b	12t
		case(0x35): OP_dec();	break;	// DEC (HL)				1b	12t
		case(0x36): OP_ld();	break;	// LD (HL), u8			2b	12t
		case(0x37): OP_scf();	break;	// SCF					1b	4t
		case(0x38): OP_jr(registers.getFlagC());	break;	// JR C, i8				2b	8t-12t
		case(0x39): OP_add16();	break;	// ADD HL, SP			1b	8t
		case(0x3A): OP_ld();	break;	// LD A, (HL-)			1b	8t
		case(0x3B):	OP_dec16();	break;	// DEC SP				1b	8t
		case(0x3C):	OP_inc();	break;	// INC A				1b	4t
		case(0x3D): OP_dec();	break;	// DEC A				1b	4t
		case(0x3E): OP_ld();	break;	// LD A, u8				2b	8t
		case(0x3F): OP_ccf();	break;	// CCF					1b	4t
		case(0x40): OP_ld();	break;	// LD B, B				1b	4t
		case(0x41):	OP_ld();	break;	// LD B, C				1b	4t
		case(0x42):	OP_ld();	break;	// LD B, D				1b	4t
		case(0x43):	OP_ld();	break;	// LD B, E				1b	4t
		case(0x44):	OP_ld();	break;	// LD B, H				1b	4t
		case(0x45):	OP_ld();	break;	// LD B, L				1b	4t
		case(0x46):	OP_ld();	break;	// LD B, (HL)			1b	8t
		case(0x47):	OP_ld();	break;	// LD B, A				1b	4t
		case(0x48):	OP_ld();	break;	// LD C, B				1b	4t
		case(0x49):	OP_ld();	break;	// LD C, C				1b	4t
		case(0x4A):	OP_ld();	break;	// LD C, D				1b	4t
		case(0x4B):	OP_ld();	break;	// LD C, E				1b	4t
		case(0x4C):	OP_ld();	break;	// LD C, H				1b	4t
		case(0x4D):	OP_ld();	break;	// LD C, L				1b	4t
		case(0x4E):	OP_ld();	break;	// LD C, (HL)			1b	8t
		case(0x4F):	OP_ld();	break;	// LD C, A				1b	4t
		case(0x50):	OP_ld();	break;	// LD D, B				1b	4t
		case(0x51):	OP_ld();	break;	// LD D, C				1b	4t
		case(0x52):	OP_ld();	break;	// LD D, D				1b	4t
		case(0x53):	OP_ld();	break;	// LD D, E				1b	4t
		case(0x54):	OP_ld();	break;	// LD D, H				1b	4t
		case(0x55):	OP_ld();	break;	// LD D, L				1b	4t
		case(0x56):	OP_ld();	break;	// LD D, (HL)			1b	8t
		case(0x57):	OP_ld();	break;	// LD D, A				1b	4t
		case(0x58):	OP_ld();	break;	// LD E, B				1b	4t
		case(0x59):	OP_ld();	break;	// LD E, C				1b	4t
		case(0x5A):	OP_ld();	break;	// LD E, D				1b	4t
		case(0x5B):	OP_ld();	break;	// LD E, E				1b	4t
		case(0x5C):	OP_ld();	break;	// LD E, H				1b	4t
		case(0x5D):	OP_ld();	break;	// LD E, L				1b	4t
		case(0x5E):	OP_ld();	break;	// LD E, (HL)			1b	8t
		case(0x5F):	OP_ld();	break;	// LD E, A				1b	4t
		case(0x60):	OP_ld();	break;	// LD H, B				1b	4t
		case(0x61):	OP_ld();	break;	// LD H, C				1b	4t
		case(0x62):	OP_ld();	break;	// LD H, D				1b	4t
		case(0x63):	OP_ld();	break;	// LD H, E				1b	4t
		case(0x64):	OP_ld();	break;	// LD H, H				1b	4t
		case(0x65):	OP_ld();	break;	// LD H, L				1b	4t
		case(0x66):	OP_ld();	break;	// LD H, (HL)			1b	8t
		case(0x67):	OP_ld();	break;	// LD H, A				1b	4t
		case(0x68):	OP_ld();	break;	// LD L, B				1b	4t
		case(0x69):	OP_ld();	break;	// LD L, C				1b	4t
		case(0x6A):	OP_ld();	break;	// LD L, D				1b	4t
		case(0x6B):	OP_ld();	break;	// LD L, E				1b	4t
		case(0x6C):	OP_ld();	break;	// LD L, H				1b	4t
		case(0x6D):	OP_ld();	break;	// LD L, L				1b	4t
		case(0x6E):	OP_ld();	break;	// LD L, (HL)			1b	8t
		case(0x6F):	OP_ld();	break;	// LD L, A				1b	4t
		case(0x70):	OP_ld();	break;	// LD (HL), B			1b	8t
		case(0x71):	OP_ld();	break;	// LD (HL), C			1b	8t
		case(0x72):	OP_ld();	break;	// LD (HL), D			1b	8t
		case(0x73):	OP_ld();	break;	// LD (HL), E			1b	8t
		case(0x74):	OP_ld();	break;	// LD (HL), H			1b	8t
		case(0x75):	OP_ld();	break;	// LD (HL), L			1b	8t
		case(0x76): OP_halt();	break;	// HALT					1b	4t
		case(0x77):	OP_ld();	break;	// LD (HL), A			1b	8t
		case(0x78):	OP_ld();	break;	// LD A, B				1b	4t
		case(0x79):	OP_ld();	break;	// LD A, C				1b	4t
		case(0x7A):	OP_ld();	break;	// LD A, D				1b	4t
		case(0x7B):	OP_ld();	break;	// LD A, E				1b	4t
		case(0x7C):	OP_ld();	break;	// LD A, H				1b	4t
		case(0x7D):	OP_ld();	break;	// LD A, L				1b	4t
		case(0x7E):	OP_ld();	break;	// LD A, (HL)			1b	8t
		case(0x7F):	OP_ld();	break;	// LD A, A				1b	4t
		case(0x80): OP_add();	break;	// ADD A, B				1b	4t
		case(0x81): OP_add();	break;	// ADD A, C				1b	4t
		case(0x82): OP_add();	break;	// ADD A, D				1b	4t
		case(0x83): OP_add();	break;	// ADD A, E				1b	4t
		case(0x84): OP_add();	break;	// ADD A, H				1b	4t
		case(0x85): OP_add();	break;	// ADD A, L				1b	4t
		case(0x86): OP_add();	break;	// ADD A, (HL)			1b	8t
		case(0x87): OP_add();	break;	// ADD A, A				1b	4t
		case(0x88): OP_adc();	break;	// ADC A, B				1b	4t
		case(0x89): OP_adc();	break;	// ADC A, C				1b	4t
		case(0x8A): OP_adc();	break;	// ADC A, D				1b	4t
		case(0x8B): OP_adc();	break;	// ADC A, E				1b	4t
		case(0x8C): OP_adc();	break;	// ADC A, H				1b	4t
		case(0x8D): OP_adc();	break;	// ADC A, L				1b	4t
		case(0x8E): OP_adc();	break;	// ADC A, (HL)			1b	8t
		case(0x8F): OP_adc();	break;	// ADC A, A				1b	4t
		case(0x90): OP_sub();	break;	// SUB A, B				1b	4t
		case(0x91): OP_sub();	break;	// SUB A, C				1b	4t
		case(0x92): OP_sub();	break;	// SUB A, D				1b	4t
		case(0x93): OP_sub();	break;	// SUB A, E				1b	4t
		case(0x94): OP_sub();	break;	// SUB A, H				1b	4t
		case(0x95): OP_sub();	break;	// SUB A, L				1b	4t
		case(0x96): OP_sub();	break;	// SUB A, (HL)			1b	8t
		case(0x97): OP_sub();	break;	// SUB A, A				1b	4t
		case(0x98): OP_sbc();	break;	// SBC A, B				1b	4t
		case(0x99): OP_sbc();	break;	// SBC A, C				1b	4t
		case(0x9A): OP_sbc();	break;	// SBC A, D				1b	4t
		case(0x9B): OP_sbc();	break;	// SBC A, E				1b	4t
		case(0x9C): OP_sbc();	break;	// SBC A, H				1b	4t
		case(0x9D): OP_sbc();	break;	// SBC A, L				1b	4t
		case(0x9E): OP_sbc();	break;	// SBC A, (HL)			1b	8t
		case(0x9F): OP_sbc();	break;	// SBC A, A				1b	4t
		case(0xA0): OP_and();	break;	// AND A, B				1b	4t
		case(0xA1): OP_and();	break;	// AND A, C				1b	4t
		case(0xA2): OP_and();	break;	// AND A, D				1b	4t
		case(0xA3): OP_and();	break;	// AND A, E				1b	4t
		case(0xA4): OP_and();	break;	// AND A, H				1b	4t
		case(0xA5): OP_and();	break;	// AND A, L				1b	4t
		case(0xA6): OP_and();	break;	// AND A, (HL)			1b	8t
		case(0xA7): OP_and();	break;	// AND A, A				1b	4t
		case(0xA8): OP_xor();	break;	// XOR A, B				1b	4t
		case(0xA9): OP_xor();	break;	// XOR A, C				1b	4t
		case(0xAA): OP_xor();	break;	// XOR A, D				1b	4t
		case(0xAB): OP_xor();	break;	// XOR A, E				1b	4t
		case(0xAC): OP_xor();	break;	// XOR A, H				1b	4t
		case(0xAD): OP_xor();	break;	// XOR A, L				1b	4t
		case(0xAE): OP_xor();	break;	// XOR A, (HL)			1b	8t
		case(0xAF): OP_xor();	break;	// XOR A, A				1b	4t
		case(0xB0): OP_or();	break;	// OR A, B				1b	4t
		case(0xB1): OP_or();	break;	// OR A, C				1b	4t
		case(0xB2): OP_or();	break;	// OR A, D				1b	4t
		case(0xB3): OP_or();	break;	// OR A, E				1b	4t
		case(0xB4): OP_or();	break;	// OR A, H				1b	4t
		case(0xB5): OP_or();	break;	// OR A, L				1b	4t
		case(0xB6): OP_or();	break;	// OR A,(HL)			1b	8t
		case(0xB7): OP_or();	break;	// OR A, A				1b	4t
		case(0xB8): OP_cp();	break;	// CP A, B				1b	4t
		case(0xB9): OP_cp();	break;	// CP A, C				1b	4t
		case(0xBA): OP_cp();	break;	// CP A, D				1b	4t
		case(0xBB): OP_cp();	break;	// CP A, E				1b	4t
		case(0xBC): OP_cp();	break;	// CP A, H				1b	4t
		case(0xBD): OP_cp();	break;	// CP A, L				1b	4t
		case(0xBE): OP_cp();	break;	// CP A, (HL)			1b	8t
		case(0xBF): OP_cp();	break;	// CP A, A				1b	4t
		case(0xC0): OP_ret();	break;	// RET NZ				1b	8t-20t
		case(0xC1): OP_pop();	break;	// POP BC				1b	12t
		case(0xC2): OP_jp();	break;	// JP NZ, u16			3b	12t-16t
		case(0xC3): OP_jp();	break;	// JP u16				3b	16t
		case(0xC4): OP_call();	break;	// CALL NZ, u16			3b	12t-24t
		case(0xC5):	OP_push();	break;	// PUSH BC				1b	16t
		case(0xC6): OP_add();	break;	// ADD A, u8			2b	8t
		case(0xC7): OP_rst();	break;	// RST 00h				1b	16t
		case(0xC8): OP_ret();	break;	// RET Z				1b	8t-20t
		case(0xC9): OP_ret();	break;	// RET					1b	16t
		case(0xCA): OP_jp();	break;	// JP Z, u16			3b	12t-16t
		case(0xCB):	
			opcode = read8();
			switch(opcode) {
				case(0x00): OP_rlc();	break; // RLC B			2b	8t
				case(0x01): OP_rlc();	break; // RLC C			2b	8t
				case(0x02): OP_rlc();	break; // RLC D			2b	8t
				case(0x03): OP_rlc();	break; // RLC E			2b	8t
				case(0x04): OP_rlc();	break; // RLC H			2b	8t
				case(0x05): OP_rlc();	break; // RLC L			2b	8t
				case(0x06): OP_rlc();	break; // RLC (HL)		2b	16t
				case(0x07): OP_rlc();	break; // RLC A			2b	8t
				case(0x08): OP_rrc();	break; // RRC B			2b	8t
				case(0x09): OP_rrc();	break; // RRC C			2b	8t
				case(0x0A): OP_rrc();	break; // RRC D			2b	8t
				case(0x0B): OP_rrc();	break; // RRC E			2b	8t
				case(0x0C): OP_rrc();	break; // RRC H			2b	8t
				case(0x0D): OP_rrc();	break; // RRC L			2b	8t
				case(0x0E): OP_rrc();	break; // RRC (HL)		2b	16t
				case(0x0F): OP_rrc();	break; // RRC A			2b	8t
				case(0x10): OP_rl();	break; // RL B			2b	8t
				case(0x11): OP_rl();	break; // RL C			2b	8t
				case(0x12): OP_rl();	break; // RL D			2b	8t
				case(0x13): OP_rl();	break; // RL E			2b	8t
				case(0x14): OP_rl();	break; // RL H			2b	8t
				case(0x15): OP_rl();	break; // RL L			2b	8t
				case(0x16): OP_rl();	break; // RL (HL)		2b	16t
				case(0x17): OP_rl();	break; // RL A			2b	8t
				case(0x18): OP_rr();	break; // RR B			2b	8t
				case(0x19): OP_rr();	break; // RR C			2b	8t
				case(0x1A): OP_rr();	break; // RR D			2b	8t
				case(0x1B): OP_rr();	break; // RR E			2b	8t
				case(0x1C): OP_rr();	break; // RR H			2b	8t
				case(0x1D): OP_rr();	break; // RR L			2b	8t
				case(0x1E): OP_rr();	break; // RR (HL)		2b	16t
				case(0x1F): OP_rr();	break; // RR A			2b	8t
				case(0x20): OP_sla();	break; // SLA B			2b	8t
				case(0x21): OP_sla();	break; // SLA C			2b	8t
				case(0x22): OP_sla();	break; // SLA D			2b	8t
				case(0x23): OP_sla();	break; // SLA E			2b	8t
				case(0x24): OP_sla();	break; // SLA H			2b	8t
				case(0x25): OP_sla();	break; // SLA L			2b	8t
				case(0x26): OP_sla();	break; // SLA (HL)		2b	16t
				case(0x27): OP_sla();	break; // SLA A			2b	8t
				case(0x28): OP_sra();	break; // SRA B			2b	8t	
				case(0x29): OP_sra();	break; // SRA C			2b	8t
				case(0x2A): OP_sra();	break; // SRA D			2b	8t
				case(0x2B): OP_sra();	break; // SRA E			2b	8t
				case(0x2C): OP_sra();	break; // SRA H			2b	8t
				case(0x2D): OP_sra();	break; // SRA L			2b	8t
				case(0x2E): OP_sra();	break; // SRA (HL)		2b	16t
				case(0x2F): OP_sra();	break; // SRA A			2b	8t
				case(0x30): OP_swap();	break; // SWAP B		2b	8t
				case(0x31): OP_swap();	break; // SWAP C		2b	8t
				case(0x32): OP_swap();	break; // SWAP D		2b	8t
				case(0x33): OP_swap();	break; // SWAP E		2b	8t
				case(0x34): OP_swap();	break; // SWAP H		2b	8t
				case(0x35): OP_swap();	break; // SWAP L		2b	8t
				case(0x36): OP_swap();	break; // SWAP (HL)		2b	16t
				case(0x37): OP_swap();	break; // SWAP A		2b	8t
				case(0x38): OP_srl();	break; // SRL B			2b	8t
				case(0x39): OP_srl();	break; // SRL C			2b	8t
				case(0x3A): OP_srl();	break; // SRL D			2b	8t
				case(0x3B): OP_srl();	break; // SRL E			2b	8t
				case(0x3C): OP_srl();	break; // SRL H			2b	8t
				case(0x3D): OP_srl();	break; // SRL L			2b	8t
				case(0x3E): OP_srl();	break; // SRL (HL)		2b	16t
				case(0x3F): OP_srl();	break; // SRL A			2b	8t
				case(0x40): OP_bit();	break; // BIT 0, B		2b	8t
				case(0x41): OP_bit();	break; // BIT 0, C		2b	8t
				case(0x42): OP_bit();	break; // BIT 0, D		2b	8t
				case(0x43): OP_bit();	break; // BIT 0, E		2b	8t
				case(0x44): OP_bit();	break; // BIT 0, H		2b	8t
				case(0x45): OP_bit();	break; // BIT 0, L		2b	8t
				case(0x46): OP_bit();	break; // BIT 0, (HL)	2b	12t
				case(0x47): OP_bit();	break; // BIT 0, A		2b	8t
				case(0x48): OP_bit();	break; // BIT 1, B		2b	8t
				case(0x49): OP_bit();	break; // BIT 1, C		2b	8t
				case(0x4A): OP_bit();	break; // BIT 1, D		2b	8t
				case(0x4B): OP_bit();	break; // BIT 1, E		2b	8t
				case(0x4C): OP_bit();	break; // BIT 1, H		2b	8t
				case(0x4D): OP_bit();	break; // BIT 1, L		2b	8t
				case(0x4E): OP_bit();	break; // BIT 1, (HL)	2b	12t
				case(0x4F): OP_bit();	break; // BIT 1, A		2b	8t
				case(0x50): OP_bit();	break; // BIT 2, B		2b	8t
				case(0x51): OP_bit();	break; // BIT 2, C		2b	8t
				case(0x52): OP_bit();	break; // BIT 2, D		2b	8t
				case(0x53): OP_bit();	break; // BIT 2, E		2b	8t
				case(0x54): OP_bit();	break; // BIT 2, H		2b	8t
				case(0x55): OP_bit();	break; // BIT 2, L		2b	8t
				case(0x56): OP_bit();	break; // BIT 2, (HL)	2b	12t
				case(0x57): OP_bit();	break; // BIT 2, A		2b	8t
				case(0x58): OP_bit();	break; // BIT 3, B		2b	8t
				case(0x59): OP_bit();	break; // BIT 3, C		2b	8t
				case(0x5A): OP_bit();	break; // BIT 3, D		2b	8t
				case(0x5B): OP_bit();	break; // BIT 3, E		2b	8t
				case(0x5C): OP_bit();	break; // BIT 3, H		2b	8t
				case(0x5D): OP_bit();	break; // BIT 3, L		2b	8t
				case(0x5E): OP_bit();	break; // BIT 3, (HL)	2b	12t
				case(0x5F): OP_bit();	break; // BIT 3, A		2b	8t
				case(0x60): OP_bit();	break; // BIT 4, B		2b	8t
				case(0x61): OP_bit();	break; // BIT 4, C		2b	8t
				case(0x62): OP_bit();	break; // BIT 4, D		2b	8t
				case(0x63): OP_bit();	break; // BIT 4, E		2b	8t
				case(0x64): OP_bit();	break; // BIT 4, H		2b	8t
				case(0x65): OP_bit();	break; // BIT 4, L		2b	8t
				case(0x66): OP_bit();	break; // BIT 4, (HL)	2b	12t
				case(0x67): OP_bit();	break; // BIT 4, A		2b	8t
				case(0x68): OP_bit();	break; // BIT 5, B		2b	8t
				case(0x69): OP_bit();	break; // BIT 5, C		2b	8t
				case(0x6A): OP_bit();	break; // BIT 5, D		2b	8t
				case(0x6B): OP_bit();	break; // BIT 5, E		2b	8t
				case(0x6C): OP_bit();	break; // BIT 5, H		2b	8t
				case(0x6D): OP_bit();	break; // BIT 5, L		2b	8t
				case(0x6E): OP_bit();	break; // BIT 5, (HL)	2b	12t
				case(0x6F): OP_bit();	break; // BIT 5, A		2b	8t
				case(0x70): OP_bit();	break; // BIT 6, B		2b	8t
				case(0x71): OP_bit();	break; // BIT 6, C		2b	8t
				case(0x72): OP_bit();	break; // BIT 6, D		2b	8t
				case(0x73): OP_bit();	break; // BIT 6, E		2b	8t
				case(0x74): OP_bit();	break; // BIT 6, H		2b	8t
				case(0x75): OP_bit();	break; // BIT 6, L		2b	8t
				case(0x76): OP_bit();	break; // BIT 6, (HL)	2b	12t
				case(0x77): OP_bit();	break; // BIT 6, A		2b	8t
				case(0x78): OP_bit();	break; // BIT 7, B		2b	8t
				case(0x79): OP_bit();	break; // BIT 7, C		2b	8t
				case(0x7A): OP_bit();	break; // BIT 7, D		2b	8t
				case(0x7B): OP_bit();	break; // BIT 7, E		2b	8t
				case(0x7C): OP_bit();	break; // BIT 7, H		2b	8t
				case(0x7D): OP_bit();	break; // BIT 7, L		2b	8t
				case(0x7E): OP_bit();	break; // BIT 7, (HL)	2b	12t
				case(0x7F): OP_bit();	break; // BIT 7, A		2b	8t
				case(0x80): OP_res();	break; // RES 0, B		2b	8t
				case(0x81): OP_res();	break; // RES 0, C		2b	8t
				case(0x82): OP_res();	break; // RES 0, D		2b	8t
				case(0x83): OP_res();	break; // RES 0, E		2b	8t
				case(0x84): OP_res();	break; // RES 0, H		2b	8t
				case(0x85): OP_res();	break; // RES 0, L		2b	8t
				case(0x86): OP_res();	break; // RES 0, (HL)	2b	16t
				case(0x87): OP_res();	break; // RES 0, A		2b	8t
				case(0x88): OP_res();	break; // RES 1, B		2b	8t
				case(0x89): OP_res();	break; // RES 1, C		2b	8t
				case(0x8A): OP_res();	break; // RES 1, D		2b	8t
				case(0x8B): OP_res();	break; // RES 1, E		2b	8t
				case(0x8C): OP_res();	break; // RES 1, H		2b	8t
				case(0x8D): OP_res();	break; // RES 1, L		2b	8t
				case(0x8E): OP_res();	break; // RES 1, (HL)	2b	16t
				case(0x8F): OP_res();	break; // RES 1, A		2b	8t
				case(0x90): OP_res();	break; // RES 2, B		2b	8t
				case(0x91): OP_res();	break; // RES 2, C		2b	8t
				case(0x92): OP_res();	break; // RES 2, D		2b	8t
				case(0x93): OP_res();	break; // RES 2, E		2b	8t
				case(0x94): OP_res();	break; // RES 2, H		2b	8t
				case(0x95): OP_res();	break; // RES 2, L		2b	8t
				case(0x96): OP_res();	break; // RES 2, (HL)	2b	16t
				case(0x97): OP_res();	break; // RES 2, A		2b	8t
				case(0x98): OP_res();	break; // RES 3, B		2b	8t
				case(0x99): OP_res();	break; // RES 3, C		2b	8t
				case(0x9A): OP_res();	break; // RES 3, D		2b	8t
				case(0x9B): OP_res();	break; // RES 3, E		2b	8t
				case(0x9C): OP_res();	break; // RES 3, H		2b	8t
				case(0x9D): OP_res();	break; // RES 3, L		2b	8t
				case(0x9E): OP_res();	break; // RES 3, (HL)	2b	16t
				case(0x9F): OP_res();	break; // RES 3, A		2b	8t
				case(0xA0): OP_res();	break; // RES 4, B		2b	8t
				case(0xA1): OP_res();	break; // RES 4, C		2b	8t
				case(0xA2): OP_res();	break; // RES 4, D		2b	8t
				case(0xA3): OP_res();	break; // RES 4, E		2b	8t
				case(0xA4): OP_res();	break; // RES 4, H		2b	8t
				case(0xA5): OP_res();	break; // RES 4, L		2b	8t
				case(0xA6): OP_res();	break; // RES 4, (HL)	2b	16t
				case(0xA7): OP_res();	break; // RES 4, A		2b	8t
				case(0xA8): OP_res();	break; // RES 5, B		2b	8t
				case(0xA9): OP_res();	break; // RES 5, C		2b	8t
				case(0xAA): OP_res();	break; // RES 5, D		2b	8t
				case(0xAB): OP_res();	break; // RES 5, E		2b	8t
				case(0xAC): OP_res();	break; // RES 5, H		2b	8t
				case(0xAD): OP_res();	break; // RES 5, L		2b	8t
				case(0xAE): OP_res();	break; // RES 5, (HL)	2b	16t
				case(0xAF): OP_res();	break; // RES 5, A		2b	8t
				case(0xB0): OP_res();	break; // RES 6, B		2b	8t
				case(0xB1): OP_res();	break; // RES 6, C		2b	8t
				case(0xB2): OP_res();	break; // RES 6, D		2b	8t
				case(0xB3): OP_res();	break; // RES 6, E		2b	8t
				case(0xB4): OP_res();	break; // RES 6, H		2b	8t
				case(0xB5): OP_res();	break; // RES 6, L		2b	8t
				case(0xB6): OP_res();	break; // RES 6, (HL)	2b	16t
				case(0xB7): OP_res();	break; // RES 6, A		2b	8t
				case(0xB8): OP_res();	break; // RES 7, B		2b	8t
				case(0xB9): OP_res();	break; // RES 7, C		2b	8t
				case(0xBA): OP_res();	break; // RES 7, D		2b	8t
				case(0xBB): OP_res();	break; // RES 7, E		2b	8t
				case(0xBC): OP_res();	break; // RES 7, H		2b	8t
				case(0xBD): OP_res();	break; // RES 7, L		2b	8t
				case(0xBE): OP_res();	break; // RES 7, (HL)	2b	16t
				case(0xBF): OP_res();	break; // RES 7, A		2b	8t
				case(0xC0): OP_set();	break; // SET 0, B		2b	8t
				case(0xC1): OP_set();	break; // SET 0, C		2b	8t
				case(0xC2): OP_set();	break; // SET 0, D		2b	8t
				case(0xC3): OP_set();	break; // SET 0, E		2b	8t
				case(0xC4): OP_set();	break; // SET 0, H		2b	8t
				case(0xC5): OP_set();	break; // SET 0, L		2b	8t
				case(0xC6): OP_set();	break; // SET 0, (HL)	2b	16t
				case(0xC7): OP_set();	break; // SET 0, A		2b	8t
				case(0xC8): OP_set();	break; // SET 1, B		2b	8t
				case(0xC9): OP_set();	break; // SET 1, C		2b	8t
				case(0xCA): OP_set();	break; // SET 1, D		2b	8t
				case(0xCB): OP_set();	break; // SET 1, E		2b	8t
				case(0xCC): OP_set();	break; // SET 1, H		2b	8t
				case(0xCD): OP_set();	break; // SET 1, L		2b	8t
				case(0xCE): OP_set();	break; // SET 1, (HL)	2b	16t
				case(0xCF): OP_set();	break; // SET 1, A		2b	8t
				case(0xD0): OP_set();	break; // SET 2, B		2b	8t
				case(0xD1): OP_set();	break; // SET 2, C		2b	8t
				case(0xD2): OP_set();	break; // SET 2, D		2b	8t
				case(0xD3): OP_set();	break; // SET 2, E		2b	8t
				case(0xD4): OP_set();	break; // SET 2, H		2b	8t
				case(0xD5): OP_set();	break; // SET 2, L		2b	8t
				case(0xD6): OP_set();	break; // SET 2, (HL)	2b	16t
				case(0xD7): OP_set();	break; // SET 2, A		2b	8t
				case(0xD8): OP_set();	break; // SET 3, B		2b	8t
				case(0xD9): OP_set();	break; // SET 3, C		2b	8t
				case(0xDA): OP_set();	break; // SET 3, D		2b	8t
				case(0xDB): OP_set();	break; // SET 3, E		2b	8t
				case(0xDC): OP_set();	break; // SET 3, H		2b	8t
				case(0xDD): OP_set();	break; // SET 3, L		2b	8t
				case(0xDE): OP_set();	break; // SET 3, (HL)	2b	16t
				case(0xDF): OP_set();	break; // SET 3, A		2b	8t
				case(0xE0): OP_set();	break; // SET 4, B		2b	8t
				case(0xE1): OP_set();	break; // SET 4, C		2b	8t
				case(0xE2): OP_set();	break; // SET 4, D		2b	8t
				case(0xE3): OP_set();	break; // SET 4, E		2b	8t
				case(0xE4): OP_set();	break; // SET 4, H		2b	8t
				case(0xE5): OP_set();	break; // SET 4, L		2b	8t
				case(0xE6): OP_set();	break; // SET 4, (HL)	2b	16t
				case(0xE7): OP_set();	break; // SET 4, A		2b	8t
				case(0xE8): OP_set();	break; // SET 5, B		2b	8t
				case(0xE9): OP_set();	break; // SET 5, C		2b	8t
				case(0xEA): OP_set();	break; // SET 5, D		2b	8t
				case(0xEB): OP_set();	break; // SET 5, E		2b	8t
				case(0xEC): OP_set();	break; // SET 5, H		2b	8t
				case(0xED): OP_set();	break; // SET 5, L		2b	8t
				case(0xEE): OP_set();	break; // SET 5, (HL)	2b	16t
				case(0xEF): OP_set();	break; // SET 5, A		2b	8t
				case(0xF0): OP_set();	break; // SET 6, B		2b	8t
				case(0xF1): OP_set();	break; // SET 6, C		2b	8t
				case(0xF2): OP_set();	break; // SET 6, D		2b	8t
				case(0xF3): OP_set();	break; // SET 6, E		2b	8t
				case(0xF4): OP_set();	break; // SET 6, H		2b	8t
				case(0xF5): OP_set();	break; // SET 6, L		2b	8t
				case(0xF6): OP_set();	break; // SET 6, (HL)	2b	16t
				case(0xF7): OP_set();	break; // SET 6, A		2b	8t
				case(0xF8): OP_set();	break; // SET 7, B		2b	8t
				case(0xF9): OP_set();	break; // SET 7, C		2b	8t
				case(0xFA): OP_set();	break; // SET 7, D		2b	8t
				case(0xFB): OP_set();	break; // SET 7, E		2b	8t
				case(0xFC): OP_set();	break; // SET 7, H		2b	8t
				case(0xFD): OP_set();	break; // SET 7, L		2b	8t
				case(0xFE): OP_set();	break; // SET 7, (HL)	2b	16t
				case(0xFF): OP_set();	break; // SET 7, A		2b	8t
				default: OP_nop();	break; // NOP				1b	4t
			} break;
		case(0xCC): OP_call();	break;	// CALL Z, u16			3b	12t-24t
		case(0xCD): OP_call();	break;	// CALL u16				3b	24t
		case(0xCE): OP_adc();	break;	// ADC A, u8			2b	8t
		case(0xCF):	OP_rst();	break;	// RST 08h				1b	16t
		case(0xD0): OP_ret();	break;	// RET NC				1b	8t-20t
		case(0xD1): OP_pop();	break;	// POP DE				1b	12t
		case(0xD2): OP_jp();	break;	// JP NC, u16			3b	12t-16t
		//case(0xD3):	break;
		case(0xD4): OP_call();	break;	// CALL NC, u16			3b	12t-24t
		case(0xD5): OP_push();	break;	// PUSH DE				1b	16t
		case(0xD6): OP_sub();	break;	// SUB A, u8			2b	8t
		case(0xD7): OP_rst();	break;	// RST 10h				1b	16t
		case(0xD8): OP_ret();	break;	// RET C				1b	8t-20t
		case(0xD9): OP_reti();	break;	// RETI					1b	16t
		case(0xDA):	OP_jp();	break;	// JP C, u16			3b	12t-16t
		//case(0xDB):	break;
		case(0xDC):	OP_call();	break;	// CALL C, u16			3b	12t-24t
		//case(0xDD):	break;
		case(0xDE):	OP_sbc();	break;	// SBC A, u8			2b	8t
		case(0xDF):	OP_rst();	break;	// RST 18h				1b	16t
		case(0xE0):	OP_ld();	break;	// LD (FF00+u8), A		2b	12t
		case(0xE1):	OP_pop();	break;	// POP HL				1b	12t
		case(0xE2):	OP_ld();	break;	// LD (FF00+C), A		1b	8t
		//case(0xE3):	break; 
		//case(0xE4):	break; 
		case(0xE5):	OP_push();	break;	// PUSH HL				1b	16t
		case(0xE6):	OP_and();	break;	// AND A, u8			2b	8t
		case(0xE7):	OP_rst();	break;	// RST 20h				1b	16t
		case(0xE8):	OP_add16();	break;	// ADD SP, i8			2b	16t
		case(0xE9):	OP_jp();	break;	// JP HL				1b	4t
		case(0xEA): OP_ld();	break;	// LD (u16), A			3b	16t
		//case(0xEB):	break;
		//case(0xEC):	break;
		//case(0xED):	break;
		case(0xEE): OP_xor();	break;	// XOR A,u8				2b	8t
		case(0xEF): OP_rst();	break;	// RST 28h				1b	16t
		case(0xF0):	OP_ld();	break;	// LD A, (FF00+u8)		2b	12t
		case(0xF1):	OP_pop();	break;	// POP AF				1b	12t
		case(0xF2):	OP_ld();	break;	// LD A, (FF00+C)		1b	8t
		case(0xF3): OP_di();	break;	// DI					1b	4t
		//case(0xF4):	break;
		case(0xF5):	OP_push();	break;	// PUSH AF				1b	16t
		case(0xF6):	OP_or();	break;	// OR A, u8				2b	8t
		case(0xF7):	OP_rst();	break;	// RST 30h				1b	16t
		case(0xF8):	OP_ld16();	break;	// LD HL, SP+i8			2b	12t
		case(0xF9): OP_ld16();	break;	// LD SP, HL			1b	8t
		case(0xFA): OP_ld();	break;	// LD A, (u16)			3b	16t
		case(0xFB):	OP_ei();	break;	// EI					1b	4t
		//case(0xFC):	break;
		//case(0xFD):	break;
		case(0xFE):	OP_cp();	break;	// CP A, u8				2b	8t
		case(0xFF):	OP_rst();	break;	// RST 38h				1b	16t
		default: OP_nop();	break; // NOP						1b	4t
	}
}

uint8_t Cpu::read(uint16_t address) {
	return memoryController->read(address);
}

void Cpu::write(uint16_t address, uint8_t value) {
	memoryController->write(address, value);
}

uint8_t Cpu::read8() {
	return read(pc++);
}

uint16_t Cpu::read16() {
	uint8_t loByte = read8();
	uint8_t hiByte = read8();
	return (hiByte << 8) | loByte;
}

// NOP: No operation
// Opcodes: 0x00
void Cpu::OP_nop() {
	return;
}

// STOP: Enter CPU very low power mode
// Opcodes: 0x10
void Cpu::OP_stop() {
	lowPowerMode = true;
}

// JR: Relative Jump
void Cpu::OP_jr(bool cc) {
	uint8_t e = read8();
	if (cc) {
		pc += e;
	}
}

// LD 16-Bit: Load value into register
void Cpu::OP_ld16() {
	switch(opcode) {
		//LD BC, u16
		case(0x01):
			uint8_t e = read16();
			registers.setBC(e);
			break;
		//LD (u16), SP
		case(0x08):
			uint8_t e = read16();
			write(e, sp & 0x00FF);
			write(e + 1, sp >> 8);
			break;
		//LD DE, u16
		case(0x11):
			uint8_t e = read16();
			registers.setDE(e);
			break;
		//LD HL, u16 
		case(0x21):
			uint8_t e = read16();
			registers.setHL(e);
			break;
		//LD SP, u16
		case(0x31):
			uint8_t e = read16();
			sp = e;
			break;
		//LD HL,SP+i8
		case(0xF8):
			uint8_t e = read8();
			registers.setHL(sp + e);
			registers.setFlagH(sp + e, sp, e);
			registers.setFlagC(sp + e);
			break;
		//LD SP, HL
		case(0xF9):
			sp = registers.getHL();
			break;
	}
}

// LD 8-Bit: Load value into register.
// Opcodes: 0x40-0x7F (Except 0x76), 0x02, 0x12, 0x22, 0x32, 0x06, 0x16, 0x26, 0x36, 0x0A, 0x1A, 0x2A, 0x3A, 0x0E, 0x1E, 0x2E, 0x3E, 0xE0, 0xF0, 0xE2, 0xF2, 0xEA, 0xFA
void Cpu::OP_ld() {
}

// INC 16-Bit: Increment value in register by 1
void Cpu::OP_inc16() {
	switch(opcode) {
		// INC BC
		case(0x03):
			registers.setBC(registers.getBC() + 1);
			break;
		// INC DE
		case(0x13):
			registers.setDE(registers.getDE() + 1);
			break;
		// INC HL
		case(0x23):
			registers.setHL(registers.getHL() + 1);
			break;
		// INC SP
		case(0x33):
			sp += 1;
			break;
	}
}

// INC 8-Bit: Increment value in register r8 by 1.
// Opcodes: 0x04, 0x14, 0x24, 0x34, 0x0C, 0x1C, 0x2C, 0x3C
void Cpu::OP_inc() {

}

// DEC 8-Bit: Decrement value in register r8 by 1.
// Opcodes: 0x05, 0x15, 0x25, 0x35, 0x0D, 0x1D, 0x2D, 0x3D
void Cpu::OP_dec() {
}

// RLCA: Rotate register A left. C <- [7 <- 0] <- [7]
// Opcodes: 0x07
void Cpu::OP_rlca() {
}

// RLA: Rotate register A left through carry. C <- [7 <- 0] <- C
// Opcodes: 0x17
void Cpu::OP_rla() {
}

// DAA: Decimal Adjust Accumulator to get a correct BCD representation after an arithmetic instruction. 
// Opcodes: 0x27
void Cpu::OP_daa() {
}

// SCF: Set Carry Flag.
// Opcodes: 0x37
void Cpu::OP_scf() {
}

// ADD 16-Bit: Add the value in r16 to HL.
// Opcodes: 0x09, 0x19, 0x29, 0x39
void Cpu::OP_add16() {
}

// DEC 16-Bit: Decrement value in register r16 by 1.
// Opcodes: 0x0B, 0x1B, 0x2B, 0x3B
void Cpu::OP_dec16() {
}

// RRCA: Rotate register A right. [0] -> [7 -> 0] -> C
// Opcodes: 0x0F
void Cpu::OP_rrca() {
}

// RRA: Rotate register A right through carry. C -> [7 -> 0] -> C
// Opcodes: 0x1F
void Cpu::OP_rra() {
}

// CPL: ComPLement accumulator (A = ~A). 
// Opcodes: 0x2F
void Cpu::OP_cpl() {
}

// CCF: Complement Carry Flag. 
// Opcodes: 0x3F
void Cpu::OP_ccf() {
}

// HALT: Enter CPU low-power consumption mode until an interrupt occurs.
void Cpu::OP_halt() {
	lowPowerMode = true;
}

// ADD 8-Bit: Add value to A. 
// Opcodes: 0x80 - 0x87, 0xC6
void Cpu::OP_add() {
}

// ADC 8-Bit: Add the value in r8 plus the carry flag to A.
// Opcodes: 0x88 - 0x8F, 0xCE
void Cpu::OP_adc() {
}

// SUB 8-Bit: Subtract the value in r8 from A. 
// Opcodes: 0x90 - 0x97, 0xD6
void Cpu::OP_sub() {
}

// SBC 8-Bit: Subtract the value in r8 and the carry flag from A. 
// Opcodes: 0x98 - 0x9F, 0xDE
void Cpu::OP_sbc() {
}

// AND 8-Bit: Bitwise AND between the value in r8 and A. 
// Opcodes: 0xA0 - 0xA7, 0xE6
void Cpu::OP_and() {
}

// XOR 8-Bit: Bitwise XOR between the value in r8 and A. 
// Opcodes: 0xA8 - 0xAF, 0xEE
void Cpu::OP_xor() {
}

// OR 8-Bit: Store into A the bitwise OR of the value in r8 and A.
// Opcodes: 0xB0 - 0xB7, 0xF6
void Cpu::OP_or() {
}

// CP 8-Bit: Subtract the value in r8 from A and set flags accordingly, but don't store the result. This is useful for ComParing values. 
// Opcodes: 0xB8 - 0xBF, 0xFE
void Cpu::OP_cp() {
}

// RET: Return from subroutine. This is basically a POP PC (if such an instruction existed).
// Opcodes: 0xC0, 0xD0, 0xC8, 0xD8, 0xC9
void Cpu::OP_ret() {
}

// POP 16-Bit: Pop register r16 from the stack.
// Opcodes: 0xC1, 0xD1, 0xE1, 0xF1
void Cpu::OP_pop() {
}

// JP: Jump to address n16; effectively, store n16 into PC. 
// Opcodes: 0xC2, 0xD2, 0xC3, 0xE9, 0xCA, 0xDA
void Cpu::OP_jp() {
}

// DI: Disable Interrupts by clearing the IME flag. 
// Opcodes: 0xF3
void Cpu::OP_di() {
}

// CALL: Call address.
// Opcodes: 0xC4, 0xD4, 0xCC, 0xDC, 0xCD
void Cpu::OP_call() {
}

// PUSH 16-Bit: Push register into the stack.
// Opcodes: 0xC5, 0xD5, 0xE5, 0xF5
void Cpu::OP_push() {
}

// RST: Call address vec.
// Opcodes: 0xC7, 0xD7, 0xE7, 0xF7, 0xCF, 0xDF, 0xEF, 0xFF
void Cpu::OP_rst() {
}

// RETI: Return from subroutine and enable interrupts.
// Opcodes: 0xD9
void Cpu::OP_reti() {
}

// EI: Enable Interrupts by setting the IME flag. The flag is only set after the instruction following EI. 
// Opcodes: 0xFB
void Cpu::OP_ei() {
}

// RLC: Rotate register r8 left. C <- [7 <- 0] <- [7]
// Opcodes: 0x00 - 0x07
void Cpu::OP_rlc() {
}

// RRC: Rotate register r8 right. [0] -> [7 -> 0] -> C
// Opcodes: 0x08 - 0x0F
void Cpu::OP_rrc() {
}

// RL: Rotate bits in register r8 left through carry. C <- [7 <- 0] <- C
// Opcodes: 0x10 - 0x17
void Cpu::OP_rl() {
}

// RR: Rotate register r8 right through carry. C -> [7 -> 0] -> C
// Opcodes: 0x18 - 0x1F
void Cpu::OP_rr() {
}

// SLA: Shift Left Arithmetic register r8. C <- [7 <- 0] <- 0
// Opcodes: 0x20 - 0x27
void Cpu::OP_sla() {
}

// SRA: Shift Right Arithmetic register r8. [7] -> [7 -> 0] -> C
// Opcodes: 0x28 - 0x2F
void Cpu::OP_sra() {
}

// SWAP: Swap upper 4 bits in register r8 and the lower 4 ones. 
// Opcodes: 0x30 - 0x37
void Cpu::OP_swap() {
}

// SRL: Shift Right Logic register r8. 0 -> [7 -> 0] -> C
// Opcodes: 0x38 - 0x3F
void Cpu::OP_srl() {
}

// BIT: Test bit in register, set the zero flag if bit not set
void Cpu::OP_bit(uint8_t v, uint8_t reg) {
	registers.setFlagZ(reg);
	registers.setFlagN(false);
    registers.setFlagH(0xFF, 0x00, 0x00);
}

// RES: Set bit in register to 0.
// Opcodes: 0x80 - 0xBF
void Cpu::OP_res() {
}

// SET: Set bit in register to 1. 
// Opcodes: 0xC0 - 0xFF
void Cpu::OP_set() {
}