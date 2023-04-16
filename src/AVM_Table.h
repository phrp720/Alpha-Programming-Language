#ifndef AVM_TABLE_H
#define AVM_TABLE_H

#include <vector>
#include <map>
#include "AVM_Memcell.h"
using namespace std;

class AVM_Table {
public:
    AVM_Table();
    unsigned refCounter;
    static unsigned currSerialNo;
    unsigned serialNo = 0;
    vector<AVM_Memcell> getKeys();
    vector<AVM_Memcell> getValues();
    void incRefCounter();
    void decRefCounter();
    std::map<double, AVM_Memcell> numberToValue;
    std::map<std::string, AVM_Memcell> stringToValue;
    std::map<bool, AVM_Memcell> boolToValue;
    std::map<AVM_Table *, AVM_Memcell> AVM_TableToValue;
    std::map<unsigned, AVM_Memcell> userFuncToValue;
    std::map<std::string, AVM_Memcell> libFuncToValue;
    std::string toString_Util(std::map<AVM_Table *, bool> visited);
    AVM_Memcell getElem(AVM_Memcell*);
    void setElem(AVM_Memcell* key, AVM_Memcell* value);
    std::string toString();
    unsigned int getTotalMembers();
    void clear();
};


#endif //AVM_TABLE_H
