#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <cstddef>

class Memory {
public:
    class Byte {
    private:
        const uint8_t *readPtr;
        uint8_t *writePtr;
        
    public:
        Byte();
        Byte(uint8_t *ptr);
        Byte(uint8_t *readPtr, uint8_t *writePtr);
        
        inline uint8_t read() {
            return this->readPtr ? *this->readPtr : 0xFF;
        }
        
        inline void write(uint8_t b) { 
            if (this->writePtr) {
                *this->writePtr = b;
            }
        }
    };
    
private:
    Memory::Byte **mem;
    std::size_t size;

public:
    Memory(std::size_t size);
    ~Memory();
    
    void map(const std::size_t index, const std::size_t size, Memory::Byte **locations);
    
    inline uint8_t r8(const std::size_t addr) { return this->mem[addr]->read(); }
    inline void w8(const std::size_t addr, uint8_t b) { this->mem[addr]->write(b); }
};

#endif
