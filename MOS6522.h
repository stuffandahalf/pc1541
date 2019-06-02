#ifndef MOS6522_H
#define MOS6522_H

#include "Memory.h"
#include "Registers.h"

class MOS6522;

class MOS6522 {
public:
    enum RegIndex {
        PORTB = 0,
        PORTA = 1,
        DDRB  = 2,
        DDRA  = 3,
        T1CL  = 4,
        T1CH  = 5,
        T1LL  = 6,
        T1LH  = 7,
        T2LL  = 8,
        T2CL  = 9,
        T2CH  = 10,
        SR    = 11,
        ACR   = 12,
        PCR   = 13,
        IFR   = 14,
        IER   = 15,
        ORAX  = 16
    };
    
private:
    Registers *regs;
    
public:
    MOS6522();
    ~MOS6522();
    
    Registers& getRegisters();
};

#endif
