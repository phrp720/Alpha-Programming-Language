#include "AVM.h"
#include "AVM_Table.h"
#include <cassert>
#include "error_avm.h"

extern unsigned currentLine;

void AVM::executeNewtable(Instruction *inst){
    VMargs result = inst->result;

    AVM_Memcell *lv = translateOperand(&result, (AVM_Memcell *) 0);

    assert(lv && ((lv->stackIndex >= 0 && lv->stackIndex< TOP) || lv == &RETVAL));

    lv->clear();

    lv->type=table_m;
    lv->data.tableVal = new AVM_Table();
    lv->data.tableVal->incRefCounter();
}

void AVM::executeTablegetelem(Instruction *inst){
    VMargs arg1 = inst->arg1;
    VMargs arg2 = inst->arg2;
    VMargs result = inst->result;

    AVM_Memcell *lv = translateOperand(&result, (AVM_Memcell *) 0);
    AVM_Memcell *t = translateOperand(&arg1, (AVM_Memcell *) 0);
    AVM_Memcell *i = translateOperand(&arg2, &AX);

    assert(lv && ((lv->stackIndex >= 0 && lv->stackIndex< TOP) || lv == &RETVAL));
    assert(t && (t->stackIndex >= 0 && t->stackIndex< TOP));
    assert(i);

    lv->clear();
    lv->type=nil_m;

    if(t->type != avm_memcell_t::table_m){
        avm_error("Illegal use of type as table\n"); // TODO
    } else{
        AVM_Memcell content = t->data.tableVal->getElem(i);

        if(content.type != nil_m) {
            lv->assign(&content);
        }
        else {
            avm_error("%s[%s] not found!", t->toString(), i->toString());
        }
    }
}

void AVM::executeTablesetelem(Instruction *inst){
    VMargs arg1 = inst->arg1;
    VMargs arg2 = inst->arg2;
    VMargs result = inst->result;

    AVM_Memcell *t = translateOperand(&result, (AVM_Memcell *) 0);
    AVM_Memcell *i = translateOperand(&arg1, &AX);
    AVM_Memcell *c = translateOperand(&arg2, &BX);

    assert(t && (t->stackIndex >= 0 && t->stackIndex< TOP));
    assert(i && c);

    if(t->type != avm_memcell_t::table_m){
        avm_error("Illegal use of type as table\n"); // TODO
    } else {
        t->data.tableVal->setElem(i, c);
    }
}