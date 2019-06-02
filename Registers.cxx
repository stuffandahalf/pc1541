#include "Registers.h"

Registers::Registers() {
    this->size = 17;
    this->rawBuffer = new uint8_t[this->size];
    this->registers = new AddressSpace::Byte *[this->size];
    for (int i = 0; i < this->size; i++) {
        this->registers[i] = new Register(this, &(this->rawBuffer[i]));
    }
    
    this->updated = false;
}

Registers::~Registers() {
    for (int i = 0; i < this->size; i++) {
        delete this->registers[i];
    }
    delete[] this->registers;
    delete[] this->rawBuffer;
}

AddressSpace::Byte **Registers::getBytes() {
    return this->registers;
}

void Registers::setUpdated() {
    this->updated = true;
}

std::size_t Registers::getSize() {
    return this->size;
}

AddressSpace::Byte& Registers::operator [](uint8_t i) {
    return *this->registers[i];
}

Registers::Register::Register(Registers *parent, uint8_t *ptr) : AddressSpace::Byte(ptr) {
    this->parent = parent;
}
