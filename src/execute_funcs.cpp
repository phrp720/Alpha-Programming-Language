#include <string>
#include "AVM.h"
#include "AVM_Memcell.h"
#include "VMargs.h"
#include "error_avm.h"
#include "AVM_Table.h"
#include <cstring>

#define AVM_NUMACTUALS_OFFSET 4
#define AVM_SAVED_PC_OFFSET 3
#define AVM_SAVED_TOP_OFFSET 2
#define AVM_SAVEDTOPSP_OFFSET 1

extern unsigned currentLine;
extern bool executionFinished;

void AVM::executeFuncenter(Instruction * instruction) {
    VMargs res = instruction->result;
    AVM_Memcell *func = translateOperand(&res, &AX);

    assert(func);
    assert(PC == func->data.funcVal);

    RETVAL.clear();

    totalActuals = 0;
    struct userFuncInfo info = getFuncInfo(PC);
    TOPSP = TOP;
    TOP=TOP+info.locals;
}

void AVM::executeFuncexit(Instruction *i){
    unsigned oldTop = TOP;
    TOP = getEnvValue(TOPSP - AVM_SAVED_TOP_OFFSET);
    PC = getEnvValue(TOPSP - AVM_SAVED_PC_OFFSET);
    TOPSP = getEnvValue(TOPSP - AVM_SAVEDTOPSP_OFFSET);

    if(RETVAL.type == undef_m) {
        RETVAL.type = nil_m;
    }

    while(--oldTop >= TOP) { //clear the stack
        stack[oldTop].clear();
    }
}

void AVM::executePusharg(Instruction * i) {
    AVM_Memcell * arg = translateOperand(&i->arg1, &AX);
    assert(arg);
    stack[TOP].assign(arg);
    ++totalActuals;
    this->incTop();
}

void AVM::executeCall(Instruction * i ) {
    AVM_Memcell* func = translateOperand(&i->arg1, &AX);
    assert(func);
    if(executionFinished) {
        return;
    }

    switch (func->type) {
        case (userfunc_m):
            if(totalActuals < addressToFuncInfo[func->data.funcVal].formals){
                std::vector<AVM_Memcell> tmpStack;
                avm_error("Function \"%s\" called with less actuals than formals; remaining arguments will be undef\n", addressToFuncInfo[func->data.funcVal].id.c_str());
                for(unsigned i = 0; i < totalActuals; i++){
                    tmpStack.push_back(stack[--TOP]);
                }
                while(totalActuals < addressToFuncInfo[func->data.funcVal].formals){
                    stack[TOP] = AVM_Memcell(TOP);
                    ++totalActuals;
                    incTop();
                }
                while( !tmpStack.empty() ){
                    stack[TOP] = tmpStack.back();
                    tmpStack.pop_back();
                    incTop();
                }
            }
            if(executionFinished)
                return;
            callSaveEnvironment();
            PC = func->data.funcVal;
            assert(PC < code.size()+1);
            assert(code[PC].opcode == enterfunc_op);
            break;

        case (string_m):
            callSaveEnvironment();
            callLibfunc(func->data.strVal);
            break;
        case (libfunc_m):
            callSaveEnvironment();
            callLibfunc(func->data.libfuncVal);
            break;
        case (table_m):{
            AVM_Memcell paren = AVM_Memcell(string_m);
            paren.data.strVal = strdup("()");
            AVM_Memcell functor;
            while( (functor = (func->data.tableVal)->getElem(&paren)).type == table_m ){
                func->assign(&functor);
            }
            switch(functor.type){
                case(userfunc_m):
                    stack[TOP].assign(func);
                    ++totalActuals;
                    incTop();
                    callSaveEnvironment();
                    PC = functor.data.funcVal;
                    assert(PC < code.size()+1);
                    assert(code[PC].opcode == enterfunc_op);
                    break;
                case (string_m):
                    stack[TOP].assign(func);
                    ++totalActuals;
                    incTop();
                    callSaveEnvironment();
                    callLibfunc(functor.data.strVal);
                    break;
                case(libfunc_m):
                    stack[TOP].assign(func);
                    ++totalActuals;
                    incTop();
                    callSaveEnvironment();
                    callLibfunc(functor.data.libfuncVal);
                    break;
                default:
                    std::string s = func->toString();
                    avm_error("'%s': not a functor\n", s.c_str());
            }
            free(paren.data.strVal);
            paren.data.strVal = nullptr;
            break;
        }
        default:
            std::string s = func->toString();
            avm_error("Cannot bind '%s' to function!\n", s.c_str());
    }
}