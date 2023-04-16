#ifndef QUADS_H
#define QUADS_H
#include <utility>
#include <vector>
#include "expression.h"
#include "Symbol_table.h"

using namespace std;

class expr;
enum iopcode {
    assign,         add,            sub,
    mul,            div_i,          mod,
    uminus,         if_eq,          if_noteq,
    if_lesseq,      if_greatereq,   if_less,
    if_greater,     call,           param,
    ret,            getretval,      funcstart,
    funcend,        tablecreate,    jump,
    tablegetelem,   tablesetelem, and_i, or_i, not_i
};

typedef struct quad {
    iopcode      op;
    expr*        result;
    expr*        arg1;
    expr*        arg2;
    unsigned int label;
    unsigned int line;
    unsigned  tadress;
} quad;


typedef pair<vector<expr*>*, vector<pair<expr*, expr*>>*> ObjectAct;

class StmtInfo {
    public:
        vector<int> breakList, contList;
        bool isLoopStmt;
};



class Quads {
 	public:
        Quads();
        unsigned currentQuad;
		std::vector<quad> quads;
		void emit(iopcode opCode, expr *arg1, expr *arg2, expr *result, unsigned label, unsigned line);
		void printQuads();
        void patchLabel(unsigned quadNo, unsigned label);
        unsigned mergeList(unsigned list, unsigned list2);
		unsigned nextQuadLabel(void);
        void patchlist(std::vector<int> list, int label);
        void generator_();
        int newlist(int i);/*function gia ta breaks kai continue slide 26 11h dialeksh*/
		std::string opcodeMap[26] = {
			"assign",         "add",            "sub",
			"mul",            "div",          	"mod",
			"uminus",         "if_eq",          "if_noteq",
			"if_lesseq",      "if_greatereq",   "if_less",
			"if_greater",     "call",           "param",
			"return",         "getretval",      "funcstart",
			"funcend",        "tablecreate",    "jump",
			"tablegetelem",   "tablesetelem", "and_i", "or_i",
            "not_i"
		};
};


#endif
