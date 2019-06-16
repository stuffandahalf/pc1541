#include <iostream>
#include <fstream>
#include "CBM1541.h"
#include "types.h"

#ifdef _GNU_SOURCE
#include <getopt.h>
#else
#include <unistd.h>
#endif

using namespace std;

int configure(int argc, char **argv, struct config *cfg);

int main(int argc, char **argv)
{
    struct config cfg {
        .firmware = {
            .size = 0,
            .data = nullptr
        },
        .devPath = nullptr,
        .baud = 115200
    };
    if (configure(argc, argv, &cfg) < 0) {
        std::cerr << "Failed to configure pc1541" << std::endl;
        if (cfg.devPath != nullptr) {
            delete cfg.devPath;
        }
        if (cfg.firmware.data != nullptr) {
            delete[] cfg.firmware.data;
        }
        return 1;
    }
    
    CBM1541 *drive = new CBM1541(cfg);
    drive->execute();
    delete drive;
	return 0;
}

int configure(int argc, char **argv, struct config *cfg) {
    int opt;
    ifstream *firmwareStream = nullptr;
    
#ifdef _GNU_SOURCE
    struct option long_opts[] = {
        { "device", required_argument, 0, 'd' },
        { "baud", required_argument, 0, 'b' },
        { "firmware", required_argument, 0, 'f' },
        { 0, 0, 0, 0 }
    };

    while ((opt = getopt_long(argc, argv, "d:b:f:", long_opts, 0)) != -1) {
#else
    while ((opt = getopt(argc, argv, "d:b:f:")) != -1) {
#endif
        switch (opt) {
        case 'd':
            if (cfg->devPath != nullptr) {
                cerr << "Only one device may be specified." << endl;
                return -2;
            }
            std::cout << "device: " << optarg << std::endl;
            cfg->devPath = new string(optarg);
            break;
        case 'b':
            std::cout << "baud: " << optarg << std::endl;
            //cfg->baud =
            break;
        case 'f':
            if (cfg->firmware.data != nullptr) {
                cerr << "Firmware has already been initialied." << endl;
                return -2;
            }
            std::cout << "firmware: " << optarg << std::endl;
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
        default:
            return -1;
        }
    }
    
    if (cfg->devPath == nullptr || cfg->firmware.data == nullptr) {
        return -3;
    }
    
    return 1;
}
