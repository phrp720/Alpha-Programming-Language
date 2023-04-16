#include <string>
#include <iostream>
#include <assert.h>
#include "expression.h"


using namespace std;

extern Temp_fun *tempFun;
extern Quads *quads;

extern int yyerror(std::string yaccProvidedMessage);

extern int mynumLine;

expr::expr() {};

bool expr::isTmp() {
    return this->sym != nullptr && this->sym->id[0] == '_';
}

void expr::backpatch(bool list, unsigned label) {
    if (list) {
        for (unsigned i: trueList) {
            quads->patchLabel(i, label);
        }
    } else {
        for (unsigned i: falseList) {
            quads->patchLabel(i, label);
        }
    }
}

bool expr::isArithmetic() {
    return type == var_e || type == tableitem_e || type == arithexpr_e || type == constint_e || type == constdouble_e;
}

string expr::toString() {
    string tmpString;

    if (this == NULL) {
        return "";
    }


    switch (type) {
        case constdouble_e:
            return to_string(numConst);
            break;
        case constint_e:
            return to_string(intConst);
            break;
        case constbool_e:
            tmpString = (boolConst ? "true" : "false");
            return "\'" + tmpString + "\'";
            break;
        case nil_e:
            return "nil";
            break;
        case newtable_e:
            return this->name;
            break;
    }

    return this->name;
}

expr *create_lvalue_expr(std::string name, Node *sym) {
    assert(sym);

    expr *exp = new expr();
    exp->sym = sym;

    if (sym->type == Symbol_Type::global_variable
        || sym->type == Symbol_Type::formal_argument
        || sym->type == Symbol_Type::local_variable) {
        exp->type = var_e;
    }

    if (sym->type == Symbol_Type::user_function) {
        exp->type = programfunc_e;
    }

    if (sym->type == Symbol_Type::library_function) {
        exp->type = libraryfunc_e;
    }

    exp->numConst = 0.0;
    exp->intConst = 0;
    exp->strConst = "";
    exp->name = name;

    return exp;
}

expr *newexpr_constdouble(double i) {
    expr *exp = new expr();
    exp->type = constdouble_e;
    exp->index = nullptr;
    exp->sym = nullptr;
    exp->numConst = i;
    exp->strConst = "";
    exp->name = "";

    return exp;
}

expr *newexpr_constint(int i) {
    expr *exp = new expr;
    exp->type = constint_e;
    exp->intConst = i;
    exp->sym = nullptr;
    exp->index = nullptr;
    exp->name = "";
    exp->strConst = "";

    return exp;
}

expr *newexpr_conststring(string str) {
    expr *exp = new expr();
    exp->index = nullptr;
    exp->type = conststring_e;
    exp->sym = nullptr;
    exp->numConst = 0;
    exp->intConst = 0;
    exp->strConst = str;
    exp->name = "";

    return exp;
};

expr *newexpr_constnil() {
    expr *exp = new expr();
    exp->index = nullptr;
    exp->type = nil_e;
    exp->sym = nullptr;
    exp->numConst = 0;
    exp->intConst = 0;
    exp->strConst = "";
    exp->name = "";

    return exp;
}

expr *newexpr_constbool(bool value) {
    expr *exp = new expr();
    exp->type = constbool_e;
    exp->name = "";
    exp->strConst = "";
    exp->sym = nullptr;
    exp->index = nullptr;
    exp->boolConst = value;

    return exp;
}

expr *emit_iftableitem(expr *exp) {
    if (exp->type == tableitem_e) {
        expr *result = new expr();
        Temp_fun *tmpFun = new Temp_fun();
        result->sym = tmpFun->newtemp();
        result->name = tmpFun->getCurrentTempNum();
        quads->emit(iopcode::tablegetelem, exp, exp->index, result, 0, mynumLine);
        return result;
    } else {

        return exp;
    }
}

expr *member_item(expr *lv, string name) {
    lv = emit_iftableitem(lv);
    expr *exp = new expr();
    exp->type = tableitem_e;
    exp->sym = exp->sym;
    exp->index = newexpr_conststring(name);
    return exp;
}

expr *make_call(expr *lv, list<expr *> *elist) {
    expr *func = emit_iftableitem(lv);
    expr *result = new expr();
    result->type = var_e;
    bool checkTmp = true;

    list<expr *>::iterator it;
    for (it = elist->begin(); it != elist->end(); ++it) {
        quads->emit(iopcode::param, *it, NULL, NULL, 0, mynumLine);
        if (checkTmp && (*it)->isTmp()) {
            result->sym = (*it)->sym;
            result->name = (*it)->name;
            checkTmp = false;
        }
    }
    quads->emit(iopcode::call, func, NULL, NULL, 0, mynumLine);

    if (checkTmp) {
        result->sym = tempFun->newtemp();

        result->name = tempFun->getCurrentTempNum();

    }
    quads->emit(iopcode::getretval, NULL, NULL, result, 0, mynumLine);

    return result;
}

void comperror(string format, const string context);

void check_arith(expr *e) {
    if (e->type == constbool_e ||
        e->type == conststring_e ||
        e->type == nil_e ||
        e->type == newtable_e ||
        e->type == programfunc_e ||
        e->type == libraryfunc_e ||
        e->type == boolexpr_e)
        yyerror("Illegal expr used!");
}
