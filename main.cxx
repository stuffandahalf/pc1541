#include <iostream>
#include <fstream>
#include <cstddef>
#include <cstdint>
#include "CBM1541.h"

using namespace std;

int main(int argc, char **argv)
{
    ifstream firmwareFile;
    firmwareFile.open("firmware/dos1541");
    
    firmwareFile.seekg(0, firmwareFile.end);
    std::size_t firmwareSize = firmwareFile.tellg();
    firmwareFile.seekg(0, firmwareFile.beg);
    
    cout << firmwareSize << endl;
    
    uint8_t *firmware = new uint8_t[firmwareSize];
    firmwareFile.read((char *)firmware, firmwareSize);
    
    CBM1541 *drive = new CBM1541();
    
    delete[] firmware;
    delete drive;
	return 0;
}
