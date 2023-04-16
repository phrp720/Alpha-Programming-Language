#include <string>
#include "tempfunctions.h"

using namespace std;

extern Symbol_table * symbol_table;
extern int scope;
extern CurrentsScopeOffset * offsetCount;
unsigned int tmp_var_counter = 0;
extern int mynumLine;

Temp_fun::Temp_fun() {
    current_temp_num = -1;
}

string Temp_fun::newTempName() {
    return "_t" + std::to_string(current_temp_num++);
}
void Temp_fun::resetTemp() {
    current_temp_num = -1;
}

Node* Temp_fun::newtemp() {
    string tmp_name = newTempName();
	Node * node;
    node = symbol_table->lookup(tmp_name, scope);

	if (node == NULL) {
        symbol_table->insert(
            tmp_name,
            mynumLine,
            (scope == 0) ? (Symbol_Type::global_variable): (Symbol_Type::local_variable),
            scope,
            offsetCount->currentScopeSpace(),
            offsetCount->currScopeOffset()
        );
		return symbol_table->lookup(tmp_name, scope);
	}

    return node;
}

string Temp_fun::getCurrentTempNum() {
    return "_t" + std::to_string(current_temp_num);
}

