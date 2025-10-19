#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>
#include <ctype.h>

#include "spustruct.h"
#include "logfile.h"
#include "workwithfile.h"
#include "stackstruct.h"

struct Assembler_t 
{
    int *labels;
    StackElement_t *bytecode;
    size_t bytecode_size;
};

void InitASM(Assembler_t *assembler);
size_t Assembling(const char *buff, Assembler_t *assembler);
void DestroyASM(Assembler_t *assembler);

#define DEBUGASM
#define DEBUGCMD
#define DEBUGLBL

#define STARTSIZE buff_size / 2
#define LABELSIZE 30
#define NOLABELPTR -1
#define Brackets 2
#define StrSize 20

#define Bytecode_step       assembler->bytecode++;              \
                            size++;                             \
                            buff += offset;
 
#endif

#ifdef DEBUGASM
    #define ONDEBUGASM(func) func
#else
    #define ONDEBUGASM(func) 
#endif
#ifdef DEBUGCMD
    #define ONDEBUGCMD(func) func
#else
    #define ONDEBUGCMD(func) 
#endif
#ifdef DEBUGLBL
    #define ONDEBUGLBL(func) func
#else
    #define ONDEBUGLBL(func) 
#endif