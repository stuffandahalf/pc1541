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
        
    },
    { // 0x9*
        
    },
    { // 0xA*
        
    },
    { // 0xB*
        
    },
    { // 0xC*
        
    },
    { // 0xD*
        
    },
    { // 0xE*
        
    },
    { // 0xF*
        
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
                this->addrSpace.w8(0x100 | this->SP--, this->PC.B.h);
                break;
            case 5:
                this->addrSpace.w8(0x100 | this->SP--, this->PC.B.l);
                break;
            case 4:
                this->setFlag(true, Flags::BREAK);
                this->setFlag(true, Flags::IRQ);
                this->addrSpace.w8(0x100 | this->SP--, this->P);
                break;
            case 3:
                tmp[0] = this->addrSpace.r8((uint16_t)Vectors::BRK);      // target addr low
                break;
            case 2:
                tmp[1] = this->addrSpace.r8((uint16_t)Vectors::BRK + 1);  // target addr high
                break;
            case 1:
                this->PC.B.l = tmp[0];
                this->PC.B.h = tmp[1];
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
                this->A |= this->addrSpace.r8(this->addrSpace.r8(this->PC.W++));
                break;
            case 1:
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
                this->addrSpace.w8(tmp[0], tmp[2]);
                break;
            case 1:
                this->setFlag(!tmp[2], Flags::ZERO);
                this->setFlag(tmp[2] & 0x80, Flags::NEGATIVE);
                this->setFlag(tmp[1] & 0x80, Flags::CARRY);
                break;
            }
            break;
        case 0x08:  // PHP
            switch (this->counter) {
            case 2:
                this->addrSpace.w8(0x100 | this->SP, this->P); 
                break;
            case 1:
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
        /*default:    // Unsupported opcode
            std::cerr << "Instruction not implemented yet" << std::endl;
            this->counter = 0;*/
        }
    }
    
    this->counter--;
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
