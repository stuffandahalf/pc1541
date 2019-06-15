#ifndef MOS6502_H
#define MOS6502_H

#include <iostream>
#include <cstdint>
#include "cpuemu.h"
#include "types.h"
#include "Memory.h"
#include "IClockable.h"

#define DEBUG
#define addrout(addr) std::cout << std::hex << (addr) << std::dec << std::endl;

class MOS6502 : IClockable {
private:
    uint8_t IR;     // Instruction register (current opcode)
    uint8_t A;      // Accumulator
    uint8_t X;      // X index
    uint8_t Y;      // Y index
    uint8_t SP;     // Stack pointer
    word_t PC;      // Program counter
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
        INDIRECT_INDEXED
    };

    MOS6502(AddressSpace *addrSpace);

    void reset();
    void step();
    virtual void cycle() override;
    bool checkFlag(Flags f) const;
    bool setFlag(bool condition, Flags f);
    
    friend std::ostream& operator <<(std::ostream& os, const MOS6502& cpu);
    
private:
    

    void ORA(AddressMode addressMode, ...);
    void AND(AddressMode addressMode, ...);
    void EOR(AddressMode addressMode, ...);
    void ADC(AddressMode addressMode, ...);
    void STA(AddressMode addressMode, ...);
    void LDA(AddressMode addressMode, ...);
    void CMP(AddressMode addressMode, ...);
    void SBC(AddressMode addressMode, ...);
    
    void ASL(AddressMode addressMode, ...);
    void ROL(AddressMode addressMode, ...);
    void LSR(AddressMode addressMode, ...);
    void ROR(AddressMode addressMode, ...);
    void STX(AddressMode addressMode, ...);
    void LDX(AddressMode addressMode, ...);
    void DEC(AddressMode addressMode, ...);
    void INC(AddressMode addressMode, ...);

    typedef void (MOS6502::*operation_t)(MOS6502::AddressMode addressMode, ...);
    operation_t operations[4][8];
};

#endif
