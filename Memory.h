#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <cstddef>

class Memory {
private:
    uint8_t *init_ptr;
    uint8_t **mem;
    std::size_t size;

public:
    Memory(std::size_t size);
    ~Memory();
    
    void add(const std::size_t index, const std::size_t range, uint8_t **locations);
    
    uint8_t& operator [](const std::size_t i);
};

#endif
