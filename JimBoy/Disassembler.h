#pragma once
#include <iostream>

static const std::string Translation[] = {    // TEMPORARY SOLUTION
    "NOP", "LD BC, u16", "LD (BC), A", "INC BC", "INC B", "DEC B", "LD B, u8", "RLCA", "LD (u16), SP", "ADD HL, BC", "LD A, (BC)", "DEC BC", "INC C", "DEC C", "LD C, u8", "RRCA",
    "STOP", "LD DE, u16", "LD (DE), A", "INC DE", "INC D", "DEC D", "LD D, u8", "RLA", "JR i8", "ADD HL, DE", "LD A, (DE)", "DEC DE", "INC E", "DEC E", "LD E, u8", "RRA",
    "JR NZ, i8", "LD HL, u16", "LD (HL+), A", "INC HL", "INC H", "DEC H", "LD H, u8", "DAA", "JR Z, i8", "ADD HL, HL", "LD A, (HL+)", "DEC HL", "INC L", "DEC L", "LD L, u8", "CPL",
    "JR NC, i8", "LD SP, u16", "LD (HL-), A", "INC SP", "INC (HL)", "DEC (HL)", "LD (HL), u8", "SCF", "JR C, i8", "ADD HL, SP", "LD A, (HL-)", "DEC SP", "INC A", "DEC A", "LD A, u8", "CCF",

    "LD B, B", "LD B, C", "LD B, D", "LD B, E", "LD B, H", "LD B, L", "LD B, (HL)", "LD B, A", "LD C, B", "LD C, C", "LD C, D", "LD C, E", "LD C, H", "LD C, L", "LD C, (HL)", "LD C, A",
    "LD D, B", "LD D, C", "LD D, D", "LD D, E", "LD D, H", "LD D, L", "LD D, (HL)", "LD D, A", "LD E, B", "LD E, C", "LD E, D", "LD E, E", "LD E, H", "LD E, L", "LD E, (HL)", "LD E, A",
    "LD H, B", "LD H, C", "LD H, D", "LD H, E", "LD H, H", "LD H, L", "LD H, (HL)", "LD H, A", "LD L, B", "LD L, C", "LD L, D", "LD L, E", "LD L, H", "LD L, L", "LD L, (HL)", "LD L, A",
    "LD (HL), B", "LD (HL), C", "LD (HL), D", "LD (HL), E", "LD (HL), H", "LD (HL), L", "HALT", "LD (HL), A", "LD A, B", "LD A, C", "LD A, D", "LD A, E", "LD A, H", "LD A, L", "LD A, (HL)", "LD A, A",

    "ADD A, B", "ADD A, C", "ADD A, D", "ADD A, E", "ADD A, H", "ADD A, L", "ADD A, (HL)", "ADD A, A", "ADC A, B", "ADC A, C", "ADC A, D", "ADC A, E", "ADC A, H", "ADC A, L", "ADC A, (HL)", "ADC A, A",
    "SUB A, B", "SUB A, C", "SUB A, D", "SUB A, E", "SUB A, H", "SUB A, L", "SUB A, (HL)", "SUB A, A", "SBC A, B", "SBC A, C", "SBC A, D", "SBC A, E", "SBC A, H", "SBC A, L", "SBC A, (HL)", "SBC A, A",
    "AND A, B", "AND A, C", "AND A, D", "AND A, E", "AND A, H", "AND A, L", "AND A, (HL)", "AND A, A", "XOR A, B", "XOR A, C", "XOR A, D", "XOR A, E", "XOR A, H", "XOR A, L", "XOR A, (HL)", "XOR A, A",
    "OR A, B", "OR A, C", "OR A, D", "OR A, E", "OR A, H", "OR A, L", "OR A,(HL)", "OR A, A", "CP A, B", "CP A, C", "CP A, D", "CP A, E", "CP A, H", "CP A, L", "CP A, (HL)", "CP A, A",

    "RET NZ", "POP BC", "JP NZ, u16", "JP u16", "CALL NZ, u16", "PUSH BC", "ADD A, u8", "RST 00h", "RET Z", "RET", "JP Z, u16", "PREFIX 0xCB", "CALL Z, u16", "CALL u16", "ADC A, u8", "RST 08h",
    "RET NC", "POP DE", "JP NC, u16", "INVALID", "CALL NC, u16", "PUSH DE", "SUB A, u8", "RST 10h", "RET C", "RETI", "JP C, u16", "INVALID", "CALL C, u16", "INVALID", "SBC A, u8", "RST 18h",
    "LD (FF00+u8), A", "POP HL", "LD (FF00+C), A", "INVALID", "INVALID", "PUSH HL", "AND A, u8", "RST 20h", "ADD SP, i8", "JP HL", "LD (u16), A", "INVALID", "INVALID", "INVALID", "XOR A,u8", "RST 28h",
    "LD A, (FF00+u8)", "POP AF", "LD A, (FF00+C)", "DI", "INVALID", "PUSH AF", "OR A, u8", "RST 30h", "LD HL, SP+i8", "LD SP, HL", "LD A, (u16)", "EI", "INVALID", "INVALID", "CP A, u8", "RST 38h"
};

class Disassembler {
public:
	Disassembler();
private:
};

