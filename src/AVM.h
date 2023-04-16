#ifndef AVM_H
#define AVM_H
#include <cassert>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include "Instruction.h"
#include "AVM_Memcell.h"


#define AVM_STACKENV_SIZE 4
#define AVM_NUMACTUALS_OFFSET 4
#define AVM_STACK_SIZE 4096
#define AVM_SAVEDTOPSP_OFFSET 1
#define AVM_SAVED_TOP_OFFSET 2
#define AVM_SAVED_PC_OFFSET 3
#define AVM_NUM_ACTUALS_OFFSET 4

using namespace std;

struct userFuncInfo{
    unsigned addr;
    unsigned locals;
    unsigned formals;
    string id;
};

class AVM {
public:
    AVM(std::ifstream&);
    void executeCycle();
    bool str_check(ifstream&,char**);
    void general_inits(size_t count);
    void libfuncsinit();
    AVM_Memcell* translateOperand(VMargs* arg, AVM_Memcell* reg);
    void                                 executeAssign(Instruction *);
    //Arithmetic
    void 								 executeArithmetic(Instruction *);
    //Branches
    void                                 executeJump(Instruction *);
    void                                 executeJeq(Instruction *);
    void                                 executeJne(Instruction *);
    void                                 executeJle(Instruction *);
    void                                 executeJge(Instruction *);
    void                                 executeJlt(Instruction *);
    void                                 executeJgt(Instruction *);
    void								 executeBranch(Instruction *);
    //Functions
    void                               executeCall(Instruction *);
    void                                 executePusharg(Instruction *);
    void                                 executeFuncenter(Instruction *);
    void                                 executeFuncexit(Instruction *);
    //Tables
    void                                 executeNewtable(Instruction *);
    void                                 executeTablegetelem(Instruction *);
    void                                 executeTablesetelem(Instruction *);
    //NOP
    void                                 executeNop(Instruction *);
    //arithmetic operands
    double add_impl(double x,double y);
    double sub_impl(double x,double y);
    double mul_impl(double x,double y);
    double div_impl(double x,double y);
    double mod_impl(double x,double y );


    //Library Functions
    void                                 libfuncPrint();
    void                                 libfuncTypeOf();
    void                                 libfuncTotalArguments();
    void                                 libfuncArgument();
    void                                 libfuncStrToNum();
    void                                 libfuncSqrt();
    void                                 libfuncCos();
    void                                 libfuncSin();
    void                                 libfuncInput();
    void                                 libfuncObjectTotalMembers();
    void                                 libfuncObjectMemberKeys();
    void                                 libfuncObjectCopy();

    unsigned                             PC;
    AVM_Memcell                          RETVAL;
    unsigned                             TOP;
    unsigned                             TOPSP;
    AVM_Memcell                          CALLMEM;
    AVM_Memcell                          DATAMEM;
    AVM_Memcell                          AX;
    AVM_Memcell                          BX;
    AVM_Memcell                          CX;

    //Type Declarations
    typedef void (AVM::*LibraryFunc_t)(void);
    typedef void (AVM::*ExecuteFunc_t)(Instruction*);
    typedef double (AVM::*ArithmeticFunc_t)(double, double);
    typedef bool (AVM::*ToBoolFunc_t)(AVM_Memcell*);


    //Helpers
    bool								 AVM_toBool(AVM_Memcell*);
    void                                 callLibfunc(std::string);
    void                                 callLibfunc(unsigned);
    void                                 callSaveEnvironment();
    void                                 incStack();
    void                                 pushEnvValue(unsigned);
    userFuncInfo                         getFuncInfo(unsigned address);
    unsigned                             getEnvValue(unsigned);
    LibraryFunc_t                        getLibraryFunc(std::string id);
    unsigned                             getTotalActuals();
    AVM_Memcell&                         getActual(unsigned);
    void                                 registerLibfunc(std::string id, LibraryFunc_t implementation);
    void                                 incTop(void);

    //Boolean Convertors
    bool                                 numberToBool(AVM_Memcell*);
    bool                                 stringToBool(AVM_Memcell*);
    bool                                 boolToBool(AVM_Memcell*);
    bool                                 tableToBool(AVM_Memcell*);
    bool                                 userfuncToBool(AVM_Memcell*);
    bool                                 libfuncToBool(AVM_Memcell*);
    bool                                 nilToBool(AVM_Memcell*);
    bool                                 undefToBool(AVM_Memcell*);


    ExecuteFunc_t  executeDispatcher[21] = {
            &AVM::executeAssign,
            &AVM::executeArithmetic,
            &AVM::executeArithmetic,
            &AVM::executeArithmetic,
            &AVM::executeArithmetic,
            &AVM::executeArithmetic,
            &AVM::executeCall,
            &AVM::executeFuncenter,
            &AVM::executeFuncexit,
            &AVM::executeJump,
            &AVM::executeJeq,
            &AVM::executeJne,
            &AVM::executeJgt,
            &AVM::executeJlt,
            &AVM::executeJge,
            &AVM::executeJle,
            &AVM::executeNewtable,
            &AVM::executeTablegetelem,
            &AVM::executeTablesetelem,
            &AVM::executePusharg,
            &AVM::executeNop
    };

    ArithmeticFunc_t arithmeticDispatcher[5] = {
            &AVM::add_impl,
            &AVM::sub_impl,
            &AVM::mul_impl,
            &AVM::div_impl,
            &AVM::mod_impl
    };

    ToBoolFunc_t toBoolDispatcher[8] = {
            &AVM::numberToBool,
            &AVM::stringToBool,
            &AVM::boolToBool,
            &AVM::tableToBool,
            &AVM::userfuncToBool,
            &AVM::libfuncToBool,
            &AVM::nilToBool,
            &AVM::undefToBool
    };

    vector<string> strConsts;
    vector<double > numConsts;
    vector<int> intConsts;
    vector<string> libFuncsUsed;
    unsigned codeSize = 0;
    unsigned pc = 0;
    vector<Instruction> code;
    map<unsigned,struct userFuncInfo> addressToFuncInfo;
    map<string, LibraryFunc_t> libfuncNameToCode;
    vector<AVM_Memcell> stack;
    unsigned totalActuals;
};

#endif //AVM_H
