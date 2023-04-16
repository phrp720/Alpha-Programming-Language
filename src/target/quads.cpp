#include <iostream>
#include <iomanip>
#include <vector>
#include <assert.h>
#include "quads.h"
#include "Dispatcher.h"

using namespace std;

Quads::Quads(){
    currentQuad = 0;
}

void Quads::emit(iopcode opCode, expr *arg1, expr *arg2, expr *result, unsigned label, unsigned line) {
    quad q = {opCode, result, arg1, arg2, label, line};
    quads.push_back(q);
}

void Quads::printQuads() {
    cout << "Quad#" << setfill(' ') << setw(10);
    cout << " opcode " << setfill(' ') << setw(10);
    cout << " result " << setfill(' ') << setw(10);
    cout << " arg1 " << setfill(' ') << setw(10);
    cout << " arg2 " << setfill(' ') << setw(10);
    cout << " label " << endl << endl;
    for (unsigned int i = 0; i < quads.size(); i++) {
        cout << to_string(i + 1) << ".";
        cout << setfill(' ') << setw(10);
        cout << opcodeMap[quads[i].op] << ":";
        cout << setfill(' ') << setw(10);
        cout << quads[i].result->toString();
        cout << setfill(' ') << setw(10);
        cout << quads[i].arg1->toString();
        cout << setfill(' ') << setw(10);
        cout << quads[i].arg2->toString();
        cout << setfill(' ') << setw(10);
        if (quads[i].label == 0) {
            cout << "";
        } else {
            cout << quads[i].label;
        }
        cout << setfill(' ') << setw(10);
        cout << " [line " << quads[i].line + 1 << "] " << endl;

    }
}

unsigned Quads::mergeList(unsigned list, unsigned list2) {
    if (!list) {
        return list2;
    }

    if (!list2) {
        return list;
    }

    unsigned i = list;
    while (quads[i].label) {
        i = quads[i].label;
        quads[i].label = list2;
    }

    return list;
}
int Quads::newlist(int i){
    quads[i].label=0;
    return i;
}

void Quads::patchLabel(unsigned quadNo,unsigned label) {
    assert(quadNo < nextQuadLabel());
    quads[quadNo-1].label = label;
}

unsigned Quads::nextQuadLabel(void) {
    return quads.size() + 1;
}

void Quads::patchlist(vector<int> list, int label) {
    int index = 0;
    while (index < list.size()) {
        int next = quads[index].label;
        quads[index].label = label;
        index = next;
    }
}
void Quads::generator_(){
    for(unsigned i=0;i<quads.size();i++){
        this->currentQuad++;
        (*generators[quads[i].op])(quads[i]);
    }
}

