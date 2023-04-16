#include <cstring>
#include <stdarg.h>
#include "error_avm.h"

using namespace std;
extern unsigned currentLine;
extern bool executionFinished;

void avm_error(string format, ...) {
    va_list errList;
    va_start(errList, format.c_str());
    vfprintf(stderr, format.c_str(), errList);
    fprintf(stderr, " on line %d \n", currentLine);
    executionFinished = true;
    va_end(errList);
}