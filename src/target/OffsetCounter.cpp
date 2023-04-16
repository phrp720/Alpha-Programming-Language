#include <assert.h>
#include<iostream>
#include "OffsetCounter.h"

CurrentsScopeOffset::CurrentsScopeOffset(){
    formalArgOffset = 0;
    programVarOffset = 0;
    localVarOffset = 0;
    scopeSpaceCounter = 1;
};

void CurrentsScopeOffset::inccurrScopeOffset() {
    switch (currentScopeSpace()) {
        case programVar:
            ++programVarOffset;
            break;
        case functionLocal:
            ++localVarOffset;
            break;
        case formalArg:
            ++formalArgOffset;
            break;
        default:
            assert(0);
    }
};

void CurrentsScopeOffset::exitScopeSpace() {
    assert(scopeSpaceCounter > 1);
    --scopeSpaceCounter;
};

void CurrentsScopeOffset::enterScopeSpace() {
    ++scopeSpaceCounter;
};

size_t CurrentsScopeOffset::currScopeOffset() {
    switch (currentScopeSpace()){
        case programVar:
            return programVarOffset;
        case functionLocal:
            return localVarOffset;
        case formalArg:
            return formalArgOffset;
        default:
            assert(0);
    }
};

ScopeSpace_Type CurrentsScopeOffset::currentScopeSpace() {
    
    if (scopeSpaceCounter == 1) {
        return ScopeSpace_Type::programVar;
    }

    if (scopeSpaceCounter % 2 == 0) {
        return ScopeSpace_Type::formalArg;
    }

    return ScopeSpace_Type::functionLocal;
};
void CurrentsScopeOffset::resetformalargsoffset(void){
    formalArgOffset = 0;
}
void CurrentsScopeOffset::resetfunctionlocalsoffset(void){
    localVarOffset = 0;
}
void CurrentsScopeOffset::restorecurrscopeoffset(unsigned int n){
    switch (this->currentScopeSpace())
    {
        case programVar: programVarOffset = n; break;
        case functionLocal: localVarOffset = n; break;
        case formalArg: formalArgOffset = n; break;
        default: assert(0);
    }
}

/*







unsigned nextquadlabel(void){
    return currQuad;
}

void patchlabel(unsigned quadNo, unsigned label){
    assert(quadNo < currQuad);
    quads[quadNo].label = label;
}

*/
