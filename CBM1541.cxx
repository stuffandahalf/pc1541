#include "CBM1541.h"

CBM1541::CBM1541(std::string romPath) {
    this->addrSpace = new AddressSpace(0x10000);
    this->rom = new ROM(romPath);
    this->ram = new RAM(0x0800);
    this->cpu = new MOS6502(this->addrSpace);
    this->serialVia = new MOS6522();
    this->motorHeadVia = new MOS6522();
    
    this->addrSpace->map(0x0000, *this->ram);
    //this->addrSpace->map(0x1800, this->serialVia->getRegisters());
    //this->addrSpace->map(0x1C00, this->motorHeadVia->getRegisters());
    this->addrSpace->map(0xC000, *this->rom);
    
    /*for (int i = 0xC000; i <= 0xFFFF; i++) {
        printf("%X\n", this->addrSpace->r8(i));
    }*/
    //printf("%X\n", this->addrSpace->r8(0xEAA0));
    
    this->cpu->reset();
    
    //std::cout << *this->cpu << std::endl;
}

CBM1541::~CBM1541() {
    delete this->motorHeadVia;
    delete this->serialVia;
    delete this->cpu;
    delete this->rom;
    delete this->addrSpace;
}

void CBM1541::execute() {
    for(;;) {
        // load data from arduino into serial via port
        this->cpu->step();
    }
}
