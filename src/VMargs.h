#ifndef VMARGS_H
#define VMARGS_H

#include <string>

using namespace std;

enum VMargs_Type {
    global_a, local_a, formal_a, bool_a, string_a, number_a, nil_a, userfunc_a, libfunc_a, label_a, invalid_a,
    retval_a
};


enum VMOpcode {
    assign_op, add_op, sub_op, mul_op, div_op, mod_op, callfunc_op, enterfunc_op, exitfunc_op,
    jump_op, jeq_op, jne_op, jgt_op, jlt_op, jge_op, jle_op,
    newtable_op, tablegetelem_op, tablesetelem_op,
    pusharg_op, nop_op, and_op, or_op, not_op
};

class VMargs {
public:
    VMargs();
    VMargs(VMargs_Type t, unsigned int v): type(t), value(v) {};
    unsigned int value;
    VMargs_Type type;
    string toString();
};
#endif // VMARGS_H
