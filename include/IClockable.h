#ifndef ICLOCKABLE_H
#define ICLOCKABLE_H

class IClockable {
public:
    virtual int cycle() = 0;
    virtual ~IClockable() { }
};

#endif
