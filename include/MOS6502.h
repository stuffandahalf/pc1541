#ifndef MOS6502_H
#define MOS6502_H

#include <iostream>
#include <cstdint>
#include "cpuemu.h"
#include "CBMDriveEmu.h"
#include "Memory.h"
#include "IClockable.h"

#define addrout(addr) std::cout << std::hex << (addr) << std::dec << std::endl

class MOS6502 : IClockable {
private:
    uint8_t IR;     // Instruction register (current opcode)
    uint8_t A;      // Accumulator
    uint8_t X;      // X index
    uint8_t Y;      // Y index
    uint8_t SP;     // Stack pointer
    uint16_t PC;      // Program counter
    uint8_t P;      // Processor flags

    AddressSpace& addrSpace;

    uint8_t counter;    // remaining cycles for opcode
    size_t cycles;      // total executed cycles since reset

public:
    enum class Vectors : uint16_t {
        NMI = 0xFFFA,
        RESET = 0xFFFC,
        IRQ = 0xFFFE,
        BRK = 0xFFFE
    };

    enum class Flags : uint8_t {
        CARRY = 1,
        ZERO = 2,
        IRQ = 4,
        DECIMAL = 8,
        BREAK = 16,
        // 1
        OVERFLOW = 64,
        NEGATIVE = 128
    };

    enum class AddressMode {
        ACCUMULATOR,
        IMMEDIATE,
        ZERO_PAGE,
        ZERO_PAGE_X,
        ZERO_PAGE_Y,
        RELATIVE,
        ABSOLUTE,
        ABSOLUTE_X,
        ABSOLUTE_Y,
        INDIRECT,
        INDEXED_INDIRECT,
        INDIRECT_INDEXED,
        IMPLIED
    };

    MOS6502(AddressSpace *addrSpace);

    void reset();
    void step();
    virtual int cycle() override;
    bool checkFlag(Flags f) const;
    bool setFlag(bool condition, Flags f);
    inline uint8_t getCounter() { return this->counter; }

    friend std::ostream& operator <<(std::ostream& os, const MOS6502& cpu);

private:
    int BIT(AddressMode addressMode, ...);
    int JMP_INDIRECT(AddressMode addressMode, ...);
    int JMP_ABSOLUTE(AddressMode addressMode, ...);
    int STY(AddressMode addressMode, ...);
    int LDY(AddressMode addressMode, ...);
    int CPY(AddressMode addressMode, ...);
    int CPX(AddressMode addressMode, ...);

    int ORA(AddressMode addressMode, ...);
    int AND(AddressMode addressMode, ...);
    int EOR(AddressMode addressMode, ...);
    int ADC(AddressMode addressMode, ...);
    int STA(AddressMode addressMode, ...);
    int LDA(AddressMode addressMode, ...);
    int CMP(AddressMode addressMode, ...);
    int SBC(AddressMode addressMode, ...);

    int ASL(AddressMode addressMode, ...);
    int ROL(AddressMode addressMode, ...);
    int LSR(AddressMode addressMode, ...);
    int ROR(AddressMode addressMode, ...);
    int STX(AddressMode addressMode, ...);
    int LDX(AddressMode addressMode, ...);
    int DEC(AddressMode addressMode, ...);
    int INC(AddressMode addressMode, ...);

    int BRK(AddressMode addressMode, ...);
    int JSR(AddressMode addressMode, ...);
    int RTI(AddressMode addressMode, ...);
    int RTS(AddressMode addressMode, ...);

    int PHP(AddressMode addressMode, ...);
    int PLP(AddressMode addressMode, ...);
    int PHA(AddressMode addressMode, ...);
    int PLA(AddressMode addressMode, ...);
    int DEY(AddressMode addressMode, ...);
    int TAY(AddressMode addressMode, ...);
    int INY(AddressMode addressMode, ...);
    int INX(AddressMode addressMode, ...);

    int CLC(AddressMode addressMode, ...);
    int SEC(AddressMode addressMode, ...);
    int CLI(AddressMode addressMode, ...);
    int SEI(AddressMode addressMode, ...);
    int TYA(AddressMode addressMode, ...);
    int CLV(AddressMode addressMode, ...);
    int CLD(AddressMode addressMode, ...);
    int SED(AddressMode addressMode, ...);

    int TXA(AddressMode addressMode, ...);
    int TXS(AddressMode addressMode, ...);
    int TAX(AddressMode addressMode, ...);
    int TSX(AddressMode addressMode, ...);
    int DEX(AddressMode addressMode, ...);
    int NOP(AddressMode addressMode, ...);

    int BPL(AddressMode addressMode, ...);
    int BMI(AddressMode addressMode, ...);
    int BVC(AddressMode addressMode, ...);
    int BVS(AddressMode addressMode, ...);
    int BCC(AddressMode addressMode, ...);
    int BCS(AddressMode addressMode, ...);
    int BNE(AddressMode addressMode, ...);
    int BEQ(AddressMode addressMode, ...);

    typedef int (MOS6502::*operation_t)(MOS6502::AddressMode addressMode, ...);
    operation_t operations[3][8];
    operation_t branchOps[8];
    operation_t flagOps[8];
    operation_t lowNibbleAOps[6];
    operation_t lowNibbleEightHighEvenTwoCycleOps[4];
};

#endif
