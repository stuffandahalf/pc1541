#ifndef IINTERRUPTIBLE_H
#define IINTERRUPTIBLE_H

#include <cstdint>

class IInterruptible {
public:
    virtual void interrupt() = 0;
    virtual bool pending() = 0;
    virtual void dismiss() = 0;
    virtual ~IInterruptible() { }
};

#endif
