#include "Cpu.h"
#include "MemoryController.h"

Cpu::Cpu(MemoryController *memoryController) {
	this->memoryController = memoryController;

	// DEBUG
	pc = 0x100;
	sp = 0xFFFE;
}

Cpu::~Cpu() {
}

void Cpu::cycle() {
	if (cycles == 0) {
		opcode = read(pc);
		pc++;
		executeInstruction();
		//std::cout << "Instruction: " << "0x" << std::hex << (unsigned)opcode << " | " << (unsigned)read(pc) << std::dec << std::endl;
	}
}

void Cpu::executeInstruction() {
	std::cout << "Instruction: " << "0x" << std::hex << (unsigned)(opcode) << std::dec << std::endl;
	switch(opcode) {
		case(0x00): OP_nop();	break;	// NOP
		case(0x01): OP_ld16();	break;	// LD BC, u16
		case(0x02): OP_ld();	break;	// LD (BC), A
		case(0x03): OP_inc16();	break;	// INC BC
		case(0x04): OP_inc();	break;	// INC B
		case(0x05): OP_dec();	break;	// DEC B
		case(0x06):	OP_ld();	break;	// LD B, u8		case(0x07):	OP_rlca();	break;	// RLCA		case(0x08):	OP_ld16();	break;	// LD (u16), SP		case(0x09):	OP_add16();	break;	// ADD HL, BC		case(0x0A):	OP_ld();	break;	// LD A, (BC)		case(0x0B):	OP_dec16();	break;	// DEC BC		case(0x0C):	OP_inc();	break;	// INC C		case(0x0D):	OP_dec();	break;	// DEC C		case(0x0E):	OP_ld();	break;	// LD C, u8		case(0x0F):	OP_rrca();	break;	// RRCA		case(0x10):	OP_stop();	break;	// STOP		case(0x11):	OP_ld16();	break;	// LD DE, u16		case(0x12):	OP_ld();	break;	// LD (DE), A		case(0x13):	OP_inc16();	break;	// INC DE		case(0x14):	OP_inc();	break;	// INC D		case(0x15):	OP_dec();	break;	// DEC D
		case(0x16):	OP_ld();	break;	// LD D, u8
		case(0x17):	OP_rla();	break;	// RLA
		case(0x18):	OP_jr();	break;	// JR i8
		case(0x19):	OP_add16();	break;	// ADD HL, DE
		case(0x1A):	OP_ld();	break;	// LD A, (DE)
		case(0x1B): OP_dec16();	break;	// DEC DE
		case(0x1C): OP_inc();	break;	// INC E
		case(0x1D): OP_inc();	break;	// DEC E
		case(0x1E):	OP_ld();	break;	// LD E, u8
		case(0x1F): OP_rra();	break;	// RRA
		case(0x20): OP_jr();	break;	// JR NZ,i8
		case(0x21): OP_ld16();	break;	// LD HL, u16
		case(0x22): OP_ld();	break;	// LD (HL+), A
		case(0x23): OP_inc16();	break;	// INC HL
		case(0x24): OP_inc();	break;	// INC H
		case(0x25): OP_dec();	break;	// DEC H
		case(0x26): OP_ld();	break;	// LD H, u8
		case(0x27): OP_daa();	break;	// DAA
		case(0x28): OP_jr();	break;	// JR Z, i8
		case(0x29): OP_add16();	break;	// ADD HL, HL
		case(0x2A): OP_ld();	break;	// LD A, (HL+)
		case(0x2B): OP_dec16();	break;	// DEC HL
		case(0x2C): OP_inc();	break;	// INC L
		case(0x2D): OP_dec();	break;	// DEC L
		case(0x2E): OP_ld();	break;	// LD L, u8
		case(0x2F): OP_cpl();	break;	// CPL
		case(0x30): OP_jr();	break;	// JR NC, i8
		case(0x31):	OP_ld16();	break;	// LD SP, u16
		case(0x32): OP_ld();	break;	// LD (HL-), A
		case(0x33): OP_inc16();	break;	// INC SP
		case(0x34): OP_inc();	break;	// INC (HL)
		case(0x35): OP_dec();	break;	// DEC (HL)
		case(0x36): OP_ld();	break;	// LD (HL), u8
		case(0x37): OP_scf();	break;	// SCF
		case(0x38): OP_jr();	break;	// JR C, i8
		case(0x39): OP_add16();	break;	// ADD HL, SP
		case(0x3A): OP_ld();	break;	// LD A, (HL-)
		case(0x3B):	OP_dec16();	break;	// DEC SP
		case(0x3C):	OP_inc();	break;	// INC A
		case(0x3D): OP_dec();	break;	// DEC A
		case(0x3E): OP_ld();	break;	// LD A, u8
		case(0x3F): OP_ccf();	break;	// CCF
		case(0x40): OP_ld();	break;	// LD B, B
		case(0x41):	OP_ld();	break;	// LD B, C
		case(0x42):	OP_ld();	break;	// LD B, D
		case(0x43):	OP_ld();	break;	// LD B, E
		case(0x44):	OP_ld();	break;	// LD B, H
		case(0x45):	OP_ld();	break;	// LD B, L
		case(0x46):	OP_ld();	break;	// LD B, (HL)
		case(0x47):	OP_ld();	break;	// LD B, A
		case(0x48):	OP_ld();	break;	// LD C, B
		case(0x49):	OP_ld();	break;	// LD C, C
		case(0x4A):	OP_ld();	break;	// LD C, D
		case(0x4B):	OP_ld();	break;	// LD C, E
		case(0x4C):	OP_ld();	break;	// LD C, H
		case(0x4D):	OP_ld();	break;	// LD C, L
		case(0x4E):	OP_ld();	break;	// LD C, (HL)
		case(0x4F):	OP_ld();	break;	// LD C, A
		case(0x50):	OP_ld();	break;	// LD D, B
		case(0x51):	OP_ld();	break;	// LD D, C
		case(0x52):	OP_ld();	break;	// LD D, D
		case(0x53):	OP_ld();	break;	// LD D, E
		case(0x54):	OP_ld();	break;	// LD D, H
		case(0x55):	OP_ld();	break;	// LD D, L
		case(0x56):	OP_ld();	break;	// LD D, (HL)
		case(0x57):	OP_ld();	break;	// LD D, A
		case(0x58):	OP_ld();	break;	// LD E, B
		case(0x59):	OP_ld();	break;	// LD E, C
		case(0x5A):	OP_ld();	break;	// LD E, D
		case(0x5B):	OP_ld();	break;	// LD E, E
		case(0x5C):	OP_ld();	break;	// LD E, H
		case(0x5D):	OP_ld();	break;	// LD E, L
		case(0x5E):	OP_ld();	break;	// LD E, (HL)
		case(0x5F):	OP_ld();	break;	// LD E, A
		case(0x60):	OP_ld();	break;	// LD H, B
		case(0x61):	OP_ld();	break;	// LD H, C
		case(0x62):	OP_ld();	break;	// LD H, D
		case(0x63):	OP_ld();	break;	// LD H, E
		case(0x64):	OP_ld();	break;	// LD H, H
		case(0x65):	OP_ld();	break;	// LD H, L
		case(0x66):	OP_ld();	break;	// LD H, (HL)
		case(0x67):	OP_ld();	break;	// LD H, A
		case(0x68):	OP_ld();	break;	// LD L, B
		case(0x69):	OP_ld();	break;	// LD L, C
		case(0x6A):	OP_ld();	break;	// LD L, D
		case(0x6B):	OP_ld();	break;	// LD L, E
		case(0x6C):	OP_ld();	break;	// LD L, H
		case(0x6D):	OP_ld();	break;	// LD L, L
		case(0x6E):	OP_ld();	break;	// LD L, (HL)
		case(0x6F):	OP_ld();	break;	// LD L, A
		case(0x70):	OP_ld();	break;	// LD (HL), B
		case(0x71):	OP_ld();	break;	// LD (HL), C
		case(0x72):	OP_ld();	break;	// LD (HL), D
		case(0x73):	OP_ld();	break;	// LD (HL), E
		case(0x74):	OP_ld();	break;	// LD (HL), H
		case(0x75):	OP_ld();	break;	// LD (HL), L
		case(0x76): OP_halt();	break;	// HALT 
		case(0x77):	OP_ld();	break;	// LD (HL), A
		case(0x78):	OP_ld();	break;	// LD A, B
		case(0x79):	OP_ld();	break;	// LD A, C
		case(0x7A):	OP_ld();	break;	// LD A, D
		case(0x7B):	OP_ld();	break;	// LD A, E
		case(0x7C):	OP_ld();	break;	// LD A, H
		case(0x7D):	OP_ld();	break;	// LD A, L
		case(0x7E):	OP_ld();	break;	// LD A, (HL)
		case(0x7F):	OP_ld();	break;	// LD A, A
		case(0x80): OP_add();	break;	// ADD A, B
		case(0x81): OP_add();	break;	// ADD A, C
		case(0x82): OP_add();	break;	// ADD A, D
		case(0x83): OP_add();	break;	// ADD A, E
		case(0x84): OP_add();	break;	// ADD A, H
		case(0x85): OP_add();	break;	// ADD A, L
		case(0x86): OP_add();	break;	// ADD A, (HL)
		case(0x87): OP_add();	break;	// ADD A, A
		case(0x88): OP_adc();	break;	// ADC A, B
		case(0x89): OP_adc();	break;	// ADC A, C
		case(0x8A): OP_adc();	break;	// ADC A, D
		case(0x8B): OP_adc();	break;	// ADC A, E
		case(0x8C): OP_adc();	break;	// ADC A, H
		case(0x8D): OP_adc();	break;	// ADC A, L
		case(0x8E): OP_adc();	break;	// ADC A, (HL)
		case(0x8F): OP_adc();	break;	// ADC A, A
		case(0x90): OP_sub();	break;	// SUB A, B
		case(0x91): OP_sub();	break;	// SUB A, C
		case(0x92): OP_sub();	break;	// SUB A, D
		case(0x93): OP_sub();	break;	// SUB A, E
		case(0x94): OP_sub();	break;	// SUB A, H
		case(0x95): OP_sub();	break;	// SUB A, L
		case(0x96): OP_sub();	break;	// SUB A, (HL)
		case(0x97): OP_sub();	break;	// SUB A, A
		case(0x98): OP_sbc();	break;	// SBC A, B
		case(0x99): OP_sbc();	break;	// SBC A, C		case(0x9A): OP_sbc();	break;	// SBC A, D
		case(0x9B): OP_sbc();	break;	// SBC A, E
		case(0x9C): OP_sbc();	break;	// SBC A, H
		case(0x9D): OP_sbc();	break;	// SBC A, L
		case(0x9E): OP_sbc();	break;	// SBC A, (HL)
		case(0x9F): OP_sbc();	break;	// SBC A, A
		case(0xA0): OP_and();	break;	// AND A, B
		case(0xA1): OP_and();	break;	// AND A, C
		case(0xA2): OP_and();	break;	// AND A, D
		case(0xA3): OP_and();	break;	// AND A, E
		case(0xA4): OP_and();	break;	// AND A, H
		case(0xA5): OP_and();	break;	// AND A, L
		case(0xA6): OP_and();	break;	// AND A, (HL)
		case(0xA7): OP_and();	break;	// AND A, A
		case(0xA8): OP_xor();	break;	// XOR A, B
		case(0xA9): OP_xor();	break;	// XOR A, C
		case(0xAA): OP_xor();	break;	// XOR A, D
		case(0xAB): OP_xor();	break;	// XOR A, E
		case(0xAC): OP_xor();	break;	// XOR A, H
		case(0xAD): OP_xor();	break;	// XOR A, L
		case(0xAE): OP_xor();	break;	// XOR A, (HL)
		case(0xAF): OP_xor();	break;	// XOR A, A
		case(0xB0): OP_or();	break;	// OR A, B
		case(0xB1): OP_or();	break;	// OR A, C
		case(0xB2): OP_or();	break;	// OR A, D
		case(0xB3): OP_or();	break;	// OR A, E
		case(0xB4): OP_or();	break;	// OR A, H
		case(0xB5): OP_or();	break;	// OR A, L
		case(0xB6): OP_or();	break;	// OR A,(HL)
		case(0xB7): OP_or();	break;	// OR A, A
		case(0xB8): OP_cp();	break;	// CP A, B
		case(0xB9): OP_cp();	break;	// CP A, C
		case(0xBA): OP_cp();	break;	// CP A, D
		case(0xBB): OP_cp();	break;	// CP A, E
		case(0xBC): OP_cp();	break;	// CP A, H
		case(0xBD): OP_cp();	break;	// CP A, L
		case(0xBE): OP_cp();	break;	// CP A, (HL)
		case(0xBF): OP_cp();	break;	// CP A, A
		case(0xC0): OP_ret();	break;	// RET NZ
		case(0xC1): OP_pop();	break;	// POP BC
		case(0xC2): OP_jp();	break;	// JP NZ, u16
		case(0xC3): OP_jp();	break;	// JP u16
		case(0xC4): OP_call();	break;	// CALL NZ, u16
		case(0xC5):	OP_push();	break;	// PUSH BC
		case(0xC6): OP_add();	break;	// ADD A, u8
		case(0xC7): OP_rst();	break;	// RST 00h
		case(0xC8): OP_ret();	break;	// RET Z
		case(0xC9): OP_ret();	break;	// RET
		case(0xCA): OP_jp();	break;	// JP Z, u16
		case(0xCB):	
			switch(0) {	// GET NEXT BYTE
				case(0x00): OP_rlc();	break; // RLC B
				case(0x01): OP_rlc();	break; // RLC C
				case(0x02): OP_rlc();	break; // RLC D
				case(0x03): OP_rlc();	break; // RLC E
				case(0x04): OP_rlc();	break; // RLC H
				case(0x05): OP_rlc();	break; // RLC L
				case(0x06): OP_rlc();	break; // RLC (HL)
				case(0x07): OP_rlc();	break; // RLC A
				case(0x08): OP_rrc();	break; // RRC B
				case(0x09): OP_rrc();	break; // RRC C
				case(0x0A): OP_rrc();	break; // RRC D
				case(0x0B): OP_rrc();	break; // RRC E
				case(0x0C): OP_rrc();	break; // RRC H
				case(0x0D): OP_rrc();	break; // RRC L
				case(0x0E): OP_rrc();	break; // RRC (HL)
				case(0x0F): OP_rrc();	break; // RRC A
				case(0x10): OP_rl();	break; // RL B
				case(0x11): OP_rl();	break; // RL C
				case(0x12): OP_rl();	break; // RL D
				case(0x13): OP_rl();	break; // RL E
				case(0x14): OP_rl();	break; // RL H
				case(0x15): OP_rl();	break; // RL L
				case(0x16): OP_rl();	break; // RL (HL)
				case(0x17): OP_rl();	break; // RL A
				case(0x18): OP_rr();	break; // RR B
				case(0x19): OP_rr();	break; // RR C
				case(0x1A): OP_rr();	break; // RR D
				case(0x1B): OP_rr();	break; // RR E
				case(0x1C): OP_rr();	break; // RR H
				case(0x1D): OP_rr();	break; // RR L
				case(0x1E): OP_rr();	break; // RR (HL)
				case(0x1F): OP_rr();	break; // RR A
				case(0x20): OP_sla();	break; // SLA B
				case(0x21): OP_sla();	break; // SLA C
				case(0x22): OP_sla();	break; // SLA D
				case(0x23): OP_sla();	break; // SLA E
				case(0x24): OP_sla();	break; // SLA H
				case(0x25): OP_sla();	break; // SLA L
				case(0x26): OP_sla();	break; // SLA (HL)
				case(0x27): OP_sla();	break; // SLA A
				case(0x28): OP_sra();	break; // SRA B
				case(0x29): OP_sra();	break; // SRA C
				case(0x2A): OP_sra();	break; // SRA D
				case(0x2B): OP_sra();	break; // SRA E
				case(0x2C): OP_sra();	break; // SRA H
				case(0x2D): OP_sra();	break; // SRA L
				case(0x2E): OP_sra();	break; // SRA (HL)
				case(0x2F): OP_sra();	break; // SRA A
				case(0x30): OP_swap();	break; // SWAP B
				case(0x31): OP_swap();	break; // SWAP C
				case(0x32): OP_swap();	break; // SWAP D
				case(0x33): OP_swap();	break; // SWAP E
				case(0x34): OP_swap();	break; // SWAP H
				case(0x35): OP_swap();	break; // SWAP L
				case(0x36): OP_swap();	break; // SWAP (HL)
				case(0x37): OP_swap();	break; // SWAP A
				case(0x38): OP_srl();	break; // SRL B
				case(0x39): OP_srl();	break; // SRL C
				case(0x3A): OP_srl();	break; // SRL D
				case(0x3B): OP_srl();	break; // SRL E
				case(0x3C): OP_srl();	break; // SRL H
				case(0x3D): OP_srl();	break; // SRL L
				case(0x3E): OP_srl();	break; // SRL (HL)
				case(0x3F): OP_srl();	break; // SRL A
				case(0x40): OP_bit();	break; // BIT 0, B
				case(0x41): OP_bit();	break; // BIT 0, C
				case(0x42): OP_bit();	break; // BIT 0, D
				case(0x43): OP_bit();	break; // BIT 0, E
				case(0x44): OP_bit();	break; // BIT 0, H
				case(0x45): OP_bit();	break; // BIT 0, L
				case(0x46): OP_bit();	break; // BIT 0, (HL)
				case(0x47): OP_bit();	break; // BIT 0, A
				case(0x48): OP_bit();	break; // BIT 1, B
				case(0x49): OP_bit();	break; // BIT 1, C
				case(0x4A): OP_bit();	break; // BIT 1, D
				case(0x4B): OP_bit();	break; // BIT 1, E
				case(0x4C): OP_bit();	break; // BIT 1, H
				case(0x4D): OP_bit();	break; // BIT 1, L
				case(0x4E): OP_bit();	break; // BIT 1, (HL)
				case(0x4F): OP_bit();	break; // BIT 1, A
				case(0x50): OP_bit();	break; // BIT 2, B
				case(0x51): OP_bit();	break; // BIT 2, C
				case(0x52): OP_bit();	break; // BIT 2, D
				case(0x53): OP_bit();	break; // BIT 2, E
				case(0x54): OP_bit();	break; // BIT 2, H
				case(0x55): OP_bit();	break; // BIT 2, L
				case(0x56): OP_bit();	break; // BIT 2, (HL)
				case(0x57): OP_bit();	break; // BIT 2, A
				case(0x58): OP_bit();	break; // BIT 3, B
				case(0x59): OP_bit();	break; // BIT 3, C
				case(0x5A): OP_bit();	break; // BIT 3, D
				case(0x5B): OP_bit();	break; // BIT 3, E
				case(0x5C): OP_bit();	break; // BIT 3, H
				case(0x5D): OP_bit();	break; // BIT 3, L
				case(0x5E): OP_bit();	break; // BIT 3, (HL)
				case(0x5F): OP_bit();	break; // BIT 3, A
				case(0x60): OP_bit();	break; // BIT 4, B
				case(0x61): OP_bit();	break; // BIT 4, C
				case(0x62): OP_bit();	break; // BIT 4, D
				case(0x63): OP_bit();	break; // BIT 4, E
				case(0x64): OP_bit();	break; // BIT 4, H
				case(0x65): OP_bit();	break; // BIT 4, L
				case(0x66): OP_bit();	break; // BIT 4, (HL)
				case(0x67): OP_bit();	break; // BIT 4, A
				case(0x68): OP_bit();	break; // BIT 5, B
				case(0x69): OP_bit();	break; // BIT 5, C
				case(0x6A): OP_bit();	break; // BIT 5, D
				case(0x6B): OP_bit();	break; // BIT 5, E
				case(0x6C): OP_bit();	break; // BIT 5, H
				case(0x6D): OP_bit();	break; // BIT 5, L
				case(0x6E): OP_bit();	break; // BIT 5, (HL)
				case(0x6F): OP_bit();	break; // BIT 5, A
				case(0x70): OP_bit();	break; // BIT 6, B
				case(0x71): OP_bit();	break; // BIT 6, C
				case(0x72): OP_bit();	break; // BIT 6, D
				case(0x73): OP_bit();	break; // BIT 6, E
				case(0x74): OP_bit();	break; // BIT 6, H
				case(0x75): OP_bit();	break; // BIT 6, L
				case(0x76): OP_bit();	break; // BIT 6, (HL)
				case(0x77): OP_bit();	break; // BIT 6, A
				case(0x78): OP_bit();	break; // BIT 7, B
				case(0x79): OP_bit();	break; // BIT 7, C
				case(0x7A): OP_bit();	break; // BIT 7, D
				case(0x7B): OP_bit();	break; // BIT 7, E
				case(0x7C): OP_bit();	break; // BIT 7, H
				case(0x7D): OP_bit();	break; // BIT 7, L
				case(0x7E): OP_bit();	break; // BIT 7, (HL)
				case(0x7F): OP_bit();	break; // BIT 7, A
				case(0x80): OP_res();	break; // RES 0, B
				case(0x81): OP_res();	break; // RES 0, C
				case(0x82): OP_res();	break; // RES 0, D
				case(0x83): OP_res();	break; // RES 0, E
				case(0x84): OP_res();	break; // RES 0, H
				case(0x85): OP_res();	break; // RES 0, L
				case(0x86): OP_res();	break; // RES 0, (HL)
				case(0x87): OP_res();	break; // RES 0, A
				case(0x88): OP_res();	break; // RES 1, B
				case(0x89): OP_res();	break; // RES 1, C
				case(0x8A): OP_res();	break; // RES 1, D
				case(0x8B): OP_res();	break; // RES 1, E
				case(0x8C): OP_res();	break; // RES 1, H
				case(0x8D): OP_res();	break; // RES 1, L
				case(0x8E): OP_res();	break; // RES 1, (HL)
				case(0x8F): OP_res();	break; // RES 1, A
				case(0x90): OP_res();	break; // RES 2, B
				case(0x91): OP_res();	break; // RES 2, C
				case(0x92): OP_res();	break; // RES 2, D
				case(0x93): OP_res();	break; // RES 2, E
				case(0x94): OP_res();	break; // RES 2, H
				case(0x95): OP_res();	break; // RES 2, L
				case(0x96): OP_res();	break; // RES 2, (HL)
				case(0x97): OP_res();	break; // RES 2, A
				case(0x98): OP_res();	break; // RES 3, B
				case(0x99): OP_res();	break; // RES 3, C
				case(0x9A): OP_res();	break; // RES 3, D
				case(0x9B): OP_res();	break; // RES 3, E
				case(0x9C): OP_res();	break; // RES 3, H
				case(0x9D): OP_res();	break; // RES 3, L
				case(0x9E): OP_res();	break; // RES 3, (HL)
				case(0x9F): OP_res();	break; // RES 3, A
				case(0xA0): OP_res();	break; // RES 4, B
				case(0xA1): OP_res();	break; // RES 4, C
				case(0xA2): OP_res();	break; // RES 4, D
				case(0xA3): OP_res();	break; // RES 4, E
				case(0xA4): OP_res();	break; // RES 4, H
				case(0xA5): OP_res();	break; // RES 4, L
				case(0xA6): OP_res();	break; // RES 4, (HL)
				case(0xA7): OP_res();	break; // RES 4, A
				case(0xA8): OP_res();	break; // RES 5, B
				case(0xA9): OP_res();	break; // RES 5, C
				case(0xAA): OP_res();	break; // RES 5, D
				case(0xAB): OP_res();	break; // RES 5, E
				case(0xAC): OP_res();	break; // RES 5, H
				case(0xAD): OP_res();	break; // RES 5, L
				case(0xAE): OP_res();	break; // RES 5, (HL)
				case(0xAF): OP_res();	break; // RES 5, A
				case(0xB0): OP_res();	break; // RES 6, B
				case(0xB1): OP_res();	break; // RES 6, C
				case(0xB2): OP_res();	break; // RES 6, D
				case(0xB3): OP_res();	break; // RES 6, E
				case(0xB4): OP_res();	break; // RES 6, H
				case(0xB5): OP_res();	break; // RES 6, L
				case(0xB6): OP_res();	break; // RES 6, (HL)
				case(0xB7): OP_res();	break; // RES 6, A
				case(0xB8): OP_res();	break; // RES 7, B
				case(0xB9): OP_res();	break; // RES 7, C
				case(0xBA): OP_res();	break; // RES 7, D
				case(0xBB): OP_res();	break; // RES 7, E
				case(0xBC): OP_res();	break; // RES 7, H
				case(0xBD): OP_res();	break; // RES 7, L
				case(0xBE): OP_res();	break; // RES 7, (HL)
				case(0xBF): OP_res();	break; // RES 7, A
				case(0xC0): OP_set();	break; // SET 0, B
				case(0xC1): OP_set();	break; // SET 0, C
				case(0xC2): OP_set();	break; // SET 0, D
				case(0xC3): OP_set();	break; // SET 0, E
				case(0xC4): OP_set();	break; // SET 0, H
				case(0xC5): OP_set();	break; // SET 0, L
				case(0xC6): OP_set();	break; // SET 0, (HL)
				case(0xC7): OP_set();	break; // SET 0, A
				case(0xC8): OP_set();	break; // SET 1, B
				case(0xC9): OP_set();	break; // SET 1, C
				case(0xCA): OP_set();	break; // SET 1, D
				case(0xCB): OP_set();	break; // SET 1, E
				case(0xCC): OP_set();	break; // SET 1, H
				case(0xCD): OP_set();	break; // SET 1, L
				case(0xCE): OP_set();	break; // SET 1, (HL)
				case(0xCF): OP_set();	break; // SET 1, A
				case(0xD0): OP_set();	break; // SET 2, B
				case(0xD1): OP_set();	break; // SET 2, C
				case(0xD2): OP_set();	break; // SET 2, D
				case(0xD3): OP_set();	break; // SET 2, E
				case(0xD4): OP_set();	break; // SET 2, H
				case(0xD5): OP_set();	break; // SET 2, L
				case(0xD6): OP_set();	break; // SET 2, (HL)
				case(0xD7): OP_set();	break; // SET 2, A
				case(0xD8): OP_set();	break; // SET 3, B
				case(0xD9): OP_set();	break; // SET 3, C
				case(0xDA): OP_set();	break; // SET 3, D
				case(0xDB): OP_set();	break; // SET 3, E
				case(0xDC): OP_set();	break; // SET 3, H
				case(0xDD): OP_set();	break; // SET 3, L
				case(0xDE): OP_set();	break; // SET 3, (HL)
				case(0xDF): OP_set();	break; // SET 3, A
				case(0xE0): OP_set();	break; // SET 4, B 
				case(0xE1): OP_set();	break; // SET 4, C
				case(0xE2): OP_set();	break; // SET 4, D
				case(0xE3): OP_set();	break; // SET 4, E
				case(0xE4): OP_set();	break; // SET 4, H
				case(0xE5): OP_set();	break; // SET 4, L
				case(0xE6): OP_set();	break; // SET 4, (HL)
				case(0xE7): OP_set();	break; // SET 4, A
				case(0xE8): OP_set();	break; // SET 5, B
				case(0xE9): OP_set();	break; // SET 5, C
				case(0xEA): OP_set();	break; // SET 5, D
				case(0xEB): OP_set();	break; // SET 5, E
				case(0xEC): OP_set();	break; // SET 5, H
				case(0xED): OP_set();	break; // SET 5, L
				case(0xEE): OP_set();	break; // SET 5, (HL)
				case(0xEF): OP_set();	break; // SET 5, A
				case(0xF0): OP_set();	break; // SET 6, B
				case(0xF1): OP_set();	break; // SET 6, C
				case(0xF2): OP_set();	break; // SET 6, D
				case(0xF3): OP_set();	break; // SET 6, E
				case(0xF4): OP_set();	break; // SET 6, H
				case(0xF5): OP_set();	break; // SET 6, L
				case(0xF6): OP_set();	break; // SET 6, (HL)
				case(0xF7): OP_set();	break; // SET 6, A
				case(0xF8): OP_set();	break; // SET 7, B
				case(0xF9): OP_set();	break; // SET 7, C
				case(0xFA): OP_set();	break; // SET 7, D
				case(0xFB): OP_set();	break; // SET 7, E
				case(0xFC): OP_set();	break; // SET 7, H
				case(0xFD): OP_set();	break; // SET 7, L
				case(0xFE): OP_set();	break; // SET 7, (HL)
				case(0xFF): OP_set();	break; // SET 7, A
				default: OP_nop();	break; // NOP
			}
			break;
		case(0xCC): OP_call();	break;	// CALL Z, u16
		case(0xCD): OP_call();	break;	// CALL u16
		case(0xCE): OP_adc();	break;	// ADC A, u8
		case(0xCF):	OP_rst();	break;	// RST 08h
		case(0xD0): OP_ret();	break;	// RET NC
		case(0xD1): OP_pop();	break;	// POP DE
		case(0xD2): OP_jp();	break;	// JP NC, u16
		//case(0xD3):	break;
		case(0xD4): OP_call();	break;	// CALL NC, u16
		case(0xD5): OP_push();	break;	// PUSH DE
		case(0xD6): OP_sub();	break;	// SUB A, u8
		case(0xD7): OP_rst();	break;	// RST 10h
		case(0xD8): OP_ret();	break;	// RET C
		case(0xD9): OP_reti();	break;	// RETI
		case(0xDA):	OP_jp();	break;	// JP C, u16
		//case(0xDB):	break;
		case(0xDC):	OP_call();	break;	// CALL C, u16
		//case(0xDD):	break;
		case(0xDE):	OP_sbc();	break;	// SBC A, u8
		case(0xDF):	OP_rst();	break;	// RST 18h
		case(0xE0):	OP_ld();	break;	// LD (FF00+u8), A
		case(0xE1):	OP_pop();	break;	// POP HL
		case(0xE2):	OP_ld();	break;	// LD (FF00+C), A
		//case(0xE3):	break; 
		//case(0xE4):	break; 
		case(0xE5):	OP_push();	break;	// PUSH HL
		case(0xE6):	OP_and();	break;	// AND A, u8
		case(0xE7):	OP_rst();	break;	// RST 20h
		case(0xE8):	OP_add16();	break;	// ADD SP, i8
		case(0xE9):	OP_jp();	break;	// JP HL
		case(0xEA): OP_ld();	break;	// LD (u16), A
		//case(0xEB):	break;
		//case(0xEC):	break;
		//case(0xED):	break;
		case(0xEE): OP_xor();	break;	// XOR A,u8
		case(0xEF): OP_rst();	break;	// RST 28h
		case(0xF0):	OP_ld();	break;	// LD A, (FF00+u8)
		case(0xF1):	OP_pop();	break;	// POP AF
		case(0xF2):	OP_ld();	break;	// LD A, (FF00+C)
		case(0xF3): OP_di();	break;	// DI
		//case(0xF4):	break;
		case(0xF5):	OP_push();	break;	// PUSH AF
		case(0xF6):	OP_or();	break;	// OR A, u8
		case(0xF7):	OP_rst();	break;	// RST 30h
		case(0xF8):	OP_ld16();	break;	// LD HL, SP+i8
		case(0xF9): OP_ld16();	break;	// LD SP, HL
		case(0xFA): OP_ld();	break;	// LD A, (u16)		case(0xFB):	OP_ei();	break;	// EI
		//case(0xFC):	break;
		//case(0xFD):	break;
		case(0xFE):	OP_cp();	break;	// CP A, u8
		case(0xFF):	OP_rst();	break;	// RST 38h
		default: OP_nop();	break; // NOP
	}
}

uint8_t Cpu::read(uint16_t address) {
	return memoryController->read(address);
}

void Cpu::write(uint16_t address, uint8_t value) {
	memoryController->write(address, value);
}
