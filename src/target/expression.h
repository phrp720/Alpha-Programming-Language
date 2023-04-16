#ifndef EXPRESSION_H
#define EXPRESSION_H
#include <string>
#include <vector>
#include <list>
#include <utility>
#include "quads.h"
#include "expression.h"
#include "Symbol_table.h"
#include "tempfunctions.h"


using namespace std;

enum expr_t {
	var_e,
	tableitem_e,
	programfunc_e,
	libraryfunc_e,
	arithexpr_e,
	boolexpr_e,
	assignexpr_e,
	newtable_e,
	constint_e,
	constdouble_e,
	constbool_e,
	conststring_e,
	nil_e
};

class expr {
    public:
        expr();
        string toString();
        string name;
        bool isTmp();
        expr_t type;
        Node 	*sym;
        expr    *index;
        int 	intConst;
        double	numConst;
        string 	strConst;
        bool boolConst;
        vector<unsigned> trueList;
        vector<unsigned> falseList;
        void backpatch(bool list, unsigned label);
		bool isArithmetic();
};


class Socaller {
    public:
        list<expr*>* elist;
        unsigned char method;
        string name;
};
class for_things{
    public:
        unsigned  test;
        unsigned enter;
};

expr * create_lvalue_expr(std::string name, Node * sym);
expr * newexpr_constdouble(double i);
expr * newexpr_constint(int i);
expr * newexpr_conststring(string str);
expr * newexpr_constnil();
expr * newexpr_constbool(bool value);
expr * make_call(expr* lv,list<expr *>* elist);
expr* emit_iftableitem(expr* exp);
void  check_arith(expr* e);
expr* member_item (expr* lv, string name);

#endif
