#include "Memory.h"
#include <fstream>

Memory::Memory(std::size_t size) {
    this->size = size;
    this->mem = new Byte *[size];
    /*for (int i = 0; i < size; i++) {
        this->mem[i] = init_ptr + i;
    }*/
}

Memory::~Memory() {
    delete[] this->mem;
    //delete[] this->init_ptr;
}

/*Memory::Byte& Memory::operator [](const std::size_t i) {
    return *(this->mem[i]);
}*/


/*void Memory::map(const std::size_t index, const std::size_t size, uint8_t **locations) {
    for (int i = 0; i < size; i++) {
        this->mem[index + i] = locations[i];
    }
}*/

void Memory::map(const std::size_t index, const std::size_t size, Memory::Byte **locations) {
    for (int i = 0; i < size; i++) {
        this->mem[index + i] = locations[i];
    }
}

Memory::Byte::Byte() {
    this->readPtr = nullptr;
    this->writePtr = nullptr;
}

Memory::Byte::Byte(uint8_t *ptr) {
    this->readPtr = ptr;
    this->writePtr = ptr;
}

Memory::Byte::Byte(uint8_t *readPtr, uint8_t *writePtr) {
    this->readPtr = readPtr;
    this->writePtr = writePtr;
}

/*inline uint8_t Memory::Byte::read() {
    return this->readPtr ? *this->readPtr : 0xFF;
}

inline void Memory::Byte::write(uint8_t b) {
    if (this->writePtr) {
        *this->writePtr = b;
    }
}*/

ROM::ROM(std::string firmwarePath) {
    std::ifstream firmwareFile(firmwarePath);
    
    firmwareFile.seekg(0, firmwareFile.end);
    this->size = firmwareFile.tellg();
    firmwareFile.seekg(0, firmwareFile.beg);
    
    this->rawBuffer = new uint8_t[this->size];
    firmwareFile.read((char *)this->rawBuffer, this->size);
    firmwareFile.close();
    
    this->buffer = new Memory::Byte *[this->size];
    for (int i = 0; i < this->size; i++) {
        this->buffer[i] = new Memory::Byte(&(this->rawBuffer[i]), nullptr);
    }
}

ROM::~ROM() {
    for (int i = 0; i < this->size; i++) {
        delete this->buffer[i];
    }
    delete[] this->buffer;
    delete[] this->rawBuffer;
}

std::size_t ROM::getSize() {
    return this->size;
}

Memory::Byte **ROM::getRom() {
    return this->buffer;
}
