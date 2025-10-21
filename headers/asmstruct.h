#ifndef ASMSTRUCT_H
#define ASMSTRUCT_H

#include "stackstruct.h"

struct Label_t
{
    char *name;
    size_t hash;
    int address;
};

struct Assembler_t 
{
    StackElement_t *bytecode;
    size_t bytecode_size;
    Label_t *labels;
    int nlabels;
};

struct Command_t 
{
    int code;
    const char *name;
    size_t hash;
    int argtype;
};

struct Registr_t 
{
    int number;
    size_t hash;
    const char *name;
};

enum argtypes
{
    END_T     = 0,
    SINGLE_T  = 1,
    DOUBLE_T  = 2,
    REGISTR_T = 3,
    LBL_T     = 4,
    RET_T     = 5,
    RAM_T     = 6,
};

#endif