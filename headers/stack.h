#ifndef STACK_H
#define STACK_H

#include "stackstruct.h"
#include "global.h"

#define DEBUG
#define CANARY_DEBUG

#define ERROR        fprintf(Logfile, "error: %d line: %d\n", err.code, err.line);
#define INFO         fprintf(Logfile, "stack size = %ld, capacity = %ld\n", stk->size, stk->capacity);
#define NEWLINE      fprintf(Logfile, "\n");
#define VERIFY(stk)  StackVerify(stk, line)
#define CANARY       0xEDAA
#define POISON       0xDEAD
#define ERRORPOINT   0xBADDD
#define WARNINGPOINT 0xBED
#define NOERROR      0x0
#define CHECKFUNCERROR(error)   if (error.type == ERRORPOINT)                                         \
                                {                                                                     \
                                fprintf(Logfile, "err.code = %d, line: %d\n", error.code, error.line);\
                                return error;                                                         \
                                } 

enum errors 
{
    NULLSTKPTR        = 1<<0, //e
    NULLDATAPTR       = 1<<1, //e
    NEGATIVECAPACITY  = 1<<2, //e
    STKATTACKLEFT     = 1<<3, //e
    STKATTACKRIGHT    = 1<<4, //e
    HASHTRASH         = 1<<5, //e
    STRUCTATTACKLEFT  = 1<<6, //e
    STRUCTATTACKRIGHT = 1<<7, //e
    ERRORS_END        = 1<<8,
    POISONING         = 1<<9, //w
    SIZELIMIT         = 1<<10, //w 
};

StackErr_t StackInit(Stack_t *stk, int capacity, int line);
StackErr_t StackPush(Stack_t *stk, int value, int line);
StackErr_t StackPop(Stack_t *stk, StackElement_t *last_input, int line);
StackErr_t StackDestroy(Stack_t *stk, int line);
StackErr_t StackVerify(Stack_t *stk, int line);
StackErr_t StackDump(Stack_t *stk, int line, const char *filename, const char *stkname);
StackErr_t AllocationStk(Stack_t *stk);
StackErr_t StackOk(StackErr_t err, int line);
int ErrorType(StackErr_t err);
Ssize_t CalcHASH(Stack_t *stk);
void CheckSize(Stack_t *stk);

#ifdef DEBUG
    #define ONDEBUG(func) func
#else
    #define ONDEBUG(func) 
#endif

#ifdef CANARY_DEBUG
    #define CANDEBUG(func) func
#else
    #define CANDEBUG(func) 
#endif

#endif

