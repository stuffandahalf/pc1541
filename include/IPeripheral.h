#ifndef IPERIPHERAL_H
#define IPERIPHERAL_H

template <typename T>
class IPeripheral {
public:
    virtual void setDirection(T ddr) = 0;
    virtual void setPort(T port) = 0;
    virtual T getPort() = 0;
    virtual ~IPeripheral() { }
};

#endif
