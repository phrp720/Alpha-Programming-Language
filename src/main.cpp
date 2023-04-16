#include <iostream>
#include <fstream>
#include "error_avm.h"
#include "AVM.h"

using namespace std;

int main(int argc, char ** argv) {
    ifstream program;

    if (argc < 2) {
        cout << "Arguments are wrong you must pass a filename." << endl;
        cout << "Usage: avm <filename>" << endl;
        return 0;
    }

    program.open(argv[1], ios::in | ios::binary);

    if (!program.is_open()) {
        avm_error("Could not load program %s \n", argv[1]);
        return 1;
    }

    AVM virtualMachine = AVM(program);
    virtualMachine.executeCycle();

    program.close();
    return 0;
}