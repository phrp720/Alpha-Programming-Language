#include "VMargs.h"
#include "expression.h"
#include "Symbol_table.h"
#include "quads.h"
#include "Instructions.h"
#include <iostream>
#include <assert.h>
#include <string>

using namespace std;

extern Quads *quads;
extern InstructionTable* instruction_table;

VMargs::VMargs() {
    this->type=invalid_a;
    this->value=0;
}
VMargs::VMargs(expr* e) {
    switch (e->type) {
        case var_e:
        case tableitem_e:
        case arithexpr_e:
        case assignexpr_e:
        case boolexpr_e:
        case newtable_e: {
            assert(e->sym);
            this->value = e->sym->offset;

            switch (e->sym->space_t) {
                case programVar:
                    this->type = global_a;
                    break;
                case functionLocal:
                    this->type = local_a;
                    break;
                case formalArg:
                    this->type = formal_a;
                    break;
                default:
                    assert(0);
            }
            break;
        }

        case constbool_e: {
            this->value = e->boolConst;
            this->type = bool_a;
            break;
        }

        case conststring_e: {
            this->value = instruction_table->newStringConst( e->strConst );
            this->type = string_a;
            break;
        }

        case constint_e: {
            this->value = instruction_table->newIntConst(e->intConst );
            this->type = number_a;
            break;
        }

        case constdouble_e: {
            this->value = instruction_table->newDoubleConst(e->numConst );
            this->type = number_a;
            break;
        }

        case nil_e:{
            this->type = nil_a;
            break;
        }

        case programfunc_e:{
            this->type = userfunc_a;
            this->value = quads->quads[e->sym->functionAddress].tadress;
            break;
        }

        case libraryfunc_e: {
            this->type = libfunc_a;
            this->value = instruction_table->newLibFuncsUsed( e->name );
            break;
        }

        default:
            assert(0);
    }
}

/*
 * isws edw theme functions akoma
 * */
string VMargs::toString() {
    return this->vmargsToString[type] + " (" + std::to_string(this->value) + ")";
}