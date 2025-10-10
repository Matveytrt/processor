#include "stackstruct.h"

#ifndef CPUSTRUCT_H
#define CPUSTRUCT_H

typedef int StackElement_t;

struct Processor
{
    Stack_t stk;
    StackElement_t *bytecode;
    size_t ncommands;
};


#endif