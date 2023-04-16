#include "AVM.h"
#include "AVM_Table.h"
#include "error_avm.h"
#include <regex>
#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <vector>

using namespace std;

void AVM::libfuncPrint() {
    unsigned n = getTotalActuals();
    for (unsigned i = 0; i < n; ++i) {
        std::string s = getActual(i).toString();
        std::cout << s;
    }
    RETVAL.clear();
    RETVAL.type = nil_m;
}

void AVM::libfuncTypeOf(){
    unsigned n = getTotalActuals();

    if(n != 1){
        avm_error("one argument (not %d) expected in 'typeof'!", n);
    } else{
        RETVAL.clear();
        RETVAL.type=string_m;
        RETVAL.data.strVal = strdup(getActual(0).type_toString().c_str());
    }
}

void AVM::libfuncTotalArguments(){
    unsigned prev_topSP = getEnvValue(TOPSP - AVM_SAVEDTOPSP_OFFSET);
    unsigned n = getTotalActuals();

    if(n != 0){
        avm_error("one argument (not %d) expected in 'totalargument'!\", n");}
    if(prev_topSP == 0){
        avm_error("'totalarguments' called outside a function!");
        RETVAL.clear();
        RETVAL.type=nil_m;
    } else {
        RETVAL.type=number_m;
        RETVAL.data.numVal = getEnvValue(prev_topSP - AVM_NUM_ACTUALS_OFFSET);
    }
}

void AVM::libfuncArgument(){
    unsigned prev_topSP = getEnvValue(TOPSP - AVM_SAVEDTOPSP_OFFSET);
    unsigned n = getTotalActuals();

    if(n != 1){
        avm_error("one argument (not %d) expected in 'argument'!", n);
    } else if(prev_topSP == 0){
        RETVAL.clear();
        RETVAL.type=nil_m;
    } else {
        unsigned numOfArgs = getEnvValue(prev_topSP - AVM_NUM_ACTUALS_OFFSET);
        AVM_Memcell& arg = getActual(0);
        if(arg.type != number_m) {
            avm_error("argument %s is not a number", arg.toString().c_str());
        } else if(arg.data.numVal >= numOfArgs){
            avm_error("argument is out of index");
        } else{
            //get the argument
            RETVAL.clear();
            RETVAL.assign(&stack[prev_topSP - AVM_STACKENV_SIZE - 1 - arg.data.numVal]);
        }
    }
}


void AVM::libfuncSqrt(){
    unsigned n = getTotalActuals();
    AVM_Memcell& arg = getActual(0);

    if(n != 1) {
        avm_error("one argument (not %d) expected in 'sqrt'!", n);
    } else if(arg.type != number_m){
        RETVAL.clear();
        RETVAL.type=nil_m;
        RETVAL.type = nil_m;
    }
    else if(arg.data.numVal < 0){
        RETVAL.clear();
        RETVAL.type = nil_m;
    }
    else{
        RETVAL.clear();
        RETVAL.type = nil_m;
        RETVAL.data.numVal = sqrt(arg.data.numVal);
    }
}

void AVM::libfuncCos(){
    unsigned n = getTotalActuals();
    AVM_Memcell& arg = getActual(0);

    if(n != 1) {
        avm_error("one argument (not %d) expected in 'cos'!", n);
    }
    else if(arg.type != number_m){
        RETVAL.clear();
        RETVAL.type=nil_m;
    }
    else{
        RETVAL.clear();
        RETVAL.type=number_m;
        RETVAL.data.numVal = cos(arg.data.numVal * 3.14159265 / 180.0);
    }
}

void AVM::libfuncSin(){
    unsigned n = getTotalActuals();
    AVM_Memcell& arg = getActual(0);

    if(n != 1) {
        avm_error("one argument (not %d) expected in 'sin'!", n);
    }
    else if(arg.type != number_m){
        RETVAL.clear();
        RETVAL.type = nil_m;
    }
    else{
        RETVAL.clear();
        RETVAL.type = number_m;
        RETVAL.data.numVal = sin(arg.data.numVal * 3.14159265 / 180.0);
    }
}

void AVM::libfuncObjectTotalMembers(){
    unsigned n = getTotalActuals();
    AVM_Memcell& arg = getActual(0);

    if(n != 1){
        avm_error("one argument (not %d) expected in 'objecttotalmembers'!", n);
    } else if(arg.type != table_m){
        avm_error("argument is not a table", n);
    } else{
        RETVAL.clear();
        RETVAL.type = number_m;
        RETVAL.data.numVal = arg.data.tableVal->getTotalMembers();
    }

}
void AVM::libfuncObjectMemberKeys(){
    unsigned n = getTotalActuals();
    AVM_Memcell& arg = getActual(0);

    if(n != 1){
        avm_error("one argument (not %d) expected in 'objectmemberkeys'!", n);
       // error(Error, __currentLine__, "objectmemberkeys: Too many or too few arguments\n");
    }
    else if(arg.type!= table_m){
      //  error(Error, __currentLine__, "objectmemberkeys: Argument is not a table\n");
    }
    else{
        RETVAL.clear();
        RETVAL.type=table_m;
        RETVAL.data.tableVal = new AVM_Table();
        RETVAL.data.tableVal->incRefCounter();

        std::vector<AVM_Memcell> keys = arg.data.tableVal->getKeys();
        AVM_Memcell key;
        AVM_Memcell value;

        for(unsigned int i = 0; i < arg.data.tableVal->getTotalMembers(); i++){
            key = AVM_Memcell();
            AVM_Memcell tmp = keys[i];
            key.type=number_m;
            key.data.numVal = i;

            value.assign(&tmp);

            RETVAL.data.tableVal->setElem(&key, &value);

            if(keys[i].type == string_m || keys[i].type == libfunc_m)
                keys[i].clear();
            value.clear();
        }
    }
}
void AVM::libfuncObjectCopy() {
    unsigned n = getTotalActuals();
    AVM_Memcell &arg = getActual(0);

    if (n != 1) {
        //  error(Error, __currentLine__, "objectcopy: Too many or too few arguments\n");
    } else if (arg.type!= table_m) {
        //error(Error, __currentLine__, "objectcopy: Argument is not a table\n");
    } else {
        RETVAL.clear();
        RETVAL.type = table_m;
        RETVAL.data.tableVal = new AVM_Table();
        RETVAL.data.tableVal->incRefCounter();

        std::vector <AVM_Memcell> keys = arg.data.tableVal->getKeys();
        std::vector <AVM_Memcell> values = arg.data.tableVal->getValues();
        AVM_Memcell key;
        AVM_Memcell value;

        for (unsigned int i = 0; i < arg.data.tableVal->getTotalMembers(); i++) {
            key = keys[i];
            value = values[i];

            RETVAL.data.tableVal->setElem(&key, &value);
            if (key.type == string_m || key.type == libfunc_m)
                key.clear();
        }
    }
}

void AVM::libfuncInput() {
    std::string  str;
    cin>>str;
    int digitcount = 0;
    int dotcount = 0;
    int alphacount = 0;
    for(int i=0;i<str.size();i++) {
        if (isdigit(str[i])) {
            digitcount++;
        }
         else if (str[i] == '.') {
            dotcount++;
        } else if (isalpha(str[i])) {
            alphacount++;
        }

    }

    if (digitcount == str.size() && (dotcount == 0 ) && ( alphacount == 0) ){ //AKERAIOI
        RETVAL.type = number_m;
        RETVAL.data.numVal = stoi( str );
    }
    else if( (digitcount == str.size()) && (dotcount > 0 ) && ( alphacount == 0) ){ //DOUBLES
        RETVAL.type = number_m;
        RETVAL.data.numVal = stod( str );
    } else if( alphacount > 0 ){ //STRINGS
        RETVAL.type = string_m;
        RETVAL.data.strVal = strdup(str.c_str());
    } else if( !(str.compare("nil")) ){
        RETVAL.type = nil_m;
    } else if( !(str.compare("true"))|| !(str.compare("false")) ){
        RETVAL.type = bool_m;
        if( !(str.compare("true")) ){
            RETVAL.data.boolVal = 1;
        }
        else{
            RETVAL.data.boolVal = 0;
        }
    }
}

