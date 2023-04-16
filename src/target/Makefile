GPP := g++ -std=c++11
CPPFLAGS := -Wall
FLEX := flex
BISON := bison

lexer:
	@$(FLEX) ./lexer.l

parser:
	@$(BISON)  ./parser.y

lexer.o: lexer
	@$(GPP) -g	-c ./lexer.cpp;

parser.o: parser
	@$(GPP)	-g -c ./parser.cpp

scanner.o:
	@$(GPP) -g	-c ./AlphaScanner.cpp

symboltable.o:
	@$(GPP) -g -c ./Symbol_table.cpp

quads.o:
	@$(GPP) -g -c ./quads.cpp

tempfucntions.o:
	@$(GPP) -g -c ./tempfucntions.cpp

OffsetCounter.o:
	@$(GPP) -g -c ./OffsetCounter.cpp

expression.o:
	@$(GPP) -g -c ./expression.cpp

expression_methods.o:
	@$(GPP) -g -c ./expression_methods.cpp

VMargs.o:
	@$(GPP) -g -c ./VMargs.cpp

Instructions.o:
	@$(GPP) -g -c ./Instructions.cpp

alpha: clean parser.o lexer.o scanner.o symboltable.o quads.o expression_methods.o expression.o OffsetCounter.o tempfucntions.o VMargs.o Instructions.o
	@$(GPP) -g ./main.cpp parser.o  AlphaScanner.o Symbol_table.o quads.o tempfucntions.o expression_methods.o expression.o VMargs.o Instructions.o OffsetCounter.o lexer.o -o alpha_lexer

clean:
	@rm -rvf ./parser.cpp ./lexer.cpp ./parser.hpp *.o alpha_lexer
