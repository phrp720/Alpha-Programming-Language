#ifndef ERROR_AVM_H
#define ERROR_AVM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string>

using namespace std;

void avm_error(string format, ...);

#endif //ERROR_AVM_H