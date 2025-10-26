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
    int ncommands;
    int npasses;
    Label_t *labels;
    int nlabels;
};

struct Command_t 
{
    int code;
    const char *name;
    size_t hash;
    char *(*ptr_func) (Assembler_t *, char *, size_t *);
};

struct Registr_t 
{
    int number;
    size_t hash;
    const char *name;
};

#endif