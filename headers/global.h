#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>
#include <iostream>
#include <ctype.h>
#include <math.h>
//#include <TXLib.h>

extern FILE *Logfile;

#define Ncommands 27
#define Nregs 16
#define RetSize 20
#define Nfuncs 26
#define RamSize 900
#define DrawStep 30

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
    RAX = 0,
    RBX = 1,
    RCX = 2,
    RDX = 3,
    AX  = 4,
    BX  = 5,
    CX  = 6,
    DX  = 7,
    RAY = 8,
    RBY = 9,
    RCY = 10,
    RDY = 11,
    AY  = 12,
    BY  = 13,
    CY  = 14,
    DY  = 15,
};

#endif