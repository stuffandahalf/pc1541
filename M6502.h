#ifndef _6502_H
#define _6502_H

#include <cstdint>
#include "types.h"
#include "Memory.h"

class M6502 {
private:
    uint8_t A;      // Accumulator
    uint8_t X;      // X index
    uint8_t Y;      // Y index
    uint8_t SP;     // Stack pointer
    word_t PC;      // Program counter
    uint8_t P;      // Processor flags
    
    Memory *mem;
    
    uint8_t counter;    // remaining cycles for opcode
    uint8_t opcode;
    
public:
    enum Flags {
        CARRY = 1,
        ZERO = 2,
        IRQ = 4,
        DECIMAL = 8,
        BREAK = 16,
        // 1
        OVERFLOW = 64,
        NEGATIVE = 128,
    };

    M6502(Memory *mem);

    void step();
};

#endif
