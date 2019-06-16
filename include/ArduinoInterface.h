#ifndef ARDUINO_INTERFACE_H
#define ARDUINO_INTERFACE_H

#include <iostream>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "IClockable.h"

class ArduinoInterface : public IClockable {
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
    
    int open();
    void close();
    virtual void cycle() override;
};

#endif
