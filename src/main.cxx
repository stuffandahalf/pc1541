#include <iostream>
#include <fstream>
#include "CBMDriveEmu.h"
#include "CBM1541.h"

#ifdef _GNU_SOURCE
#include <getopt.h>
#else
#include <unistd.h>
#endif

using namespace std;

int configure(int argc, char **argv, struct config *cfg);
void printHelp();
void deleteConfig(struct config *cfg);

int main(int argc, char **argv) {
    struct config cfg {
        .firmware = {
            .size = 0,
            .data = nullptr
        },
        .devPath = nullptr,
        .baud = 115200
    };
    int configureState = configure(argc, argv, &cfg);
    if (configureState <= 0) {
        deleteConfig(&cfg);
        if (configureState < 0) {
            std::cerr << "Failed to configure pc1541" << std::endl;
            return 1;
        }
        return 0;
    }
    
    
    /*CBM1541 *drive = new CBM1541(cfg);
    if (drive->initialize() < 0) {
        delete drive;
        deleteConfig(&cfg);
        return 1;
    }
    //drive->execute();
    delete drive;*/
    
    
    ArduinoInterface *iface = new ArduinoInterface(*cfg.devPath);
    iface->open();
    //iface->write("Hello World", 11);
    iface->setDirection(0x55);
    iface->close();
    delete iface;
    
    deleteConfig(&cfg);
	return 0;
}

int configure(int argc, char **argv, struct config *cfg) {
    int opt = 0;
    char *endPtr = nullptr;
    ifstream *firmwareStream = nullptr;
    
#ifdef _GNU_SOURCE
    struct option long_opts[] = {
        { "device", required_argument, 0, 'd' },
        { "baud", required_argument, 0, 'b' },
        { "firmware", required_argument, 0, 'f' },
        { "help", no_argument, 0, 'h' },
        { 0, 0, 0, 0 }
    };

    while ((opt = getopt_long(argc, argv, "d:b:f:h", long_opts, 0)) != -1) {
#else
    while ((opt = getopt(argc, argv, "d:b:f:h")) != -1) {
#endif
        switch (opt) {
        case 'd':
            if (cfg->devPath != nullptr) {
                cerr << "Only one device may be specified." << endl;
                return -2;
            }
            //std::cout << "device: " << optarg << std::endl;
            printdf("Device: %s\n", optarg);
            cfg->devPath = new string(optarg);
            break;
        case 'b':
            //std::cout << "baud: " << optarg << std::endl;
            printdf("Baud: %s\n", optarg);
            cfg->baud = strtol(optarg, &endPtr, 0);
            if (*endPtr != '\0') {
                cerr << "Baud rate must a number in bits per second." << endl;
                return -2;
            }
            break;
        case 'f':
            if (cfg->firmware.data != nullptr) {
                cerr << "Firmware has already been initialied." << endl;
                return -2;
            }
            //std::cout << "firmware: " << optarg << std::endl;
            printdf("Firmware: %s\n", optarg);
            firmwareStream = new ifstream(optarg);
                
            if (!firmwareStream->is_open()) {
                cerr << "Failed to open firmware file " << optarg << '.' << endl;
                return -2;
            }
                
            firmwareStream->seekg(0, firmwareStream->end);
            cfg->firmware.size = firmwareStream->tellg();
            firmwareStream->seekg(0, firmwareStream->beg);
            
            cfg->firmware.data = new uint8_t[cfg->firmware.size];
            firmwareStream->read((char *)cfg->firmware.data, cfg->firmware.size);
            firmwareStream->close();
                
            delete firmwareStream;
            firmwareStream = nullptr;
            break;
        case 'h':
            printHelp();
            return 0;
            break;
        default:
            return -1;
        }
    }
    
    if (cfg->devPath == nullptr || cfg->firmware.data == nullptr) {
        return -3;
    }
    
    return 1;
}

void printHelp() {
    cout
        << "Commodore Business Machines floppy drive emulator"
        << endl
        << "Written by Gregory Norton [Gregory.Norton@me.com]"
        << endl
        << endl
        
        << "\t-d\t"
#ifdef _GNU_SOURCE
        << "--device\t"
#endif
        << "The serial port where the interface is connected."
        << endl
        
        << "\t-b\t"
#ifdef _GNU_SOURCE
        << "--baud\t"
#endif
        << "Select the baud rate of the serial connection to the interface."
        << endl
        
        << "\t-f\t"
#ifdef _GNU_SOURCE
        << "--firmware\t"
#endif
        << "Path to the original commodore drive firmware."
        << endl
        
        << "\t-h\t"
#ifdef _GNU_SOURCE
        << "--help\t"
#endif
        << "Print this help message."
        << endl;
}

void deleteConfig(struct config *cfg) {
    if (cfg->devPath != nullptr) {
        delete cfg->devPath;
    }
    if (cfg->firmware.data != nullptr) {
        delete[] cfg->firmware.data;
    }
}
