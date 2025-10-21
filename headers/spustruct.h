#ifndef SPUSTRUCT_H
#define SPUSTRUCT_H

#include "stackstruct.h"

struct Processor_t
{
    StackElement_t *bytecode;
    StackElement_t *regs;
    StackElement_t *ram;
    Stack_t retaddr;
    Stack_t stk;
    int ip;
    size_t ncommands;
};

struct CmdFunc_t
{
    int code;
    void (*ptr_func) (Processor_t *);
};

#endif