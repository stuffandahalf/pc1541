#include "MOS6502.h"
#include <cstdio>
#include <cstdarg>

typedef unsigned int uint;
/*typedef Instruction i_t;
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
};*/


MOS6502::MOS6502(AddressSpace *addrSpace) : addrSpace(*addrSpace) {
    this->IR = 0;
    this->A = 0;
    this->X = 0;
    this->Y = 0;
    this->SP = 0;
    this->PC = 0xFFFC;    // Reset vector
    this->P = 32;           // All flags 0 except for unused flag

    this->counter = 0;
    this->cycles = 0;

    operation_t ops[3][8] = {
        { nullptr, &MOS6502::BIT, &MOS6502::JMP_INDIRECT, &MOS6502::JMP_ABSOLUTE, &MOS6502::STY, &MOS6502::LDY, &MOS6502::CPY, &MOS6502::CPX },
        { &MOS6502::ORA, &MOS6502::AND, &MOS6502::EOR, &MOS6502::ADC, &MOS6502::STA, &MOS6502::LDA, &MOS6502::CMP, &MOS6502::SBC },
        { &MOS6502::ASL, &MOS6502::ROL, &MOS6502::LSR, &MOS6502::ROR, &MOS6502::STX, &MOS6502::LDX, &MOS6502::DEC, &MOS6502::INC }
    };

    operation_t flagOps[8] = {
        &MOS6502::CLC, &MOS6502::SEC, &MOS6502::CLI, &MOS6502::SEI, &MOS6502::TYA, &MOS6502::CLV, &MOS6502::CLD, &MOS6502::SED
    };

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 8; j++) {
            this->operations[i][j] = ops[i][j];
        }
    }

    for (int i = 0; i < 8; i++) {
        this->flagOps[i] = flagOps[i];
    }
}

void MOS6502::reset() {
    uint16_t start = 0;
    start |= this->addrSpace.r8(this->PC++);
    start |= this->addrSpace.r8(this->PC) << 8;
    printf("%X\n", start);
    this->PC = start;

    this->P |= (uint8_t)Flags::IRQ;

    this->IR = this->addrSpace.r8(this->PC++);
}

void MOS6502::step() {
    do {
        this->cycle();
    } while (this->counter);
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
       << "PC:\t" << std::hex << (int)cpu.PC << std::endl
       << "IR:\t" << std::hex << (int)cpu.IR << std::endl
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

int MOS6502::cycle() {
    static uint8_t tmp[3] = { 0 };  // temporary data used between cycles
    //static Instruction *currentInstruction = &(instructions[this->IR >> 4][this->IR & 0x0F]);

    this->counter++;

    uint8_t instructionGroup = 0;
    uint8_t addressMode = 0;
    uint8_t instruction = 0;

    instructionGroup = this->IR & 0x2;
    addressMode = (this->IR & 0x1C) >> 2;
    instruction = (this->IR & 0xE0) >> 5;

    switch (this->IR) {
    case 0x00:  // BRK
        break;
    case 0x20:  // JSR absolute
        break;
    case 0x40:  // RTI
        break;
    case 0x60:  // RTS
        break;

    case 0x08:  // PHP
        break;
    case 0x28:  // PLP
        break;
    case 0x48:  // PHA
        break;
    case 0x68:  // PLA
        break;
    case 0x88:  // DEY
        break;
    case 0xA8:  // TAY
        break;
    case 0xC8:  // INY
        break;
    case 0xE8:  // INX
        break;

    case 0x18:  // CLC
    case 0x38:  // SEC
    case 0x58:  // CLI
    case 0x78:  // SEI
    case 0x98:  // TYA
    case 0xB8:  // CLV
    case 0xD8:  // CLD
    case 0xF8:  // SED
        switch (this->counter) {
        case 1:
            if ((this->*(flagOps[((this->IR >> 4) - 1) / 2]))(AddressMode::IMPLIED) < 0) {
                return -1;
            }
            break;
        case 2:
            this->IR = this->addrSpace.r8(this->PC++);
            this->counter = 0;
            break;
        }
        break;

    case 0x8A:  // TXA
        break;
    case 0x9A:  // TXS
        break;
    case 0xAA:  // TAX
        break;
    case 0xBA:  // TSX
        break;
    case 0xCA:  // DEX
        break;
    case 0xEA:  // NOP
        break;
    default:
        switch (this->IR & 0x1F) {
        case 0x1000:
            switch (this->IR >> 5) {
            case 0b000:
                break;
            case 0b001:
                break;
            case 0b010:
                break;
            case 0b011:
                break;
            case 0b100:
                break;
            case 0b101:
                break;
            case 0b110:
                break;
            case 0b111:
                break;
            }
            break;
        default:
            switch (instructionGroup) {   // instruction group
            case 0b00:
                switch (addressMode) {
                case 0b000: // immediate
                    switch (this->counter) {
                    case 1:
                        if ((this->*(operations[instructionGroup][instruction]))(AddressMode::IMMEDIATE, this->PC++) < 0) {
                            return -1;
                        }
                        break;
                    case 2:
                        this->IR = this->addrSpace.r8(this->PC++);
                        this->counter = 0;
                        break;
                    }
                    break;
                case 0b001: // zero page
                    switch (this->counter) {
                    case 1:
                        tmp[0] = this->addrSpace.r8(this->PC++);
                        break;
                    case 2:
                        if ((this->*(operations[instructionGroup][instruction]))(AddressMode::ZERO_PAGE, tmp[0])) {
                            return -1;
                        }
                        break;
                    case 3:
                        this->IR = this->addrSpace.r8(this->PC++);
                        this->counter = 0;
                        break;
                    }
                    break;
                case 0b011: // absolute
                    switch (this->counter) {
                    case 1:
                        tmp[0] = this->addrSpace.r8(this->PC++);   // address low
                        break;
                    case 2:
                        tmp[1] = this->addrSpace.r8(this->PC++);   // address high
                        break;
                    case 3:
                        if ((this->*(operations[instructionGroup][instruction]))(AddressMode::ABSOLUTE, (tmp[1] << 8) + tmp[0]) < 0) {
                            return -1;
                        }
                        break;
                    case 4:
                        this->IR = this->addrSpace.r8(this->PC++);
                        this->counter = 0;
                        break;
                    }
                    break;
                case 0b101: // zero page, x
                    break;
                case 0b111: // absolute, x
                    break;
                }
                break;
            case 0b01:
                switch (addressMode) {   // address mode
                case 0b000: // (zero page/indirect, X)
                    switch (this->counter) {
                    case 1:
                        tmp[0] = this->addrSpace.r8(this->PC++);
                        break;
                    case 2:
                        tmp[0] += this->X;
                        break;
                    case 3:
                        tmp[1] = this->addrSpace.r8(tmp[0]);                    // address low
                        break;
                    case 4:
                        tmp[2] = this->addrSpace.r8((tmp[0] + 1) & 0xFF);       // address high
                        break;
                    case 5:
                        // Forgive me father for I have sinned
                        if ((this->*(this->operations[instructionGroup][instruction]))(AddressMode::INDEXED_INDIRECT, (tmp[2] << 8) + tmp[1]) < 0) {
                            return -1;
                        }
                        break;
                    case 6:
                        this->IR = this->addrSpace.r8(this->PC++);
                        this->counter = 0;
                        break;
                    }
                    break;
                case 0b001: // zero page
                    switch (this->counter) {
                    case 1:
                        tmp[0] = this->addrSpace.r8(this->PC++);
                        break;
                    case 2:
                        if ((this->*(this->operations[1][instruction]))(AddressMode::ZERO_PAGE, tmp[0]) < 0) {
                            return -1;
                        }
                        break;
                    case 3:
                        this->IR = this->addrSpace.r8(this->PC++);
                        this->counter = 0;
                        break;
                    }
                    break;
                case 0b010: // immediate
                    switch (this->counter) {
                    case 1:
                        tmp[0] = this->addrSpace.r8(this->PC++);
                        if ((this->*(this->operations[instructionGroup][instruction]))(AddressMode::IMMEDIATE, tmp[0]) < 0) {
                            return -1;
                        }
                        break;
                    case 2:
                        this->IR = this->addrSpace.r8(this->PC++);
                        this->counter = 0;
                    }
                    break;
                case 0b011: // absolute
                    switch (this->counter) {
                    case 1:
                        tmp[0] = this->addrSpace.r8(this->PC++);  // address low
                        break;
                    case 2:
                        tmp[1] = this->addrSpace.r8(this->PC++);  // address high
                        break;
                    case 3:
                        if ((this->*(this->operations[instructionGroup][instruction]))(AddressMode::IMMEDIATE, (tmp[1] << 8) + tmp[0]) < 0) {
                            return -1;
                        }
                        break;
                    case 4:
                        this->IR = this->addrSpace.r8(this->PC++);
                        this->counter = 0;
                        break;
                    }
                    break;
                case 0b100: // (zero page), Y
                    switch (this->counter) {
                    case 1:
                        tmp[0] = this->addrSpace.r8(this->PC++);  // zero page ptr
                        break;
                    case 2:
                        tmp[1] = this->addrSpace.r8(tmp[0]);        // target address low
                        break;
                    case 3:
                        tmp[2] = this->addrSpace.r8(tmp[0] + 1);    // target address high
                        break;
                    case 4:
                        if ((((tmp[2] << 8) + tmp[1] + this->Y) & 0xFF) != (tmp[1] + this->Y)) {
                            break;
                        }
                        this->counter++;
                    case 5:
                        if ((this->*(this->operations[instructionGroup][instruction]))(AddressMode::INDIRECT_INDEXED, (uint16_t)((tmp[2] << 8) + tmp[1] + this->Y)) < 0) {
                            return -1;
                        }
                        break;
                    case 6:
                        this->IR = this->addrSpace.r8(this->PC++);
                        this->counter = 0;
                        break;
                    }
                    break;
                case 0b101: // zero page, X
                    switch (this->counter) {
                    case 1:
                        tmp[0] = this->addrSpace.r8(this->PC++);  // zero page address
                        break;
                    case 2:
                        tmp[0] += this->X;
                    case 3:
                        if ((this->*(this->operations[instructionGroup][instruction]))(AddressMode::ZERO_PAGE_X, tmp[0]) < 0) {
                            return -1;
                        }
                    case 4:
                        this->IR = this->addrSpace.r8(this->PC++);
                        this->counter = 0;
                        break;
                    }
                    break;
                case 0b110: // absolute, Y
                case 0b111: // absolute, X
                    tmp[2] = (addressMode & 1) ? this->X : this->Y; // set index register
                    switch (this->counter) {
                    case 1:
                        tmp[0] = this->addrSpace.r8(this->PC++);  // address low
                        break;
                    case 2:
                        tmp[1] = this->addrSpace.r8(this->PC++);  // address high
                    case 3:
                        if (((uint16_t)tmp[0] + tmp[2]) & 0xFF00) {
                            break;
                        }
                        this->counter++;
                    case 4:
                        if ((this->*(this->operations[instructionGroup][instruction]))((instruction & 1) ? AddressMode::ABSOLUTE_X : AddressMode::ABSOLUTE_Y, (tmp[1] << 8) + tmp[0] + tmp[2])) {
                            return -1;
                        }
                    case 5:
                        this->IR = this->addrSpace.r8(this->PC++);
                        this->counter = 0;
                        break;
                    }
                    break;
                //case 0b111: // absolute, X
                    //break;
                }
                break;
            case 0b10:
                switch (addressMode) {  // address mode
                case 0b000: // immediate
                    switch (this->counter) {
                    case 1:
                        if ((this->*(this->operations[instructionGroup][instruction]))(AddressMode::IMMEDIATE, this->PC++) < 0) {   // need to validate that PC is incremented after the instruction
                            return -1;
                        }
                        break;
                    case 2:
                        this->IR = this->addrSpace.r8(this->PC++);
                        this->counter = 0;
                        break;
                    }
                    break;
                case 0b001: // zero page
                    switch (instruction >> 1) { // 3 or 5 cycle instruction
                    case 0b10:
                        switch (this->counter) {
                        case 1:
                            tmp[0] = this->addrSpace.r8(this->PC++);
                            break;
                        case 2:
                            if ((this->*(this->operations[instructionGroup][instruction]))(AddressMode::ZERO_PAGE, tmp[0]) < 0) {
                                return -1;
                            }
                            break;
                        case 3:
                            this->IR = this->addrSpace.r8(this->PC++);
                            this->counter = 0;
                            break;
                        }
                        break;
                    default:
                        switch (this->counter) {
                        case 1:
                            tmp[0] = this->addrSpace.r8(this->PC++);
                            break;
                        case 2:
                            tmp[1] = this->addrSpace.r8(tmp[0]);
                            break;
                        case 3:
                            if ((this->*(this->operations[instructionGroup][instruction]))(AddressMode::ZERO_PAGE, &tmp[1]) < 0) {
                                return -1;
                            }
                            break;
                        case 4:
                            this->addrSpace.w8(tmp[0], tmp[1]);
                            break;
                        case 5:
                            this->IR = this->addrSpace.r8(this->PC++);
                            this->counter = 0;
                            break;
                        }
                        break;
                    }
                    break;
                case 0b010: // accumulator
                    switch (this->counter) {
                    case 1:
                        if ((this->*(operations[instructionGroup][instruction]))(AddressMode::ACCUMULATOR, &this->A) < 0) {
                            return -1;
                        }
                        break;
                    case 2:
                        this->IR = this->addrSpace.r8(this->PC++);
                        this->counter = 0;
                        break;
                    }
                    break;
                case 0b011: // absolute
                    switch (instruction >> 1) {
                    case 0b10:  // STX and LDX
                        switch (this->counter) {
                        case 1:
                            tmp[0] = this->addrSpace.r8(this->PC++);    // target address low
                            break;
                        case 2:
                            tmp[1] = this->addrSpace.r8(this->PC++);    // target address high
                            break;
                        case 3:
                            if ((this->*(operations[instructionGroup][instruction]))(AddressMode::ABSOLUTE, (tmp[1] << 8) + tmp[0]) < 0) {
                                return -1;
                            }
                            break;
                        case 4:
                            this->IR = this->addrSpace.r8(this->PC++);
                            this->counter = 0;
                            break;
                        }
                        break;
                    default:
                        switch (this->counter) {
                        case 1:
                            tmp[0] = this->addrSpace.r8(this->PC++);    // target address low
                            break;
                        case 2:
                            tmp[1] = this->addrSpace.r8(this->PC++);    // target address high
                            break;
                        case 3:
                            tmp[2] = this->addrSpace.r8((tmp[1] << 8) + tmp[0]);
                            break;
                        case 4:
                            if ((this->*(operations[instructionGroup][instruction]))(AddressMode::ABSOLUTE, &tmp[2]) < 0) {
                                return -1;
                            }
                            break;
                        case 5:
                            this->addrSpace.w8((tmp[1] << 8) + tmp[0], tmp[2]);
                            break;
                        case 6:
                            this->IR = this->addrSpace.r8(this->PC++);
                            this->counter = 0;
                            break;
                        }
                        break;
                    }
                    break;
                case 0b101: // zero page, x/y
                    switch (instruction >> 1) {
                    case 0b10:  // STX and LDX
                        switch (this->counter) {
                        case 1:
                            tmp[0] = this->addrSpace.r8(this->PC++);
                            break;
                        case 2:
                            tmp[0] += this->Y;
                            break;
                        case 3:
                            if ((this->*(operations[instructionGroup][instruction]))(AddressMode::ZERO_PAGE_Y, tmp[0]) < 0) {
                                return -1;
                            }
                            break;
                        case 4:
                            this->IR = this->addrSpace.r8(this->PC++);
                            this->counter = 0;
                            break;
                        }
                        break;
                    default:
                        switch (this->counter) {
                        case 1:
                            tmp[0] = this->addrSpace.r8(this->PC++);
                            break;
                        case 2:
                            tmp[0] += this->X;
                            break;
                        case 3:
                            tmp[1] = this->addrSpace.r8(tmp[0]);
                            break;
                        case 4:
                            if ((this->*(operations[instructionGroup][instruction]))(AddressMode::ZERO_PAGE_X, &tmp[1]) < 0) {
                                return -1;
                            }
                            break;
                        case 5:
                            this->addrSpace.w8(tmp[0], tmp[1]);
                            break;
                        case 6:
                            this->IR = this->addrSpace.r8(this->PC++);
                            this->counter = 0;
                            break;
                        }
                        break;
                    }
                    break;
                case 0b111: // absolute, x/y
                    switch (instruction >> 1) {
                    case 0b10:
                        switch (this->counter) {
                        case 1:
                            tmp[0] = this->addrSpace.r8(this->PC++);    // low address byte
                            break;
                        case 2:
                            tmp[1] = this->addrSpace.r8(this->PC++);    // high address byte
                            break;
                        case 3:
                            if (((uint16_t)tmp[0] + this->Y) > 0xFF) {
                                break;
                            }
                        case 4:
                            if ((this->*(operations[instructionGroup][instruction]))(AddressMode::ABSOLUTE_Y, (tmp[1] << 8) + tmp[0] + this->Y) < 0) {
                                return -1;
                            }
                            if (this->counter == 3) {
                                this->counter++;
                            }
                            break;
                        case 5:
                            this->IR = this->addrSpace.r8(this->PC++);
                            this->counter = 0;
                            break;
                        }
                        break;
                    default:
                        switch (this->counter) {
                        case 1:
                            tmp[0] = this->addrSpace.r8(this->PC++);    // target address low
                            break;
                        case 2:
                            tmp[1] = this->addrSpace.r8(this->PC++);    // target address high
                            tmp[2] = tmp[0];
                            tmp[0] += this->X;
                            break;
                        case 3:
                            tmp[1] += ((uint16_t)tmp[2] + this->X) >> 8;
                            break;
                        case 4:
                            tmp[2] = this->addrSpace.r8((tmp[1] << 8) + tmp[0]);
                            break;
                        case 5:
                            if ((this->*(operations[instructionGroup][instruction]))(AddressMode::ABSOLUTE_X, &tmp[2]) < 0) {
                                return -1;
                            }
                            break;
                        case 6:
                            this->addrSpace.w8((tmp[1] << 8) + tmp[0], tmp[2]);
                            break;
                        case 7:
                            this->IR = this->addrSpace.r8(this->PC++);
                            this->counter = 0;
                            break;
                        }
                        break;
                    }
                    break;
                }
                break;
            default:
                exit(0);
            }
        }
        break;
    }

    this->cycles++;
    return 1;
}

/* Instruction group 0 */
inline int MOS6502::BIT(MOS6502::AddressMode addressMode, ...) {
    printdf("BIT\n");
    va_list args;
    va_start(args, addressMode);

    uint8_t val;
    uint8_t result;

    switch (addressMode) {
    case AddressMode::ZERO_PAGE:
    case AddressMode::ABSOLUTE:
        val = (uint8_t)va_arg(args, uint32_t);
        break;
    }

    result = this->A & val;

    this->setFlag(!result, Flags::ZERO);
    this->setFlag(val & 0x80, Flags::NEGATIVE);
    this->setFlag(val & 0x40, Flags::OVERFLOW);

    va_end(args);
    return 1;
}
inline int MOS6502::JMP_INDIRECT(MOS6502::AddressMode addressMode, ...) {
    printdf("JMP INDIRECT\n");
    va_list args;
    va_start(args, addressMode);

    switch (addressMode) {
    case AddressMode::ABSOLUTE:
        break;
    default:
        return -1;
    }

    this->PC = (uint16_t)va_arg(args, uint32_t);

    va_end(args);
    return 1;
}
inline int MOS6502::JMP_ABSOLUTE(MOS6502::AddressMode addressMode, ...) {
    printdf("JMP ABSOLUTE\n");
    va_list args;
    va_start(args, addressMode);

    switch (addressMode) {
    case AddressMode::ABSOLUTE:
        break;
    default:
        return -1;
    }

    va_end(args);
    return 1;
}
inline int MOS6502::STY(MOS6502::AddressMode addressMode, ...) {
    printdf("STY\n");
    va_list args;
    va_start(args, addressMode);

    uint16_t addr;

    switch (addressMode) {
    case AddressMode::ZERO_PAGE:
    case AddressMode::ABSOLUTE:
    case AddressMode::ZERO_PAGE_X:
        addr = (uint16_t)va_arg(args, uint32_t);
        break;
    }

    this->addrSpace.w8(addr, this->Y);

    va_end(args);
    return 1;
}
inline int MOS6502::LDY(MOS6502::AddressMode addressMode, ...) {
    printdf("LDY\n");
    va_list args;
    va_start(args, addressMode);

    uint16_t addr;

    switch (addressMode) {
    case AddressMode::IMMEDIATE:
    case AddressMode::ZERO_PAGE:
    case AddressMode::ABSOLUTE:
    case AddressMode::ZERO_PAGE_X:
    case AddressMode::ABSOLUTE_X:
        addr = (uint16_t)va_arg(args, uint32_t);
        break;
    }

    this->Y = this->addrSpace.r8(addr);

    this->setFlag(!this->Y, Flags::ZERO);
    this->setFlag(this->Y & 0x80, Flags::NEGATIVE);

    va_end(args);
    return 1;
}
inline int MOS6502::CPY(MOS6502::AddressMode addressMode, ...) {
    printdf("CPY\n");
    va_list args;
    va_start(args, addressMode);

    uint16_t addr;
    uint8_t var;

    switch (addressMode) {
    case AddressMode::IMMEDIATE:
    case AddressMode::ZERO_PAGE:
    case AddressMode::ABSOLUTE:
        addr = (uint16_t)va_arg(args, uint32_t);
        break;
    }

    var = this->addrSpace.r8(addr);
    this->setFlag(this->Y == var, Flags::ZERO);
    this->setFlag(this->Y >= var, Flags::CARRY);
    this->setFlag((this->Y - var) & 0x80, Flags::NEGATIVE);

    va_end(args);
    return 1;
}
inline int MOS6502::CPX(MOS6502::AddressMode addressMode, ...) {
    printdf("CPX\n");
    va_list args;
    va_start(args, addressMode);

    uint16_t addr;
    uint8_t var;

    switch (addressMode) {
    case AddressMode::IMMEDIATE:
    case AddressMode::ZERO_PAGE:
    case AddressMode::ABSOLUTE:
        addr = (uint16_t)va_arg(args, uint32_t);
        break;
    }

    var = this->addrSpace.r8(addr);

    this->setFlag(this->X == var, Flags::ZERO);
    this->setFlag(this->X >= var, Flags::CARRY);
    this->setFlag((this->X - var) & 0x80, Flags::NEGATIVE);

    va_end(args);
    return 1;
}

/* Instruction group 1 */
inline int MOS6502::ORA(MOS6502::AddressMode addressMode, ...) {
    printdf("ORA\n");
    va_list args;
    va_start(args, addressMode);

    uint8_t var = 0;

    switch (addressMode) {
    case AddressMode::IMMEDIATE:
        var = (uint8_t)va_arg(args, uint);
        break;
    case AddressMode::ZERO_PAGE:
    case AddressMode::ZERO_PAGE_X:
    case AddressMode::ABSOLUTE:
    case AddressMode::ABSOLUTE_X:
    case AddressMode::ABSOLUTE_Y:
    case AddressMode::INDIRECT_INDEXED:
    case AddressMode::INDEXED_INDIRECT:
        var = this->addrSpace.r8((uint16_t)va_arg(args, uint));
        break;
    //default:
        // die
    }

    va_end(args);

    this->A |= var;
    this->setFlag(!this->A, Flags::ZERO);
    this->setFlag(this->A & 0x80, Flags::NEGATIVE);
    return 1;
}
inline int MOS6502::AND(MOS6502::AddressMode addressMode, ...) {
    printdf("AND\n");
    va_list args;
    va_start(args, addressMode);

    uint8_t var = 0;

    switch (addressMode) {
    case AddressMode::IMMEDIATE:
        var = (uint8_t)va_arg(args, uint);
        break;
    case AddressMode::ZERO_PAGE:
    case AddressMode::ZERO_PAGE_X:
    case AddressMode::ABSOLUTE:
    case AddressMode::ABSOLUTE_X:
    case AddressMode::ABSOLUTE_Y:
    case AddressMode::INDIRECT_INDEXED:
    case AddressMode::INDEXED_INDIRECT:
        var = this->addrSpace.r8((uint16_t)va_arg(args, uint));
        break;
    }

    va_end(args);

    this->A &= var;
    this->setFlag(!this->A, Flags::ZERO);
    this->setFlag(this->A & 0x80, Flags::NEGATIVE);
    return 1;
}
inline int MOS6502::EOR(MOS6502::AddressMode addressMode, ...) {
    printdf("EOR\n");
    va_list args;
    va_start(args, addressMode);

    uint8_t var = 0;

    switch (addressMode) {
    case AddressMode::IMMEDIATE:
        var = (uint8_t)va_arg(args, uint);
        break;
    case AddressMode::ZERO_PAGE:
    case AddressMode::ZERO_PAGE_X:
    case AddressMode::ABSOLUTE:
    case AddressMode::ABSOLUTE_X:
    case AddressMode::ABSOLUTE_Y:
    case AddressMode::INDIRECT_INDEXED:
    case AddressMode::INDEXED_INDIRECT:
        var = this->addrSpace.r8((uint16_t)va_arg(args, uint));
        break;
    }

    va_end(args);

    this->A ^= var;
    this->setFlag(!this->A, Flags::ZERO);
    this->setFlag(this->A & 0x80, Flags::NEGATIVE);
    return 1;
}
inline int MOS6502::ADC(MOS6502::AddressMode addressMode, ...) {
    printdf("ADC\n");
    va_list args;
    va_start(args, addressMode);

    uint8_t var = 0;

    switch (addressMode) {
    case AddressMode::IMMEDIATE:
        var = (uint8_t)va_arg(args, uint);
        break;
    case AddressMode::ZERO_PAGE:
    case AddressMode::ZERO_PAGE_X:
    case AddressMode::ABSOLUTE:
    case AddressMode::ABSOLUTE_X:
    case AddressMode::ABSOLUTE_Y:
    case AddressMode::INDIRECT_INDEXED:
    case AddressMode::INDEXED_INDIRECT:
        var = this->addrSpace.r8((uint16_t)va_arg(args, uint));
        break;
    }

    va_end(args);

    uint16_t result = (uint16_t)this->A + var;
    this->A = (uint8_t)result;
    this->setFlag(!this->A, Flags::ZERO);
    this->setFlag(this->A & 0x80, Flags::NEGATIVE);
    this->setFlag(result & 0xFF00, Flags::OVERFLOW);
    return 1;
}
inline int MOS6502::STA(MOS6502::AddressMode addressMode, ...) {
    printdf("STA\n");
    va_list args;
    va_start(args, addressMode);

    uint16_t targetAddr = 0;

    switch (addressMode) {
    case AddressMode::ZERO_PAGE:
    case AddressMode::ZERO_PAGE_X:
    case AddressMode::ABSOLUTE:
    case AddressMode::ABSOLUTE_X:
    case AddressMode::ABSOLUTE_Y:
    case AddressMode::INDIRECT_INDEXED:
    case AddressMode::INDEXED_INDIRECT:
        targetAddr = (uint16_t)va_arg(args, uint);
        break;
    }

    va_end(args);

    this->addrSpace.w8(targetAddr, this->A);
    return 1;
}
inline int MOS6502::LDA(MOS6502::AddressMode addressMode, ...) {
    printdf("LDA\n");
    va_list args;
    va_start(args, addressMode);

    uint8_t var = 0;

    switch (addressMode) {
    case AddressMode::IMMEDIATE:
        var = (uint8_t)va_arg(args, uint);
        break;
    case AddressMode::ZERO_PAGE:
    case AddressMode::ZERO_PAGE_X:
    case AddressMode::ABSOLUTE:
    case AddressMode::ABSOLUTE_X:
    case AddressMode::ABSOLUTE_Y:
    case AddressMode::INDIRECT_INDEXED:
    case AddressMode::INDEXED_INDIRECT:
        var = this->addrSpace.r8((uint16_t)va_arg(args, uint));
        break;
    }

    va_end(args);

    this->A = var;
    this->setFlag(!this->A, Flags::ZERO);
    this->setFlag(this->A & 0x80, Flags::NEGATIVE);
    return 1;
}
inline int MOS6502::CMP(MOS6502::AddressMode addressMode, ...) {
    printdf("CMP\n");
    va_list args;
    va_start(args, addressMode);

    uint8_t var = 0;

    switch (addressMode) {
    case AddressMode::IMMEDIATE:
        var = (uint8_t)va_arg(args, uint);
        break;
    case AddressMode::ZERO_PAGE:
    case AddressMode::ZERO_PAGE_X:
    case AddressMode::ABSOLUTE:
    case AddressMode::ABSOLUTE_X:
    case AddressMode::ABSOLUTE_Y:
    case AddressMode::INDIRECT_INDEXED:
    case AddressMode::INDEXED_INDIRECT:
        var = this->addrSpace.r8((uint16_t)va_arg(args, uint));
        break;
    }

    va_end(args);

    this->setFlag(this->A == var, Flags::ZERO);
    this->setFlag(this->A >= var, Flags::CARRY);
    this->setFlag((this->A - var) & 0x80, Flags::NEGATIVE);
    return 1;
}
inline int MOS6502::SBC(MOS6502::AddressMode addressMode, ...) {
    printdf("SBC\n");
    va_list args;
    va_start(args, addressMode);

    uint8_t var = 0;

    switch (addressMode) {
    case AddressMode::IMMEDIATE:
        var = (uint8_t)va_arg(args, uint);
        break;
    case AddressMode::ZERO_PAGE:
    case AddressMode::ZERO_PAGE_X:
    case AddressMode::ABSOLUTE:
    case AddressMode::ABSOLUTE_X:
    case AddressMode::ABSOLUTE_Y:
    case AddressMode::INDIRECT_INDEXED:
    case AddressMode::INDEXED_INDIRECT:
        var = this->addrSpace.r8((uint16_t)va_arg(args, uint));
        break;
    }

    va_end(args);

    int16_t correctResult = this->A - var - (1 - this->checkFlag(Flags::CARRY));
    this->A = correctResult & 0xFF;
    this->setFlag(this->A != correctResult, Flags::CARRY);
    this->setFlag(this->A != correctResult, Flags::OVERFLOW);
    this->setFlag(!this->A, Flags::ZERO);
    return 1;
}

/* instruction group 2 */
inline int MOS6502::ASL(MOS6502::AddressMode addressMode, ...) {
    printdf("ASL\n");
    va_list args;
    va_start(args, addressMode);

    uint8_t *val;

    switch (addressMode) {
    case AddressMode::ZERO_PAGE:
    case AddressMode::ACCUMULATOR:
    case AddressMode::ABSOLUTE:
    case AddressMode::ZERO_PAGE_X:
    case AddressMode::ABSOLUTE_X:
        val = va_arg(args, uint8_t *);
        break;
    }

    this->setFlag(*val & 0x80, Flags::CARRY);
    *val <<= 1;
    this->setFlag(*val & 0x80, Flags::NEGATIVE);
    this->setFlag(!(*val), Flags::ZERO);


    va_end(args);
    return 1;
}
inline int MOS6502::ROL(MOS6502::AddressMode addressMode, ...) {
    printdf("ROL\n");
    va_list args;
    va_start(args, addressMode);

    uint8_t *val;

    switch (addressMode) {
    case AddressMode::ZERO_PAGE:
    case AddressMode::ACCUMULATOR:
    case AddressMode::ABSOLUTE:
    case AddressMode::ZERO_PAGE_X:
    case AddressMode::ABSOLUTE_X:
        val = va_arg(args, uint8_t *);
        break;
    }

    uint8_t oldVal = *val;
    *val <<= 1;
    *val |= this->checkFlag(Flags::CARRY);
    printdf("Carry flag value 0x%X", this->checkFlag(Flags::CARRY));

    this->setFlag(oldVal & 0x80, Flags::CARRY);
    this->setFlag(*val & 0x80, Flags::NEGATIVE);
    this->setFlag(!(*val), Flags::ZERO);

    va_end(args);
    return 1;
}
inline int MOS6502::LSR(MOS6502::AddressMode addressMode, ...) {
    printdf("LSR\n");
    va_list args;
    va_start(args, addressMode);

    uint8_t *val;

    switch (addressMode) {
    case AddressMode::ZERO_PAGE:
    case AddressMode::ACCUMULATOR:
    case AddressMode::ABSOLUTE:
    case AddressMode::ZERO_PAGE_X:
    case AddressMode::ABSOLUTE_X:
        val = va_arg(args, uint8_t *);
        break;
    }

    this->setFlag(*val & 1, Flags::CARRY);
    *val >>= 1;
    this->setFlag(!(*val), Flags::ZERO);
    this->setFlag(*val & 0x80, Flags::NEGATIVE);    // pointless? bit 7 will always be 0

    va_end(args);
    return 1;
}
inline int MOS6502::ROR(MOS6502::AddressMode addressMode, ...) {
    printdf("ROR\n");
    va_list args;
    va_start(args, addressMode);

    uint8_t *val;

    switch (addressMode) {
    case AddressMode::ZERO_PAGE:
    case AddressMode::ACCUMULATOR:
    case AddressMode::ABSOLUTE:
    case AddressMode::ZERO_PAGE_X:
    case AddressMode::ABSOLUTE_X:
        val = va_arg(args, uint8_t *);
        break;
    }

    uint8_t oldVal = *val;
    *val >>= 1;
    *val |= (this->checkFlag(Flags::CARRY) << 7);

    this->setFlag(oldVal & 1, Flags::CARRY);
    this->setFlag(*val & 0x80, Flags::NEGATIVE);
    this->setFlag(!(*val), Flags::ZERO);

    va_end(args);
    return 1;
}
inline int MOS6502::STX(MOS6502::AddressMode addressMode, ...) {
    printdf("STX\n");
    va_list args;
    va_start(args, addressMode);

    uint16_t address;

    switch (addressMode) {
    case AddressMode::ZERO_PAGE:
    case AddressMode::ABSOLUTE:
    case AddressMode::ZERO_PAGE_Y:
        address = (uint16_t)va_arg(args, int);
        break;
    default:
        return -1;
    }

    this->addrSpace.w8(address, this->X);

    va_end(args);
    return 1;
}
inline int MOS6502::LDX(MOS6502::AddressMode addressMode, ...) {
    printdf("LDX\n");
    va_list args;
    va_start(args, addressMode);

    uint8_t val;

    switch (addressMode) {
    case AddressMode::IMMEDIATE:
    case AddressMode::ZERO_PAGE:
    case AddressMode::ABSOLUTE:
    case AddressMode::ZERO_PAGE_Y:
    case AddressMode::ABSOLUTE_Y:
        val = this->addrSpace.r8((uint16_t)va_arg(args, int));
        break;
    }

    this->X = val;
    this->setFlag(!this->X, Flags::ZERO);
    this->setFlag(this->X & 0x80, Flags::NEGATIVE);

    va_end(args);
    return 1;
}
inline int MOS6502::DEC(MOS6502::AddressMode addressMode, ...) {
    printdf("DEC\n");
    va_list args;
    va_start(args, addressMode);

    uint8_t *val;

    switch (addressMode) {
    case AddressMode::ZERO_PAGE:
    case AddressMode::ABSOLUTE:
    case AddressMode::ZERO_PAGE_X:
    case AddressMode::ABSOLUTE_X:
        val = va_arg(args, uint8_t *);
        break;
    }

    (*val)--;
    this->setFlag(!(*val), Flags::ZERO);
    this->setFlag(*val & 0x80, Flags::NEGATIVE);

    va_end(args);
    return 1;
}
inline int MOS6502::INC(MOS6502::AddressMode addressMode, ...) {
    printdf("INC\n");
    va_list args;
    va_start(args, addressMode);

    uint8_t *val;

    switch (addressMode) {
    case AddressMode::ZERO_PAGE:
    case AddressMode::ABSOLUTE:
    case AddressMode::ZERO_PAGE_X:
    case AddressMode::ABSOLUTE_X:
        val = va_arg(args, uint8_t *);
        break;
    }

    (*val)++;
    this->setFlag(!(*val), Flags::ZERO);
    this->setFlag(*val & 0x80, Flags::NEGATIVE);

    va_end(args);
    return 1;
}

/* Subroutine instructions */
inline int MOS6502::BRK(MOS6502::AddressMode addressMode, ...) {
    printdf("BRK\n");
    return -1;
}
inline int MOS6502::JSR(MOS6502::AddressMode addressMode, ...) {
    printdf("JSR\n");
    return -1;
}
inline int MOS6502::RTI(MOS6502::AddressMode addressMode, ...) {
    printdf("RTI\n");
    return -1;
}
inline int MOS6502::RTS(MOS6502::AddressMode addressMode, ...) {
    printdf("RTS\n");
    return -1;
}

/* Index register instructions */
inline int MOS6502::PHP(MOS6502::AddressMode addressMode, ...) {
    printdf("PHP\n");
    return -1;
}
inline int MOS6502::PLP(MOS6502::AddressMode addressMode, ...) {
    printdf("PLP\n");
    return -1;
}
inline int MOS6502::PHA(MOS6502::AddressMode addressMode, ...) {
    printdf("PHA\n");
    return -1;
}
inline int MOS6502::PLA(MOS6502::AddressMode addressMode, ...) {
    printdf("PLA\n");
    return -1;
}
inline int MOS6502::DEY(MOS6502::AddressMode addressMode, ...) {
    printdf("DEY\n");
    return -1;
}
inline int MOS6502::TAY(MOS6502::AddressMode addressMode, ...) {
    printdf("TAY\n");
    return -1;
}
inline int MOS6502::INY(MOS6502::AddressMode addressMode, ...) {
    printdf("INY\n");
    return -1;
}
inline int MOS6502::INX(MOS6502::AddressMode addressMode, ...) {
    printdf("INX\n");
    return -1;
}

/* Flag instructions */
inline int MOS6502::CLC(MOS6502::AddressMode addressMode, ...) {
    printdf("CLC\n");
    return -1;
}
inline int MOS6502::SEC(MOS6502::AddressMode addressMode, ...) {
    printdf("SEC\n");
    switch (addressMode) {
    case AddressMode::IMPLIED:
        break;
    default:
        return -1;
    }
    this->setFlag(true, Flags::CARRY);
    return 1;
}
inline int MOS6502::SEI(MOS6502::AddressMode addressMode, ...) {
    printdf("SEI\n");
    switch (addressMode) {
    case AddressMode::IMPLIED:
        break;
    default:
        return -1;
    }
    this->setFlag(true, Flags::IRQ);
    return 1;
}
inline int MOS6502::CLI(MOS6502::AddressMode addressMode, ...) {
    printdf("CLI\n");
    return -1;
}
inline int MOS6502::TYA(MOS6502::AddressMode addressMode, ...) {
    printdf("TYA\n");
    return -1;
}
inline int MOS6502::CLV(MOS6502::AddressMode addressMode, ...) {
    printdf("CLV\n");
    return -1;
}
inline int MOS6502::CLD(MOS6502::AddressMode addressMode, ...) {
    printdf("CLD\n");
    switch (addressMode) {
    case AddressMode::IMPLIED:
        break;
    default:
        return -1;
    }
    this->setFlag(false, Flags::DECIMAL);
    return 1;
}
inline int MOS6502::SED(MOS6502::AddressMode addressMode, ...) {
    printdf("SED\n");
    return -1;
}

/* Transfer instructions */
inline int MOS6502::TXA(MOS6502::AddressMode addressMode, ...) {
    printdf("TXA\n");
    return -1;
}
inline int MOS6502::TXS(MOS6502::AddressMode addressMode, ...) {
    printdf("TXS\n");
    return -1;
}
inline int MOS6502::TAX(MOS6502::AddressMode addressMode, ...) {
    printdf("TAX\n");
    return -1;
}
inline int MOS6502::TSX(MOS6502::AddressMode addressMode, ...) {
    printdf("TSX\n");
    return -1;
}
inline int MOS6502::DEX(MOS6502::AddressMode addressMode, ...) {
    printdf("DEX\n");
    return -1;
}
inline int MOS6502::NOP(MOS6502::AddressMode addressMode, ...) {
    printdf("NOP\n");
    return -1;
}

/* Branch instructions */
inline int MOS6502::BPL(MOS6502::AddressMode addressMode, ...) {
    printdf("BPL\n");
    return -1;
}
inline int MOS6502::BMI(MOS6502::AddressMode addressMode, ...) {
    printdf("BMI\n");
    return -1;
}
inline int MOS6502::BVC(MOS6502::AddressMode addressMode, ...) {
    printdf("BVC\n");
    return -1;
}
inline int MOS6502::BVS(MOS6502::AddressMode addressMode, ...) {
    printdf("BVS\n");
    return -1;
}
inline int MOS6502::BCC(MOS6502::AddressMode addressMode, ...) {
    printdf("BCC\n");
    return -1;
}
inline int MOS6502::BCS(MOS6502::AddressMode addressMode, ...) {
    printdf("BCS\n");
    return -1;
}
inline int MOS6502::BNE(MOS6502::AddressMode addressMode, ...) {
    printdf("BNE\n");
    return -1;
}
inline int MOS6502::BEQ(MOS6502::AddressMode addressMode, ...) {
    printdf("BEQ\n");
    return -1;
}
