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
		if (halted) {
			return;
		}
		opcode = read8();
		executeInstruction();
		//std::cout << "Instruction: " << "0x" << std::hex << (unsigned)opcode << " | " << (unsigned)read(pc) << std::dec << std::endl;
		//std::cout << Translation[opcode] << std::endl;
	}
}

void Cpu::executeInstruction() {
	switch(opcode) {
		case(0x00): OP_nop();											break;	// NOP					1b	4t
		case(0x01): registers.setBC(read16());							break;	// LD BC, u16			3b	12t
		case(0x02): write(registers.getBC(), registers.a);				break;	// LD (BC), A			1b	8t
		case(0x03): registers.setBC(OP_inc16(registers.getBC()));		break;	// INC BC				1b	8t
		case(0x04): registers.b = OP_inc(registers.b);					break;	// INC B				1b	4t
		case(0x05): registers.b = OP_dec(registers.b);					break;	// DEC B				1b	4t
		case(0x06):	registers.b = read8();								break;	// LD B, u8				2b	8t
		case(0x07):	OP_rlca();											break;	// RLCA					1b	4t
		case(0x08):	OP_ld16(read16()); 									break;	// LD (u16), SP			3b	20t
		case(0x09):	registers.setHL(OP_add16(registers.getHL(), registers.getBC()));	break;	// ADD HL, BC			1b	8t
		case(0x0A):	registers.a = read(registers.getBC());				break;	// LD A, (BC)			1b	8t
		case(0x0B):	registers.setBC(OP_dec16(registers.getBC()));		break;	// DEC BC				1b	8t
		case(0x0C):	registers.c = OP_inc(registers.c);					break;	// INC C				1b	4t
		case(0x0D):	registers.c = OP_dec(registers.c);					break;	// DEC C				1b	4t
		case(0x0E):	registers.c = read8();								break;	// LD C, u8				2b	8t
		case(0x0F):	OP_rrca();											break;	// RRCA					1b	4t
		case(0x10):	OP_stop();											break;	// STOP					2b	4t
		case(0x11):	registers.setDE(read16());							break;	// LD DE, u16			3b	12t
		case(0x12):	write(registers.getDE(), registers.a);				break;	// LD (DE), A			1b	8t
		case(0x13):	registers.setDE(OP_inc16(registers.getDE()));		break;	// INC DE				1b	8t
		case(0x14):	registers.d = OP_inc(registers.d);					break;	// INC D				1b	4t
		case(0x15):	registers.d = OP_dec(registers.d);					break;	// DEC D				1b	4t
		case(0x16):	registers.d = read8();								break;	// LD D, u8				2b	8t
		case(0x17):	OP_rla();											break;	// RLA					1b	4t
		case(0x18):	OP_jr(true);										break;	// JR i8				2b	12t
		case(0x19):	registers.setHL(OP_add16(registers.getHL(), registers.getDE()));	break;	// ADD HL, DE			1b	8t
		case(0x1A):	registers.a = read(registers.getDE());				break;	// LD A, (DE)			1b	8t
		case(0x1B): registers.setDE(OP_dec16(registers.getDE()));		break;	// DEC DE				1b	8t
		case(0x1C): registers.e = OP_inc(registers.e);					break;	// INC E				1b	4t
		case(0x1D): registers.e = OP_dec(registers.e);					break;	// DEC E				1b	4t
		case(0x1E): registers.e = read8();								break;	// LD E, u8				2b	8t
		case(0x1F): OP_rra();											break;	// RRA					1b	4t
		case(0x20): OP_jr(!registers.getFlag(FLAGS::ZERO));				break;	// JR NZ, i8			2b	8t-12t
		case(0x21): registers.setHL(read16());							break;	// LD HL, u16			3b	12t
		case(0x22): write(registers.getHL() + 1, registers.a);			break;	// LD (HL+), A			1b	8t
		case(0x23): registers.setHL(OP_inc16(registers.getHL()));		break;	// INC HL				1b	8t
		case(0x24): registers.h = OP_inc(registers.h);					break;	// INC H				1b	4t
		case(0x25): registers.h = OP_dec(registers.h);					break;	// DEC H				1b	4t
		case(0x26): registers.h = read8();								break;	// LD H, u8				2b	8t
		case(0x27): OP_daa();											break;	// DAA					1b	4t
		case(0x28): OP_jr(registers.getFlag(FLAGS::ZERO));				break;	// JR Z, i8				2b	8t-12t
		case(0x29): registers.setHL(OP_add16(registers.getHL(), registers.getHL()));	break;	// ADD HL, HL			1b	8t
		case(0x2A): registers.a = read(registers.getHL() + 1);			break;	// LD A, (HL+)			1b	8t
		case(0x2B): registers.setHL(OP_dec16(registers.getHL()));		break;	// DEC HL				1b	8t
		case(0x2C): registers.l = OP_inc(registers.l);					break;	// INC L				1b	4t
		case(0x2D): registers.l = OP_dec(registers.l);					break;	// DEC L				1b	4t
		case(0x2E): registers.l = read8();								break;	// LD L, u8				2b	8t
		case(0x2F): OP_cpl();											break;	// CPL					1b	4t
		case(0x30): OP_jr(!registers.getFlag(FLAGS::CARRY));			break;	// JR NC, i8			2b	8t-12t
		case(0x31):	sp = read16();										break;	// LD SP, u16			3b	12t
		case(0x32): write(registers.getHL() - 1, registers.a);			break;	// LD (HL-), A			1b	8t
		case(0x33): sp = OP_inc16(sp);									break;	// INC SP				1b	8t
		case(0x34): write(registers.getHL(), OP_inc(read(registers.getHL())));	break;	// INC (HL)				1b	12t
		case(0x35): write(registers.getHL(), OP_dec(read(registers.getHL())));	break;	// DEC (HL)				1b	12t
		case(0x36): write(registers.getHL(), read8());					break;	// LD (HL), u8			2b	12t
		case(0x37): OP_scf();											break;	// SCF					1b	4t
		case(0x38): OP_jr(registers.getFlag(FLAGS::CARRY));				break;	// JR C, i8				2b	8t-12t
		case(0x39): registers.setHL(OP_add16(registers.getHL(), sp));	break;	// ADD HL, SP			1b	8t
		case(0x3A): registers.a = read(registers.getHL() - 1);			break;	// LD A, (HL-)			1b	8t
		case(0x3B):	sp = OP_dec16(sp);									break;	// DEC SP				1b	8t
		case(0x3C):	registers.a = OP_inc(registers.a);					break;	// INC A				1b	4t
		case(0x3D): registers.a = OP_dec(registers.a);					break;	// DEC A				1b	4t
		case(0x3E): registers.b = read8();								break;	// LD A, u8				2b	8t
		case(0x3F): OP_ccf();											break;	// CCF					1b	4t
		case(0x40): registers.b = registers.b;							break;	// LD B, B				1b	4t
		case(0x41):	registers.b = registers.c;							break;	// LD B, C				1b	4t
		case(0x42):	registers.b = registers.d;							break;	// LD B, D				1b	4t
		case(0x43): registers.b = registers.e;							break;	// LD B, E				1b	4t
		case(0x44):	registers.b = registers.h;							break;	// LD B, H				1b	4t
		case(0x45):	registers.b = registers.l;							break;	// LD B, L				1b	4t
		case(0x46):	registers.b = read(registers.getHL());				break;	// LD B, (HL)			1b	8t
		case(0x47):	registers.b = registers.a;							break;	// LD B, A				1b	4t
		case(0x48):	registers.c = registers.b;							break;	// LD C, B				1b	4t
		case(0x49):	registers.c = registers.c;							break;	// LD C, C				1b	4t
		case(0x4A):	registers.c = registers.d;							break;	// LD C, D				1b	4t
		case(0x4B):	registers.c = registers.e;							break;	// LD C, E				1b	4t
		case(0x4C):	registers.c = registers.h;							break;	// LD C, H				1b	4t
		case(0x4D):	registers.c = registers.l;							break;	// LD C, L				1b	4t
		case(0x4E):	registers.c = read(registers.getHL());				break;	// LD C, (HL)			1b	8t
		case(0x4F):	registers.c = registers.a;							break;	// LD C, A				1b	4t
		case(0x50):	registers.d = registers.b;							break;	// LD D, B				1b	4t
		case(0x51):	registers.d = registers.c;							break;	// LD D, C				1b	4t
		case(0x52):	registers.d = registers.d;							break;	// LD D, D				1b	4t
		case(0x53):	registers.d = registers.e;							break;	// LD D, E				1b	4t
		case(0x54):	registers.d = registers.h;							break;	// LD D, H				1b	4t
		case(0x55):	registers.d = registers.l;							break;	// LD D, L				1b	4t
		case(0x56):	registers.d = read(registers.getHL());				break;	// LD D, (HL)			1b	8t
		case(0x57):	registers.d = registers.a;							break;	// LD D, A				1b	4t
		case(0x58):	registers.e = registers.b;							break;	// LD E, B				1b	4t
		case(0x59):	registers.e = registers.c;							break;	// LD E, C				1b	4t
		case(0x5A):	registers.e = registers.d;							break;	// LD E, D				1b	4t
		case(0x5B):	registers.e = registers.e;							break;	// LD E, E				1b	4t
		case(0x5C):	registers.e = registers.h;							break;	// LD E, H				1b	4t
		case(0x5D):	registers.e = registers.l;							break;	// LD E, L				1b	4t
		case(0x5E):	registers.e = read(registers.getHL());				break;	// LD E, (HL)			1b	8t
		case(0x5F):	registers.e = registers.a;							break;	// LD E, A				1b	4t
		case(0x60):	registers.h = registers.b;							break;	// LD H, B				1b	4t
		case(0x61):	registers.h = registers.c;							break;	// LD H, C				1b	4t
		case(0x62):	registers.h = registers.d;							break;	// LD H, D				1b	4t
		case(0x63):	registers.h = registers.e;							break;	// LD H, E				1b	4t
		case(0x64):	registers.h = registers.h;							break;	// LD H, H				1b	4t
		case(0x65):	registers.h = registers.l;							break;	// LD H, L				1b	4t
		case(0x66):	registers.h = read(registers.getHL());				break;	// LD H, (HL)			1b	8t
		case(0x67):	registers.h = registers.a;							break;	// LD H, A				1b	4t
		case(0x68):	registers.l = registers.b;							break;	// LD L, B				1b	4t
		case(0x69): registers.l = registers.c;							break;	// LD L, C				1b	4t
		case(0x6A):	registers.l = registers.d;							break;	// LD L, D				1b	4t
		case(0x6B):	registers.l = registers.e;							break;	// LD L, E				1b	4t
		case(0x6C):	registers.l = registers.h;							break;	// LD L, H				1b	4t
		case(0x6D):	registers.l = registers.l;							break;	// LD L, L				1b	4t
		case(0x6E):	registers.l = read(registers.getHL());				break;	// LD L, (HL)			1b	8t
		case(0x6F):	registers.l = registers.a;							break;	// LD L, A				1b	4t
		case(0x70):	write(registers.getHL(), registers.b);				break;	// LD (HL), B			1b	8t
		case(0x71):	write(registers.getHL(), registers.c);				break;	// LD (HL), C			1b	8t
		case(0x72):	write(registers.getHL(), registers.d);				break;	// LD (HL), D			1b	8t
		case(0x73): write(registers.getHL(), registers.e);				break;	// LD (HL), E			1b	8t
		case(0x74):	write(registers.getHL(), registers.h);				break;	// LD (HL), H			1b	8t
		case(0x75):	write(registers.getHL(), registers.l);				break;	// LD (HL), L			1b	8t
		case(0x76): OP_halt();											break;	// HALT					1b	4t
		case(0x77):	write(registers.getHL(), registers.a);				break;	// LD (HL), A			1b	8t
		case(0x78):	registers.a = registers.b;							break;	// LD A, B				1b	4t
		case(0x79):	registers.a = registers.c;							break;	// LD A, C				1b	4t
		case(0x7A):	registers.a = registers.d;							break;	// LD A, D				1b	4t
		case(0x7B):	registers.a = registers.e;							break;	// LD A, E				1b	4t
		case(0x7C):	registers.a = registers.h;							break;	// LD A, H				1b	4t
		case(0x7D):	registers.a = registers.l;							break;	// LD A, L				1b	4t
		case(0x7E):	registers.a = read(registers.getHL());				break;	// LD A, (HL)			1b	8t
		case(0x7F):	registers.a = registers.a;							break;	// LD A, A				1b	4t
		case(0x80): OP_add(registers.b);								break;	// ADD A, B				1b	4t
		case(0x81): OP_add(registers.c);								break;	// ADD A, C				1b	4t
		case(0x82): OP_add(registers.d);								break;	// ADD A, D				1b	4t
		case(0x83): OP_add(registers.e);								break;	// ADD A, E				1b	4t
		case(0x84): OP_add(registers.h);								break;	// ADD A, H				1b	4t
		case(0x85): OP_add(registers.l);								break;	// ADD A, L				1b	4t
		case(0x86): OP_add(read(registers.getHL()));					break;	// ADD A, (HL)			1b	8t
		case(0x87): OP_add(registers.a);								break;	// ADD A, A				1b	4t
		case(0x88): OP_adc(registers.b);								break;	// ADC A, B				1b	4t
		case(0x89): OP_adc(registers.c);								break;	// ADC A, C				1b	4t
		case(0x8A): OP_adc(registers.d);								break;	// ADC A, D				1b	4t
		case(0x8B): OP_adc(registers.e);								break;	// ADC A, E				1b	4t
		case(0x8C): OP_adc(registers.h);								break;	// ADC A, H				1b	4t
		case(0x8D): OP_adc(registers.l);								break;	// ADC A, L				1b	4t
		case(0x8E): OP_adc(read(registers.getHL()));					break;	// ADC A, (HL)			1b	8t
		case(0x8F): OP_adc(registers.a);								break;	// ADC A, A				1b	4t
		case(0x90): OP_sub(registers.b);								break;	// SUB A, B				1b	4t
		case(0x91): OP_sub(registers.c);								break;	// SUB A, C				1b	4t
		case(0x92): OP_sub(registers.d);								break;	// SUB A, D				1b	4t
		case(0x93): OP_sub(registers.e);								break;	// SUB A, E				1b	4t
		case(0x94): OP_sub(registers.h);								break;	// SUB A, H				1b	4t
		case(0x95): OP_sub(registers.l);								break;	// SUB A, L				1b	4t
		case(0x96): OP_sub(read(registers.getHL()));					break;	// SUB A, (HL)			1b	8t
		case(0x97): OP_sub(registers.a);								break;	// SUB A, A				1b	4t
		case(0x98): OP_sbc(registers.b);								break;	// SBC A, B				1b	4t
		case(0x99): OP_sbc(registers.c);								break;	// SBC A, C				1b	4t
		case(0x9A): OP_sbc(registers.d);								break;	// SBC A, D				1b	4t
		case(0x9B): OP_sbc(registers.e);								break;	// SBC A, E				1b	4t
		case(0x9C): OP_sbc(registers.h);								break;	// SBC A, H				1b	4t
		case(0x9D): OP_sbc(registers.l);								break;	// SBC A, L				1b	4t
		case(0x9E): OP_sbc(read(registers.getHL()));					break;	// SBC A, (HL)			1b	8t
		case(0x9F): OP_sbc(registers.a);								break;	// SBC A, A				1b	4t
		case(0xA0): OP_and(registers.b);								break;	// AND A, B				1b	4t
		case(0xA1): OP_and(registers.c);								break;	// AND A, C				1b	4t
		case(0xA2): OP_and(registers.d);								break;	// AND A, D				1b	4t
		case(0xA3): OP_and(registers.e);								break;	// AND A, E				1b	4t
		case(0xA4): OP_and(registers.h);								break;	// AND A, H				1b	4t
		case(0xA5): OP_and(registers.l);								break;	// AND A, L				1b	4t
		case(0xA6): OP_and(read(registers.getHL()));					break;	// AND A, (HL)			1b	8t
		case(0xA7): OP_and(registers.a);								break;	// AND A, A				1b	4t
		case(0xA8): OP_xor(registers.b);								break;	// XOR A, B				1b	4t
		case(0xA9): OP_xor(registers.c);								break;	// XOR A, C				1b	4t
		case(0xAA): OP_xor(registers.d);								break;	// XOR A, D				1b	4t
		case(0xAB): OP_xor(registers.e);								break;	// XOR A, E				1b	4t
		case(0xAC): OP_xor(registers.h);								break;	// XOR A, H				1b	4t
		case(0xAD): OP_xor(registers.l);								break;	// XOR A, L				1b	4t
		case(0xAE): OP_xor(read(registers.getHL()));					break;	// XOR A, (HL)			1b	8t
		case(0xAF): OP_xor(registers.a);								break;	// XOR A, A				1b	4t
		case(0xB0): OP_or(registers.b);									break;	// OR A, B				1b	4t
		case(0xB1): OP_or(registers.c);									break;	// OR A, C				1b	4t
		case(0xB2): OP_or(registers.d);									break;	// OR A, D				1b	4t
		case(0xB3): OP_or(registers.e);									break;	// OR A, E				1b	4t
		case(0xB4): OP_or(registers.h);									break;	// OR A, H				1b	4t
		case(0xB5): OP_or(registers.l);									break;	// OR A, L				1b	4t
		case(0xB6): OP_or(read(registers.getHL()));						break;	// OR A, (HL)			1b	8t
		case(0xB7): OP_or(registers.a);									break;	// OR A, A				1b	4t
		case(0xB8): OP_cp(registers.b);									break;	// CP A, B				1b	4t
		case(0xB9): OP_cp(registers.c);									break;	// CP A, C				1b	4t
		case(0xBA): OP_cp(registers.d);									break;	// CP A, D				1b	4t
		case(0xBB): OP_cp(registers.e);									break;	// CP A, E				1b	4t
		case(0xBC): OP_cp(registers.h);									break;	// CP A, H				1b	4t
		case(0xBD): OP_cp(registers.l);									break;	// CP A, L				1b	4t
		case(0xBE): OP_cp(read(registers.getHL()));						break;	// CP A, (HL)			1b	8t
		case(0xBF): OP_cp(registers.a);									break;	// CP A, A				1b	4t
		case(0xC0): OP_ret(!registers.getFlag(FLAGS::ZERO));			break;	// RET NZ				1b	8t-20t
		case(0xC1): registers.setBC(OP_pop());							break;	// POP BC				1b	12t
		case(0xC2): OP_jp(!registers.getFlag(FLAGS::ZERO));				break;	// JP NZ, u16			3b	12t-16t
		case(0xC3): OP_jp(true);										break;	// JP u16				3b	16t
		case(0xC4): OP_call(!registers.getFlag(FLAGS::ZERO));			break;	// CALL NZ, u16			3b	12t-24t
		case(0xC5):	OP_push(registers.getBC());							break;	// PUSH BC				1b	16t
		case(0xC6): OP_add(read8());									break;	// ADD A, u8			2b	8t
		case(0xC7): OP_rst(0x00);										break;	// RST 00h				1b	16t
		case(0xC8): OP_ret(registers.getFlag(FLAGS::ZERO));				break;	// RET Z				1b	8t-20t
		case(0xC9): OP_ret(true);										break;	// RET					1b	16t
		case(0xCA): OP_jp(registers.getFlag(FLAGS::ZERO));				break;	// JP Z, u16			3b	12t-16t
		case(0xCB):
			switch(read8()) {
				case(0x00): registers.b = OP_rlc(registers.b);			break;	// RLC B			2b	8t
				case(0x01): registers.c = OP_rlc(registers.c);			break;	// RLC C			2b	8t
				case(0x02): registers.d = OP_rlc(registers.d);			break;	// RLC D			2b	8t
				case(0x03): registers.e = OP_rlc(registers.e);			break;	// RLC E			2b	8t
				case(0x04): registers.h = OP_rlc(registers.h);			break;	// RLC H			2b	8t
				case(0x05): registers.l = OP_rlc(registers.l);			break;	// RLC L			2b	8t
				case(0x06): write(registers.getHL(), OP_rlc(read(registers.getHL())));				break;	// RLC (HL)		2b	16t
				case(0x07): registers.a = OP_rlc(registers.a);			break;	// RLC A			2b	8t
				case(0x08): registers.b = OP_rrc(registers.b);			break;	// RRC B			2b	8t
				case(0x09): registers.c = OP_rrc(registers.c);			break;	// RRC C			2b	8t
				case(0x0A): registers.d = OP_rrc(registers.d);			break;	// RRC D			2b	8t
				case(0x0B): registers.e = OP_rrc(registers.e);			break;	// RRC E			2b	8t
				case(0x0C): registers.h = OP_rrc(registers.h);			break;	// RRC H			2b	8t
				case(0x0D): registers.l = OP_rrc(registers.l);			break;	// RRC L			2b	8t
				case(0x0E): write(registers.getHL(), OP_rrc(read(registers.getHL())));				break;	// RRC (HL)		2b	16t
				case(0x0F): registers.a = OP_rrc(registers.a);			break;	// RRC A			2b	8t
				case(0x10): registers.b = OP_rl(registers.b);			break;	// RL B			2b	8t
				case(0x11): registers.c = OP_rl(registers.c);			break;	// RL C			2b	8t
				case(0x12): registers.d = OP_rl(registers.d);			break;	// RL D			2b	8t
				case(0x13): registers.e = OP_rl(registers.e);			break;	// RL E			2b	8t
				case(0x14): registers.h = OP_rl(registers.h);			break;	// RL H			2b	8t
				case(0x15): registers.l = OP_rl(registers.l);			break;	// RL L			2b	8t
				case(0x16): write(registers.getHL(), OP_rl(read(registers.getHL())));				break;	// RL (HL)		2b	16t
				case(0x17): registers.a = OP_rl(registers.a);			break;	// RL A			2b	8t
				case(0x18): registers.b = OP_rr(registers.b);			break;	// RR B			2b	8t
				case(0x19): registers.c = OP_rr(registers.c);			break;	// RR C			2b	8t
				case(0x1A): registers.d = OP_rr(registers.d);			break;	// RR D			2b	8t
				case(0x1B): registers.e = OP_rr(registers.e);			break;	// RR E			2b	8t
				case(0x1C): registers.h = OP_rr(registers.h);			break;	// RR H			2b	8t
				case(0x1D): registers.l = OP_rr(registers.l);			break;	// RR L			2b	8t
				case(0x1E): write(registers.getHL(), OP_rr(read(registers.getHL())));				break;	// RR (HL)		2b	16t
				case(0x1F): registers.a = OP_rr(registers.a);			break;	// RR A			2b	8t
				case(0x20): registers.b = OP_sla(registers.b);			break;	// SLA B			2b	8t
				case(0x21): registers.c = OP_sla(registers.c);			break;	// SLA C			2b	8t
				case(0x22): registers.d = OP_sla(registers.d);			break;	// SLA D			2b	8t
				case(0x23): registers.e = OP_sla(registers.e);			break;	// SLA E			2b	8t
				case(0x24): registers.h = OP_sla(registers.h);			break;	// SLA H			2b	8t
				case(0x25): registers.l = OP_sla(registers.l);			break;	// SLA L			2b	8t
				case(0x26): write(registers.getHL(), OP_sla(read(registers.getHL())));				break;	// SLA (HL)		2b	16t
				case(0x27): registers.a = OP_sla(registers.a);			break;	// SLA A			2b	8t
				case(0x28): registers.b = OP_sra(registers.b);			break;	// SRA B			2b	8t	
				case(0x29): registers.c = OP_sra(registers.c);			break;	// SRA C			2b	8t
				case(0x2A): registers.d = OP_sra(registers.d);			break;	// SRA D			2b	8t
				case(0x2B): registers.e = OP_sra(registers.e);			break;	// SRA E			2b	8t
				case(0x2C): registers.h = OP_sra(registers.h);			break;	// SRA H			2b	8t
				case(0x2D): registers.l = OP_sra(registers.l);			break;	// SRA L			2b	8t
				case(0x2E): write(registers.getHL(), OP_sra(read(registers.getHL())));				break;	// SRA (HL)		2b	16t
				case(0x2F): registers.a = OP_sra(registers.a);			break;	// SRA A			2b	8t
				case(0x30): registers.b = OP_swap(registers.b);			break;	// SWAP B		2b	8t
				case(0x31): registers.c = OP_swap(registers.c);			break;	// SWAP C		2b	8t
				case(0x32): registers.d = OP_swap(registers.d);			break;	// SWAP D		2b	8t
				case(0x33): registers.e = OP_swap(registers.e);			break;	// SWAP E		2b	8t
				case(0x34): registers.h = OP_swap(registers.h);			break;	// SWAP H		2b	8t
				case(0x35): registers.l = OP_swap(registers.l);			break;	// SWAP L		2b	8t
				case(0x36): write(registers.getHL(), OP_swap(read(registers.getHL())));				break;	// SWAP (HL)		2b	16t
				case(0x37): registers.a = OP_swap(registers.a);			break;	// SWAP A		2b	8t
				case(0x38): registers.b = OP_srl(registers.b);			break;	// SRL B			2b	8t
				case(0x39): registers.c = OP_srl(registers.c);			break;	// SRL C			2b	8t
				case(0x3A): registers.d = OP_srl(registers.d);			break;	// SRL D			2b	8t
				case(0x3B): registers.e = OP_srl(registers.e);			break;	// SRL E			2b	8t
				case(0x3C): registers.h = OP_srl(registers.h);			break;	// SRL H			2b	8t
				case(0x3D): registers.l = OP_srl(registers.l);			break;	// SRL L			2b	8t
				case(0x3E): write(registers.getHL(), OP_srl(read(registers.getHL())));				break;	// SRL (HL)		2b	16t
				case(0x3F): registers.a = OP_srl(registers.a);			break;	// SRL A			2b	8t
				case(0x40): OP_bit(0x01, registers.b);					break;	// BIT 0, B		2b	8t
				case(0x41): OP_bit(0x01, registers.c);					break;	// BIT 0, C		2b	8t
				case(0x42): OP_bit(0x01, registers.d);					break;	// BIT 0, D		2b	8t
				case(0x43): OP_bit(0x01, registers.e);					break;	// BIT 0, E		2b	8t
				case(0x44): OP_bit(0x01, registers.h);					break;	// BIT 0, H		2b	8t
				case(0x45): OP_bit(0x01, registers.l);					break;	// BIT 0, L		2b	8t
				case(0x46): OP_bit(0x01, read(registers.getHL()));		break;	// BIT 0, (HL)	2b	12t
				case(0x47): OP_bit(0x01, registers.a);					break;	// BIT 0, A		2b	8t
				case(0x48): OP_bit(0x02, registers.b);					break;	// BIT 1, B		2b	8t
				case(0x49): OP_bit(0x02, registers.c);					break;	// BIT 1, C		2b	8t
				case(0x4A): OP_bit(0x02, registers.d);					break;	// BIT 1, D		2b	8t
				case(0x4B): OP_bit(0x02, registers.e);					break;	// BIT 1, E		2b	8t
				case(0x4C): OP_bit(0x02, registers.h);					break;	// BIT 1, H		2b	8t
				case(0x4D): OP_bit(0x02, registers.l);					break;	// BIT 1, L		2b	8t
				case(0x4E): OP_bit(0x02, read(registers.getHL()));		break;	// BIT 1, (HL)	2b	12t
				case(0x4F): OP_bit(0x02, registers.a);					break;	// BIT 1, A		2b	8t
				case(0x50): OP_bit(0x04, registers.b);					break;	// BIT 2, B		2b	8t
				case(0x51): OP_bit(0x04, registers.c);					break;	// BIT 2, C		2b	8t
				case(0x52): OP_bit(0x04, registers.d);					break;	// BIT 2, D		2b	8t
				case(0x53): OP_bit(0x04, registers.e);					break;	// BIT 2, E		2b	8t
				case(0x54): OP_bit(0x04, registers.h);					break;	// BIT 2, H		2b	8t
				case(0x55): OP_bit(0x04, registers.l);					break;	// BIT 2, L		2b	8t
				case(0x56): OP_bit(0x04, read(registers.getHL()));		break;	// BIT 2, (HL)	2b	12t
				case(0x57): OP_bit(0x04, registers.a);					break;	// BIT 2, A		2b	8t
				case(0x58): OP_bit(0x08, registers.b);					break;	// BIT 3, B		2b	8t
				case(0x59): OP_bit(0x08, registers.c);					break;	// BIT 3, C		2b	8t
				case(0x5A): OP_bit(0x08, registers.d);					break;	// BIT 3, D		2b	8t
				case(0x5B): OP_bit(0x08, registers.e);					break;	// BIT 3, E		2b	8t
				case(0x5C): OP_bit(0x08, registers.h);					break;	// BIT 3, H		2b	8t
				case(0x5D): OP_bit(0x08, registers.l);					break;	// BIT 3, L		2b	8t
				case(0x5E): OP_bit(0x08, read(registers.getHL()));		break;	// BIT 3, (HL)	2b	12t
				case(0x5F): OP_bit(0x08, registers.a);					break;	// BIT 3, A		2b	8t
				case(0x60): OP_bit(0x10, registers.b);					break;	// BIT 4, B		2b	8t
				case(0x61): OP_bit(0x10, registers.c);					break;	// BIT 4, C		2b	8t
				case(0x62): OP_bit(0x10, registers.d);					break;	// BIT 4, D		2b	8t
				case(0x63): OP_bit(0x10, registers.e);					break;	// BIT 4, E		2b	8t
				case(0x64): OP_bit(0x10, registers.h);					break;	// BIT 4, H		2b	8t
				case(0x65): OP_bit(0x10, registers.l);					break;	// BIT 4, L		2b	8t
				case(0x66): OP_bit(0x10, read(registers.getHL()));		break;	// BIT 4, (HL)	2b	12t
				case(0x67): OP_bit(0x10, registers.a);					break;	// BIT 4, A		2b	8t
				case(0x68): OP_bit(0x20, registers.b);					break;	// BIT 5, B		2b	8t
				case(0x69): OP_bit(0x20, registers.c);					break;	// BIT 5, C		2b	8t
				case(0x6A): OP_bit(0x20, registers.d);					break;	// BIT 5, D		2b	8t
				case(0x6B): OP_bit(0x20, registers.e);					break;	// BIT 5, E		2b	8t
				case(0x6C): OP_bit(0x20, registers.h);					break;	// BIT 5, H		2b	8t
				case(0x6D): OP_bit(0x20, registers.l);					break;	// BIT 5, L		2b	8t
				case(0x6E): OP_bit(0x20, read(registers.getHL()));		break;	// BIT 5, (HL)	2b	12t
				case(0x6F): OP_bit(0x20, registers.a);					break;	// BIT 5, A		2b	8t
				case(0x70): OP_bit(0x40, registers.b);					break;	// BIT 6, B		2b	8t
				case(0x71): OP_bit(0x40, registers.c);					break;	// BIT 6, C		2b	8t
				case(0x72): OP_bit(0x40, registers.d);					break;	// BIT 6, D		2b	8t
				case(0x73): OP_bit(0x40, registers.e);					break;	// BIT 6, E		2b	8t
				case(0x74): OP_bit(0x40, registers.h);					break;	// BIT 6, H		2b	8t
				case(0x75): OP_bit(0x40, registers.l);					break;	// BIT 6, L		2b	8t
				case(0x76): OP_bit(0x40, read(registers.getHL()));		break;	// BIT 6, (HL)	2b	12t
				case(0x77): OP_bit(0x40, registers.a);					break;	// BIT 6, A		2b	8t
				case(0x78): OP_bit(0x80, registers.b);					break;	// BIT 7, B		2b	8t
				case(0x79): OP_bit(0x80, registers.c);					break;	// BIT 7, C		2b	8t
				case(0x7A): OP_bit(0x80, registers.d);					break;	// BIT 7, D		2b	8t
				case(0x7B): OP_bit(0x80, registers.e);					break;	// BIT 7, E		2b	8t
				case(0x7C): OP_bit(0x80, registers.h);					break;	// BIT 7, H		2b	8t
				case(0x7D): OP_bit(0x80, registers.l);					break;	// BIT 7, L		2b	8t
				case(0x7E): OP_bit(0x80, read(registers.getHL())); 		break;	// BIT 7, (HL)	2b	12t
				case(0x7F): OP_bit(0x80, registers.a);					break;	// BIT 7, A		2b	8t
				case(0x80): registers.b = OP_res(0x01, registers.b);	break;	// RES 0, B		2b	8t
				case(0x81): registers.c = OP_res(0x01, registers.c);	break;	// RES 0, C		2b	8t
				case(0x82): registers.d = OP_res(0x01, registers.d);	break;	// RES 0, D		2b	8t
				case(0x83): registers.e = OP_res(0x01, registers.e);	break;	// RES 0, E		2b	8t
				case(0x84): registers.h = OP_res(0x01, registers.h);	break;	// RES 0, H		2b	8t
				case(0x85): registers.l = OP_res(0x01, registers.l);	break;	// RES 0, L		2b	8t
				case(0x86): write(registers.getHL(), OP_res(0x01, read(registers.getHL())));		break;	// RES 0, (HL)	2b	16t
				case(0x87): registers.a = OP_res(0x01, registers.a);	break;	// RES 0, A		2b	8t
				case(0x88): registers.b = OP_res(0x02, registers.b);	break;	// RES 1, B		2b	8t
				case(0x89): registers.c = OP_res(0x02, registers.c);	break;	// RES 1, C		2b	8t
				case(0x8A): registers.d = OP_res(0x02, registers.d);	break;	// RES 1, D		2b	8t
				case(0x8B): registers.e = OP_res(0x02, registers.e);	break;	// RES 1, E		2b	8t
				case(0x8C): registers.h = OP_res(0x02, registers.h);	break;	// RES 1, H		2b	8t
				case(0x8D): registers.l = OP_res(0x02, registers.l);	break;	// RES 1, L		2b	8t
				case(0x8E): write(registers.getHL(), OP_res(0x02, read(registers.getHL())));		break;	// RES 1, (HL)	2b	16t
				case(0x8F): registers.a = OP_res(0x02, registers.a);	break;	// RES 1, A		2b	8t
				case(0x90): registers.b = OP_res(0x04, registers.b);	break;	// RES 2, B		2b	8t
				case(0x91): registers.c = OP_res(0x04, registers.c);	break;	// RES 2, C		2b	8t
				case(0x92): registers.d = OP_res(0x04, registers.d);	break;	// RES 2, D		2b	8t
				case(0x93): registers.e = OP_res(0x04, registers.e);	break;	// RES 2, E		2b	8t
				case(0x94): registers.h = OP_res(0x04, registers.h);	break;	// RES 2, H		2b	8t
				case(0x95): registers.l = OP_res(0x04, registers.l);	break;	// RES 2, L		2b	8t
				case(0x96): write(registers.getHL(), OP_res(0x04, read(registers.getHL())));		break;	// RES 2, (HL)	2b	16t
				case(0x97): registers.a = OP_res(0x04, registers.a);	break;	// RES 2, A		2b	8t
				case(0x98): registers.b = OP_res(0x08, registers.b);	break;	// RES 3, B		2b	8t
				case(0x99): registers.c = OP_res(0x08, registers.c);	break;	// RES 3, C		2b	8t
				case(0x9A): registers.d = OP_res(0x08, registers.d);	break;	// RES 3, D		2b	8t
				case(0x9B): registers.e = OP_res(0x08, registers.e);	break;	// RES 3, E		2b	8t
				case(0x9C): registers.h = OP_res(0x08, registers.h);	break;	// RES 3, H		2b	8t
				case(0x9D): registers.l = OP_res(0x08, registers.l);	break;	// RES 3, L		2b	8t
				case(0x9E): write(registers.getHL(), OP_res(0x08, read(registers.getHL())));		break;	// RES 3, (HL)	2b	16t
				case(0x9F): registers.a = OP_res(0x08, registers.a);	break;	// RES 3, A		2b	8t
				case(0xA0): registers.b = OP_res(0x10, registers.b);	break;	// RES 4, B		2b	8t
				case(0xA1): registers.c = OP_res(0x10, registers.c);	break;	// RES 4, C		2b	8t
				case(0xA2): registers.d = OP_res(0x10, registers.d);	break;	// RES 4, D		2b	8t
				case(0xA3): registers.e = OP_res(0x10, registers.e);	break;	// RES 4, E		2b	8t
				case(0xA4): registers.h = OP_res(0x10, registers.h);	break;	// RES 4, H		2b	8t
				case(0xA5): registers.l = OP_res(0x10, registers.l);	break;	// RES 4, L		2b	8t
				case(0xA6): write(registers.getHL(), OP_res(0x10, read(registers.getHL())));		break;	// RES 4, (HL)	2b	16t
				case(0xA7): registers.a = OP_res(0x10, registers.a);	break;	// RES 4, A		2b	8t
				case(0xA8): registers.b = OP_res(0x20, registers.b);	break;	// RES 5, B		2b	8t
				case(0xA9): registers.c = OP_res(0x20, registers.c);	break;	// RES 5, C		2b	8t
				case(0xAA): registers.d = OP_res(0x20, registers.d);	break;	// RES 5, D		2b	8t
				case(0xAB): registers.e = OP_res(0x20, registers.e);	break;	// RES 5, E		2b	8t
				case(0xAC): registers.h = OP_res(0x20, registers.h);	break;	// RES 5, H		2b	8t
				case(0xAD): registers.l = OP_res(0x20, registers.l);	break;	// RES 5, L		2b	8t
				case(0xAE): write(registers.getHL(), OP_res(0x20, read(registers.getHL())));		break;	// RES 5, (HL)	2b	16t
				case(0xAF): registers.a = OP_res(0x20, registers.a);	break;	// RES 5, A		2b	8t
				case(0xB0): registers.b = OP_res(0x40, registers.b);	break;	// RES 6, B		2b	8t
				case(0xB1): registers.c = OP_res(0x40, registers.c);	break;	// RES 6, C		2b	8t
				case(0xB2): registers.d = OP_res(0x40, registers.d);	break;	// RES 6, D		2b	8t
				case(0xB3): registers.e = OP_res(0x40, registers.e);	break;	// RES 6, E		2b	8t
				case(0xB4): registers.h = OP_res(0x40, registers.h);	break;	// RES 6, H		2b	8t
				case(0xB5): registers.l = OP_res(0x40, registers.l);	break;	// RES 6, L		2b	8t
				case(0xB6): write(registers.getHL(), OP_res(0x40, read(registers.getHL())));		break;	// RES 6, (HL)	2b	16t
				case(0xB7): registers.a = OP_res(0x40, registers.a);	break;	// RES 6, A		2b	8t
				case(0xB8): registers.b = OP_res(0x80, registers.b);	break;	// RES 7, B		2b	8t
				case(0xB9): registers.c = OP_res(0x80, registers.c);	break;	// RES 7, C		2b	8t
				case(0xBA): registers.d = OP_res(0x80, registers.d);	break;	// RES 7, D		2b	8t
				case(0xBB): registers.e = OP_res(0x80, registers.e);	break;	// RES 7, E		2b	8t
				case(0xBC): registers.h = OP_res(0x80, registers.h);	break;	// RES 7, H		2b	8t
				case(0xBD): registers.l = OP_res(0x80, registers.l);	break;	// RES 7, L		2b	8t
				case(0xBE): write(registers.getHL(), OP_res(0x80, read(registers.getHL())));		break;	// RES 7, (HL)	2b	16t
				case(0xBF): registers.a = OP_res(0x80, registers.a);	break;	// RES 7, A		2b	8t
				case(0xC0): registers.b = OP_set(0x01, registers.b);	break;	// SET 0, B		2b	8t
				case(0xC1): registers.c = OP_set(0x01, registers.c);	break;	// SET 0, C		2b	8t
				case(0xC2): registers.d = OP_set(0x01, registers.d);	break;	// SET 0, D		2b	8t
				case(0xC3): registers.e = OP_set(0x01, registers.e);	break;	// SET 0, E		2b	8t
				case(0xC4): registers.h = OP_set(0x01, registers.h);	break;	// SET 0, H		2b	8t
				case(0xC5): registers.l = OP_set(0x01, registers.l);	break;	// SET 0, L		2b	8t
				case(0xC6): write(registers.getHL(), OP_set(0x01, read(registers.getHL())));		break;	// SET 0, (HL)	2b	16t
				case(0xC7): registers.a = OP_set(0x01, registers.a);	break;	// SET 0, A		2b	8t
				case(0xC8): registers.b = OP_set(0x02, registers.b);	break;	// SET 1, B		2b	8t
				case(0xC9): registers.c = OP_set(0x02, registers.c);	break;	// SET 1, C		2b	8t
				case(0xCA): registers.d = OP_set(0x02, registers.d);	break;	// SET 1, D		2b	8t
				case(0xCB): registers.e = OP_set(0x02, registers.e);	break;	// SET 1, E		2b	8t
				case(0xCC): registers.h = OP_set(0x02, registers.h);	break;	// SET 1, H		2b	8t
				case(0xCD): registers.l = OP_set(0x02, registers.l);	break;	// SET 1, L		2b	8t
				case(0xCE): write(registers.getHL(), OP_set(0x02, read(registers.getHL())));		break;	// SET 1, (HL)	2b	16t
				case(0xCF): registers.a = OP_set(0x02, registers.a);	break;	// SET 1, A		2b	8t
				case(0xD0): registers.b = OP_set(0x04, registers.b);	break;	// SET 2, B		2b	8t
				case(0xD1): registers.c = OP_set(0x04, registers.c);	break;	// SET 2, C		2b	8t
				case(0xD2): registers.d = OP_set(0x04, registers.d);	break;	// SET 2, D		2b	8t
				case(0xD3): registers.e = OP_set(0x04, registers.e);	break;	// SET 2, E		2b	8t
				case(0xD4): registers.h = OP_set(0x04, registers.h);	break;	// SET 2, H		2b	8t
				case(0xD5): registers.l = OP_set(0x04, registers.l);	break;	// SET 2, L		2b	8t
				case(0xD6): write(registers.getHL(), OP_set(0x04, read(registers.getHL())));		break;	// SET 2, (HL)	2b	16t
				case(0xD7): registers.a = OP_set(0x04, registers.a);	break;	// SET 2, A		2b	8t
				case(0xD8): registers.b = OP_set(0x08, registers.b);	break;	// SET 3, B		2b	8t
				case(0xD9): registers.c = OP_set(0x08, registers.c);	break;	// SET 3, C		2b	8t
				case(0xDA): registers.d = OP_set(0x08, registers.d);	break;	// SET 3, D		2b	8t
				case(0xDB): registers.e = OP_set(0x08, registers.e);	break;	// SET 3, E		2b	8t
				case(0xDC): registers.h = OP_set(0x08, registers.h);	break;	// SET 3, H		2b	8t
				case(0xDD): registers.l = OP_set(0x08, registers.l);	break;	// SET 3, L		2b	8t
				case(0xDE): write(registers.getHL(), OP_set(0x08, read(registers.getHL())));		break;	// SET 3, (HL)	2b	16t
				case(0xDF): registers.a = OP_set(0x08, registers.a);	break;	// SET 3, A		2b	8t
				case(0xE0): registers.b = OP_set(0x10, registers.b);	break;	// SET 4, B		2b	8t
				case(0xE1): registers.c = OP_set(0x10, registers.c);	break;	// SET 4, C		2b	8t
				case(0xE2): registers.d = OP_set(0x10, registers.d);	break;	// SET 4, D		2b	8t
				case(0xE3): registers.e = OP_set(0x10, registers.e);	break;	// SET 4, E		2b	8t
				case(0xE4): registers.h = OP_set(0x10, registers.h);	break;	// SET 4, H		2b	8t
				case(0xE5): registers.l = OP_set(0x10, registers.l);	break;	// SET 4, L		2b	8t
				case(0xE6): write(registers.getHL(), OP_set(0x10, read(registers.getHL())));		break;	// SET 4, (HL)	2b	16t
				case(0xE7): registers.a = OP_set(0x10, registers.a);	break;	// SET 4, A		2b	8t
				case(0xE8): registers.b = OP_set(0x20, registers.b);	break;	// SET 5, B		2b	8t
				case(0xE9): registers.c = OP_set(0x20, registers.c);	break;	// SET 5, C		2b	8t
				case(0xEA): registers.d = OP_set(0x20, registers.d);	break;	// SET 5, D		2b	8t
				case(0xEB): registers.e = OP_set(0x20, registers.e);	break;	// SET 5, E		2b	8t
				case(0xEC): registers.h = OP_set(0x20, registers.h);	break;	// SET 5, H		2b	8t
				case(0xED): registers.l = OP_set(0x20, registers.l);	break;	// SET 5, L		2b	8t
				case(0xEE): write(registers.getHL(), OP_set(0x20, read(registers.getHL())));		break;	// SET 5, (HL)	2b	16t
				case(0xEF): registers.a = OP_set(0x20, registers.a);	break;	// SET 5, A		2b	8t
				case(0xF0): registers.b = OP_set(0x40, registers.b);	break;	// SET 6, B		2b	8t
				case(0xF1): registers.c = OP_set(0x40, registers.c);	break;	// SET 6, C		2b	8t
				case(0xF2): registers.d = OP_set(0x40, registers.d);	break;	// SET 6, D		2b	8t
				case(0xF3): registers.e = OP_set(0x40, registers.e);	break;	// SET 6, E		2b	8t
				case(0xF4): registers.h = OP_set(0x40, registers.h);	break;	// SET 6, H		2b	8t
				case(0xF5): registers.l = OP_set(0x40, registers.l);	break;	// SET 6, L		2b	8t
				case(0xF6): write(registers.getHL(), OP_set(0x40, read(registers.getHL())));		break;	// SET 6, (HL)	2b	16t
				case(0xF7): registers.a = OP_set(0x40, registers.a);	break;	// SET 6, A		2b	8t
				case(0xF8): registers.b = OP_set(0x80, registers.b);	break;	// SET 7, B		2b	8t
				case(0xF9): registers.c = OP_set(0x80, registers.c);	break;	// SET 7, C		2b	8t
				case(0xFA): registers.d = OP_set(0x80, registers.d);	break;	// SET 7, D		2b	8t
				case(0xFB): registers.e = OP_set(0x80, registers.e);	break;	// SET 7, E		2b	8t
				case(0xFC): registers.h = OP_set(0x80, registers.h);	break;	// SET 7, H		2b	8t
				case(0xFD): registers.l = OP_set(0x80, registers.l);	break;	// SET 7, L		2b	8t
				case(0xFE): write(registers.getHL(), OP_set(0x80, read(registers.getHL())));		break;	// SET 7, (HL)	2b	16t
				case(0xFF): registers.a = OP_set(0x80, registers.a);	break;	// SET 7, A		2b	8t
				default: OP_nop();										break;	// NOP				1b	4t
			} break;
		case(0xCC): OP_call(registers.getFlag(FLAGS::ZERO));			break;	// CALL Z, u16			3b	12t-24t
		case(0xCD): OP_call(true);										break;	// CALL u16				3b	24t
		case(0xCE): OP_adc(read8());									break;	// ADC A, u8			2b	8t
		case(0xCF):	OP_rst(0x08);										break;	// RST 08h				1b	16t
		case(0xD0): OP_ret(!registers.getFlag(FLAGS::CARRY));			break;	// RET NC				1b	8t-20t
		case(0xD1): registers.setDE(OP_pop());							break;	// POP DE				1b	12t
		case(0xD2): OP_jp(!registers.getFlag(FLAGS::CARRY));			break;	// JP NC, u16			3b	12t-16t
		//case(0xD3):	break;
		case(0xD4): OP_call(!registers.getFlag(FLAGS::CARRY));			break;	// CALL NC, u16			3b	12t-24t
		case(0xD5): OP_push(registers.getDE());							break;	// PUSH DE				1b	16t
		case(0xD6): OP_sub(read8());									break;	// SUB A, u8			2b	8t
		case(0xD7): OP_rst(0x10);										break;	// RST 10h				1b	16t
		case(0xD8): OP_ret(registers.getFlag(FLAGS::CARRY));			break;	// RET C				1b	8t-20t
		case(0xD9): OP_reti();											break;	// RETI					1b	16t
		case(0xDA):	OP_jp(registers.getFlag(FLAGS::CARRY));				break;	// JP C, u16			3b	12t-16t
		//case(0xDB):	break;
		case(0xDC):	OP_call(registers.getFlag(FLAGS::CARRY));			break;	// CALL C, u16			3b	12t-24t
		//case(0xDD):	break;
		case(0xDE):	OP_sbc(read8());									break;	// SBC A, u8			2b	8t
		case(0xDF):	OP_rst(0x18);										break;	// RST 18h				1b	16t
		case(0xE0):	write(0xFF00 + read8(), registers.a);				break;	// LD (FF00+u8), A		2b	12t
		case(0xE1):	registers.setHL(OP_pop());							break;	// POP HL				1b	12t
		case(0xE2):	write(0xFF00 + registers.c, registers.a);			break;	// LD (FF00+C), A		1b	8t
		//case(0xE3):	break; 
		//case(0xE4):	break; 
		case(0xE5):	OP_push(registers.getHL());							break;	// PUSH HL				1b	16t
		case(0xE6):	OP_and(read8());									break;	// AND A, u8			2b	8t
		case(0xE7):	OP_rst(0x20);										break;	// RST 20h				1b	16t
		case(0xE8):	sp = OP_add16(sp, read8());							break;	// ADD SP, i8			2b	16t
		case(0xE9):	pc = registers.getHL();								break;	// JP HL				1b	4t
		case(0xEA): write(read16(), registers.a);						break;	// LD (u16), A			3b	16t
		//case(0xEB):	break;
		//case(0xEC):	break;
		//case(0xED):	break;
		case(0xEE): OP_xor(read8());									break;	// XOR A, u8				2b	8t
		case(0xEF): OP_rst(0x28);										break;	// RST 28h				1b	16t
		case(0xF0):	registers.a = read(0xFF00 + read8());				break;	// LD A, (FF00+u8)		2b	12t
		case(0xF1):	registers.setAF(OP_pop());							break;	// POP AF				1b	12t
		case(0xF2):	registers.a = read(0xFF00 + registers.c);			break;	// LD A, (FF00+C)		1b	8t
		case(0xF3): OP_di();											break;	// DI					1b	4t
		//case(0xF4):	break;
		case(0xF5):	OP_push(registers.getAF());							break;	// PUSH AF				1b	16t
		case(0xF6):	OP_or(read8());										break;	// OR A, u8				2b	8t
		case(0xF7):	OP_rst(0x30);										break;	// RST 30h				1b	16t
		case(0xF8):	OP_ld16(read8());									break;	// LD HL, SP+i8			2b	12t
		case(0xF9): sp = registers.getHL();								break;	// LD SP, HL			1b	8t
		case(0xFA): registers.a = read(read16());						break;	// LD A, (u16)			3b	16t
		case(0xFB):	OP_ei();											break;	// EI					1b	4t
		//case(0xFC):	break;
		//case(0xFD):	break;
		case(0xFE):	OP_cp(read8());										break;	// CP A, u8				2b	8t
		case(0xFF):	OP_rst(0x38);										break;	// RST 38h				1b	16t
		default: OP_nop();												break;	// NOP						1b	4t
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