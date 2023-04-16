#include <string>
#include "AVM.h"
#include "AVM_Memcell.h"
#include "VMargs.h"


extern unsigned currentLine;

void AVM::executeAssign(Instruction *instruction){
    VMargs res = instruction->result;
    VMargs arg1 = instruction->arg1;
    AVM_Memcell *lValue = this->translateOperand(&res, nullptr);
    AVM_Memcell *rValue = this->translateOperand(&arg1, &AX);

    assert(lValue && ( (lValue->stackIndex >= 0 && lValue->stackIndex < TOP) || lValue == &RETVAL) );
    assert(rValue);

    lValue->assign(rValue);
    if(rValue == &RETVAL){
        RETVAL.clear();
        RETVAL.type = nil_m;
    }
}