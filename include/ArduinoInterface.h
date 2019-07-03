#ifndef ARDUINO_INTERFACE_H
#define ARDUINO_INTERFACE_H

#include <iostream>
#include <string>
#if defined(_WIN32)
#include <Windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif
#include <fcntl.h>
#include "IClockable.h"
#include "ISynchronizable.h"

class ArduinoInterface : public IClockable, public ISynchronizable<uint8_t> {
private:
#if defined (_WIN32)
	HANDLE portHandle;
#else
    int fd;
#endif
    std::string devPath;
    int baud;

public:
    ArduinoInterface(std::string devName, int baud);
    ArduinoInterface(std::string devName);
    ArduinoInterface(const char *devName, int baud);
    ArduinoInterface(const char *devName);
    ~ArduinoInterface();

#ifndef _WIN32
    void write(const char *str, std::size_t count);
#endif
    bool isOpen();
    int open();
    void close();
    
    virtual int cycle() override;
    
    virtual void setDirection(uint8_t ddr) override;
    virtual void setPort(uint8_t port) override;
    virtual uint8_t getPort() override;
};

#endif
