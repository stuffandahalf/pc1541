#include <iostream>
#include "M6502.h"
#include "Memory.h"

using namespace std;

int main(int argc, char **argv)
{
    Memory *mem = new Memory(0x10000);
    M6502 *cpu = new M6502(mem);
    
    cpu->reset();
    
    cout << *cpu << endl;
    
    /*for (int i = 0; i < 0x10000; i++) {
        (*mem)[i] = i;
        cout << (int)(*mem)[i] << endl;
    }*/
    
    delete cpu;
    delete mem;
	return 0;
}
