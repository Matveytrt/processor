#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "workwithfile.h"
#include "cpustruct.h"
#include "stackstruct.h"
#include "stack.h"

enum commandcodes
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
};

enum registers
{
    RAX = 0,
    RBX = 1,
    RCX = 2,
    RDX = 3,
    AX  = 4,
    BX  = 5,
    CX  = 6,
    DX  = 7,
};

enum argtypes
{
    END_T     = 0,
    SINGLE_T  = 1,
    DOUBLE_T  = 2,
    REGISTR_T = 3,
    JMP_T     = 4,
};

void Executor(StackElement_t data[], StackElement_t regs_data[]);

#define NREGS 8
#define STARTCAPACITY 10

#define INIT(stk, capacity) StackInit(&stk, capacity, __LINE__);

#define PUSH(stk, value)    StackPush(&stk, value, __LINE__);

#define POP(stk, value)     StackPop(&stk, &value, __LINE__);

#define DESTROY(stk)        StackDestroy(&stk, __LINE__);

#define DUMP(stk)           StackDump(&stk, __LINE__, __FILE__);

/*#define CHECKERROR(error)   if (error.type == ERRORPOINT)                       \
                            {                                                   \
                                break;                                          \
                            } */