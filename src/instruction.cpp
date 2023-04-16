#include "Instruction.h"

Instruction::Instruction(VMOpcode op, VMargs re, VMargs a1, VMargs a2, unsigned li): opcode(op), result(re), arg1(a1), arg2(a2), line(li) {

}