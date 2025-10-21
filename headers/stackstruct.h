#ifndef STACKSTRUCT_H
#define STACKSTRUCT_H

typedef int StackElement_t;
typedef signed long int Ssize_t;

struct Stack_t
{
    StackElement_t  canaryleft;
    StackElement_t *data;
    Ssize_t          size;
    Ssize_t         capacity;
    Ssize_t         hash;
    int             flag;
    StackElement_t  canaryright;
};

struct StackErr_t
{
    int code;
    int line;
    int type;
};

#endif 
 