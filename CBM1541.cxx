#include "CBM1541.h"
#include <fstream>

CBM1541::CBM1541(std::string romPath) {
    this->mem = new Memory(0x10000);
    this->cpu = new MOS6502(this->mem);
    this->serialVia = new MOS6522();
    this->motorHeadVia = new MOS6522();
    
    std::ifstream firmwareFile(romPath);
    
    firmwareFile.seekg(0, firmwareFile.end);
    std::size_t firmwareSize = firmwareFile.tellg();
    firmwareFile.seekg(0, firmwareFile.beg);
    
    uint8_t *firmware = new uint8_t[firmwareSize];
    firmwareFile.read((char *)firmware, firmwareSize);
    
    Memory::Byte **romBytes = new Memory::Byte *[firmwareSize];
    for (int i = 0; i < firmwareSize; i++) {
        romBytes[i] = new Memory::Byte(&(firmware[i]), nullptr);
    }
    
    this->mem->map(0xC000, firmwareSize, romBytes);
    
    for (int i = 0xC000; i < 0x10000; i++) {
        std::cout << (int)this->mem->r8(i) << std::endl;
    }
    
    
    this->cpu->reset();
    
    //std::cout << *this->cpu << std::endl;
}

CBM1541::~CBM1541() {
    delete this->motorHeadVia;
    delete this->serialVia;
    delete this->cpu;
    delete this->mem;
}

void CBM1541::execute() {
    for(;;) {
        // load data from arduino into serial via port
        this->cpu->step();
    }
}
