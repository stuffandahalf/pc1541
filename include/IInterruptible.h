#ifndef IINTERRUPTIBLE_H
#define IINTERRUPTIBLE_H

#include <cstdint>

class IInterruptible {
public:
    virtual int interrupt(uint8_t level) = 0;
    virtual ~IInterruptible() { }
};

#endif
