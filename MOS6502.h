#ifndef MOS6502_H
#define MOS6502_H

#include <iostream>
#include <cstdint>
#include "types.h"
#include "Memory.h"

class MOS6502 {
private:
    uint8_t A;      // Accumulator
    uint8_t X;      // X index
    uint8_t Y;      // Y index
    uint8_t SP;     // Stack pointer
    word_t PC;      // Program counter
    uint8_t P;      // Processor flags
    
    AddressSpace& addrSpace;
    
    uint8_t counter;    // remaining cycles for opcode
    uint8_t opcode;
    
public:
    enum class Flags : uint8_t {
        CARRY = 1,
        ZERO = 2,
        IRQ = 4,
        DECIMAL = 8,
        BREAK = 16,
        // 1
        OVERFLOW = 64,
        NEGATIVE = 128,
    };

    MOS6502(AddressSpace *addrSpace);

    void reset();
    void step();
    bool inline checkFlag(Flags f) const;
    
    friend std::ostream& operator <<(std::ostream& os, const MOS6502& cpu);
};

#endif
