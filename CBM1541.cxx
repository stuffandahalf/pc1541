#include "CBM1541.h"

CBM1541::CBM1541(std::string romPath) {
    this->mem = new Memory(0x10000);
    this->rom = new ROM("firmware/dos1541");
    this->cpu = new MOS6502(this->mem);
    this->serialVia = new MOS6522();
    this->motorHeadVia = new MOS6522();
    
    
    this->mem->map(0xC000, this->rom->getSize(), this->rom->getRom());
    
    
    this->cpu->reset();
    
    //std::cout << *this->cpu << std::endl;
}

CBM1541::~CBM1541() {
    delete this->motorHeadVia;
    delete this->serialVia;
    delete this->cpu;
    delete this->rom;
    delete this->mem;
}

void CBM1541::execute() {
    for(;;) {
        // load data from arduino into serial via port
        this->cpu->step();
    }
}
