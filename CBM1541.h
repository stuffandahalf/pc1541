#ifndef CBM1541_H
#define CBM1541_H

//#include <string>
#include "Memory.h"
#include "MOS6502.h"
#include "MOS6522.h"

class CBM1541 {
private:
    AddressSpace *addrSpace;
    MOS6502 *cpu;
    MOS6522 *serialVia;
    MOS6522 *motorHeadVia;
    ROM *rom;
    RAM *ram;
    
public:
    CBM1541(std::string romPath);
    ~CBM1541();
    
    void execute();
    
};

#endif
