#include "MOS6502.h"
#include <cstdio>

typedef Instruction i_t;
static Instruction instructions[16][16] = {
    { // 0x0*
        { "BRK", 0x00, 1, 7,  i_t::CycleMod::NONE },
        { "ORA", 0x01, 2, 6,  i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "ORA", 0x05, 2, 3, i_t::CycleMod::NONE },
        { "ASL", 0x06, 2, 5, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "PHP", 0x08, 1, 3, i_t::CycleMod::NONE },
        { "ORA", 0x09, 2, 2, i_t::CycleMod::NONE },
        { "ASL", 0x0A, 1, 2, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "ORA", 0x0D, 3, 4, i_t::CycleMod::NONE },
        { "ASL", 0x0E, 3, 6, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE }
    },
    { // 0x1*
        { "BPL", 0x10, 2, 2, i_t::CycleMod::MOD2 },
        { "ORA", 0x11, 2, 5, i_t::CycleMod::MOD1 },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "ORA", 0x15, 2, 4, i_t::CycleMod::NONE },
        { "ASL", 0x16, 2, 6, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "CLC", 0x18, 1, 2, i_t::CycleMod::NONE },
        { "ORA", 0x19, 3, 4, i_t::CycleMod::MOD1 },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "ORA", 0x1D, 3, 4, i_t::CycleMod::MOD1 },
        { "ASL", 0x1E, 3, 7, i_t::CycleMod:: NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE }
    },
    { // 0x2*
        { "JSR", 0x20, 3, 6, i_t::CycleMod::NONE },
        { "AND", 0x21, 2, 6, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "BIT", 0x24, 2, 3, i_t::CycleMod::NONE },
        { "AND", 0x25, 2, 3, i_t::CycleMod::NONE },
        { "ROL", 0x26, 2, 5, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "PLP", 0x28, 1, 4, i_t::CycleMod::NONE },
        { "AND", 0x29, 2, 2, i_t::CycleMod::NONE },
        { "ROL", 0x2A, 1, 2, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "BIT", 0x2C, 3, 4, i_t::CycleMod::NONE },
        { "AND", 0x2D, 3, 4, i_t::CycleMod::NONE },
        { "ROL", 0x2E, 3, 6, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE }
    },
    { // 0x3*
        { "BMI", 0x30, 2, 2, i_t::CycleMod::MOD2 },
        { "AND", 0x31, 2, 5, i_t::CycleMod::MOD1 },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "AND", 0x35, 2, 4, i_t::CycleMod::NONE },
        { "ROL", 0x36, 2, 6, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "SEC", 0x38, 1, 2, i_t::CycleMod::NONE },
        { "AND", 0x39, 3, 4, i_t::CycleMod::MOD1 },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "AND", 0x3D, 3, 4, i_t::CycleMod::MOD1 },
        { "ROL", 0x3E, 3, 7, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE }
    },
    { // 0x4*
        { "RTI", 0x40, 1, 6, i_t::CycleMod::NONE },
        { "EOR", 0x41, 2, 6, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "EOR", 0x45, 2, 3, i_t::CycleMod::NONE },
        { "LSR", 0x46, 2, 5, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "PHA", 0x48, 1, 3, i_t::CycleMod::NONE },
        { "EOR", 0x49, 2, 2, i_t::CycleMod::NONE },
        { "LSR", 0x4A, 1, 2, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "JMP", 0x4C, 3, 3, i_t::CycleMod::NONE },
        { "EOR", 0x4D, 3, 4, i_t::CycleMod::NONE },
        { "LSR", 0x4E, 3, 6, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE }
    },
    { // 0x5*
        { "BVC", 0x50, 2, 2, i_t::CycleMod::MOD2 },
        { "EPR", 0x51, 2, 5, i_t::CycleMod::MOD1 },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "EOR", 0x55, 2, 4, i_t::CycleMod::NONE },
        { "LSR", 0x56, 2, 6, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "CLI", 0x58, 1, 2, i_t::CycleMod::NONE },
        { "EOR", 0x59, 3, 4, i_t::CycleMod::MOD1 },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "EOR", 0x5D, 3, 4, i_t::CycleMod::MOD1 },
        { "LSR", 0x5E, 3, 7, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
    },
    { // 0x6*
        { "RTS", 0x60, 1, 6, i_t::CycleMod::NONE },
        { "ADC", 0x61, 2, 6, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "ADC", 0x65, 2, 3, i_t::CycleMod::NONE },
        { "ROR", 0x66, 2, 5, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "PLA", 0x68, 1, 4, i_t::CycleMod::NONE },
        { "ADC", 0x69, 2, 2, i_t::CycleMod::NONE },
        { "ROR", 0x6A, 1, 2, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "JMP", 0x6C, 3, 5, i_t::CycleMod::NONE },
        { "ADC", 0x6D, 3, 4, i_t::CycleMod::NONE },
        { "ROR", 0x6E, 3, 6, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE }
    },
    { // 0x7*
        { "BVS", 0x70, 2, 2, i_t::CycleMod::MOD2 },
        { "ADC", 0x71, 2, 5, i_t::CycleMod::MOD1 },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 0, i_t::CycleMod::NONE },
        { "ADC", 0x75, 2, 4, i_t::CycleMod::NONE },
        { "ROR", 0x76, 2, 6, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "SEI", 0x78, 1, 2, i_t::CycleMod::NONE },
        { "ADC", 0x79, 3, 4, i_t::CycleMod::MOD1 },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "ADC", 0x7D, 3, 4, i_t::CycleMod::MOD1 },
        { "ROR", 0x7E, 3, 7, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE }
    },
    { // 0x8*
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "STA", 0x81, 2, 6, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "STY", 0x84, 2, 3, i_t::CycleMod::NONE },
        { "STA", 0x85, 2, 3, i_t::CycleMod::NONE },
        { "STX", 0x86, 2, 3, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "DEY", 0x88, 1, 2, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "TXA", 0x8A, 1, 2, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "STY", 0x8C, 3, 4, i_t::CycleMod::NONE },
        { "STA", 0x8D, 3, 4, i_t::CycleMod::NONE },
        { "STX", 0x8E, 3, 4, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE }
    },
    { // 0x9*
        { "BCC", 0x90, 2, 2, i_t::CycleMod::MOD2 },
        { "STA", 0x91, 2, 6, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "STY", 0x94, 2, 4, i_t::CycleMod::NONE },
        { "STA", 0x95, 2, 4, i_t::CycleMod::NONE },
        { "STX", 0x96, 2, 4, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "TYA", 0x98, 1, 2, i_t::CycleMod::NONE },
        { "STA", 0x99, 3, 5, i_t::CycleMod::NONE },
        { "TXS", 0x9A, 1, 2, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "STA", 0x9D, 3, 5, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE }
    },
    { // 0xA*
        { "LDY", 0xA0, 2, 2, i_t::CycleMod::NONE },
        { "LDA", 0xA1, 2, 6, i_t::CycleMod::NONE },
        { "LDX", 0xA2, 2, 2, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "LDY", 0xA4, 2, 3, i_t::CycleMod::NONE },
        { "LDA", 0xA5, 2, 3, i_t::CycleMod::NONE },
        { "LDX", 0xA6, 2, 3, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "TAY", 0xA8, 1, 2, i_t::CycleMod::NONE },
        { "LDA", 0xA9, 2, 2, i_t::CycleMod::NONE },
        { "TAX", 0xAA, 1, 2, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "LDY", 0xAC, 3, 4, i_t::CycleMod::NONE },
        { "LDA", 0xAD, 3, 4, i_t::CycleMod::NONE },
        { "LDX", 0xAE, 3, 4, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE }
    },
    { // 0xB*
        { "BCS", 0xB0, 2, 2, i_t::CycleMod::MOD2 },
        { "LDA", 0xB1, 2, 5, i_t::CycleMod::MOD1 },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "LDY", 0xB4, 2, 4, i_t::CycleMod::NONE },
        { "LDA", 0xB5, 2, 4, i_t::CycleMod::NONE },
        { "LDX", 0xB6, 2, 4, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "CLV", 0xB8, 1, 2, i_t::CycleMod::NONE },
        { "LDA", 0xB9, 3, 4, i_t::CycleMod::MOD1 },
        { "TSX", 0xBA, 1, 2, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "LDY", 0xBC, 3, 4, i_t::CycleMod::MOD1 },
        { "LDA", 0xBD, 3, 4, i_t::CycleMod::MOD1 },
        { "LDX", 0xBE, 3, 4, i_t::CycleMod::MOD1 },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE }
    },
    { // 0xC*
        { "CPY", 0xC0, 2, 2, i_t::CycleMod::NONE },
        { "CMP", 0xC1, 2, 6, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "CPY", 0xC4, 2, 3, i_t::CycleMod::NONE },
        { "CMP", 0xC5, 2, 3, i_t::CycleMod::NONE },
        { "DEC", 0xC6, 2, 5, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INY", 0xC8, 1, 2, i_t::CycleMod::NONE },
        { "CMP", 0xC9, 2, 2, i_t::CycleMod::NONE },
        { "INY", 0xCA, 1, 2, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "CPY", 0xCC, 3, 4, i_t::CycleMod::NONE },
        { "CMP", 0xCD, 3, 4, i_t::CycleMod::NONE },
        { "DEC", 0xCD, 3, 6, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE }
    },
    { // 0xD*
        { "BNE", 0xD0, 2, 2, i_t::CycleMod::MOD2 },
        { "CMP", 0xD1, 2, 5, i_t::CycleMod::MOD1 },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "CMP", 0xD5, 2, 4, i_t::CycleMod::NONE },
        { "DEC", 0xD6, 2, 6, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "CLD", 0xD8, 1, 2, i_t::CycleMod::NONE },
        { "CMP", 0xD9, 3, 4, i_t::CycleMod::MOD1 },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "CMP", 0xDD, 3, 4, i_t::CycleMod::MOD1 },
        { "DEC", 0xDE, 3, 7, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE }
    },
    { // 0xE*
        { "CPX", 0xE0, 2, 2, i_t::CycleMod::NONE },
        { "SBC", 0xE1, 2, 6, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "CPX", 0xE4, 2, 3, i_t::CycleMod::NONE },
        { "SBC", 0xE5, 2, 3, i_t::CycleMod::NONE },
        { "INC", 0xE6, 2, 5, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INX", 0xE8, 1, 2, i_t::CycleMod::NONE },
        { "SBC", 0xE9, 2, 2, i_t::CycleMod::NONE },
        { "NOP", 0xEA, 1, 2, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "CPX", 0xE4, 3, 4, i_t::CycleMod::NONE },
        { "SBC", 0xE5, 3, 4, i_t::CycleMod::NONE },
        { "INC", 0xE6, 3, 6, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE }
    },
    { // 0xF*
        { "BEQ", 0xF0, 2, 2, i_t::CycleMod::MOD2 },
        { "SBC", 0xF1, 2, 5, i_t::CycleMod::MOD1 },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "SBC", 0xF5, 2, 4, i_t::CycleMod::NONE },
        { "INC", 0xF6, 2, 6, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "SED", 0xF8, 1, 2, i_t::CycleMod::NONE },
        { "SBC", 0xF9, 3, 4, i_t::CycleMod::MOD1 },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE },
        { "SBC", 0xFD, 3, 4, i_t::CycleMod::MOD1 },
        { "INC", 0xFE, 3, 7, i_t::CycleMod::NONE },
        { "INVAL", 0, 0, 1, i_t::CycleMod::NONE }
    }
};


MOS6502::MOS6502(AddressSpace *addrSpace) : addrSpace(*addrSpace) {
    this->IR = 0;
    this->A = 0;
    this->X = 0;
    this->Y = 0;
    this->SP = 0;
    this->PC.W = 0xFFFC;    // Reset vector
    this->P = 32;           // All flags 0 except for unused flag
    
    this->counter = 0;
    this->cycles = 0;
}

void MOS6502::reset() {
    uint16_t start = 0;
    start |= this->addrSpace.r8(this->PC.W++);
    start |= this->addrSpace.r8(this->PC.W) << 8;
    printf("%X\n", start);
    this->PC.W = start;
    
    this->P |= (uint8_t)Flags::IRQ;
}

void MOS6502::step() {
    do {
        this->cycle();
    } while (this->counter);
}

void MOS6502::cycle() {
    static uint8_t tmp[3] = { 0 };  // temporary data used between cycles
    
    if (this->counter == 0) {
        this->IR = this->addrSpace.r8(PC.W++);
        printf("Current opcode: %X\n", this->IR);
        Instruction *i = &(instructions[this->IR >> 4][this->IR & 0x0F]);
        this->counter = i->cycles;
        std::cout << i->mnemonic << std::endl;
    }
    else {
        switch (this->IR) {
        case 0x00:  // BRK
            switch (this->counter) {
            case 6:
                // dummy read
                this->PC.W++;
                break;
            case 5:
                this->setFlag(true, Flags::BREAK);
                this->setFlag(true, Flags::IRQ);
                this->addrSpace.w8(0x100 | this->SP--, this->PC.B.h);
                break;
            case 4:
                this->addrSpace.w8(0x100 | this->SP--, this->PC.B.l);
                break;
            case 3:
                this->addrSpace.w8(0x100 | this->SP--, this->P);
                break;
            case 2:
                this->PC.B.l = this->addrSpace.r8((uint16_t)Vectors::BRK);      // target addr low
                break;
            case 1:
                this->PC.B.h = this->addrSpace.r8((uint16_t)Vectors::BRK + 1);  // target addr high
            }
            break;
        case 0x01:  // ORA, indexed, indirect, X
            switch (this->counter) {
            case 5:
                tmp[0] = this->addrSpace.r8(PC.W++) + this->X;
                break;
            case 4:
                tmp[1] = this->addrSpace.r8(tmp[0]);        // effective address low
                break;
            case 3:
                tmp[2] = this->addrSpace.r8(tmp[0] + 1);    // effective address high
                break;
            case 2:
                this->A |= this->addrSpace.r8((tmp[2] << 8) + tmp[1]);
                break;
            case 1:
                this->setFlag(!this->A, Flags::ZERO);
                this->setFlag(this->A & 0x80, Flags::NEGATIVE);
                break;
            }
            break;
        case 0x05:  // ORA, zero page
            switch (this->counter) {
            case 2:
                tmp[0] = this->addrSpace.r8(this->PC.W++);
                break;
            case 1:
                this->A |= this->addrSpace.r8(tmp[0]);
                this->setFlag(!this->A, Flags::ZERO);
                this->setFlag(this->A & 0x80, Flags::NEGATIVE);
                break;
            }
            break;
        case 0x06:  // ASL, zero page
            switch (this->counter) {
            case 4:
                tmp[0] = this->addrSpace.r8(this->PC.W++);  // zero page address
                break;
            case 3:
                tmp[1] = this->addrSpace.r8(tmp[0]);    // original value
                tmp[2] = tmp[1] << 1;                   // updated value
                break;
            case 2:
                this->addrSpace.w8(tmp[0], tmp[1]);
                break;
            case 1:
                this->addrSpace.w8(tmp[0], tmp[2]);
                this->setFlag(!tmp[2], Flags::ZERO);
                this->setFlag(tmp[2] & 0x80, Flags::NEGATIVE);
                this->setFlag(tmp[1] & 0x80, Flags::CARRY);
                break;
            }
            break;
        case 0x08:  // PHP
            switch (this->counter) {
            case 2:
                // dummy cycle
                break;
            case 1:
                this->addrSpace.w8(0x100 | this->SP, this->P); 
                this->SP--;
                break;
            }
            break;
        case 0x09:  // ORA, immediate
            switch (this->counter) {
            case 1:
                this->A |= this->addrSpace.r8(this->PC.W++);
                
                this->setFlag(!this->A, Flags::ZERO);
                this->setFlag(this->A & 0x80, Flags::NEGATIVE);
                break;
            }
            break;
        case 0x0A:  // ASL, accum
            switch (this->counter) {
            case 1:
                this->setFlag(this->A & 0x80, Flags::CARRY);
                
                this->A <<= 1;
                
                this->setFlag(!this->A, Flags::ZERO);
                this->setFlag(this->A & 0x80, Flags::NEGATIVE);
                break;
            }
            break;
        case 0x0D:  // ORA, absolute
            switch (this->counter) {
            case 3:
                tmp[0] = this->addrSpace.r8(this->PC.W++);  // target address low
                break;
            case 2:
                tmp[1] = this->addrSpace.r8(this->PC.W++);  // target address high
                break;
            case 1:
                this->A |= this->addrSpace.r8((tmp[1] << 8) + tmp[0]);
            
                this->setFlag(!this->A, Flags::ZERO);
                this->setFlag(this->A & 0x80, Flags::NEGATIVE);
                break;
            }
            break;
        case 0x0E:  // ASL, absolute
            switch (this->counter) {
            case 5:
                tmp[0] = this->addrSpace.r8(this->PC.W++);     // target address low
                break;
            case 4:
                tmp[1] = this->addrSpace.r8(this->PC.W++);     // target address high
                break;
            case 3:
                tmp[2] = this->addrSpace.r8((tmp[1] << 8) + tmp[0]);
                break;
            case 2:
                //this->addrSpace.w8(tmp[2], (tmp[1] << 8) + tmp[0]); // dummy write, might not need?
            
                this->setFlag(tmp[2] & 0x80, Flags::CARRY);
                
                tmp[2] <<= 1;
                
                this->setFlag(tmp[2] & 0x80, Flags::NEGATIVE);
                this->setFlag(!tmp[2], Flags::ZERO);
                break;
            case 1:
                this->addrSpace.w8((tmp[1] << 8) + tmp[0], tmp[2]);
                break;
            }
            break;
        
        case 0x10:  // BPL, relative
            switch (this->counter) {
            case 1:
                switch (tmp[1]) {
                case 0:
                    tmp[0] = this->addrSpace.r8(this->PC.W);                // branch offset
                    //uint16_t target = this->PC.W + (int8_t)tmp[0];          // branch target
                    tmp[1] = (this->PC.W / 256 == (this->PC.W + (int8_t)tmp[0]) / 256) ? 1 : 2;    // extra cycles
                    //this->counter += tmp[1];
                    this->counter++;
                    this->PC.W++;
                    break;
                case 2:
                    tmp[1]--;
                    this->counter++;
                    break;
                case 1:
                    //this->PC.W = this->PC.W
                    if (this->P & (uint8_t)Flags::NEGATIVE) {
                        this->PC.W++;
                    }
                    else {
                        this->PC.W += (int8_t)tmp[0];
                    }
                    break;
                }
                break;
            }
            break;
        case 0x11:  // ORA, indirect, indexed, Y
            switch (this->counter) {
            case 4:
                tmp[0] = this->addrSpace.r8(this->PC.W++);
                break;
            case 3:
                tmp[1] = this->addrSpace.r8(tmp[0]) + this->Y;  // effective address low
                break;
            case 2:
                tmp[2] = ((uint16_t)this->addrSpace.r8(tmp[0]) + this->Y) >> 8; // carry
                if (tmp[2]) {
                    this->counter = 5;
                }
                tmp[2] += this->addrSpace.r8(tmp[0] + 1);       // effective address high
                break;
            case 1:
                this->A |= this->addrSpace.r8((tmp[2] << 8) + tmp[1]);
            
                this->setFlag(!this->A, Flags::ZERO);
                this->setFlag(this->A & 0x80, Flags::NEGATIVE);
                break;
            case 5:
                this->counter = 1;
                break;
            }
            break;
        case 0x15:  // ORA, zero page, X
            switch (this->counter) {
            case 3:
                tmp[0] = this->addrSpace.r8(this->PC.W++);
                break;
            case 2:
                tmp[0] += this->X;
                break;
            case 1:
                this->A |= this->addrSpace.r8(tmp[0]);
                
                this->setFlag(!this->A, Flags::ZERO);
                this->setFlag(this->A & 0x80, Flags::NEGATIVE);
                break;
            }
            break;
        case 0x16:  // ASL, zero page, X
            switch (this->counter) {
            case 5:
                tmp[0] = this->addrSpace.r8(this->PC.W++);
                break;
            case 4:
                tmp[0] += this->X;
                break;
            case 3:
                tmp[1] = this->addrSpace.r8(tmp[0]);
                break;
            case 2:
                this->setFlag(tmp[1] & 0x80, Flags::CARRY);
                tmp[1] <<= 1;
                break;
            case 1:
                this->addrSpace.w8(tmp[0], tmp[1]);
                
                this->setFlag(!tmp[1], Flags::ZERO);
                this->setFlag(tmp[1] & 0x80, Flags::NEGATIVE);
                break;
            }
            break;
        case 0x18:  // CLC
            switch (this->counter) {
            case 1:
                this->setFlag(false, Flags::CARRY);
            }
            break;
        case 0x19:
            switch (this->counter) {
                
            }
            break;
            
        case 0x4C:  // JMP, absolute
            switch (this->counter) {
            case 2:
                tmp[0] = this->addrSpace.r8(this->PC.W++);
                break;
            case 1:
                this->PC.B.h = this->addrSpace.r8(this->PC.W);
                this->PC.B.l = tmp[0];
                break;
            }
            break;
            
        case 0x78:  // SEI
            switch (this->counter) {
            case 1:
                this->setFlag(true, Flags::IRQ);
            }
            break;
            
        case 0x8A:  // TXA
            switch (this->counter) {
            case 1:
                this->A = this->X;
                this->setFlag(!this->A, Flags::ZERO);
                this->setFlag(this->A & 0x80, Flags::NEGATIVE);
                break;
            }
            break;
            
        case 0x8E:  // STX, absolute
            switch (this->counter) {
            case 3:
                tmp[0] = this->addrSpace.r8(this->PC.W++);
                break;
            case 2:
                tmp[1] = this->addrSpace.r8(this->PC.W++);
                break;
            case 1:
                //std::cout << std::hex << (tmp[1] << 8) + tmp[0] << std::dec << std::endl;
                this->addrSpace.w8((tmp[1] << 8) + tmp[0], this->X);
                break;
            }
            break;
            
        case 0x8D:  // STA, absolute
            switch (this->counter) {
            case 3:
                tmp[0] = this->addrSpace.r8(this->PC.W++);  // target address low
                break;
            case 2:
                tmp[1] = this->addrSpace.r8(this->PC.W++);  // target address high
                break;
            case 1:
                this->addrSpace.w8((tmp[1] << 8) + tmp[0], this->A);
                break;
            }
            break;
            
        case 0x95:  // STA, zero page, indexed X
            switch (this->counter) {
            case 3:
                tmp[0] = this->addrSpace.r8(this->PC.W++);
                break;
            case 2:
                //tmp[1] = this->addrSpace.r8((uint8_t)(tmp[0] + this->X));
                // dummy read
                break;
            case 1:
                this->addrSpace.w8((uint8_t)(tmp[0] + this->X), this->A);
                break;
            }
            break;
            
        case 0xA0:  // LDY, immediate
            switch (this->counter) {
            case 1:
                this->Y = this->addrSpace.r8(this->PC.W++);
                this->setFlag(!this->Y, Flags::ZERO);
                this->setFlag(this->Y & 0x80, Flags::NEGATIVE);
            }
            break;
        
        case 0xA2:  // LDX, immediate
            switch (this->counter) {
            case 1:
                this->X = this->addrSpace.r8(this->PC.W++);
                this->setFlag(!this->X, Flags::ZERO);
                break;
            }
            break;
            
        case 0xA9:  // LDA, immediate
            switch (this->counter) {
            case 1:
                this->A = this->addrSpace.r8(this->PC.W++);
                this->setFlag(!this->A, Flags::ZERO);
                this->setFlag(this->A & 0x80, Flags::NEGATIVE);
            }
            break;
            
        case 0xD0:  // BNE, relative
            switch (this->counter) {
            case 1:
                tmp[0] = this->addrSpace.r8(this->PC.W++);
                std::cout << (int)(int8_t)tmp[0] << std::endl;
                this->counter = 3;  // decrements to 2
                break;
            case 2:
                if (!this->checkFlag(Flags::ZERO)) {
                    tmp[1] = this->PC.B.l;
                    tmp[2] = this->PC.B.h;
                    this->PC.B.l += (int8_t)tmp[0];
                    if (tmp[2] != this->PC.B.h) {
                        this->counter = 4;  // decrements to 3
                        break;
                    }
                }
                this->counter = 1;
                break;
            case 3:
                this->PC.B.l = tmp[1];
                this->PC.W += (int8_t)tmp[0];
                this->counter = 1;
                break;
            }
            break;
            
        case 0xD8:  // CLD
            switch (this->counter) {
            case 1:
                this->setFlag(false, Flags::DECIMAL);
                break;
            }
            break;
            
        case 0xE8:  // INX
            switch (this->counter) {
            case 1:
                this->X++;
                this->setFlag(!this->X, Flags::ZERO);
                this->setFlag(this->X & 0x80, Flags::NEGATIVE);
                break;
            }
            break;
        /*default:    // Unsupported opcode
            std::cerr << "Instruction not implemented yet" << std::endl;
            this->counter = 0;*/
        }
    }
    
    this->counter--;
    this->cycles++;
}

inline bool MOS6502::checkFlag(Flags f) const {
    return this->P & (uint8_t)f;
}

inline bool MOS6502::setFlag(bool condition, Flags f) {
    if (condition) {
        this->P |= (uint8_t)f;
    }
    else {
        this->P &= ~(uint8_t)f;
    }
    return this->P & (uint8_t)f;
}

std::ostream& operator <<(std::ostream& os, const MOS6502& cpu) {
    os << "A:\t" << std::hex << (int)cpu.A << std::endl
       << "X:\t" << std::hex << (int)cpu.X << std::endl
       << "Y:\t" << std::hex << (int)cpu.Y << std::endl
       << "SP:\t" << std::hex << (int)cpu.SP << std::endl
       << "PC:\t" << std::hex << (int)cpu.PC.W << std::endl
       << "Flags" << std::endl
       /*<< "N: " << cpu.checkFlag(MOS6502::Flags::NEGATIVE) << " "
       << "V: " << cpu.checkFlag(MOS6502::Flags::OVERFLOW) << " "
       << "B: " << cpu.checkFlag(MOS6502::Flags::BREAK) << " "
       << "D: " << cpu.checkFlag(MOS6502::Flags::DECIMAL) << " "
       << "I: " << cpu.checkFlag(MOS6502::Flags::IRQ) << " "
       << "Z: " << cpu.checkFlag(MOS6502::Flags::ZERO) << " "
       << "C: " << cpu.checkFlag(MOS6502::Flags::CARRY) << " "*/
       << "NV BDIZC" << std::endl
       << cpu.checkFlag(MOS6502::Flags::NEGATIVE) << cpu.checkFlag(MOS6502::Flags::OVERFLOW) << " "
       << cpu.checkFlag(MOS6502::Flags::BREAK) << cpu.checkFlag(MOS6502::Flags::DECIMAL) << cpu.checkFlag(MOS6502::Flags::IRQ)
       << cpu.checkFlag(MOS6502::Flags::ZERO) << cpu.checkFlag(MOS6502::Flags::CARRY)
       << std::endl
       << "Cycles:\t" << std::dec << cpu.cycles << std::endl;
       
    return os;
}
