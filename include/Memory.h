#ifndef MEMORY_H
#define MEMORY_H

#include <string>
#include <cstdint>
#include <cstddef>

class IMemory;

class AddressSpace {
public:
    class Byte {
    private:
        const uint8_t *readPtr;
        uint8_t *writePtr;
        
    public:
        Byte();
        Byte(uint8_t *ptr);
        Byte(uint8_t *readPtr, uint8_t *writePtr);
        virtual ~Byte() { }
        
        virtual inline uint8_t read() {
            return this->readPtr ? *this->readPtr : 0xFF;
        }
        
        virtual inline void write(uint8_t b) { 
            if (this->writePtr) {
                *this->writePtr = b;
            }
        }
    };
    
private:
    AddressSpace::Byte **mem;
    std::size_t size;

public:
    AddressSpace(std::size_t size);
    ~AddressSpace();
    
    void map(const std::size_t index, const std::size_t size, AddressSpace::Byte **locations);
    void map(const std::size_t index, IMemory& mem);
    
    inline uint8_t r8(const std::size_t addr) { return this->mem[addr]->read(); }
    inline void w8(const std::size_t addr, uint8_t b) { this->mem[addr]->write(b); }
};

class IMemory {
public:
    virtual std::size_t getSize() = 0;
    virtual AddressSpace::Byte **getBytes() = 0;
    virtual ~IMemory() { }
};

class ROM : public IMemory {
private:
    std::size_t size;
    uint8_t *rawBuffer;
    AddressSpace::Byte **buffer;

public:
    ROM(std::string& firmwarePath);
    ~ROM();
    
    virtual std::size_t getSize();
    virtual AddressSpace::Byte **getBytes();
};

class RAM : public IMemory {
private:
    std::size_t size;
    uint8_t *rawBuffer;
    AddressSpace::Byte **buffer;
    
public:
    RAM(std::size_t size);
    virtual ~RAM();
    
    virtual std::size_t getSize();
    virtual AddressSpace::Byte **getBytes();
};

#endif
