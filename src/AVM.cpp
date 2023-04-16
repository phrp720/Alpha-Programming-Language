#include <fstream>
#include <assert.h>
#include <stack>
#include <cstring>
#include <map>
#include "AVM.h"
#include "error_avm.h"

using namespace std;

unsigned currentLine = 0;
bool executionFinished = false;

//Diplo pointer gia dereference meta
bool AVM::str_check(ifstream& program,char** str){
    size_t check;
    if (!program.read((char *)&check, sizeof(size_t))){
        avm_error("Error ,Bad input file format");
        return 0;
    }
    if (!(*str != nullptr && check <= strlen(*str)+1)) {
        if (*str != nullptr) delete[] *str;
        *str = new char[check];
    }
    if (!program.read(*str,check)) {
        avm_error("Error ,Bad input file format");
        return 0;
    }
    return 1;
}

void AVM::general_inits(size_t count){
    PC=1;
    TOP=count;
    TOPSP=0;
    for(size_t i=0;i<AVM_STACK_SIZE;i++){
        stack.push_back(AVM_Memcell(i));
    }
}

void AVM::libfuncsinit() {
    registerLibfunc("print", &AVM::libfuncPrint);
    registerLibfunc("input", &AVM::libfuncInput);
    registerLibfunc("objectmemberkeys",&AVM::libfuncObjectMemberKeys);
    registerLibfunc("objecttotalmembers",&AVM::libfuncObjectTotalMembers);
    registerLibfunc("objectcopy",&AVM::libfuncObjectCopy);
    registerLibfunc("totalarguments",&AVM::libfuncTotalArguments);
    registerLibfunc("argument",&AVM::libfuncArgument);
    registerLibfunc("typeof",&AVM::libfuncTypeOf);
    /*registerLibfunc("strtonum",&AVM::);*/
    registerLibfunc("sqrt",&AVM::libfuncSqrt);
    registerLibfunc("cos",&AVM::libfuncCos);
    registerLibfunc("sin",&AVM::libfuncSin);
}

AVM::AVM(std::ifstream& program) {
    unsigned int magic_number;
    program.read((char *) &magic_number, sizeof(unsigned int));
    if (magic_number != 340200501) {
        avm_error("Not a valid executable");
        return;
    }
    size_t num;
    if (!program.read((char *) &num, sizeof(size_t))) {
        printf("bad file format");
        avm_error("Bad file format");
        return;
    }
    char* str=nullptr;
    for(size_t i=0;i<num;i++){
        if(str_check(program,&str)) {
            strConsts.push_back(str);
        } else {
            return;
        }
    }
   /* if (!program.read((char *) &num, sizeof(size_t))) {
        avm_error("Bad file format");
        return;
    }
    int intnum;
    for (size_t i = 0; i < num; i++) {
        if (!program.read((char*)&intnum, sizeof(int))) {
            avm_error("Bad file format");
            return;
        }
        numConsts.push_back( (double) intnum);
    }*/
    if (!program.read((char *) &num, sizeof(size_t))) {
        avm_error("Bad file format");
        return;
    }
    double diplotatos_arithmotatos;
    for (size_t i = 0; i < num; i++) {
        if (!program.read((char*)&diplotatos_arithmotatos, sizeof(double))) {
            avm_error("Bad file format");
            return;
        }
        numConsts.push_back(diplotatos_arithmotatos);
    }
    if (!program.read((char *) &num, sizeof(size_t))) {
        avm_error("Bad file format");
        return;
    }

    unsigned addr,local,formal;
    for (size_t i = 0; i < num; i++) {
  //checks the validity of address locals and formalargs
        if (!program.read((char*)&addr, sizeof(unsigned))) {
            avm_error("Bad file format");
            return;
        }
        if (!program.read((char*)&local, sizeof(unsigned))) {
            avm_error("Bad file format");
            return;
        }
        if (!program.read((char *) &formal, sizeof(unsigned))) {
            avm_error("Bad file format");
            return;
        }
        if(!str_check(program,&str))return;
        addressToFuncInfo[addr] = {addr, local, formal, std::string(str)};
    }

    if (!program.read((char *) &num, sizeof(size_t))) {
        avm_error("Bad file format");
        return;
    }
    for (size_t i = 0; i < num; i++) {
        if (!str_check(program, &str)) return;
        this->libFuncsUsed.push_back(str);
    }

    if (!program.read((char *) &num, sizeof(size_t))) {
        avm_error("Bad file format");
        return;
    }
    uint8_t optype,resulttype,arg1type,arg2type;
    unsigned arg1val,arg2val,line,resultval;
    size_t count = 0;
    if (!program.read((char*)&count, sizeof(size_t))) {
        avm_error("Bad file format");
        return;
    }
    this->code.push_back( Instruction(nop_op, VMargs(), VMargs(), VMargs(), 0) );

    for (size_t i = 0; i < num; i++) {
        if (!program.read((char*)&optype, sizeof(uint8_t))) {
            avm_error("Bad file format");
            return;
        }
        if (!program.read((char*)&resulttype, sizeof(uint8_t))) {
            avm_error("Bad file format");
            return;
        }
        if (!program.read((char*)&resultval, sizeof(unsigned))) {
            avm_error("Bad file format");
            return;
        }

        if (!program.read((char*)&arg1type, sizeof(uint8_t))) {
            avm_error("Bad file format");
            return;
        }
        if (!program.read((char*)&arg1val, sizeof(unsigned))) {
            avm_error("Bad file format");
            return;
        }

        if (!program.read((char*)&arg2type, sizeof(uint8_t))) {
            avm_error("Bad file format");
            return;
        }
        if (!program.read((char*)&arg2val, sizeof(unsigned))) {
            avm_error("Bad file format");
            return;
        }

        if (!program.read((char*)&line, sizeof(unsigned))) {
            avm_error("Bad file format");
            return;
        }
        this->code.push_back(
            Instruction((VMOpcode)optype,
                        VMargs((VMargs_Type) resulttype, resultval),
                        VMargs((VMargs_Type) arg1type, arg1val),
                        VMargs((VMargs_Type) arg2type, arg2val),
                        line
            )
        );
    }
    general_inits(count);
    libfuncsinit();
}

AVM_Memcell* AVM::translateOperand(VMargs *arg, AVM_Memcell *reg) {
    switch(arg->type){
        case global_a:
            return &stack[arg->value];
        case local_a:
            return &stack[arg->value + TOPSP];
        case formal_a:
            return &stack[TOPSP - AVM_STACKENV_SIZE - 1 - arg->value];
        case retval_a:
            return &RETVAL;
        case number_a:
            reg->clear();
            reg->type = number_m;
            reg->data.numVal = numConsts[arg->value];
            return reg;
        case string_a:
            reg->clear();
            reg->type = string_m;
            reg->data.strVal = strdup(strConsts[ arg->value ].c_str());
            return reg;

        case bool_a:
            reg->clear();
            reg->type = bool_m;
            reg->data.boolVal = arg->value;
            return reg;
        case nil_a:
            reg->clear();
            reg->type = nil_m;
            return reg;
        case userfunc_a:
            reg->clear();
            reg->type = userfunc_m;
            reg->data.funcVal = arg->value;//function address stored in VMarg
            return reg;
        case libfunc_a:
            reg->clear();
            reg->type = libfunc_m;
            reg->data.libfuncVal = strdup(libFuncsUsed[ arg->value ].c_str());
            return reg;
        default:
            assert(0);
    }
}
//arithmetic operands
double AVM::add_impl(double x, double y) {
    return x+y;
}
double AVM::sub_impl(double x, double y){
    return x-y;
}
double AVM::mul_impl(double x, double y){
    return x*y;
}
double AVM::div_impl(double x, double y){
    if (y == 0) {
        printf("Division with 0 at line %d\n", currentLine);
        avm_error("Division with 0");
        return -1;
    }
    return x/y;
}
double AVM::mod_impl(double x, double y) {

    return ((unsigned )x) % ((unsigned ) y);
}

bool AVM::AVM_toBool(AVM_Memcell* cell){
    assert(cell);

    assert(cell->type >= 0 && cell->type < avm_memcell_t::undef_m);

    return (this->*toBoolDispatcher[cell->type])(cell);
}

bool AVM::numberToBool(AVM_Memcell* cell){
    return (cell->data.numVal != 0);
}

bool AVM::stringToBool(AVM_Memcell* cell){
    assert(cell->data.strVal);

    return (cell->data.strVal[0] != 0);
}

bool AVM::boolToBool(AVM_Memcell* cell){
    return cell->data.boolVal;
}

bool AVM::tableToBool(AVM_Memcell* cell){
    return true;
}

bool AVM::userfuncToBool(AVM_Memcell* cell){
    return true;
}

bool AVM::libfuncToBool(AVM_Memcell* cell){
    return true;
}

bool AVM::nilToBool(AVM_Memcell* cell){
    return false;
}

bool AVM::undefToBool(AVM_Memcell* cell){
    assert(false);

    return false;
}

unsigned AVM::getEnvValue(unsigned i) {
    assert(stack[i].type == number_m);
    unsigned val = (unsigned) stack[i].data.numVal;
    assert(stack[i].data.numVal == ((double) val));
    return val;
}

unsigned AVM::getTotalActuals() {
    return this->getEnvValue(TOPSP - AVM_NUMACTUALS_OFFSET);
}

struct userFuncInfo AVM::getFuncInfo(unsigned address){
    return addressToFuncInfo[address];
}

AVM_Memcell& AVM::getActual(unsigned i) {
    assert(i < this->getTotalActuals());
    return this->stack[TOPSP - AVM_STACKENV_SIZE - 1 - i];
}

void AVM::registerLibfunc(std::string id, LibraryFunc_t implementation) {
    libfuncNameToCode[id] = implementation;
}

void AVM::pushEnvValue(unsigned val) {
    this->stack[TOP].type = number_m;
    this->stack[TOP].data.numVal = val;
    this->incTop();
}

void AVM::callSaveEnvironment() {
    this->pushEnvValue(totalActuals);
    this->pushEnvValue(PC+1);
    this->pushEnvValue(TOP-totalActuals-2);
    this->pushEnvValue(TOPSP);
}

AVM::LibraryFunc_t AVM::getLibraryFunc(string id){
    return libfuncNameToCode[id];
}

void AVM::callLibfunc(string fName){
    LibraryFunc_t func = getLibraryFunc(fName);

    if(!func) {
        avm_error("unsupported lib func '%s' called!", fName.c_str());
    } else {
        TOPSP = TOP;
        totalActuals = 0;

        (this->*func)();

        if(!executionFinished){
            executeFuncexit((Instruction *) nullptr);
        }
    }
}

void AVM::callLibfunc(unsigned index){
    assert(index < libFuncsUsed.size());

    LibraryFunc_t f = libfuncNameToCode[libFuncsUsed[index]];

    TOPSP = TOP;
    totalActuals = 0;

    (this->*f)();

    if(!executionFinished){
        executeFuncexit((Instruction *) nullptr);
    }
}

void AVM::incTop(void) {
    if (TOP >= AVM_STACK_SIZE) {
        avm_error("Stack overflow");
        return;
    } else {
        TOP++;
    }
}

void AVM::executeCycle() {
    while (PC < code.size()) {
        if (executionFinished) {
            return;
        }

        Instruction instruction = code[PC];
        assert(instruction.opcode >= assign_op && instruction.opcode <= nop_op);
        unsigned oldPC = PC;
        currentLine = instruction.line;
        (this->*executeDispatcher[instruction.opcode])(&instruction);
        if (PC == oldPC) ++PC;
    }
}

void AVM::executeNop(Instruction *) {}