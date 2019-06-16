#ifndef CBM1541_H
#define CBM1541_H

#include "CBMDriveEmu.h"
#include "IDrive.h"
#include "Memory.h"
#include "MOS6502.h"
#include "MOS6522.h"
#include "ArduinoInterface.h"

class CBM1541 : public IDrive {
private:
    AddressSpace *addrSpace;
    MOS6502 *cpu;
    MOS6522 *serialVia;
    MOS6522 *motorHeadVia;
    ROM *rom;
    RAM *ram;
    ArduinoInterface *interface;
    
public:
    CBM1541(struct config& cfg);
    ~CBM1541();
    
    virtual void execute() override;
    
};

#endif
