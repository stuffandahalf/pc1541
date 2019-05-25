#include "M6502.h"

M6502::M6502(Memory *mem)  {
    this->A = 0;
    this->X = 0;
    this->Y = 0;
    this->SP = 0;
    this->PC.W = 0xFFFC;    // Reset vector
    this->P = 32;   // All flags 0 except for unused flag
    
    this->mem = mem;
    
    this->counter = 0;
    this->opcode = 0;
}

/*void M6502::init() {
    
}*/

void M6502::step() {
    if (this->counter == 0) {
        //opcode
    }
}
