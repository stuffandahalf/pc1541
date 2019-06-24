#include "ArduinoInterface.h"

ArduinoInterface::ArduinoInterface(std::string devName, int baud) {
    this->fd = -1;
    this->devPath = devName;
    this->baud = baud;
}

ArduinoInterface::ArduinoInterface(std::string devName) : ArduinoInterface(devName, 115200) { }
ArduinoInterface::ArduinoInterface(const char *devName) : ArduinoInterface(std::string(devName)) { }
ArduinoInterface::ArduinoInterface(const char *devName, int baud) : ArduinoInterface(std::string(devName), baud) { }

ArduinoInterface::~ArduinoInterface() {
    if (this->fd >= 0) {
        ::close(this->fd);
    }
}

bool ArduinoInterface::isOpen() {
    return this->fd >= 0;
}

// Logic from https://stackoverflow.com/questions/31663776/ubuntu-c-termios-h-example-program
int ArduinoInterface::open() {
    using std::cerr;
    using std::endl;
    
    if (this->isOpen()) {
        return 0;
    }

    this->fd = ::open(this->devPath.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    fcntl(this->fd, F_SETFL, 0);
    if (this->fd < 0) {
        cerr << "Failed to open serial device." << endl;
        return -1;
    }
    
    if (!isatty(this->fd)) {
        ::close(this->fd);
        this->fd = -1;
        cerr << "Opened device is not a serial device." << endl;
        return -1;
    }

    struct termios serial;
    struct termios serialOld;

    if (tcgetattr(this->fd, &serial) < 0) {
        ::close(this->fd);
        this->fd = -1;
        cerr << "Failed to acquire serial port configuration." << endl;
        return -2;
    }

    serialOld = serial;

    serial.c_ispeed = (speed_t)this->baud;
    serial.c_ospeed = (speed_t)this->baud;
    //cfsetispeed(&serial, B115200);
    //cfsetospeed(&serial, B115200);

    /*serial.c_cflag &= ~PARENB;
    serial.c_cflag &= ~CSTOPB;
    serial.c_cflag &= ~CSIZE;
    serial.c_cflag |= CS8;

    serial.c_cflag &= ~CRTSCTS;
    //serial.c_cc[CMIN] = 1;
    serial.c_cc[VMIN] = 1;
    //serial.c_cc[VTIME] = 5;
    serial.c_cc[VTIME] = 0;
    serial.c_cflag |= CREAD | CLOCAL;

    cfmakeraw(&serial);

    tcflush(this->fd, TCIFLUSH);*/
    
    serial.c_iflag &= ~( IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | IXON );
    //serial.c_iflag |= IXON;
    //serial.c_iflag |= IXOFF;
    //serial.c_iflag = 0;
    serial.c_oflag = 0;
    
    serial.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
    
    serial.c_cflag &= ~(CSIZE | PARENB);
    //serial.c_cflag |= CS8;
    serial.c_cflag = CS8 | CREAD | CLOCAL;
    
    serial.c_cc[VMIN] = 1;
    serial.c_cc[VTIME] = 0;

    int ioctlFlags = TIOCM_RTS | TIOCM_DTR;
    ioctl(this->fd, TIOCMBIS, &ioctlFlags);
    if (tcsetattr(this->fd, TCSAFLUSH, &serial) < 0) {
        ::close(this->fd);
        this->fd = -1;
        cerr << "Failed to set serial configuration." << endl;
        return -3;
    }
    
    InterfaceProtocol com = InterfaceProtocol::Invalid;
    int count;
    //while ((count = ::read(this->fd, &com, sizeof(InterfaceProtocol))) < 0);
    do {
        count = ::read(this->fd, &com, sizeof(InterfaceProtocol));
        printdf("count: %d\n", count);
        printdf("incoming byte: %d\n", (uint8_t)com);
    } while (count < 0);
    if (com != InterfaceProtocol::Ready) {
        cerr << "Failed to initialize arduino interface." << endl;
        this->fd = -1;
        return -4;
    }
    else {
        ::write(this->fd, &com, sizeof(InterfaceProtocol));
        printdf("Arduino initialized successfully.\n");
    }
    

    return 1;
}

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

int ArduinoInterface::cycle() {
    this->write("Hello\n", 6);
    return 1;
}

void ArduinoInterface::setDirection(uint8_t ddr) {
    
}

void ArduinoInterface::setPort(uint8_t port) {
    
}

uint8_t ArduinoInterface::getPort() {
    InterfaceProtocol com = InterfaceProtocol::GetPort;
    ::write(this->fd, &com, sizeof(InterfaceProtocol));
    do {
        ::read(this->fd, &com, sizeof(InterfaceProtocol));
    } while (com == InterfaceProtocol::Invalid);
    
    return (uint8_t)com;
}

void ArduinoInterface::close() {
    ::close(this->fd);
    this->fd = -1;
}
