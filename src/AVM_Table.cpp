#include <assert.h>
#include <sstream>
#include <cstring>
#include <iomanip>
#include "AVM_Table.h"
#include "error_avm.h"

extern unsigned currentLine;

unsigned currentSerialNo = 0;

using namespace std;

void AVM_Table::clear()
{
    for (auto it = this->numberToValue.begin(); it != this->numberToValue.end(); it++) {
        it->second.clear();
    }
    for (auto it = this->stringToValue.begin(); it != this->stringToValue.end(); it++) {
        it->second.clear();
    }
    for (auto it = this->boolToValue.begin(); it != this->boolToValue.end(); it++) {
        it->second.clear();
    }
    for (auto it = AVM_TableToValue.begin(); it != AVM_TableToValue.end(); it++) {
        it->second.clear();
    }
    for (auto it = this->userFuncToValue.begin(); it != this->userFuncToValue.end(); it++) {
        it->second.clear();
    }
    for (auto it = this->libFuncToValue.begin(); it != this->libFuncToValue.end(); it++) {
        it->second.clear();
    }
}

AVM_Table::AVM_Table() {
    this->refCounter = 0;
    this->serialNo = currentSerialNo++;
}

void AVM_Table::incRefCounter() {
    this->refCounter++;
}

void AVM_Table::decRefCounter() {
    assert(refCounter > 0);
    if (!--this->refCounter) {
        this->clear();
    }
}

AVM_Memcell AVM_Table::getElem(AVM_Memcell *e) {
    switch (e->type) {
        case (number_m): {
            auto n2v = numberToValue.find(e->data.numVal);
            if (n2v == numberToValue.end()) {
                return AVM_Memcell(nil_m);
            }
            return n2v->second;
        }
        case (string_m): {
            auto s2v = stringToValue.find(e->data.strVal);
            if (s2v == stringToValue.end()) {
                return AVM_Memcell(nil_m);
            }
            return s2v->second;
        }
        case(bool_m): {
            auto s2v = boolToValue.find(e->data.boolVal);
            if (s2v == boolToValue.end()) {
                return AVM_Memcell(nil_m);
            }
            return s2v->second;
        }
        case(userfunc_m): {
            auto s2v = userFuncToValue.find(e->data.funcVal);
            if (s2v == userFuncToValue.end()) {
                return AVM_Memcell(nil_m);
            }
            return s2v->second;
        }
        case(libfunc_m): {
            auto s2v = libFuncToValue.find(e->data.libfuncVal);
            if (s2v == libFuncToValue.end()) {
                return AVM_Memcell(nil_m);
            }
            return s2v->second;
        }
        case(table_m): {
            auto s2v = AVM_TableToValue.find(e->data.tableVal);
            if (s2v == AVM_TableToValue.end()) {
                return AVM_Memcell(nil_m);
            }
            return s2v->second;
        }
        default:
            avm_error("Unknown table index type");
            return AVM_Memcell(nonexistent_m);
    }
}

void AVM_Table::setElem(AVM_Memcell *key, AVM_Memcell *value)
{
    switch (key->type) {
        case (number_m): {
            auto n2v = this->numberToValue.find(key->data.numVal);
            if (n2v == this->numberToValue.end()) {
                if (value->type != nil_m) {
                    numberToValue[key->data.numVal].assign(value);
                }
            } else {
                if (value->type != nil_m) {
                    n2v->second.clear();
                    n2v->second.assign(value);
                }
                else {
                    n2v->second.clear();
                    numberToValue.erase(n2v);
                }
            }
            return;
        }
        case string_m: {
            auto s2v = stringToValue.find(key->data.strVal);
            if (s2v == stringToValue.end())
            {
                if (value->type != nil_m) {
                    stringToValue[key->data.strVal].assign(value);
                }
            }
            else {
                if (value->type != nil_m) {
                    s2v->second.clear();
                    s2v->second.assign(value);
                }
                else{
                    s2v->second.clear();
                    stringToValue.erase(s2v);
                }
            }
            return;
        }
        case bool_m: {
            auto s2v = boolToValue.find(key->data.boolVal);
            if (s2v == boolToValue.end()) {
                if (value->type != nil_m)
                    boolToValue[key->data.boolVal].assign(value);
            }
            else {
                if (value->type != nil_m) {
                    s2v->second.clear();
                    s2v->second.assign(value);
                }
                else {
                    s2v->second.clear();
                    boolToValue.erase(s2v);
                }
            }
            return;
        }
        case userfunc_m: {
            auto s2v = userFuncToValue.find(key->data.funcVal);
            if (s2v == userFuncToValue.end())
            {
                if (value->type != nil_m) {
                    userFuncToValue[key->data.funcVal].assign(value);
                }
            } else {
                if (value->type != nil_m) {
                    s2v->second.clear();
                    s2v->second.assign(value);
                } else {
                    s2v->second.clear();
                    userFuncToValue.erase(s2v);
                }
            }
            return;
        }
        case libfunc_m: {
            auto s2v = libFuncToValue.find(key->data.libfuncVal);
            if (s2v == libFuncToValue.end()) {
                if (value->type != nil_m) {
                    libFuncToValue[key->data.libfuncVal].assign(value);
                }
            } else {
                if (value->type != nil_m) {
                    s2v->second.clear();
                    s2v->second.assign(value);
                } else {
                    s2v->second.clear();
                    libFuncToValue.erase(s2v);
                }
            }
            return;
        }
        case table_m: {
            auto s2v = AVM_TableToValue.find(key->data.tableVal);
            if (s2v == AVM_TableToValue.end()) {
                if (value->type != nil_m) {
                    AVM_TableToValue[key->data.tableVal].assign(value);
                }
            } else {
                if (value->type != nil_m) {
                    s2v->second.clear();
                    s2v->second.assign(value);
                } else {
                    s2v->second.clear();
                    AVM_TableToValue.erase(s2v);
                }
            }
            return;
        }
        default:
            avm_error("Unknown table index type");
            return;
    }
}

string AVM_Table::toString_Util(std::map<AVM_Table *, bool> visited) {
    string toReturn;
    ostringstream out;
    string tmp;

    toReturn = "[ ";

    visited[this] = true;

    for (map<double, AVM_Memcell>::iterator it = numberToValue.begin();
         it != numberToValue.end(); it++) {
        out << setprecision(3) << it->first;
        toReturn += "{ " + out.str() + " : ";
        if (it->second.type == table_m && visited.find(it->second.data.tableVal) != visited.end()) {
            toReturn += string("table #") + to_string(it->second.data.tableVal->serialNo) + " }, ";
        } else {
            tmp = it->second.type == table_m ? it->second.data.tableVal->toString_Util(visited) : it->second.toString();
            toReturn += (it->second.type ==
                         string_m ? "\"" + tmp + "\"" : tmp) + " }, ";
        }
        out.str(string());
    }

    for (map<string, AVM_Memcell>::iterator it = stringToValue.begin();
         it != stringToValue.end(); it++) {
        toReturn += "{ \"" + it->first + "\" : ";
        if (it->second.type == table_m && visited.find(it->second.data.tableVal) != visited.end()) {
            toReturn += string("table #") + to_string(it->second.data.tableVal->serialNo) + " }, ";
        } else {
            tmp = it->second.type == table_m ? it->second.data.tableVal->toString_Util(visited) : it->second.toString();
            toReturn += (it->second.type  == string_m ? "\"" + tmp + "\"" : tmp) + " }, ";
        }
    }

    for (map<bool, AVM_Memcell>::iterator it = boolToValue.begin();
         it != boolToValue.end(); it++) {
        toReturn += "{ " + (it->first ? string("true") :
                            string("false")) + " : " ;
        if (it->second.type == table_m && visited.find(it->second.data.tableVal) != visited.end()) {
            toReturn += string("table #") + to_string(it->second.data.tableVal->serialNo) + " }, ";
        } else {
            tmp = it->second.type == table_m ? it->second.data.tableVal->toString_Util(visited) : it->second.toString();
            toReturn += (it->second.type == string_m ? "\"" + tmp + "\"" : tmp) + " }, ";
        }
    }

    for (map<AVM_Table *, AVM_Memcell>::iterator it = AVM_TableToValue.begin();
         it != AVM_TableToValue.end(); it++) {
        toReturn += "{ " + it->first->toString() + " : ";
        if (it->second.type == table_m && visited.find(it->second.data.tableVal) != visited.end()) {
            toReturn += string("table #") + to_string(it->second.data.tableVal->serialNo) + " }, ";
        } else {
            tmp = it->second.type == table_m ? it->second.data.tableVal->toString_Util(visited) : it->second.toString();
            toReturn += (it->second.type == string_m ? "\"" + tmp + "\"" : tmp) + " }, ";
        }
    }

    for (map<unsigned, AVM_Memcell>::iterator it = userFuncToValue.begin();
         it != userFuncToValue.end(); it++) {
        toReturn += "{ " + string("user function ") +
                    to_string(it->first) + " : ";
        if (it->second.type == table_m && visited.find(it->second.data.tableVal) != visited.end()) {
            toReturn += string("table #") + to_string(it->second.data.tableVal->serialNo) + " }, ";
        } else {
            tmp = it->second.type == table_m ? it->second.data.tableVal->toString_Util(visited) : it->second.toString();
            toReturn += (it->second.type == string_m ? "\"" + tmp + "\"" : tmp) + " }, ";
        }
    }

    for (map<string, AVM_Memcell>::iterator it = libFuncToValue.begin();
         it != libFuncToValue.end(); it++) {
        toReturn += "{ " + string("library function ") + it->first
                    + " : ";
        if (it->second.type == table_m && visited.find(it->second.data.tableVal) != visited.end()) {
            toReturn += string("table #") + to_string(it->second.data.tableVal->serialNo) + " }, ";
        } else {
            tmp = it->second.type == table_m ? it->second.data.tableVal->toString_Util(visited) : it->second.toString();
            toReturn += (it->second.type == string_m ? "\"" + tmp + "\"" : tmp) + " }, ";
        }
    }

    if(toReturn == "[ ") {
        toReturn = " [ ] ";
    } else if (toReturn[toReturn.length()-2] == ',') {
        toReturn.replace(toReturn.length() - 2, 2, " ] ");
    }

    return toReturn;
}

string AVM_Table::toString()
{
    map<AVM_Table *, bool> visited;

    return this->toString_Util(visited);
}

unsigned int AVM_Table::getTotalMembers()
{
    return this->numberToValue.size() + this->stringToValue.size() + this->boolToValue.size() +
            this->AVM_TableToValue.size() + this->userFuncToValue.size() + this->libFuncToValue.size();
}

vector<AVM_Memcell> AVM_Table::getKeys(){
    vector<AVM_Memcell> toReturn;

    AVM_Memcell cell;

    for(map<double, AVM_Memcell>::iterator it = numberToValue.begin(); it != numberToValue.end(); it++){
        cell.type = number_m;
        cell.data.numVal = it->first;
        toReturn.push_back(cell);
    }

    for(map<string, AVM_Memcell>::iterator it = stringToValue.begin(); it != stringToValue.end(); it++){
        cell.type = string_m;
        cell.data.strVal = strdup(it->first.c_str());
        toReturn.push_back(cell);
    }

    for(map<bool, AVM_Memcell>::iterator it = boolToValue.begin(); it != boolToValue.end(); it++){
        cell.type = bool_m;
        cell.data.boolVal = it->first;
        toReturn.push_back(cell);
    }

    for(map<AVM_Table *, AVM_Memcell>::iterator it = AVM_TableToValue.begin(); it != AVM_TableToValue.end(); it++){
        cell.type = table_m;
        cell.data.tableVal = it->first;
        toReturn.push_back(cell);
    }

    for(map<unsigned, AVM_Memcell>::iterator it = userFuncToValue.begin(); it != userFuncToValue.end(); it++){
        cell.type = userfunc_m;
        cell.data.funcVal = it->first;
        toReturn.push_back(cell);
    }

    for(map<string, AVM_Memcell>::iterator it = libFuncToValue.begin(); it != libFuncToValue.end(); it++){
        cell.type = libfunc_m;
        cell.data.libfuncVal = strdup(it->first.c_str());
        toReturn.push_back(cell);
    }

    return toReturn;
}

vector<AVM_Memcell> AVM_Table::getValues(){
    vector<AVM_Memcell> toReturn;

    for(map<double, AVM_Memcell>::iterator it = this->numberToValue.begin(); it != this->numberToValue.end(); it++){
        toReturn.push_back(it->second);
    }

    for(map<string, AVM_Memcell>::iterator it = stringToValue.begin(); it != stringToValue.end(); it++){
        toReturn.push_back(it->second);
    }

    for(map<bool, AVM_Memcell>::iterator it = boolToValue.begin(); it != boolToValue.end(); it++){
        toReturn.push_back(it->second);
    }

    for(map<AVM_Table *, AVM_Memcell>::iterator it = AVM_TableToValue.begin(); it != AVM_TableToValue.end(); it++){
        toReturn.push_back(it->second);
    }

    for(map<unsigned, AVM_Memcell>::iterator it = userFuncToValue.begin(); it != userFuncToValue.end(); it++){
        toReturn.push_back(it->second);
    }

    for(map<string, AVM_Memcell>::iterator it = libFuncToValue.begin(); it != libFuncToValue.end(); it++){
        toReturn.push_back(it->second);
    }

    return toReturn;
}