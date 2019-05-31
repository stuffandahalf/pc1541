#include <iostream>
#include "CBM1541.h"

using namespace std;

int main(int argc, char **argv)
{
    CBM1541 *drive = new CBM1541("firmware/dos1541");
    drive->execute();
    delete drive;
	return 0;
}
