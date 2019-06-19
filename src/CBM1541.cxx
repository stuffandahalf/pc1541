#include "CBM1541.h"

CBM1541::CBM1541(struct config& cfg) {
    this->addrSpace = new AddressSpace(0x10000);
    this->rom = new ROM(0x4000);
    this->ram = new RAM(0x0800);
    this->cpu = new MOS6502(this->addrSpace);
    this->serialVia = new MOS6522();
    this->motorHeadVia = new MOS6522();
    this->interface = new ArduinoInterface(*cfg.devPath, cfg.baud);
    
    this->rom->load(cfg.firmware.size, cfg.firmware.data);
    
    this->addrSpace->map(0x0000, *this->ram);
    this->addrSpace->map(0x1800, this->serialVia->getRegisters());
    this->addrSpace->map(0x1C00, this->motorHeadVia->getRegisters());
    this->addrSpace->map(0xC000, *this->rom);
}

CBM1541::~CBM1541() {
    this->interface->close();
    
    delete this->motorHeadVia;
    delete this->serialVia;
    delete this->cpu;
    delete this->ram;
    delete this->rom;
    delete this->addrSpace;
}

int CBM1541::initialize() {
    if (this->interface->open() < 0) {
        return -1;
    }
    this->cpu->reset();
    return 1;
}

void CBM1541::execute() {
    //char in = 's';
    
    std::cout << *this->cpu << std::endl;
    for (;;) {
        // load data from arduino into serial via port
        this->cpu->step();
        std::cout << *this->cpu << std::endl;
        //std::cin >> in;
    }
    //this->cpu->step();
}
