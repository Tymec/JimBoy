#include "Cpu.h"

static const bool LOGGING_ENABLED = true;

// NOP: No operation.
// Opcodes: 0x00
void Cpu::OP_nop() {
	if (LOGGING_ENABLED)
		std::cout << "NOP" << std::endl;
	return;
}

// STOP: Enter CPU very low power mode. Also used to switch between double and normal speed CPU modes in GBC.
// Opcodes: 0x10
void Cpu::OP_stop() {
	lowPowerMode = true;
	if (LOGGING_ENABLED)
		std::cout << "STOP" << std::endl;
}

// JR: Relative Jump by adding e8 to the current address if condition cc is met. 
// Opcodes: 0x20, 0x30, 0x18, 0x28, 0x38
void Cpu::OP_jr() {
}

// LD 16-Bit: Load value n16 into register r16. 
// Opcodes: 0x01, 0x11, 0x21, 0x31, 0x08, 0xF9
void Cpu::OP_ld16() {
}

// LD 8-Bit: Load value n8 into register r8. 
// Opcodes: 0x40-0x7F (Except 0x76), 0x02, 0x12, 0x22, 0x32, 0x06, 0x16, 0x26, 0x36, 0x0A, 0x1A, 0x2A, 0x3A, 0x0E, 0x1E, 0x2E, 0x3E, 0xE0, 0xF0, 0xE2, 0xF2, 0xEA, 0xFA
void Cpu::OP_ld() {
}

// INC 16-Bit: Increment value in register r16 by 1.
// Opcodes: 0x03, 0x13, 0x23, 0x33 
void Cpu::OP_inc16() {
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
// Opcodes: 0x76
void Cpu::OP_halt() {
}

// ADD 8-Bit: Add the value in r8 to A. 
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

// CALL: Call address n16.
// Opcodes: 0xC4, 0xD4, 0xCC, 0xDC, 0xCD
void Cpu::OP_call() {
}

// PUSH 16-Bit: Push register AF into the stack.
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

// SRA:Shift Right Arithmetic register r8. [7] -> [7 -> 0] -> C
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

// BIT: Test bit u3 in register r8, set the zero flag if bit not set. 
// Opcodes: 0x40 - 0x7F
void Cpu::OP_bit() {
}

// RES: Set bit u3 in register r8 to 0. Bit 0 is the rightmost one, bit 7 the leftmost one. 
// Opcodes: 0x80 - 0xBF
void Cpu::OP_res() {
}

// SET: Set bit u3 in register r8 to 1. Bit 0 is the rightmost one, bit 7 the leftmost one. 
// Opcodes: 0xC0 - 0xFF
void Cpu::OP_set() {
}