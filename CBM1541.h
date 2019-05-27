#ifndef CBM1541_H
#define CBM1541_H

#include "Memory.h"
#include "MOS6502.h"
#include "MOS6522.h"

class CBM1541 {
private:
    Memory *mem;
    MOS6502 *cpu;
    MOS6522 *serialVia;
    MOS6522 *motorHeadVia;
    
public:
    CBM1541();
    ~CBM1541();
    
    void execute();
    
};

#endif
