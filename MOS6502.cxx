#include "MOS6502.h"

MOS6502::MOS6502(Memory *mem) : mem(*mem) {
    this->A = 0;
    this->X = 0;
    this->Y = 0;
    this->SP = 0;
    this->PC.W = 0xFFFC;    // Reset vector
    this->P = 32;           // All flags 0 except for unused flag
    
    this->counter = 0;
    this->opcode = 0;
}

void MOS6502::reset() {
    uint16_t start = 0;
    start |= this->mem.r8(this->PC.W++);
    start |= this->mem.r8(this->PC.W) << 8;
    this->PC.W = start;
    
    this->P |= (uint8_t)Flags::IRQ;
}

void MOS6502::step() {
    if (this->counter == 0) {
        //opcode
        this->opcode = this->mem.r8(PC.W++);
        /*switch (this->opcode) {
        case 0x00
        }*/
    }
    else {
        switch (opcode) {
            
        }
    }
    
    this->counter--;
}

bool inline MOS6502::checkFlag(Flags f) const {
    return this->P & (uint8_t)f;
}

std::ostream& operator <<(std::ostream& os, const MOS6502& cpu) {
    os << "A:\t" << (int)cpu.A << std::endl
       << "X:\t" << (int)cpu.X << std::endl
       << "Y:\t" << (int)cpu.Y << std::endl
       << "SP:\t" << (int)cpu.SP << std::endl
       << "PC:\t" << (int)cpu.PC.W << std::endl
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
       << std::endl;
       
    return os;
}

static struct {
    char mnemonic[5];
    uint8_t opcode;
    uint8_t cycles;
} instructions[] = {
    { "BRK", 0x00, 7 },
    { "ORA", 0x01, 6 },
    
    
    
    { "ORA", 0x05, 3 },
    { "ASL", 0x06, 5 },
    
    { "PHP", 0x08, 3 },
    { "ORA", 0x09, 2 },
    { "ASL", 0x0A, 2 },
    
    
    { "ORA", 0x0D, 4 },
    { "ASL", 0x0E, 6 },
    
    { "BPL", 0x10, 2 },
    { "ORA", 0x11, 2 },
};
