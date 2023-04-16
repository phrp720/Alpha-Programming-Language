#ifndef TEMP_FUN
#define TEMP_FUN

#include "Symbol_table.h"

using namespace std;

class Temp_fun {
    public:
        Temp_fun();
        unsigned int current_temp_num;
        string newTempName();
        void resetTemp();
        string getCurrentTempNum();
        Node* newtemp();
};

#endif
