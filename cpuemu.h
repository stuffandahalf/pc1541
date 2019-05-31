#ifndef CPUEMU_H
#define CPUEMU_H

struct Instruction {
    enum class CycleMod : uint8_t {
        NONE,
        MOD1,
        MOD2
    };
    
    char mnemonic[6];
    uint8_t opcode;
    uint8_t bytes;
    uint8_t cycles;
    CycleMod cycleMod;
};

#endif
