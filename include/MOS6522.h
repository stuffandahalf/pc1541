#ifndef MOS6522_H
#define MOS6522_H

#include <iostream>
#include "Memory.h"
#include "Registers.h"
#include "IClockable.h"
#include "IInterruptible.h"
#include "ISynchronizable.h"

class MOS6522;

class MOS6522 : IClockable {
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
    IInterruptible *interruptible;

public:
    MOS6522();
    ~MOS6522();

    void assignInterruptible(IInterruptible *interruptible);
    void removeInterruptible();
    int synchronizePortA(ISynchronizable<uint8_t> *syncDev);
    int synchronizePortB(ISynchronizable<uint8_t> *syncDev);
    
    virtual int cycle() override;
    
    inline Registers& getRegisters() { return *this->regs; }
    
    friend std::ostream& operator <<(std::ostream& os, const MOS6522& via);
    
private:
    enum class Port : uint8_t {
        A,
        B
    };
    int synchronizePort(Port port, ISynchronizable<uint8_t> *syncDev);
};

#endif
