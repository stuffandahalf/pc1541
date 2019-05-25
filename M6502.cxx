#include "M6502.h"

M6502::M6502(Memory *mem) : mem(*mem) {
    this->A = 0;
    this->X = 0;
    this->Y = 0;
    this->SP = 0;
    this->PC.W = 0xFFFC;    // Reset vector
    this->P = 32;           // All flags 0 except for unused flag
    
    //this->mem(*mem);
    
    this->counter = 0;
    this->opcode = 0;
}

void M6502::reset() {
    std::cout << (int)this->mem[this->PC.W] << std::endl;
    uint16_t start = 0;
    start |= this->mem[this->PC.W++];
    start |= this->mem[this->PC.W] << 8;
    this->PC.W = start;
    
    this->P |= Flags::IRQ;
}

void M6502::step() {
    if (this->counter == 0) {
        //opcode
    }
}

bool inline M6502::checkFlag(Flags f) const {
    return this->P & f;
}

std::ostream& operator <<(std::ostream& os, const M6502& cpu) {
    os << "A:\t" << (int)cpu.A << std::endl
       << "X:\t" << (int)cpu.X << std::endl
       << "Y:\t" << (int)cpu.Y << std::endl
       << "SP:\t" << (int)cpu.SP << std::endl
       << "PC:\t" << (int)cpu.PC.W << std::endl
       << "Flags" << std::endl
       << "N: " << cpu.checkFlag(M6502::Flags::NEGATIVE) << " "
       << "V: " << cpu.checkFlag(M6502::Flags::OVERFLOW) << " "
       << "B: " << cpu.checkFlag(M6502::Flags::BREAK) << " "
       << "D: " << cpu.checkFlag(M6502::Flags::DECIMAL) << " "
       << "I: " << cpu.checkFlag(M6502::Flags::IRQ) << " "
       << "Z: " << cpu.checkFlag(M6502::Flags::ZERO) << " "
       << "C: " << cpu.checkFlag(M6502::Flags::CARRY) << " "
       << std::endl;
       
    return os;
}
