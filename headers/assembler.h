#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "global.h"
#include "workwithfile.h"
#include "asmstruct.h"
#include "stackstruct.h"

void InitASM(Assembler_t *assembler);
size_t Assembling(const char *buff, Assembler_t *assembler);
void DestroyASM(Assembler_t *assembler);

#define DEBUGASM
#define DEBUGCMD
#define DEBUGLBL

#define STARTSIZE buff_size / 2
#define LABELSIZE 30
#define NOLABELPTR -1
#define StrSize 30
#define AlphabetSize 31

#define Bytecode_step       assembler->bytecode++;              \
                            size++;                             \
                            buff += offset;
 
#define CheckHash(data, size)                       \
    for (int i = 0; i < size; i++)                  \
    {                                               \
        if (data[i].hash == 0)                      \
        {                                           \
            data[i].hash = CalcHash(data[i].name);  \
        }                                           \
        else                                        \
        {                                           \
            break;                                  \
        }                                           \
    } 

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