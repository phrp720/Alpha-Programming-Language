#include "AVM_Memcell.h"
#include "AVM.h"
#include "error_avm.h"

extern unsigned currentLine;

void AVM::executeArithmetic(Instruction *instruction) {
    VMargs res = instruction->result;
    VMargs arg1 = instruction->arg1;
    VMargs arg2 = instruction->arg2;
    AVM_Memcell *lv = translateOperand(&res, (AVM_Memcell *) nullptr);

    AVM_Memcell *rv1 = translateOperand(&arg1, &AX);
    AVM_Memcell *rv2 = translateOperand(&arg2, &BX);

    assert(lv && ((lv->stackIndex >= 0 && lv->stackIndex < TOP) || lv == &RETVAL));
    assert(rv1 && rv2);

    if (rv1->type != number_m || rv2->type != number_m) {
        avm_error("Arithmetic operand applied to non-numerical values in line %d \n", currentLine);
    } else {
        AVM::ArithmeticFunc_t op = arithmeticDispatcher[instruction->opcode - add_op];
        lv->clear();
        lv->type = avm_memcell_t::number_m;
        lv->data.numVal = (this->*op)(rv1->data.numVal, rv2->data.numVal);
    }
}