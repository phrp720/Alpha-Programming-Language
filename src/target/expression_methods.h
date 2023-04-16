#ifndef EXPRESSION_METHODS_H
#define EXPRESSION_METHODS_H
#include "quads.h"
#include"expression.h"

expr * arithmetic_op_Expr(iopcode op, expr* first, expr* second);
expr * relop_op_Expr(iopcode op, expr* first, expr* second);
expr * bool_op_Expr(iopcode op, expr* first, expr* second);


#endif
