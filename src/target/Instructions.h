#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <string>
#include <vector>
#include "quads.h"
#include "VMargs.h"

using namespace std;

void generate(VMOpcode, quad&);

void generate_ADD(quad&);
void generate_SUB(quad&);
void generate_MUL(quad&);
void generate_DIV(quad&);
void generate_MOD(quad&);
void generate_UMINUS(quad&);

void generate_NEWTABLE(quad&);
void generate_TABLEGETELEM(quad&);
void generate_TABLESETELEM(quad&);
void generate_ASSIGN(quad&);
void generate_NOP(quad&);

void generate_RELATIONAL(VMOpcode, quad&);

void generate_JUMP(quad&);
void generate_IF_EQ(quad&);
void generate_IF_NOTEQ(quad&);
void generate_IF_GREATER(quad&);
void generate_IF_GREATEREQ(quad&);
void generate_IF_LESS(quad&);
void generate_IF_LESSEQ(quad&);
void generate_NOT(quad&);
void generate_AND(quad&);
void generate_OR(quad&);
void generate_PARAM(quad&);
void generate_CALL(quad&);
void generate_GETRETVAL(quad&);
void generate_FUNCSTART(quad&);
void generate_RETURN(quad&);
void generate_FUNCEND(quad&);

typedef struct incompleteJump {
    unsigned instructionNo;
    unsigned iaddress;
} incompleteJum;

class Instruction {
public:
    VMOpcode opcode;
    VMargs result;
    VMargs arg1;
    VMargs arg2;
    unsigned l;
    Instruction(VMOpcode op, VMargs res, VMargs ar1, VMargs ar2, unsigned line) ;
    /*int jump_validator();*/
    void code_byte(ofstream& binary);
};

struct function_table{
    unsigned addr;
    unsigned locals;
    unsigned formals;
    string id;
};

class InstructionTable {
public:
    InstructionTable();
    vector<function_table> func_list;
    vector<incompleteJum> incompleteJumps;
    vector<Instruction> instructions;
    vector<string> StringConsts;
    vector<double> DoubleConsts;
    vector<int> IntConsts;
    vector<string> libFuncsUsed;
    void emit(Instruction instruction);
    void insertIncompleteJump(unsigned instructionNo, unsigned iaddress);
    unsigned newDoubleConst(double num);
    unsigned newIntConst(int num);
    unsigned newStringConst(string str);
    unsigned newLibFuncsUsed(string name);
    void patchIncJump(void);
    size_t nextInstructionLabel();
    void newUserFunction(unsigned addr,unsigned local,string id);
    void code_byte(ofstream& binary);
    void printInstructionTable();
    std::string opcodeMap[26] = {
            "assign",         "add",            "sub",
            "mul",            "div",            "mod",
            "callfunc",       "enterfunc",      "exitfunc",
            "jump",           "jeq",            "jne",
            "jgt",            "jlt",            "jge",
            "jle",            "newtable",       "tablegetelem",
            "tablesetelem",   "pusharg",        "nop_op"
    };

};
#endif //INSTRUCTIONS_H
