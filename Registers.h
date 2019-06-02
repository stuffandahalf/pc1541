#ifndef REGISTERS_H
#define REGISTERS_H

#include "Memory.h"

class Registers : public IMemory {
public:
    class Register : public AddressSpace::Byte {
    private:
        Registers *parent;
        
    public:
        Register(Registers *parent, uint8_t *ptr);

        virtual inline void write(uint8_t b) override {
            AddressSpace::Byte::write(b);
            parent->setUpdated();
        }
    };
    
private:
    uint8_t *rawBuffer;
    AddressSpace::Byte **registers;
    uint8_t size;
    bool updated;
    
public:
    Registers();
    ~Registers();
    
    virtual AddressSpace::Byte **getBytes() override;
    virtual std::size_t getSize() override;
    void setUpdated();
    
    AddressSpace::Byte& operator [](uint8_t i);
};

#endif
