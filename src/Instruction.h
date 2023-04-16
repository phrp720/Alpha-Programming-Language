#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "VMargs.h"

class Instruction {
public:
    Instruction(VMOpcode op, VMargs re, VMargs a1, VMargs a2, unsigned li);
    VMargs arg1;
    VMargs arg2;
    VMargs result;
    unsigned line;
    VMOpcode opcode;
};


#endif //INSTRUCTION_H
