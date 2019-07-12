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
    if (this->interface->isOpen()) {
        this->interface->close();
    }

    delete this->interface;
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
    using std::cout;
    using std::cerr;
    using std::endl;

    //this->serialVia->synchronizePortB(this->interface);
    for (;;) {
        //printdf("interface port: %X\n", this->interface->getPort());
        // load data from arduino into serial via port
        
        /*if (!this->cpu->getCounter()) {
            cout << *this->cpu << endl;
            cout << *this->serialVia << endl;
            //printdf("Serial VIA direction B = %X\n", this->serialVia->getRegisters()[MOS6522::RegIndex::DDRB].read());
            //printdf("Serial VIA port B = %X\n", this->serialVia->getRegisters()[MOS6522::RegIndex::PORTB].read());
        }*/
        
        /*if (this->cpu->cycle() < 0) {
            cerr << "CPU encountered an invalid instruction." << endl;
            break;
        }
        
        if (this->serialVia->cycle() < 0) {
            cerr << "Serial controller encountered an error." << endl;
            break;
        }*/
        //cout << *this->serialVia << endl;
        //break;
        /*if (this->motorHeadVia->cycle() < 0) {
            cerr << "Disk head and motor controller encountered an error." << endl;
            break;
        }*/
    }
    cout << *this->cpu << endl;
}
