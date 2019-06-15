#include "ArduinoInterface.h"

ArduinoInterface::ArduinoInterface(std::string& devName, int baud) {
    this->dev = -1;
    this->devPath = devName;
    this->baud = baud;
}

// Logic from https://stackoverflow.com/questions/31663776/ubuntu-c-termios-h-example-program
int ArduinoInterface::open() {
    this->dev = ::open(this->devPath.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (this->dev < 0) {
        std::cerr << "Failed to open serial device." << std::endl;
        return -1;
    }
    
    struct termios serial;
    struct termios serialOld;
    
    if (tcgetattr(this->dev, &serial) < 0) {
        std::cerr << "Failed to acquire serial port configuration." << std::endl;
        return -2;
    }
    
    serialOld = serial;
    
    cfsetospeed(&serial, (speed_t)this->baud);
    cfsetispeed(&serial, (speed_t)this->baud);
    
    serial.c_cflag &= ~PARENB;
    serial.c_cflag &= ~CSTOPB;
    serial.c_cflag &= ~CSIZE;
    serial.c_cflag |= CS8;
    
    serial.c_cflag &= ~CRTSCTS;
    serial.c_cc[CMIN] = 1;
    serial.c_cc[VTIME] = 5;
    serial.c_cflag |= CREAD | CLOCAL;
    
    cfmakeraw(&serial);
    
    tcflush(this->dev, TCIFLUSH);
    
    if (tcsetattr(this->dev, TCSANOW, &serial) < 0) {
        std::cerr << "Failed to set serial configuration." << std::endl;
        return -3;
    }
    
    return 1;
}

/*void ArduinoInterface::test() {
    fprintf(this->dev, "This is a test");
}*/

void ArduinoInterface::close() {
    ::close(this->dev);
    this->dev = -1;
}