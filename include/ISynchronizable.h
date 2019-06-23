#ifndef ISYNCHRONIZABLE_H
#define ISYNCHRONIZABLE_H

template <typename T>
class ISynchronizable {
public:
    virtual void setDirection(T ddr) = 0;
    virtual void setPort(T port) = 0;
    virtual T getPort() = 0;
    virtual ~ISynchronizable() { }
};

#endif
