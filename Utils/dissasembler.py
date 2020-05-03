import fire
import os

instructions = [
    "NOP", "LD BC, u16", "LD (BC), A", "INC BC", "INC B", "DEC B", "LD B, u8", "RLCA", "LD (u16), SP", "ADD HL, BC", "LD A, (BC)", "DEC BC", "INC C", "DEC C", "LD C, u8", "RRCA",
    "STOP", "LD DE, u16", "LD (DE), A", "INC DE", "INC D", "DEC D", "LD D, u8", "RLA", "JR e8", "ADD HL, DE", "LD A, (DE)", "DEC DE", "INC E", "DEC E", "LD E, u8", "RRA",
    "JR NZ, e8", "LD HL, u16", "LD (HL+), A", "INC HL", "INC H", "DEC H", "LD H, u8", "DAA", "JR Z, e8", "ADD HL, HL", "LD A, (HL+)", "DEC HL", "INC L", "DEC L", "LD L, u8", "CPL",
    "JR NC, e8", "LD SP, u16", "LD (HL-), A", "INC SP", "INC (HL)", "DEC (HL)", "LD (HL), u8", "SCF", "JR C, e8", "ADD HL, SP", "LD A, (HL-)", "DEC SP", "INC A", "DEC A", "LD A, u8", "CCF",

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
    "LD (FF00+u8), A", "POP HL", "LD (FF00+C), A", "INVALID", "INVALID", "PUSH HL", "AND A, u8", "RST 20h", "ADD SP, e8", "JP HL", "LD (u16), A", "INVALID", "INVALID", "INVALID", "XOR A,u8", "RST 28h",
    "LD A, (FF00+u8)", "POP AF", "LD A, (FF00+C)", "DI", "INVALID", "PUSH AF", "OR A, u8", "RST 30h", "LD HL, SP+e8", "LD SP, HL", "LD A, (u16)", "EI", "INVALID", "INVALID", "CP A, u8", "RST 38h"
]
prefix_instructions = [ 
    "RLC B", "RLC C", "RLC D", "RLC E", "RLC H", "RLC L", "RLC (HL)", "RLC A", "RRC B", "RRC C", "RRC D", "RRC E", "RRC H", "RRC L", "RRC (HL)", "RRC A",
    "RL B", "RL C", "RL D", "RL E", "RL H", "RL L", "RL (HL)", "RL A", "RR B", "RR C", "RR D", "RR E", "RR H", "RR L", "RR (HL)", "RR A",
    "SLA B", "SLA C", "SLA D", "SLA E", "SLA H", "SLA L", "SLA (HL)", "SLA A", "SRA B", "SRA C", "SRA D", "SRA E", "SRA H", "SRA L", "SRA (HL)", "SRA A",
    "SWAP B", "SWAP C", "SWAP D", "SWAP E", "SWAP H", "SWAP L", "SWAP (HL)", "SWAP A", "SRL B", "SRL C", "SRL D", "SRL E", "SRL H", "SRL L", "SRL (HL)", "SRL A",

    "BIT 0, B", "BIT 0, C", "BIT 0, D", "BIT 0, E", "BIT 0, H", "BIT 0, L", "BIT 0, (HL)", "BIT 0, A", "BIT 1, B", "BIT 1, C", "BIT 1, D", "BIT 1, E", "BIT 1, H", "BIT 1, L", "BIT 1, (HL)", "BIT 1, A",
    "BIT 2, B", "BIT 2, C", "BIT 2, D", "BIT 2, E", "BIT 2, H", "BIT 2, L", "BIT 2, (HL)", "BIT 2, A", "BIT 3, B", "BIT 3, C", "BIT 3, D", "BIT 3, E", "BIT 3, H", "BIT 3, L", "BIT 3, (HL)", "BIT 3, A",
    "BIT 4, B", "BIT 4, C", "BIT 4, D", "BIT 4, E", "BIT 4, H", "BIT 4, L", "BIT 4, (HL)", "BIT 4, A", "BIT 5, B", "BIT 5, C", "BIT 5, D", "BIT 5, E", "BIT 5, H", "BIT 5, L", "BIT 5, (HL)", "BIT 5, A",
    "BIT 6, B", "BIT 6, C", "BIT 6, D", "BIT 6, E", "BIT 6, H", "BIT 6, L", "BIT 6, (HL)", "BIT 6, A", "BIT 7, B", "BIT 7, C", "BIT 7, D", "BIT 7, E", "BIT 7, H", "BIT 7, L", "BIT 7, (HL)", "BIT 7, A",

    "RES 0, B", "RES 0, C", "RES 0, D", "RES 0, E", "RES 0, H", "RES 0, L", "RES 0, (HL)", "RES 0, A", "RES 1, B", "RES 1, C", "RES 1, D", "RES 1, E", "RES 1, H", "RES 1, L", "RES 1, (HL)", "RES 1, A",
    "RES 2, B", "RES 2, C", "RES 2, D", "RES 2, E", "RES 2, H", "RES 2, L", "RES 2, (HL)", "RES 2, A", "RES 3, B", "RES 3, C", "RES 3, D", "RES 3, E", "RES 3, H", "RES 3, L", "RES 3, (HL)", "RES 3, A",
    "RES 4, B", "RES 4, C", "RES 4, D", "RES 4, E", "RES 4, H", "RES 4, L", "RES 4, (HL)", "RES 4, A", "RES 5, B", "RES 5, C", "RES 5, D", "RES 5, E", "RES 5, H", "RES 5, L", "RES 5, (HL)", "RES 5, A",
    "RES 6, B", "RES 6, C", "RES 6, D", "RES 6, E", "RES 6, H", "RES 6, L", "RES 6, (HL)", "RES 6, A", "RES 7, B", "RES 7, C", "RES 7, D", "RES 7, E", "RES 7, H", "RES 7, L", "RES 7, (HL)", "RES 7, A",

    "SET 0, B", "SET 0, C", "SET 0, D", "SET 0, E", "SET 0, H", "SET 0, L", "SET 0, (HL)", "SET 0, A", "SET 1, B", "SET 1, C", "SET 1, D", "SET 1, E", "SET 1, H", "SET 1, L", "SET 1, (HL)", "SET 1, A",
    "SET 2, B", "SET 2, C", "SET 2, D", "SET 2, E", "SET 2, H", "SET 2, L", "SET 2, (HL)", "SET 2, A", "SET 3, B", "SET 3, C", "SET 3, D", "SET 3, E", "SET 3, H", "SET 3, L", "SET 3, (HL)", "SET 3, A",
    "SET 4, B", "SET 4, C", "SET 4, D", "SET 4, E", "SET 4, H", "SET 4, L", "SET 4, (HL)", "SET 4, A", "SET 5, B", "SET 5, C", "SET 5, D", "SET 5, E", "SET 5, H", "SET 5, L", "SET 5, (HL)", "SET 5, A",
    "SET 6, B", "SET 6, C", "SET 6, D", "SET 6, E", "SET 6, H", "SET 6, L", "SET 6, (HL)", "SET 6, A", "SET 7, B", "SET 7, C", "SET 7, D", "SET 7, E", "SET 7, H", "SET 7, L", "SET 7, (HL)", "SET 7, A"
]
byte_length = [
    1, 3, 1, 1, 1, 1, 2, 1, 3, 1, 1, 1, 1, 1, 2, 1,
    2, 3, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1,
    2, 3, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1,
    2, 3, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1,

    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

    1, 1, 3, 3, 3, 1, 2, 1, 1, 1, 3, 1, 3, 3, 2, 1,
    1, 1, 3, 0, 3, 1, 2, 1, 1, 1, 3, 0, 3, 0, 2, 1,
    2, 1, 1, 0, 0, 1, 2, 1, 2, 1, 3, 0, 0, 0, 2, 1,
    2, 1, 1, 1, 0, 1, 2, 1, 2, 1, 3, 1, 0, 0, 2, 1
]
prefix_byte_length = 2
illegal_opcodes = bytes([0xD3, 0xE3, 0xE4, 0xF4, 0xDB, 0xEB, 0xEC, 0xFC, 0xDD, 0xED, 0xFD])

"""NOT IN USE FOR NOW
prefix_cycles = [
    "2", "2", "2", "2", "2", "2", "4", "2", "2", "2", "2", "2", "2", "2", "4", "2",
    "2", "2", "2", "2", "2", "2", "4", "2", "2", "2", "2", "2", "2", "2", "4", "2",
    "2", "2", "2", "2", "2", "2", "4", "2", "2", "2", "2", "2", "2", "2", "4", "2",
    "2", "2", "2", "2", "2", "2", "4", "2", "2", "2", "2", "2", "2", "2", "4", "2",

    "2", "2", "2", "2", "2", "2", "3", "2", "2", "2", "2", "2", "2", "2", "3", "2",
    "2", "2", "2", "2", "2", "2", "3", "2", "2", "2", "2", "2", "2", "2", "3", "2",
    "2", "2", "2", "2", "2", "2", "3", "2", "2", "2", "2", "2", "2", "2", "3", "2",
    "2", "2", "2", "2", "2", "2", "3", "2", "2", "2", "2", "2", "2", "2", "3", "2",

    "2", "2", "2", "2", "2", "2", "4", "2", "2", "2", "2", "2", "2", "2", "4", "2",
    "2", "2", "2", "2", "2", "2", "4", "2", "2", "2", "2", "2", "2", "2", "4", "2",
    "2", "2", "2", "2", "2", "2", "4", "2", "2", "2", "2", "2", "2", "2", "4", "2",
    "2", "2", "2", "2", "2", "2", "4", "2", "2", "2", "2", "2", "2", "2", "4", "2",

    "2", "2", "2", "2", "2", "2", "4", "2", "2", "2", "2", "2", "2", "2", "4", "2",
    "2", "2", "2", "2", "2", "2", "4", "2", "2", "2", "2", "2", "2", "2", "4", "2",
    "2", "2", "2", "2", "2", "2", "4", "2", "2", "2", "2", "2", "2", "2", "4", "2",
    "2", "2", "2", "2", "2", "2", "4", "2", "2", "2", "2", "2", "2", "2", "4", "2"
]
cycles = [
    "1",    "3", "2",   "2", "1",   "1", "2", "1", "5",     "2", "2",   "2", "1",   "1", "2", "1",
    "1",    "3", "2",   "2", "1",   "1", "2", "1", "3",     "2", "2",   "2", "1",   "1", "2", "1", 
    "2-3",  "3", "2",   "2", "1",   "1", "2", "1", "2-3",   "2", "2",   "2", "1",   "1", "2", "1",
    "2-3",  "3", "2",   "2", "3",   "3", "3", "1", "2-3",   "2", "2",   "2", "1",   "1", "2", "1",

    "1",    "1", "1",   "1", "1",   "1", "2", "1", "1",     "1", "1",   "1", "1",   "1", "2", "1",
    "1",    "1", "1",   "1", "1",   "1", "2", "1", "1",     "1", "1",   "1", "1",   "1", "2", "1",
    "1",    "1", "1",   "1", "1",   "1", "2", "1", "1",     "1", "1",   "1", "1",   "1", "2", "1",
    "2",    "2", "2",   "2", "2",   "2", "1", "2", "1",     "1", "1",   "1", "1",   "1", "2", "1",

    "1",    "1", "1",   "1", "1",   "1", "2", "1", "1",     "1", "1",   "1", "1",   "1", "2", "1",
    "1",    "1", "1",   "1", "1",   "1", "2", "1", "1",     "1", "1",   "1", "1",   "1", "2", "1",
    "1",    "1", "1",   "1", "1",   "1", "2", "1", "1",     "1", "1",   "1", "1",   "1", "2", "1",
    "1",    "1", "1",   "1", "1",   "1", "2", "1", "1",     "1", "1",   "1", "1",   "1", "2", "1",

    "2-5",  "3", "3-4", "4", "3-6", "4", "2", "4", "2-5",   "4", "3-4", "0", "3-6", "6", "2", "4",
    "2-5",  "3", "3-4", "0", "3-6", "4", "2", "4", "2-5",   "4", "3-4", "0", "3-6", "0", "2", "4",
    "3",    "3", "2",   "0", "0",   "4", "2", "4", "4",     "2", "4",   "0", "0",   "0", "2", "4",
    "3",    "3", "2",   "1", "0",   "4", "2", "4", "3",     "2", "4",   "1", "0",   "0", "2", "4"
]
"""

allowed_extensions = ['.gb']

class Dissasembler:
    def __init__(self):
        self.i = 0
        self.prefix = False
        self.header = []
    
    def translate(self, opcode):
        if opcode in illegal_opcodes:
            self.i += 1
            return "INVALID", 0
    
        if self.prefix:
            self.i += prefix_byte_length
            self.prefix = False
            return prefix_instructions[opcode], prefix_byte_length
        self.i += byte_length[opcode]
        return instructions[opcode], byte_length[opcode]
    
    @staticmethod
    def byte_to_sbyte(byte_hex_str):
        unsigned = ord(bytearray.fromhex(byte_hex_str))
        sbyte = unsigned - 256 if unsigned > 127 else unsigned
        return sbyte
    
    def handle_args(self, instr, args):
        if 'u8' in instr:
            new_instr = instr.replace('u8', args[0])
        elif 'u16' in instr:
            new_instr = instr.replace('u16', ''.join(args[::-1]))
        elif 'e8' in instr:
            new_instr = instr.replace('e8', str(args[0][2:].upper()))
        else:
            new_instr = instr
        return new_instr
        
    def handle_header(self, header_buffer):
        new_header_buffer = []
        # Nintendo Logo (0x104 - 0x133)
        offset = (0x133 - 0x104) + 1
        logo = {'addr': self.buffer_header_start, 'instr': '', 'args':[], 'comment': ""}
        for i in range(0, offset):
            logo['instr'] += header_buffer[i]['opcode']
        new_header_buffer.append(logo)
        # Title (0x134 - 0x0143)
        offset = (0x143 - 0x134) + 1
        title = {'addr': self.buffer_header_start + title_offset, 'instr': '', 'args':[], 'comment': ""}
        for i in range(0, title_offset):
            if  header_buffer[i + offset]['opcode'] == '00':
                continue
            title['instr'] += header_buffer[i + offset]['opcode']
        title['instr'] = bytes.fromhex(title['instr']).decode("ASCII")
        new_header_buffer.append(title)
        # New Licensee Code (0x0144 - 0x0145)
        ne
        # SGB Flag (0x0146)
        # Cartridge Type (0x147)
        # ROM Size (0x148)
        # RAM Size (0x149)
        # Destination Code (0x14A)
        # Old Licensee Code (0x14B)
        # Mask ROM Version number (0x14C)
        # Header Checksum (0x14D)
        # Global Checksum (0x14E - 0x14F)
        
        return new_header_buffer
    
    def disassemble(self, file_path, output_file):
        buffer = []
        with open(file_path, "rb") as f:
            for i in range(0, os.path.getsize(file_path)):
                byte = f.read(1)
                opcode_hex = byte.hex()
                opcode = int.from_bytes(byte, byteorder='big', signed=False)
                
                if not i == self.i:
                    if all(x in buffer[-1]['instr'] for x in ['JR', 'e8']):
                        e8 = self.byte_to_sbyte(opcode_hex)
                        buffer[-1]['args'].append(hex((buffer[-1]['addr'] + 2) + e8))
                        continue
                    
                    buffer[-1]['args'].append(opcode_hex)
                    continue
                
                if 0x14E >= self.i >= 0x104:
                    if self.i == 0x104:
                        self.buffer_header_start = len(buffer)
                    elif self.i == 0x14E:
                        self.buffer_header_stop = len(buffer)
                    buffer.append({
                        'addr': i,
                        'opcode': opcode_hex,
                        'instr': "HEADER",
                        'bytes': 1,
                        'args': []
                    })
                    self.i += 1
                    continue
            
                if opcode == 0xCB:
                    self.prefix = True
                    self.i += 1
                    continue
                    
                _instr, _len = self.translate(opcode)
                buffer.append({
                    'addr': i,
                    'opcode': opcode_hex,
                    'instr': _instr,
                    'bytes': _len,
                    'args': [],
                    'comment': ""
                })
        
        
        header_buffer = self.handle_header(buffer[self.buffer_header_start:self.buffer_header_stop + 1])
        buffer[self.buffer_header_start:self.buffer_header_stop + 1] = header_buffer
        
        with open(output_file, "w") as f:
            for e in buffer:
                if len(e['args']) > 0:
                    e['instr'] = self.handle_args(e['instr'], e['args'])
                
                f.write(f"0x{hex(e['addr'])[2:].upper()}: {e['instr']}\n")
        return

if __name__ == "__main__":
    fire.Fire(Dissasembler().disassemble)