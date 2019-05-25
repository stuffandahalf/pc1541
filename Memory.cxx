#include "Memory.h"

Memory::Memory(std::size_t size) {
    this->size = size;
    this->init_ptr = new uint8_t[size];
    this->mem = new uint8_t *[size];
    for (int i = 0; i < size; i++) {
        this->mem[i] = init_ptr + i;
    }
}

Memory::~Memory() {
    delete[] this->mem;
    delete[] this->init_ptr;
}

uint8_t& Memory::operator [](const std::size_t i) {
    return *(this->mem[i]);
}
