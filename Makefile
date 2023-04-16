GPP := g++ -std=c++11
CPPFLAGS := -Wall

AVM.o:
	@$(GPP) -g -c src/AVM.cpp

instruction.o:
	@$(GPP) -g -c src/instruction.cpp

execute_tables.o:
	@$(GPP) -g -c src/execute_tables.cpp

execute_assign.o:
	@$(GPP) -g -c src/execute_assign.cpp

execute_jumps.o:
	@$(GPP) -g -c src/execute_jumps.cpp

execute_funcs.o:
	@$(GPP) -g -c src/execute_funcs.cpp

execute_arithmetic.o:
	@$(GPP) -g -c src/execute_arithmetic.cpp

AVM_libfuncs.o:
	@$(GPP) -g -c src/AVM_libfuncs.cpp

AVM_Memcell.o:
	@$(GPP) -g -c src/AVM_Memcell.cpp

AVM_Table.o:
	@$(GPP) -g -c src/AVM_Table.cpp

error_avm.o:
	@$(GPP) -g -c src/error_avm.cpp

VMargs.o:
	@$(GPP) -g -c src/VMargs.cpp

avm: clean-avm AVM.o instruction.o execute_tables.o execute_assign.o execute_jumps.o AVM_libfuncs.o execute_funcs.o execute_arithmetic.o AVM_Memcell.o AVM_Table.o VMargs.o error_avm.o
	@$(GPP) -g ./src/main.cpp AVM.o AVM_Table.o error_avm.o AVM_Memcell.o AVM_libfuncs.o instruction.o execute_tables.o execute_assign.o execute_jumps.o execute_funcs.o execute_arithmetic.o VMargs.o -o avm

test: compiler
	./src/target/alpha_lexer ./examples/example.al

compiler:
	@cd src/target/ && $(MAKE) alpha
	@cp src/target/alpha_lexer ./alpha_lexer

clean: clean-avm clean-compiler

clean-compiler:
	@cd src/target && $(MAKE) clean

clean-avm:
	@rm -rvf avm *.out *.o
