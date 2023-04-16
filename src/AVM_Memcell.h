#ifndef AVM_MEMCELL_H
#define AVM_MEMCELL_H
#include <string>

using namespace std;

enum avm_memcell_t {
    number_m = 0,
    string_m,
    bool_m,
    table_m,
    userfunc_m,
    libfunc_m,
    nil_m,
    undef_m,
    nonexistent_m
};

class AVM_Table;

class AVM_Memcell {
public:
    typedef std::string (AVM_Memcell::*MemDatatoString)() ;
    MemDatatoString toStringFuncs[8] = {
            &AVM_Memcell::number_toString,
            &AVM_Memcell::string_toString,
            &AVM_Memcell::bool_toString,
            &AVM_Memcell::table_toString,
            &AVM_Memcell::userfunc_toString,
            &AVM_Memcell::libfunc_toString,
            &AVM_Memcell::nil_toString,
            &AVM_Memcell::undef_toString
    };

    string number_toString();
    string string_toString();
    string bool_toString();
    string table_toString();
    string userfunc_toString();
    string libfunc_toString();
    string nil_toString();
    string undef_toString();

    avm_memcell_t type;
    unsigned stackIndex;
    union {
        double      numVal;
        char*       strVal;
        bool        boolVal;
        AVM_Table*  tableVal;
        unsigned    funcVal;
        char*       libfuncVal;
    } data;
    AVM_Memcell();
    //for the stack
    AVM_Memcell(size_t);
    AVM_Memcell(avm_memcell_t);
    void assign(AVM_Memcell *other);
    string toString();
    string type_toString();
    void clear();
};

#endif //AVM_MEMCELL_H
