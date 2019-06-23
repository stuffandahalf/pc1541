#include "MOS6522.h"

MOS6522::MOS6522() {
    this->regs = new Registers(17);
    this->interruptible = nullptr;
}

MOS6522::~MOS6522() {
    delete this->regs;
}

void MOS6522::assignInterruptible(IInterruptible *interruptible) {
    this->interruptible = interruptible;
}

void MOS6522::removeInterruptible() {
    this->interruptible = nullptr;
}

int MOS6522::cycle() {
    return 1;
}

namespace {
    enum class Port : uint8_t {
        A,
        B
    };
}

int MOS6522::synchronizePort(Port port, ISynchronizable<uint8_t> *syncDev) {
    RegIndex portIndex;
    RegIndex ddrIndex;
    
    switch (port) {
    case Port::A:
        ddrIndex = RegIndex::DDRA;
        portIndex = RegIndex::PORTA;
        break;
    case Port::B:
        ddrIndex = RegIndex::DDRB;
        portIndex = RegIndex::PORTB;
        break;
    }
    
    syncDev->setDirection(this->getRegisters()[ddrIndex].read());
    syncDev->setPort(this->getRegisters()[portIndex].read());
    uint8_t recvPort = syncDev->getPort();
    
    return 1;
}

int MOS6522::synchronizePortA(ISynchronizable<uint8_t> *syncDev) {
    return this->synchronizePort(Port::A, syncDev);
}

int MOS6522::synchronizePortB(ISynchronizable<uint8_t> *syncDev) {
    return this->synchronizePort(Port::B, syncDev);
}

std::ostream& operator <<(std::ostream& os, const MOS6522& via) {
    using std::endl;
    os
        << "PORTB: " << (int)(*via.regs)[MOS6522::RegIndex::PORTB].read() << endl
        << "PORTA: " << (int)(*via.regs)[MOS6522::RegIndex::PORTA].read() << endl
        << "DDRB: " << (int)(*via.regs)[MOS6522::RegIndex::DDRB].read() << endl
        << "DDRA: " << (int)(*via.regs)[MOS6522::RegIndex::DDRA].read() << endl
        << "T1CL: " << (int)(*via.regs)[MOS6522::RegIndex::T1CL].read() << endl
        << "T1CH: " << (int)(*via.regs)[MOS6522::RegIndex::T1CH].read() << endl
        << "T1LL: " << (int)(*via.regs)[MOS6522::RegIndex::T1LL].read() << endl
        << "T1LH: " << (int)(*via.regs)[MOS6522::RegIndex::T1LH].read() << endl
        << "T2LL: " << (int)(*via.regs)[MOS6522::RegIndex::T2LL].read() << endl
        << "T2CL: " << (int)(*via.regs)[MOS6522::RegIndex::T2CL].read() << endl
        << "T2CH: " << (int)(*via.regs)[MOS6522::RegIndex::T2CH].read() << endl
        << "SR: " << (int)(*via.regs)[MOS6522::RegIndex::SR].read() << endl
        << "ACR: " << (int)(*via.regs)[MOS6522::RegIndex::ACR].read() << endl
        << "PCR: " << (int)(*via.regs)[MOS6522::RegIndex::PCR].read() << endl
        << "IFR: " << (int)(*via.regs)[MOS6522::RegIndex::IFR].read() << endl
        << "IER: " << (int)(*via.regs)[MOS6522::RegIndex::IER].read() << endl
        << "ORAX: " << (int)(*via.regs)[MOS6522::RegIndex::ORAX].read() << endl;
}
