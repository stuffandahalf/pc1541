#include "Memory.h"
#include <fstream>

AddressSpace::AddressSpace(std::size_t size) {
    this->size = size;
    this->mem = new Byte *[size];
}

AddressSpace::~AddressSpace() {
    delete[] this->mem;
}

void AddressSpace::map(const std::size_t index, const std::size_t size, Byte **locations) {
    for (int i = 0; i < size; i++) {
        this->mem[index + i] = locations[i];
    }
}

void AddressSpace::map(const std::size_t index, IMemory& mem) {
    this->map(index, mem.getSize(), mem.getBytes());
}

AddressSpace::Byte::Byte() {
    this->readPtr = nullptr;
    this->writePtr = nullptr;
}

AddressSpace::Byte::Byte(uint8_t *ptr) {
    this->readPtr = ptr;
    this->writePtr = ptr;
}

AddressSpace::Byte::Byte(uint8_t *readPtr, uint8_t *writePtr) {
    this->readPtr = readPtr;
    this->writePtr = writePtr;
}



ROM::ROM(std::size_t size) {
    this->size = size;
    this->rawBuffer = new uint8_t[size];
    this->buffer = new AddressSpace::Byte *[this->size];
    for (int i = 0; i < this->size; i++) {
        this->rawBuffer[i] = 0xFF;
        this->buffer[i] = new AddressSpace::Byte(&(this->rawBuffer[i]), nullptr);
    }
}

ROM::~ROM() {
    for (int i = 0; i < this->size; i++) {
        if (this->buffer[i] != nullptr) {
            delete this->buffer[i];
        }
    }
    delete[] this->buffer;
    delete[] this->rawBuffer;
}

std::size_t ROM::getSize() {
    return this->size;
}

AddressSpace::Byte **ROM::getBytes() {
    return this->buffer;
}

int ROM::load(std::string& fname) {
    std::size_t dataSize;
    
    std::ifstream fileStream(fname);
    fileStream.seekg(0, fileStream.end);
    dataSize = fileStream.tellg();
    fileStream.seekg(0, fileStream.beg);
    
    if (dataSize > this->size) {
        fileStream.close();
        return -1;
    }
    
    fileStream.read((char *)this->rawBuffer, this->size);
    fileStream.close();
    
    //return this->load(dataSize, this->rawBuffer);
    return 1;
    
}

int ROM::load(std::size_t size, uint8_t *data) {
    if (size > this->size) {
        return -1;
    }
    for (int i = 0; i < this->size; i++) {
        this->rawBuffer[i] = data[i];
    }
    return 1;
}


RAM::RAM(std::size_t size) {
    this->size = size;
    this->rawBuffer = new uint8_t[this->size];
    this->buffer = new AddressSpace::Byte *[this->size];
    
    for (int i = 0; i < this->size; i++) {
        this->buffer[i] = new AddressSpace::Byte(&(this->rawBuffer[i]));
    }
}

RAM::~RAM() {
    for (int i = 0; i < this->size; i++) {
        delete this->buffer[i];
    }
    delete[] this->buffer;
    delete[] this->rawBuffer;
}

std::size_t RAM::getSize() {
    return this->size;
}

AddressSpace::Byte **RAM::getBytes() {
    return this->buffer;
}
