#include <cstring>
#include "AVM.h"
#include "AVM_Memcell.h"
#include "VMargs.h"
#include "error_avm.h"

/*extern unsigned VM_ERRNO;*/

extern unsigned currentLine;
extern bool executionFinished;
extern unsigned currentLine;


void AVM::executeJump(Instruction *inst){
    assert(inst->result.type == VMargs_Type::label_a);

    if(!executionFinished){
        PC = inst->result.value;
    }
}

void AVM::executeJeq(Instruction *inst){
    assert(inst->result.value == VMargs_Type::label_a);

    VMargs arg1 = inst->arg1;
    VMargs arg2 = inst->arg2;

    AVM_Memcell *rv1 = translateOperand(&arg1, &AX);
    AVM_Memcell *rv2 = translateOperand(&arg2, &BX);

    bool result = false;

    if(rv1->type == avm_memcell_t::undef_m || rv2->type == avm_memcell_t::undef_m){
        avm_error("Unknown type in comparison");
    }
    else if(rv1->type == avm_memcell_t::bool_m || rv2->type == avm_memcell_t::bool_m){
        result = (AVM_toBool(rv1) == AVM_toBool(rv2));
    }
    else if(rv1->type == avm_memcell_t::nil_m || rv2->type == avm_memcell_t::nil_m){
        result = (rv1->type == avm_memcell_t::nil_m && rv2->type == avm_memcell_t::nil_m);
    }
    else if(rv1->type != rv2->type){
        avm_error("Unknown type in comparison");
    }
    else{
        if(rv1->type == avm_memcell_t::number_m){
            result = (rv1->data.numVal == rv2->data.numVal);
        }
        else if(rv1->type == avm_memcell_t::string_m){
            result = (strcmp(rv1->data.strVal, rv2->data.strVal) == 0);
        }
        else if(rv1->type == avm_memcell_t::libfunc_m){
            result = (strcmp(rv1->data.libfuncVal, rv2->data.libfuncVal) == 0);
        }
        else if(rv1->type == avm_memcell_t::table_m){
            result = (rv1->data.tableVal == rv2->data.tableVal);
        }
        else if(rv1->type == avm_memcell_t::userfunc_m){
            result = (rv1->data.funcVal == rv2->data.funcVal);
        }
    }

    if(!executionFinished && result){
        PC = inst->result.value;
    }
}

void AVM::executeJne(Instruction *inst){
    assert(inst->result.type == VMargs_Type::label_a);

    VMargs arg1 = inst->arg1;
    VMargs arg2 = inst->arg2;

    AVM_Memcell *rv1 = translateOperand(&arg1, &AX);
    AVM_Memcell *rv2 = translateOperand(&arg2, &BX);

    bool result = false;

    if(rv1->type == avm_memcell_t::undef_m || rv2->type == avm_memcell_t::undef_m){
        avm_error("Unknown type in comparison");
    }
    else if(rv1->type == avm_memcell_t::bool_m || rv2->type == avm_memcell_t::bool_m){
        result = (AVM_toBool(rv1) != AVM_toBool(rv2));
    }
    else if(rv1->type == avm_memcell_t::nil_m || rv2->type == avm_memcell_t::nil_m){
        result = !(rv1->type == avm_memcell_t::nil_m && rv2->type == avm_memcell_t::nil_m);
    }
    else if(rv1->type != rv2->type){
        avm_error("Unknown type in comparison");
    }
    else{
        if(rv1->type == avm_memcell_t::number_m){
            result = (rv1->data.numVal != rv2->data.numVal);
        }
        else if(rv1->type == avm_memcell_t::string_m){
            result = (strcmp(rv1->data.strVal, rv2->data.strVal) != 0);
        }
        else if(rv1->type == avm_memcell_t::libfunc_m){
            result = (strcmp(rv1->data.libfuncVal, rv2->data.libfuncVal) != 0);
        }
        else if(rv1->type == avm_memcell_t::table_m){
            result = (rv1->data.tableVal != rv2->data.tableVal);
        }
        else if(rv1->type == avm_memcell_t::userfunc_m){
            result = (rv1->data.funcVal != rv2->data.funcVal);
        }
    }

    if(!executionFinished && result){
        PC = inst->result.value;
    }
}


void AVM::executeJle(Instruction *inst){
    assert(inst->result.type == VMargs_Type::label_a);

    VMargs arg1 = inst->arg1;
    VMargs arg2 = inst->arg2;

    AVM_Memcell *rv1 = translateOperand(&arg1, &AX);
    AVM_Memcell *rv2 = translateOperand(&arg2, &BX);

    bool result = false;

    if(rv1->type != avm_memcell_t::number_m || rv2->type != avm_memcell_t::number_m){
        avm_error("Unknown type in comparison");
    }
    else{
        result = (rv1->data.numVal <= rv2->data.numVal);
    }


    if(!executionFinished && result){
        PC = inst->result.value;
    }
}

void AVM::executeJge(Instruction *inst){
    assert(inst->result.type == VMargs_Type::label_a);

    VMargs arg1 = inst->arg1;
    VMargs arg2 = inst->arg2;

    AVM_Memcell *rv1 = translateOperand(&arg1, &AX);
    AVM_Memcell *rv2 = translateOperand(&arg2, &BX);

    bool result = false;

    if(rv1->type != avm_memcell_t::number_m || rv2->type != avm_memcell_t::number_m){
        avm_error("Unknown type in comparison");
    }
    else{
        result = (rv1->data.numVal >= rv2->data.numVal);
    }

    if(!executionFinished && result){
        PC = inst->result.value;
    }
}

void AVM::executeJlt(Instruction *inst){
    assert(inst->result.type == VMargs_Type::label_a);

    VMargs arg1 = inst->arg1;
    VMargs arg2 = inst->arg2;

    AVM_Memcell *rv1 = translateOperand(&arg1, &AX);
    AVM_Memcell *rv2 = translateOperand(&arg2, &BX);

    bool result = false;

    if(rv1->type != avm_memcell_t::number_m || rv2->type != avm_memcell_t::number_m) {
        avm_error("Unknown type in comparison");
    }
    else{
        result = (rv1->data.numVal < rv2->data.numVal);
    }

    if(!executionFinished && result){
        PC = inst->result.value;
    }
}

void AVM::executeJgt(Instruction *inst){
    assert(inst->result.type == VMargs_Type::label_a);

    VMargs arg1 = inst->arg1;
    VMargs arg2 = inst->arg2;

    AVM_Memcell*rv1 = translateOperand(&arg1, &AX);
    AVM_Memcell *rv2 = translateOperand(&arg2, &BX);

    bool result = false;

    if(rv1->type != avm_memcell_t::number_m || rv2->type != avm_memcell_t::number_m){
        avm_error("Unknown type in comparison");
    }
    else{
        result = (rv1->data.numVal > rv2->data.numVal);
    }

    if(!executionFinished && result){
        PC = inst->result.value;
    }
}
