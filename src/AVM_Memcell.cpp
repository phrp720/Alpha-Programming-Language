#include "AVM_Memcell.h"
#include "AVM_Table.h"
#include "error_avm.h"
#include <cstring>
#include <cassert>
extern unsigned currentLine;

AVM_Memcell::AVM_Memcell() {
    this->type=undef_m;
}
AVM_Memcell::AVM_Memcell(size_t mysize) {
    this->stackIndex=mysize;
    this->type=undef_m;
}
AVM_Memcell::AVM_Memcell(avm_memcell_t mytype){
    this->type=mytype;
}
void AVM_Memcell::clear() {
    if(this->type == string_m){
        free(this->data.strVal);
        this->data.strVal = nullptr;
    }
    else if(this->type == libfunc_m){
        free(this->data.libfuncVal);
        this->data.libfuncVal = nullptr;
    }
    else if(this->type == table_m){
        (this->data.tableVal)->decRefCounter();
        if(this->data.tableVal->refCounter == 0) {
            delete this->data.tableVal;
        }
        this->data.tableVal = nullptr;
    }

    this->type = undef_m;
}
string AVM_Memcell::type_toString(){
    switch(this->type){
        case number_m:
            return "number";
        case string_m:
            return "string";
        case bool_m:
            return "boolean";
        case table_m:
            return "table";
        case userfunc_m:
            return "userfunc";
        case libfunc_m:
            return "libraryfunc";
        case nil_m:
            return "nil";
        case undef_m:
            return "undefined";
        default:
            assert(0);
    }
}

void AVM_Memcell::assign(AVM_Memcell *other){
    if(this == other) {
        return;
    }
    if(this->type == table_m && other->type == table_m && this->data.tableVal == other->data.tableVal) {
        return;
    }
    if(other->type == undef_m) {
        avm_error("assign from undef content");
        return;
    }

    //take care of previous dynamic memory
    if(this->type == string_m) {
        free(this->data.strVal);
    } else if (this->type == libfunc_m) {
        free(this->data.libfuncVal);
    } else if (this->type == table_m){
        (this->data.tableVal)->decRefCounter();
        if (this->data.tableVal->refCounter == 0) {
            delete this->data.tableVal;
        }
    }

    this->type = other->type;
    this->data = other->data;

    if(this->type == string_m) {
        this->data.strVal = strdup(other->data.strVal);
    }

    if(this->type == libfunc_m) {
        this->data.libfuncVal = strdup(other->data.libfuncVal);
    } else if(this->type == table_m) {
        (this->data.tableVal)->incRefCounter();
    }

}

std::string AVM_Memcell::number_toString() {
    return to_string(this->data.numVal);
}

std::string AVM_Memcell::string_toString(){
    return std::string(this->data.strVal);
}

std::string AVM_Memcell::bool_toString(){
    return (this->data.boolVal ? "true" : "false");
}

std::string AVM_Memcell::table_toString(){
    return this->data.tableVal->toString();
}

std::string AVM_Memcell::userfunc_toString(){
    return "user function " + std::to_string(this->data.funcVal);
}

std::string AVM_Memcell::libfunc_toString(){
    return "library function " + std::string(this->data.libfuncVal);
}

std::string AVM_Memcell::nil_toString(){
    return "nil";
}

std::string AVM_Memcell::undef_toString(){
    return "undefined object";
}

std::string AVM_Memcell::toString(){
    return (this->*toStringFuncs[type])();
}