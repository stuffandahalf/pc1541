#include <iostream>
#include "CBM1541.h"

#ifdef _GNU_SOURCE
#include <getopt.h>
#else
#include <unistd.h>
#endif

using namespace std;

struct config {
    std::string *firmwarePath;
    std::string *devPath;
    int baud;
};

int configure(int argc, char **argv, struct config *cfg);

int main(int argc, char **argv)
{
    struct config cfg {
        .firmwarePath = nullptr,
        .devPath = nullptr,
        .baud = 115200
    };
    if (configure(argc, argv, &cfg) < 0) {
        std::cerr << "Failed to configure pc1541" << std::endl;
        return 1;
    }
    
    CBM1541 *drive = new CBM1541("../firmware/dos1541");
    drive->execute();
    delete drive;
	return 0;
}

int configure(int argc, char **argv, struct config *cfg) {
    int opt;
    
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
            std::cout << "device: " << optarg << std::endl;
            break;
        case 'b':
            std::cout << "baud: " << optarg << std::endl;
            break;
        case 'f':
            std::cout << "firmware: " << optarg << std::endl;
            break;
        default:
            return -1;
        }
    }
    
    if (cfg->devPath == nullptr || cfg->firmwarePath == nullptr) {
        return -2;
    }
    
    return 1;
}
