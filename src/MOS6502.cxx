#include "MOS6502.h"
#include <cstdio>
#include <cstdarg>

typedef unsigned int uint;

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
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 8; j++) {
            this->operations[i][j] = ops[i][j];
        }
    }

    operation_t flagOps[8] = {
        &MOS6502::CLC, &MOS6502::SEC, &MOS6502::CLI, &MOS6502::SEI, &MOS6502::TYA, &MOS6502::CLV, &MOS6502::CLD, &MOS6502::SED
    };
    for (int i = 0; i < 8; i++) {
        this->flagOps[i] = flagOps[i];
    }
    
    operation_t lowNibbleAOps[6] = {
        &MOS6502::TXA, &MOS6502::TXS, &MOS6502::TAX, &MOS6502::TSX, &MOS6502::DEX, &MOS6502::NOP
    };
    for (int i = 0; i < 6; i++) {
        this->lowNibbleAOps[i] = lowNibbleAOps[i];
    }
    
    operation_t lowNibbleEightHighEvenTwoCycleOps[4] = {
        &MOS6502::DEY, &MOS6502::TAY, &MOS6502::INY, &MOS6502::INX
    };
    for (int i = 0; i < 4; i++) {
        this->lowNibbleEightHighEvenTwoCycleOps[i] = lowNibbleEightHighEvenTwoCycleOps[i];
    }
    
    operation_t branchOps[8] = {
        &MOS6502::BPL, &MOS6502::BMI,
        &MOS6502::BVC, &MOS6502::BVS,
        &MOS6502::BCC, &MOS6502::BCS,
        &MOS6502::BNE, &MOS6502::BEQ
    };
    for (int i = 0; i < 8; i++) {
        this->branchOps[i] = branchOps[i];
    }
}

void MOS6502::reset() {
    uint16_t start = 0;
    start |= this->addrSpace.r8(this->PC++);
    start |= this->addrSpace.r8(this->PC) << 8;
    //printf("%X\n", start);
    this->PC = start;

    this->P |= (uint8_t)Flags::IRQ;

    this->IR = this->addrSpace.r8(this->PC++);
}

void MOS6502::step() {
    do {
        this->cycle();
    } while (this->counter);
}

int MOS6502::interrupt(uint8_t level) {
    return 1;
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

    instructionGroup = this->IR & 0x3;
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
    case 0xA8:  // TAY
    case 0xC8:  // INY
    case 0xE8:  // INX
        switch (this->counter) {
        case 1:
            if ((this->*(lowNibbleEightHighEvenTwoCycleOps[(this->IR >> 4) / 2 - 4]))(AddressMode::IMPLIED) < 0) {
                return -1;
            }
            break;
        case 2:
            this->IR = this->addrSpace.r8(this->PC++);
            this->counter = 0;
            break;
        }
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
    case 0x9A:  // TXS
    case 0xAA:  // TAX
    case 0xBA:  // TSX
    case 0xCA:  // DEX
    case 0xEA:  // NOP
        switch (this->counter) {
        case 1:
            if ((this->*(lowNibbleAOps[(this->IR >> 4) - 8]))(AddressMode::IMPLIED) < 0) {
                return -1;
            }
            break;
        case 2:
            this->IR = this->addrSpace.r8(this->PC++);
            this->counter = 0;
            break;
        }
        break;
    default:
        switch (this->IR & 0x1F) {
        case 0x10:  // Branches
            switch (this->counter) {
            case 1:
                tmp[0] = this->addrSpace.r8(this->PC++);
                break;
            case 2:
                switch ((this->*(branchOps[this->IR >> 5]))(AddressMode::RELATIVE)) {
                case -1:
                    return -1;
                case 0:
                    this->IR = this->addrSpace.r8(this->PC++);
                    this->counter = 0;
                    break;
                }
                break;
            case 3:
                if (((this->PC + (int8_t)tmp[0]) >> 8) != (this->PC >> 8)) {
                    break;
                }
            case 4:
                this->PC += (int8_t)tmp[0];
                this->IR = this->addrSpace.r8(this->PC++);
                this->counter = 0;
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
                    switch (this->counter) {
                    case 1:
                        tmp[0] = this->addrSpace.r8(this->PC++);
                        break;
                    case 2:
                        tmp[0] += this->X;
                        break;
                    case 3:
                        if ((this->*(operations[instructionGroup][instruction]))(AddressMode::ZERO_PAGE_X, tmp[0]) < 0) {
                            return -1;
                        }
                        break;
                    case 4:
                        this->IR = this->addrSpace.r8(this->PC++);
                        this->counter = 0;
                        break;
                    }
                    break;
                case 0b111: // absolute, x
                    switch (this->counter) {
                    case 1:
                        tmp[0] = this->addrSpace.r8(this->PC++);    // low address byte
                        break;
                    case 2:
                        tmp[1] = this->addrSpace.r8(this->PC++);    // high address byte
                        break;
                    case 3:
                        if (tmp[1] != ((((uint16_t)tmp[1] << 8) + tmp[0] + this->X) >> 8)) {
                            tmp[2] = 0;
                            break;
                        }
                        tmp[2] = 1;
                    case 4:
                        if ((this->*(operations[instructionGroup][instruction]))(AddressMode::ABSOLUTE_X, (((uint16_t)tmp[1] << 8) + tmp[0] + this->X)) < 0) {
                            return -1;
                        }
                        if (tmp[2]) {
                            this->counter++;
                        }
                        break;
                    case 5:
                        this->IR = this->addrSpace.r8(this->PC++);
                        this->counter = 0;
                        break;
                    }
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
                        exit(1);
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
                        if ((this->*(this->operations[instructionGroup][instruction]))(AddressMode::ABSOLUTE, (tmp[1] << 8) + tmp[0]) < 0) {
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
    default:
        return -1;
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
    
    this->PC = (uint16_t)va_arg(args, uint32_t);

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
    default:
        return -1;
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
    default:
        return -1;
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
    default:
        return -1;
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
    default:
        return -1;
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
    default:
        return -1;
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
    default:
        return -1;
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
    default:
        return -1;
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
    default:
        return -1;
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
    default:
        return -1;
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
    default:
        return -1;
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
    default:
        return -1;
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
    default:
        return -1;
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
    default:
        return -1;
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
    default:
        return -1;
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
    default:
        return -1;
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
    default:
        return -1;
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
    default:
        return -1;
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
    default:
        return -1;
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
    default:
        return -1;
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
    
    switch (addressMode) {
    case AddressMode::IMPLIED:
        break;
    default:
        return -1;
    }
    
    this->Y--;
    this->setFlag(!this->Y, Flags::ZERO);
    this->setFlag(this->Y & 0x80, Flags::NEGATIVE);
    
    return 1;
}
inline int MOS6502::TAY(MOS6502::AddressMode addressMode, ...) {
    printdf("TAY\n");
    
    switch (addressMode) {
    case AddressMode::IMPLIED:
        break;
    default:
        return -1;
    }
    
    this->Y = this->A;
    this->setFlag(!this->Y, Flags::ZERO);
    this->setFlag(this->Y & 0x80, Flags::NEGATIVE);
    
    return 1;
}
inline int MOS6502::INY(MOS6502::AddressMode addressMode, ...) {
    printdf("INY\n");
    
    switch (addressMode) {
    case AddressMode::IMPLIED:
        break;
    default:
        return -1;
    }
    
    this->Y++;
    this->setFlag(!this->Y, Flags::ZERO);
    this->setFlag(this->X & 0x80, Flags::NEGATIVE);
    
    return 1;
}
inline int MOS6502::INX(MOS6502::AddressMode addressMode, ...) {
    printdf("INX\n");
    
    switch (addressMode) {
    case AddressMode::IMPLIED:
        break;
    default:
        return -1;
    }
    
    this->X++;
    this->setFlag(!this->X, Flags::ZERO);
    this->setFlag(this->X & 0x80, Flags::NEGATIVE);
    
    return 1;
}

/* Flag instructions */
inline int MOS6502::CLC(MOS6502::AddressMode addressMode, ...) {
    printdf("CLC\n");
    
    switch (addressMode) {
    case AddressMode::IMPLIED:
        break;
    default:
        return -1;
    }
    
    this->setFlag(false, Flags::CARRY);
    
    return 1;
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
    
    switch (addressMode) {
    case AddressMode::IMPLIED:
        break;
    default:
        return -1;
    }
    
    this->A = this->Y;
    this->setFlag(!this->A, Flags::ZERO);
    this->setFlag(this->A & 0x80, Flags::NEGATIVE);
    
    return 1;
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
    switch (addressMode) {
    case AddressMode::IMPLIED:
        break;
    default:
        return -1;
    }
    
    this->A = this->X;
    this->setFlag(!this->A, Flags::ZERO);
    this->setFlag(this->A & 0x80, Flags::NEGATIVE);
    
    return 1;
}
inline int MOS6502::TXS(MOS6502::AddressMode addressMode, ...) {
    printdf("TXS\n");
    switch (addressMode) {
    case AddressMode::IMPLIED:
        break;
    default:
        return -1;
    }
    
    this->SP = this->X;
    
    return 1;
}
inline int MOS6502::TAX(MOS6502::AddressMode addressMode, ...) {
    printdf("TAX\n");
    switch (addressMode) {
    case AddressMode::IMPLIED:
        break;
    default:
        return -1;
    }
    
    this->X = this->A;
    this->setFlag(!this->X, Flags::ZERO);
    this->setFlag(this->X & 0x80, Flags::NEGATIVE);
    
    return 1;
}
inline int MOS6502::TSX(MOS6502::AddressMode addressMode, ...) {
    printdf("TSX\n");
    switch (addressMode) {
    case AddressMode::IMPLIED:
        break;
    default:
        return -1;
    }
    
    this->X = this->SP;
    this->setFlag(!this->X, Flags::ZERO);
    this->setFlag(this->X & 0x80, Flags::NEGATIVE);
    
    return 1;
}
inline int MOS6502::DEX(MOS6502::AddressMode addressMode, ...) {
    printdf("DEX\n");
    switch (addressMode) {
    case AddressMode::IMPLIED:
        break;
    default:
        return -1;
    }
    
    this->X--;
    this->setFlag(!this->X, Flags::ZERO);
    this->setFlag(this->X & 0x80, Flags::NEGATIVE);
    
    return 1;
}
inline int MOS6502::NOP(MOS6502::AddressMode addressMode, ...) {
    printdf("NOP\n");
    switch (addressMode) {
    case AddressMode::IMPLIED:
        break;
    default:
        return -1;
    }
    
    return 1;
}

/* Branch instructions */
inline int MOS6502::BPL(MOS6502::AddressMode addressMode, ...) {
    printdf("BPL\n");
    switch (addressMode) {
    case AddressMode::RELATIVE:
        break;
    default:
        return -1;
    }
    return !this->checkFlag(Flags::NEGATIVE);
}
inline int MOS6502::BMI(MOS6502::AddressMode addressMode, ...) {
    printdf("BMI\n");
    switch (addressMode) {
    case AddressMode::RELATIVE:
        break;
    default:
        return -1;
    }
    return this->checkFlag(Flags::NEGATIVE);
}
inline int MOS6502::BVC(MOS6502::AddressMode addressMode, ...) {
    printdf("BVC\n");
    switch (addressMode) {
    case AddressMode::RELATIVE:
        break;
    default:
        return -1;
    }
    return !this->checkFlag(Flags::OVERFLOW);
}
inline int MOS6502::BVS(MOS6502::AddressMode addressMode, ...) {
    printdf("BVS\n");
    switch (addressMode) {
    case AddressMode::RELATIVE:
        break;
    default:
        return -1;
    }
    return this->checkFlag(Flags::OVERFLOW);
}
inline int MOS6502::BCC(MOS6502::AddressMode addressMode, ...) {
    printdf("BCC\n");
    switch (addressMode) {
    case AddressMode::RELATIVE:
        break;
    default:
        return -1;
    }
    return !this->checkFlag(Flags::CARRY);
}
inline int MOS6502::BCS(MOS6502::AddressMode addressMode, ...) {
    printdf("BCS\n");
    switch (addressMode) {
    case AddressMode::RELATIVE:
        break;
    default:
        return -1;
    }
    return this->checkFlag(Flags::CARRY);
}
inline int MOS6502::BNE(MOS6502::AddressMode addressMode, ...) {
    printdf("BNE\n");
    switch (addressMode) {
    case AddressMode::RELATIVE:
        break;
    default:
        return -1;
    }
    return !this->checkFlag(Flags::ZERO);
}
inline int MOS6502::BEQ(MOS6502::AddressMode addressMode, ...) {
    printdf("BEQ\n");
    switch (addressMode) {
    case AddressMode::RELATIVE:
        break;
    default:
        return -1;
    }
    return this->checkFlag(Flags::ZERO);
}
