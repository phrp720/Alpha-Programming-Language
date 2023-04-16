#include <string>
#include "expression_methods.h"

extern Temp_fun *tempFun;
extern Quads *quads;
extern int yyerror (string yaccProvidedMessage);
extern int mynumLine;

expr * arithmetic_op_Expr(iopcode op, expr* first, expr* second) {
    if (first->isArithmetic() && second->isArithmetic()) {
        expr * exprReturn = new expr();
        exprReturn->type = arithexpr_e;

        if (first->isTmp()) {
            exprReturn->sym = first->sym;
            exprReturn->name = first->sym->id;
        } else if (second->isTmp()) {
            exprReturn->sym = second->sym;
            exprReturn->name = second->sym->id;
        } else{
            exprReturn->sym = tempFun->newtemp();
            exprReturn->name = tempFun->getCurrentTempNum();
        }

        quads->emit(op, first , second, exprReturn, 0, mynumLine);

        return exprReturn;
    }

    yyerror("Invalid arithmetic operand");

    return NULL;
}

expr * relop_op_Expr(iopcode op, expr* first, expr* second) {
    expr * toReturn = new expr();
    toReturn->type=boolexpr_e;
    if(first->isTmp()){
        toReturn->sym=first->sym;
        toReturn->name=first->name;
    }else if(second->isTmp()){
        toReturn->sym=second->sym;
        toReturn->name=second->name;
    }else{
        toReturn->sym=tempFun->newtemp();
        toReturn->name=tempFun->getCurrentTempNum();
    }
    quads->emit(op, first, second, NULL, quads->nextQuadLabel()+3, mynumLine);
    quads->emit(assign, newexpr_constbool(0), NULL, toReturn, 0, mynumLine);
    quads->emit(jump, NULL, NULL, NULL, quads->nextQuadLabel()+2, mynumLine);
    quads->emit(assign, newexpr_constbool(1), NULL, toReturn, 0, mynumLine);

    return toReturn;
}
