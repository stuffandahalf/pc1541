#ifndef ARDUINO_INTERFACE_H
#define ARDUINO_INTERFACE_H

#include <iostream>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "CBMDriveEmu.h"
#include "IClockable.h"
#include "IPeripheral.h"

class ArduinoInterface : public IClockable, public IPeripheral<uint8_t> {
private:
    int fd;
    std::string devPath;
    int baud;

public:
    ArduinoInterface(std::string devName, int baud);
    ArduinoInterface(std::string devName);
    ArduinoInterface(const char *devName, int baud);
    ArduinoInterface(const char *devName);
    ~ArduinoInterface();

    void write(const char *str, std::size_t count);
    bool isOpen();
    int open();
    void close();
    
    virtual int cycle() override;
    
    virtual void setDirection(uint8_t ddr) override;
    virtual void setPort(uint8_t port) override;
    virtual uint8_t getPort() override;
    
private:
    enum class InterfaceProtocol : uint8_t {
        Ready = 0,
        SetDirection = 1,
        SetPort = 2,
        GetPort = 3,
        Invalid = 0xFF
    };
};

#endif
