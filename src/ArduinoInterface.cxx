#include "ArduinoInterface.h"

ArduinoInterface::ArduinoInterface(std::string devName, int baud) {
#ifdef _WIN32
	this->portHandle = INVALID_HANDLE_VALUE;
#else
    this->fd = -1;
#endif
    this->devPath = devName;
    this->baud = baud;
}

ArduinoInterface::ArduinoInterface(std::string devName) : ArduinoInterface(devName, 115200) { }
ArduinoInterface::ArduinoInterface(const char *devName) : ArduinoInterface(std::string(devName)) { }
ArduinoInterface::ArduinoInterface(const char *devName, int baud) : ArduinoInterface(std::string(devName), baud) { }

ArduinoInterface::~ArduinoInterface() {
#ifdef _WIN32
	if (this->isOpen()) {
		::CloseHandle(this->portHandle);
		this->portHandle = INVALID_HANDLE_VALUE;
	}
#else
    if (this->fd >= 0) {
        ::close(this->fd);
		this->fd = -1;
    }
#endif
}

bool ArduinoInterface::isOpen() {
#ifdef _WIN32
	return this->portHandle != INVALID_HANDLE_VALUE;
#else
    return this->fd >= 0;
#endif
}

#ifdef _WIN32
int ArduinoInterface::open() {
	if (this->isOpen()) {
		return 0;
	}

	this->portHandle = CreateFile(this->devPath.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
	if (this->portHandle == INVALID_HANDLE_VALUE) {
		std::cerr << "Failed to open serial device." << std::endl;
		return -1;
	}

	DCB dcb;

	if (!GetCommState(this->portHandle, &dcb)) {
		std::cerr << "Failed to acquire serial port configuration." << std::endl;
		this->close();
		return -2;
	}

	dcb.BaudRate = this->baud;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;

	if (!SetCommState(this->portHandle, &dcb)) {
		std::cerr << "Failed to set port configuration." << std::endl;
		this->close();
		return -3;
	}

	return 1;
}
#else
// Logic from https://stackoverflow.com/questions/31663776/ubuntu-c-termios-h-example-program
int ArduinoInterface::open() {
    if (this->isOpen()) {
        return 0;
    }

    this->fd = ::open(this->devPath.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (this->fd < 0) {
        std::cerr << "Failed to open serial device." << std::endl;
        return -1;
    }

    struct termios serial;
    struct termios serialOld;

    if (tcgetattr(this->fd, &serial) < 0) {
        std::cerr << "Failed to acquire serial port configuration." << std::endl;
		this->close();
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

    tcflush(this->fd, TCIFLUSH);

    if (tcsetattr(this->fd, TCSANOW, &serial) < 0) {
        std::cerr << "Failed to set serial configuration." << std::endl;
		this->close();
        return -3;
    }

    return 1;
}
#endif

#ifndef _WIN32
void ArduinoInterface::write(const char *str, std::size_t count) {
    //fprintf(this->dev, "This is a test");
    int64_t received = 0;
    int64_t remainingCount = count;

    char *bufferBase = new char[count + 1];
    bufferBase[count] = '\0';
    char *buffer = bufferBase;
    ::write(this->fd, str, count);
    while (received < count) {
        int64_t incoming = ::read(this->fd, buffer, remainingCount);
        if (incoming > 0) {
            buffer += incoming;
            remainingCount -= incoming;
            received += incoming;
        }
    }

    std::cout << bufferBase << std::endl;

    delete[] bufferBase;
}
#endif

int ArduinoInterface::cycle() {
#ifndef _WIN32
    this->write("Hello\n", 6);
#endif
    return 1;
}

void ArduinoInterface::setDirection(uint8_t ddr) {
    
}

void ArduinoInterface::setPort(uint8_t port) {
    
}

uint8_t ArduinoInterface::getPort() {
    return 0;
}

void ArduinoInterface::close() {
#ifdef _WIN32
	::CloseHandle(this->portHandle);
	this->portHandle = INVALID_HANDLE_VALUE;
#else
    ::close(this->fd);
    this->fd = -1;
#endif
}
