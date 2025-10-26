#ifndef GLOBAL_H
#define GLOBAL_H

#include "TXLib.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>
#include <iostream>
#include <ctype.h>
#include <math.h>

#define DEBUGASM
#define DEBUGCMD
#define DEBUGLBL

#define DEBUGSPU

#define Ncommands 27
#define Nregs     16

#define LABELSIZE 30
#define NOLABELPTR -1
#define StrSize   30
#define AlphabetSize 31

#define RetSize   20
#define Nfuncs    26
#define RamSize   270000
#define DrawStep  300

extern FILE *Logfile;

enum Commands
{
    HLT_C    = 0,
    PUSH_C   = 1,
    POP_C    = 2,
    ADD_C    = 3,
    SUB_C    = 4,
    MUL_C    = 5,
    DIV_C    = 6,
    SQRT_C   = 7,
    IN_C     = 8,
    DUMP_C   = 9,
    CLEAR_C  = 10,
    INIT_C   = 11,
    PUSHR_C  = 12,
    POPR_C   = 13,
    JB_C     = 14,
    JBE_C    = 15,
    JA_C     = 16,
    JAE_C    = 17,
    JE_C     = 18,
    JNE_C    = 19,
    JMP_C    = 20,
    CALL_C   = 21,
    RET_C    = 22,
    PUSHM_C  = 23,
    POPM_C   = 24,
    DRAW_C   = 25,
    OUT_C    = 26,
};

enum Registers
{
    RAX_R = 0,
    RBX_R = 1,
    RCX_R = 2,
    RDX_R = 3,
    AX_R  = 4,
    BX_R  = 5,
    CX_R  = 6,
    DX_R  = 7,
    RAY_R = 8,
    RBY_R = 9,
    RCY_R = 10,
    RDY_R = 11,
    AY_R  = 12,
    BY_R  = 13,
    CY_R  = 14,
    DY_R  = 15,
};

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

#ifdef DEBUGSPU
    #define ONDEBUGSPU(func) func
#else
    #define ONDEBUGSPU(func) 
#endif