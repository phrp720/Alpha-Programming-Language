#ifndef OFFSET_COUNTER_H
#define OFFSET_COUNTER_H
#include<iomanip>

enum ScopeSpace_Type {
	programVar=0, functionLocal, formalArg
};

class CurrentsScopeOffset {
    public:
        CurrentsScopeOffset();
        size_t programVarOffset;
        size_t formalArgOffset;
        size_t localVarOffset;
        size_t scopeSpaceCounter;
        void enterScopeSpace();
        void exitScopeSpace();
        void inccurrScopeOffset();
        void resetformalargsoffset();
        void resetfunctionlocalsoffset(void);
        void restorecurrscopeoffset(unsigned int n);
        size_t currScopeOffset();
        ScopeSpace_Type currentScopeSpace();
};

#endif
