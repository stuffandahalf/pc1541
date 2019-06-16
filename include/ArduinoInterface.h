#ifndef ARDUINO_INTERFACE_H
#define ARDUINO_INTERFACE_H

#include <iostream>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "IClockable.h"

class ArduinoInterface /*: IClockable*/ {
private:
    int dev;
    std::string devPath;
    int baud;
    
public:
    ArduinoInterface(std::string& devName, int baud);
    ~ArduinoInterface();
    
    int open();
    void close();
    //virtual void cycle() override;
};

#endif
