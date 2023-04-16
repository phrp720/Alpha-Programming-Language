#include<iostream>
#include<fstream>
#include "AlphaScanner.h"
#include"OffsetCounter.h"
#include"expression.h"
#include"Symbol_table.h"
#include"quads.h"
#include"expression_methods.h"
#include"tempfunctions.h"
#include "parser.hpp"
#include "Instructions.h"

using namespace std;

AlphaScanner * lexer;
extern Symbol_table * symbol_table;
extern Quads* quads;
extern InstructionTable* instruction_table;

int main(int argc, char** argv) {
    lexer = NULL;
    std::ifstream ifs;
    std::ofstream ofs;
    if (argc > 1) {
        ifs.open(argv[1],  std::ios::in);
        if (!ifs.is_open()) {
            return 1;
        }
        if (argc > 2) {
            ofs.open(argv[2], std::ios::out);
            if (!ofs.is_open()) {
                return 2;
            }
            lexer = new AlphaScanner(&ifs, &ofs);
        } else {
            lexer = new AlphaScanner(&ifs);
        }
    } else {
        lexer = new AlphaScanner();
    }
    yyparse();
    quads->printQuads();
    quads->generator_();
    instruction_table->patchIncJump();
    ofstream binary_abc_;
    binary_abc_.open("binary.abc");
    instruction_table->code_byte(binary_abc_);
	ifs.close();
	ofs.close();
    binary_abc_.close();
    instruction_table->printInstructionTable();
    /*symbol_table->print();*/


    return 0;
}
