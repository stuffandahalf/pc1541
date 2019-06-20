#include "MOS6522.h"

MOS6522::MOS6522() {
    this->regs = new Registers(17);
}

MOS6522::~MOS6522() {
    delete this->regs;
}

Registers& MOS6522::getRegisters() {
    return *this->regs;
}

int MOS6522::cycle() {
    return 1;
}
