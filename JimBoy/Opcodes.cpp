#include "Cpu.h"
#include "Utils.h"

// NOP: No operation
void Cpu::OP_nop() {
	return;
}

// STOP: Enter CPU very low power mode
void Cpu::OP_stop() {
	halted = true;
}

// JR: Relative Jump
void Cpu::OP_jr(bool cc) {
	signed char e = read8();
	if (cc) {
		branch = true;
		pc += e;
	}
}

// LD 16-Bit: Load value into register
void Cpu::OP_ld16(uint16_t v) {
	switch(opcode) {
		//LD (u16), SP
		case(0x08):
			write(v, sp & 0x00FF);
			write(v + 1, sp >> 8);
			break;
		//LD HL, SP+i8
		case(0xF8):
			registers.setHL(sp + (signed char)v);
			registers.setFlag(ZERO, false);
			registers.setFlag(SUB, false);
			registers.setFlag(HALF, halfCarryAdd16(sp, (signed char)v));
			registers.setFlag(CARRY, ((sp + (signed char)v) >> 10) != 0);
			break;
	}
}

// LD 8-Bit: Load value into register. NEVER USED
uint8_t Cpu::OP_ld(uint8_t v) {
	return v;
}

// INC 16-Bit: Increment value in register by 1.
uint16_t Cpu::OP_inc16(uint16_t v) {
	return v + 1;
}

// INC 8-Bit: Increment value in register r8 by 1.
uint8_t Cpu::OP_inc(uint8_t v) {
	uint8_t e = v + 1;

	registers.setFlag(ZERO, e == 0);
	registers.setFlag(SUB, false);
	registers.setFlag(HALF, halfCarryAdd(v, 1));

	return e;
}

// DEC 8-Bit: Decrement value in register r8 by 1.
uint8_t Cpu::OP_dec(uint8_t v) {
	uint8_t e = v - 1;

	registers.setFlag(ZERO, e == 0);
	registers.setFlag(SUB, true);
	registers.setFlag(HALF, halfCarrySub(v, 1));

	return e;
}

// RLCA: Rotate register A left.
void Cpu::OP_rlca() {
	uint8_t v = (registers.a << 1) | (registers.a >> 7);

	registers.setFlag(ZERO, false);
	registers.setFlag(SUB, false);
	registers.setFlag(HALF, false);
	registers.setFlag(CARRY, (registers.a & 0x80) != 0);

	registers.a = v;
}

// RLA: Rotate register A left through carry.
void Cpu::OP_rla() {
	uint8_t v = (registers.a << 1) | (registers.getFlag(CARRY) ? 0x01 : 0x00);

	registers.setFlag(ZERO, false);
	registers.setFlag(SUB, false);
	registers.setFlag(HALF, false);
	registers.setFlag(CARRY, (registers.a & 0x80) != 0);

	registers.a = v;
}

// DAA: Decimal Adjust Accumulator to get a correct BCD representation after an arithmetic instruction.
void Cpu::OP_daa() {
	uint8_t result = registers.a;

	// Check if previous arithmetic instruction was addition or subtraction
	if (registers.getFlag(SUB)) {
		if (registers.getFlag(HALF))
			registers.a -= 0x06;
		if (registers.getFlag(CARRY))
			registers.a -= 0x60;
	} else {
		if (registers.getFlag(HALF) || (registers.a & 0x0F) > 0x09)
			registers.a += 0x06;
		if (registers.getFlag(CARRY) || (registers.a >> 4) > 0x09) {
			registers.a += 0x60;
			registers.setFlag(CARRY, true);
		}
	}

	registers.setFlag(ZERO, result == 0);
	registers.setFlag(HALF, false);
}

// SCF: Set Carry Flag.
void Cpu::OP_scf() {
	registers.setFlag(SUB, false);
	registers.setFlag(HALF, false);
	registers.setFlag(CARRY, true);
}

// ADD 16-Bit: Add value to register.
uint16_t Cpu::OP_add16(uint16_t reg, uint16_t v, bool s) {
	uint16_t e;
	if (s) {
		e = reg + (signed char)v;
		registers.setFlag(ZERO, false);
	} else {
		e = reg + v;
	}
	registers.setFlag(SUB, false);
	registers.setFlag(HALF, halfCarryAdd16(reg, v));
	registers.setFlag(CARRY, (e >> 10) != 0);

	return e;
}

// DEC 16-Bit: Decrement value in register r16 by 1.
uint16_t Cpu::OP_dec16(uint16_t v) {
	return v - 1;
}

// RRCA: Rotate register A right.
void Cpu::OP_rrca() {
	uint8_t v = (registers.a >> 1) | (registers.a << 7);

	registers.setFlag(ZERO, false);
	registers.setFlag(SUB, false);
	registers.setFlag(HALF, false);
	registers.setFlag(CARRY, (registers.a & 0x01) != 0);

	registers.a = v;
}

// RRA: Rotate register A right through carry.
void Cpu::OP_rra() {
	uint8_t v = (registers.a >> 1) | (registers.getFlag(CARRY) ? 0x80 : 0x00);

	registers.setFlag(ZERO, false);
	registers.setFlag(SUB, false);
	registers.setFlag(HALF, false);
	registers.setFlag(CARRY, (registers.a & 0x01) != 0);

	registers.a = v;
}

// CPL: ComPLement accumulator (A = ~A). 
void Cpu::OP_cpl() {
	registers.a = ~registers.a;

	registers.setFlag(SUB, true);
	registers.setFlag(HALF, true);
}

// CCF: Complement Carry Flag. 
void Cpu::OP_ccf() {
	registers.setFlag(SUB, false);
	registers.setFlag(HALF, false);
	registers.setFlag(CARRY, !registers.getFlag(CARRY));
}

// HALT: Enter CPU low-power consumption mode until an interrupt occurs.
void Cpu::OP_halt() {
	if (ime) {
		halted = true;
	} else {
		// If IF and IE != 0
		if (read(IF_REGISTER_ADDRESS) && read(IE_REGISTER_ADDRESS)) {
			
		}
	}
}

// ADD 8-Bit: Add value to A. 
void Cpu::OP_add(uint8_t v) {
	uint16_t e = registers.a + v;

	registers.setFlag(ZERO, e == 0);
	registers.setFlag(SUB, false);
	registers.setFlag(HALF, halfCarryAdd(registers.a, v));
	registers.setFlag(CARRY, (e >> 8) != 0);

	registers.a = e;
}

// ADC 8-Bit: Add value plus the carry flag to A.
void Cpu::OP_adc(uint8_t v) {
	uint16_t e = registers.a + v + registers.getFlag(CARRY);

	registers.setFlag(ZERO, e == 0);
	registers.setFlag(SUB, false);
	registers.setFlag(HALF, halfCarryAdd(registers.a, v + registers.getFlag(CARRY)));
	registers.setFlag(CARRY, (e >> 8) != 0);

	registers.a = e;
}

// SUB 8-Bit: Subtract the value from A. 
void Cpu::OP_sub(uint8_t v) {
	uint8_t e = registers.a - v;

	registers.setFlag(ZERO, e == 0);
	registers.setFlag(SUB, true);
	registers.setFlag(HALF, halfCarrySub(registers.a, v));
	registers.setFlag(CARRY, v > registers.a);

	registers.a = e;
}

// SBC 8-Bit: Subtract the value and the carry flag from A. 
void Cpu::OP_sbc(uint8_t v) {
	uint8_t e = registers.a - (v + registers.getFlag(CARRY));

	registers.setFlag(ZERO, e == 0);
	registers.setFlag(SUB, true);
	registers.setFlag(HALF, halfCarrySub(registers.a, v + registers.getFlag(CARRY)));
	registers.setFlag(CARRY, (v - registers.getFlag(CARRY)) > registers.a);

	registers.a = e;
}

// AND 8-Bit: Bitwise AND between the value and A. 
void Cpu::OP_and(uint8_t v) {
	uint8_t e = registers.a & v;

	registers.setFlag(ZERO, e == 0);
	registers.setFlag(SUB, false);
	registers.setFlag(HALF, true);
	registers.setFlag(CARRY, false);

	registers.a = e;
}

// XOR 8-Bit: Bitwise XOR between the value and A. 
void Cpu::OP_xor(uint8_t v) {
	uint8_t e = registers.a ^ v;

	registers.setFlag(ZERO, e == 0);
	registers.setFlag(SUB, false);
	registers.setFlag(HALF, false);
	registers.setFlag(CARRY, false);

	registers.a = e;
}

// OR 8-Bit: Store into A the bitwise OR of the value and A.
void Cpu::OP_or(uint8_t v) {
	uint8_t e = registers.a | v;

	registers.setFlag(ZERO, e == 0);
	registers.setFlag(SUB, false);
	registers.setFlag(HALF, false);
	registers.setFlag(CARRY, false);

	registers.a = e;
}

// CP 8-Bit: Subtract the value from A and set flags accordingly, but don't store the result.
void Cpu::OP_cp(uint8_t v) {
	uint8_t e = registers.a - v;

	registers.setFlag(ZERO, e == 0);
	registers.setFlag(SUB, true);
	registers.setFlag(HALF, halfCarrySub(registers.a, v));
	registers.setFlag(CARRY, registers.a < v);
}

// RET: Return from subroutine.
void Cpu::OP_ret(bool cc) {
	if (cc) {
		branch = true;
		pc = OP_pop();
	}
}

// POP: Pop register from the stack.
uint16_t Cpu::OP_pop() {
	uint8_t lo = read(sp++);
	uint8_t hi = read(sp++);

	return (hi << 8) | lo;
}

// JP: Jump to address. 
void Cpu::OP_jp(bool cc) {
	uint16_t nn = read16();
	if (cc) {
		branch = true;
		pc = nn;
	}
}

// DI: Disable Interrupts by clearing the IME flag. 
void Cpu::OP_di() {
	ime = false;
}

// CALL: Call address.
void Cpu::OP_call(bool cc) {
	if (cc) {
		branch = true;
		OP_push(pc + 2);
		pc = read16();
	} else {
		pc += 2;
	}
}

// PUSH: Push register into the stack.
void Cpu::OP_push(uint16_t v) {
	write(--sp, v >> 8);
	write(--sp, v & 0x00FF);
}

// RST: Call address vec.
void Cpu::OP_rst(uint8_t v) {
	OP_push(pc);
	pc = v;
}

// RETI: Return from subroutine and enable interrupts.
void Cpu::OP_reti() {
	OP_ei();
	OP_ret(true);
}

// EI: Enable Interrupts by setting the IME flag. The flag is only set after the instruction following EI.
void Cpu::OP_ei() {
	ime_scheduled = true;
}

// RLC: Rotate register left.
uint8_t Cpu::OP_rlc(uint8_t reg) {
	uint8_t v = (reg << 1) | (reg >> 7);

	registers.setFlag(ZERO, v == 0);
	registers.setFlag(SUB, false);
	registers.setFlag(HALF, false);
	registers.setFlag(CARRY, (reg & 0x80) != 0);

	return v;
}

// RRC: Rotate register right.
uint8_t Cpu::OP_rrc(uint8_t reg) {
	uint8_t v = (reg >> 1) | (reg << 7);

	registers.setFlag(ZERO, v == 0);
	registers.setFlag(SUB, false);
	registers.setFlag(HALF, false);
	registers.setFlag(CARRY, (reg & 0x01) != 0);

	return v;
}

// RL: Rotate register left through carry.
uint8_t Cpu::OP_rl(uint8_t reg) {
	//(reg << 1) | (reg >> 7)
	uint8_t v = (reg << 1) | (registers.getFlag(CARRY) ? 0x01 : 0x00);

	registers.setFlag(ZERO, v == 0);
	registers.setFlag(SUB, false);
	registers.setFlag(HALF, false);
	registers.setFlag(CARRY, (reg & 0x80) != 0);

	return v;
}

// RR: Rotate register right through carry.
uint8_t Cpu::OP_rr(uint8_t reg) {
	//(reg >> 1) | (reg << 7)
	uint8_t v = (reg >> 1) | (registers.getFlag(CARRY) ? 0x80 : 0x00);

	registers.setFlag(ZERO, v == 0);
	registers.setFlag(SUB, false);
	registers.setFlag(HALF, false);
	registers.setFlag(CARRY, (reg & 0x01) != 0);

	return v;
}

// SLA: Shift Left Arithmetic register.
uint8_t Cpu::OP_sla(uint8_t reg) {
	uint8_t v = reg << 1;

	registers.setFlag(ZERO, v == 0);
	registers.setFlag(SUB, false);
	registers.setFlag(HALF, false);
	registers.setFlag(CARRY, (reg & 0x80) != 0);

	return v;
}

// SRA: Shift Right Arithmetic register.
uint8_t Cpu::OP_sra(uint8_t reg) {
	uint8_t v = (reg >> 1) | (reg & 0x80);

	registers.setFlag(ZERO, v == 0);
	registers.setFlag(SUB, false);
	registers.setFlag(HALF, false);
	registers.setFlag(CARRY, (reg & 0x01) != 0);

	return v;
}

// SWAP: Swap upper 4 bits in register and the lower 4 ones. 
uint8_t Cpu::OP_swap(uint8_t reg) {
	uint8_t hi = (reg & 0xF0) >> 4;
	uint8_t lo = reg & 0x0F;

	uint8_t result = (lo << 4) | hi;
	registers.setFlag(ZERO, result == 0);
	registers.setFlag(SUB, false);
	registers.setFlag(HALF, false);
	registers.setFlag(CARRY, false);

	return result;
}

// SRL: Shift Right Logic register.
uint8_t Cpu::OP_srl(uint8_t reg) {
	uint8_t v = reg >> 1;

	registers.setFlag(ZERO, v == 0);
	registers.setFlag(SUB, false);
	registers.setFlag(HALF, false);
	registers.setFlag(CARRY, (reg & 0x01) != 0);

	return v;
}

// BIT: Test bit in register, set the zero flag if bit not set
void Cpu::OP_bit(uint8_t v, uint8_t reg) {
	registers.setFlag(ZERO, (reg & v) == 0);
	registers.setFlag(SUB, false);
	registers.setFlag(HALF, true);
}

// RES: Set bit in register to 0.
uint8_t Cpu::OP_res(uint8_t v, uint8_t reg) {
	return reg & ~v;
}

// SET: Set bit in register to 1. 
uint8_t Cpu::OP_set(uint8_t v, uint8_t reg) {
	return reg | v;
}