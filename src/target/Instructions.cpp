#include <fstream>
#include <string>
#include <iostream>
#include "Instructions.h"
#include "expression.h"
#include "quads.h"
#include "Symbol_table.h"

using namespace std;

extern Quads *quads;
extern InstructionTable* instruction_table;
extern Symbol_table *symbol_table;

InstructionTable::InstructionTable() {}
Instruction::Instruction(VMOpcode op, VMargs res, VMargs ar1, VMargs ar2, unsigned int line) :  opcode(op),result(res),arg1(ar1),arg2(ar2),l(line){}

unsigned int this_instruction = 0;

void InstructionTable::emit(Instruction instruction) {
    this->instructions.push_back(instruction);
}

void InstructionTable::insertIncompleteJump(unsigned instructionNo, unsigned iaddress){
    incompleteJum  tmp;
    tmp.instructionNo = instructionNo;
    tmp.iaddress = iaddress;
    incompleteJumps.push_back(tmp);
}
unsigned int InstructionTable::newStringConst(string str) {
    for (size_t i = 0; i < this->StringConsts.size(); i++) {
        if (this->StringConsts[i] == str) return i;
    }
    this->StringConsts.push_back(str);
    return this->StringConsts.size() - 1;
}

unsigned int InstructionTable::newIntConst(int num) {
    for (size_t i = 0; i < this->DoubleConsts.size(); i++) {
        if (this->DoubleConsts[i] == num) return i;
    }
    this->DoubleConsts.push_back((double) num);
    return this->DoubleConsts.size() - 1;
}

unsigned int InstructionTable::newDoubleConst(double num) {
    for (size_t i = 0; i < this->DoubleConsts.size(); i++) {
        if (this->DoubleConsts[i] == num) return i;
    }
    this->DoubleConsts.push_back(num);
    return this->DoubleConsts.size() - 1;
}

unsigned int InstructionTable::newLibFuncsUsed(string name) {
    for (size_t i = 0; i < this->libFuncsUsed.size(); i++) {
        if (this->libFuncsUsed[i] == name) return i;
    }
    this->libFuncsUsed.push_back(name);
    return this->libFuncsUsed.size() - 1;
}

void InstructionTable::patchIncJump() {
    for (int i = 0; i < incompleteJumps.size(); i++) {
        if (incompleteJumps[i].iaddress == quads->quads.size() + 1) {
            instructions[incompleteJumps[i].instructionNo - 1].result.type=label_a;
            instructions[incompleteJumps[i].instructionNo - 1].result.value = instructions.size()+1;
        } else {
            instructions[incompleteJumps[i].instructionNo - 1].result.type=label_a;
            instructions[incompleteJumps[i].instructionNo - 1].result.value = quads->quads[incompleteJumps[i].iaddress].tadress;
        }
    }
}

void InstructionTable::printInstructionTable() {
    cout << " Library Function names " << endl;

    for(size_t i  = 0; i < this->libFuncsUsed.size(); i++){
        cout << " \"" + this->libFuncsUsed[i] + "\" at index " + to_string(i) << endl;
    }

    cout << endl;

    cout << " String constants " << endl;

    for(size_t i  = 0; i < this->StringConsts.size(); i++){
        cout << " \"" + this->StringConsts[i] + "\" at index " + to_string(i) << endl;
    }

    cout << endl;


    cout << " Number constants " << endl;

    for(size_t i  = 0; i < this->DoubleConsts.size(); i++){
        cout << " \"" + to_string(this->DoubleConsts[i]) + "\" at index " + to_string(i) << endl;
    }

    cout << endl;

    cout << " Target Code " << endl;

    cout << std::left << std::setw(15) << "Instruction#" << std::setw(15) << "opcode" <<
         std::setw(15) << "arg1" << std::setw(15) <<  "arg2" << std::setw(15) << "result" << std::setw(15) << "line" << std::endl;

    for (size_t i = 0; i < this->instructions.size(); i++) {
        cout << std::left << std::setw(15) << std::to_string(i + 1) + ":" << std::setw(15) << this->opcodeMap[this->instructions[i].opcode] <<
            std::setw(15) << std::setw(15) << this->instructions[i].arg1.toString() <<
            std::setw(15) << std::setw(15) << this->instructions[i].arg2.toString() << std::setw(15) << this->instructions[i].result.toString() << "[ line " <<this->instructions[i].l  << " ]" << std::endl;
    }
}

size_t InstructionTable::nextInstructionLabel() {
    return this->instructions.size() + 1;
}

void InstructionTable::newUserFunction(unsigned int addr, unsigned int local, string id) {
    struct function_table func;
    func.addr = addr;
    func.locals = local;
    func.id = id;
    func_list.push_back(func);
}

/*
 * Etoimo to export byte code,etsi prepei na einai toylaxiston
 */
void InstructionTable::code_byte(ofstream &binary) {
    unsigned int magikotatos_arithmos = 340200501;
    size_t size_;
    binary.write((char *) &magikotatos_arithmos, sizeof(unsigned int));
    size_ = StringConsts.size();
    binary.write((char *) &size_, sizeof(size_t));
    for (int i = 0; i < StringConsts.size(); i++) {
        size_ = StringConsts[i].size() + 1;
        binary.write((char*) &size_, sizeof(size_t));
        binary.write(StringConsts[i].c_str(), StringConsts[i].size() + 1);
    }
    size_ = DoubleConsts.size();
    binary.write((char *) &size_, sizeof(size_t));

    for (size_t i = 0; i < DoubleConsts.size(); i++) {
        binary.write((char *) &DoubleConsts[i], sizeof(double));
    }

    size_ = func_list.size();
    binary.write((char *) &size_, sizeof(size_t));

    for (size_t i = 0; i < func_list.size(); i++) {
        binary.write((char *) &func_list[i].addr, sizeof(unsigned));
        binary.write((char *) &func_list[i].locals, sizeof(unsigned));
        binary.write((char *) &func_list[i].formals, sizeof(unsigned));
        size_=func_list[i].id.size()+1;
        binary.write((char*)&size_,sizeof(size_t));
        binary.write((char *) func_list[i].id.c_str(), func_list[i].id.size() + 1);
    }

    size_ = libFuncsUsed.size();
    binary.write((char *) &size_, sizeof(size_t));
    for (size_t i = 0; i < libFuncsUsed.size(); i++) {
        size_=libFuncsUsed[i].size()+1;
        binary.write((char*)&size_,sizeof(size_t));
        binary.write((char *) libFuncsUsed[i].c_str(), libFuncsUsed[i].size()+1);
    }

    size_ = instructions.size();
    binary.write((char *) &size_, sizeof(size_t));
    size_=symbol_table->global;
    binary.write((char*)&size_,sizeof(size_t));
    for (auto it = instructions.begin(); it != instructions.end(); it++) {
        uint8_t mybyte;
        unsigned myun;
        mybyte = (*it).opcode;
        binary.write((char *) &mybyte, sizeof(uint8_t));
        mybyte = (*it).result.type;
        binary.write((char *) &mybyte, sizeof(uint8_t));
        myun = (*it).result.value;
        binary.write((char *) &myun, sizeof(unsigned));
        mybyte = (*it).arg1.type;
        binary.write((char *) &mybyte, sizeof(uint8_t));
        myun = (*it).arg1.value;
        binary.write((char *) &myun, sizeof(unsigned));
        mybyte = (*it).arg2.type;
        binary.write((char *) &mybyte, sizeof(uint8_t));
        myun = (*it).arg2.value;
        binary.write((char *) &myun, sizeof(unsigned));
        binary.write((char *) &(*it).l, (sizeof(unsigned)));
    }
}


/*
 * prepei na tp doume autoooo kati den paei kalaaa
 */
void generate(VMOpcode op, quad &q) {
    VMargs ar2;
    if (q.arg2 != nullptr) {
        ar2 = VMargs(q.arg2);
    } else {
        ar2 = VMargs();
    }

    Instruction t = Instruction(op, VMargs(q.result), VMargs(q.arg1), ar2, q.line);
    q.tadress = instruction_table->nextInstructionLabel();
    instruction_table->emit(t);
}

void generate_ADD(quad &q) {
    generate(add_op, q);
}

void generate_SUB(quad &q) {
    generate(sub_op, q);
}

void generate_MUL(quad &q) {
    generate(mul_op, q);
}

void generate_DIV(quad &q) {
    generate(div_op, q);
}

void generate_MOD(quad &q) {
    generate(mod_op, q);
}

void generate_UMINUS(quad &q) {
    q.op = mul;
    q.arg2 = newexpr_constint(-1);
    generate(mul_op, q);
}

void generate_NEWTABLE(quad &q) {
    generate(VMOpcode::newtable_op, q);
}

void generate_TABLEGETELEM(quad &q) {
    generate(tablegetelem_op, q);
}

void generate_TABLESETELEM(quad &q) {
    generate(tablesetelem_op, q);
}

void generate_ASSIGN(quad &q) {
    generate(assign_op, q);
}

void generate_NOT(quad &q) {
    q.tadress = instruction_table->nextInstructionLabel();

    VMargs t1res = VMargs();
    VMargs t1arg2 = VMargs();
    t1arg2.type = bool_a;
    t1arg2.value = 0;
    t1res.type = label_a;
    t1res.value = instruction_table->nextInstructionLabel() + 3;
    Instruction t1(jeq_op, t1res, VMargs(q.arg1), t1arg2, q.line);

    instruction_table->emit(t1);

    VMargs t2arg1 = VMargs();
    t2arg1.type = bool_a;
    t2arg1.value = 0;
    Instruction t2(assign_op, VMargs(q.result), t2arg1, VMargs(), q.line);

    instruction_table->emit(t2);

    VMargs t3res = VMargs();
    t3res.value = instruction_table->nextInstructionLabel() + 2;
    t3res.type = label_a;
    Instruction t3(jump_op, t3res, VMargs(), VMargs(), q.line);

    instruction_table->emit(t3);

    VMargs t4arg1 = VMargs();
    t4arg1.type = bool_a;
    t4arg1.value = 1;
    Instruction t4(assign_op, VMargs(q.result), t4arg1, VMargs(), q.line);

    instruction_table->emit(t4);
}

void generate_OR(quad &q) {
    q.tadress = instruction_table->nextInstructionLabel();

    VMargs tres = VMargs();
    VMargs targ2 = VMargs();
    targ2.type = bool_a;
    targ2.value = 1;
    tres.type = label_a;
    tres.value = instruction_table->nextInstructionLabel() + 4;
    Instruction t(jeq_op, tres, VMargs(q.arg1), targ2, q.line);

    instruction_table->emit(t);

    VMargs t1res = VMargs();
    VMargs t1arg2 = VMargs();
    t1arg2.type = bool_a;
    t1arg2.value = 1;
    t1res.type = label_a;
    t1res.value = instruction_table->nextInstructionLabel() + 3;
    Instruction t1(jeq_op, t1res, VMargs(q.arg2), t1arg2, q.line);

    instruction_table->emit(t1);

    VMargs t2arg1 = VMargs();
    t2arg1.type = bool_a;
    t2arg1.value = 0;
    Instruction t2(assign_op, VMargs(q.result), t2arg1, VMargs(), q.line);

    instruction_table->emit(t2);

    VMargs t3res = VMargs();
    t3res.value = instruction_table->nextInstructionLabel() + 2;
    t3res.type = label_a;
    Instruction t3(jump_op, t3res, VMargs(), VMargs(), q.line);

    instruction_table->emit(t3);

    VMargs t4arg1 = VMargs();
    t4arg1.type = bool_a;
    t4arg1.value = 1;
    Instruction t4(assign_op, VMargs(q.result), t4arg1, VMargs(), q.line);

    instruction_table->emit(t4);
}

void generate_AND(quad &q) {
    q.tadress = instruction_table->nextInstructionLabel();

    VMargs tres = VMargs();
    VMargs targ2 = VMargs();
    targ2.type = bool_a;
    targ2.value = 0;
    tres.type = label_a;
    tres.value = instruction_table->nextInstructionLabel() + 4;
    Instruction t(jeq_op, tres, VMargs(q.arg1), targ2, q.line);

    instruction_table->emit(t);

    VMargs t1res = VMargs();
    VMargs t1arg2 = VMargs();
    t1arg2.type = bool_a;
    t1arg2.value = 0;
    t1res.type = label_a;
    t1res.value = instruction_table->nextInstructionLabel() + 3;
    Instruction t1(jeq_op, t1res, VMargs(q.arg2), t1arg2, q.line);

    instruction_table->emit(t1);

    VMargs t2arg1 = VMargs();
    t2arg1.type = bool_a;
    t2arg1.value = 1;
    Instruction t2(assign_op, VMargs(q.result), t2arg1, VMargs(), q.line);

    instruction_table->emit(t2);

    VMargs t3res = VMargs();
    t3res.value = instruction_table->nextInstructionLabel() + 2;
    t3res.type = label_a;
    Instruction t3(jump_op, t3res, VMargs(), VMargs(), q.line);

    instruction_table->emit(t3);

    VMargs t4arg1 = VMargs();
    t4arg1.type = bool_a;
    t4arg1.value = 0;
    Instruction t4(assign_op, VMargs(q.result), t4arg1, VMargs(), q.line);

    instruction_table->emit(t4);
}

void generate_NOP() {
    Instruction t(nop_op, VMargs(), VMargs(), VMargs(), 0);
    instruction_table->emit(t);
}

void generate_RELATIONAL(VMOpcode op, quad &q) {

    VMargs arg1;
    if (q.arg1) {
        arg1 = ((op == jump_op) ? VMargs() : VMargs(q.arg1));
    }
    VMargs arg2;

    if (q.arg2) {
        arg2 = ((op == jump_op) ? VMargs() : VMargs(q.arg2));
    }

    VMargs res = VMargs();
    Instruction t(op, res, arg1, arg2, q.line);
    if (q.label < quads->currentQuad) {
        res.type = label_a;
        res.value = quads->quads[q.label].tadress;
    } else {
        instruction_table->insertIncompleteJump(instruction_table->nextInstructionLabel(), q.label);
    }
    q.tadress = instruction_table->nextInstructionLabel();

    instruction_table->emit(t);
}

void generate_JUMP(quad &q) {
    generate_RELATIONAL(jump_op, q);
}

void generate_IF_EQ(quad &q) {
    generate_RELATIONAL(jeq_op, q);
}

void generate_IF_NOTEQ(quad &q) {
    generate_RELATIONAL(jne_op, q);
}

void generate_IF_GREATER(quad &q) {
    generate_RELATIONAL(jgt_op, q);
}

void generate_IF_GREATEREQ(quad &q) {
    generate_RELATIONAL(jge_op, q);
}

void generate_IF_LESS(quad &q) {
    generate_RELATIONAL(jlt_op, q);
}

void generate_IF_LESSEQ(quad &q) {
    generate_RELATIONAL(jle_op, q);
}

void generate_PARAM(quad &q) {
    q.tadress = instruction_table->nextInstructionLabel();
    Instruction t(pusharg_op,  VMargs(),  VMargs(q.arg1), VMargs(), q.line);
    instruction_table->emit(t);
}

void generate_RETURN(quad &q) {
    VMargs arg1 =  VMargs(q.result);
    VMargs res =  VMargs();

    res.type = retval_a;

    q.tadress= instruction_table->nextInstructionLabel();

    Instruction t(assign_op, res, arg1,  VMargs(), q.line);

    instruction_table->emit(t);
}

void generate_CALL(quad &q) {
    q.tadress = instruction_table->nextInstructionLabel();
    Instruction t(callfunc_op,  VMargs(),  VMargs(q.arg1),  VMargs(), q.line);
    instruction_table->emit(t);
}

void generate_GETRETVAL(quad &q) {
    q.tadress = instruction_table->nextInstructionLabel();
    VMargs res =  VMargs(q.result);
    VMargs arg1 = VMargs();
    arg1.type = retval_a;
    Instruction t(assign_op, res, arg1,  VMargs(), q.line);
    instruction_table->emit(t);
}

void generate_FUNCSTART(quad &q) {
    q.tadress = instruction_table->nextInstructionLabel();
    VMargs res = VMargs(q.result);
    Instruction t(enterfunc_op, res,  VMargs(),  VMargs(), q.line);
    instruction_table->emit(t);
    instruction_table->newUserFunction(q.result->sym->functionAddress, q.result->sym->locals,
                                  q.result->name);
}

void generate_FUNCEND(quad &q) {
    VMargs res = VMargs(q.result);

    q.tadress = instruction_table->nextInstructionLabel();

    Instruction t(exitfunc_op, res, VMargs(), VMargs(), q.line);

    instruction_table->emit(t);
}
