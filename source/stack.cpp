#include "stack.h"

StackErr_t StackInit(Stack_t *stk, int capacity, int line)
{ 
    ONDEBUG(assert(stk);)
    StackErr_t error ={};

    stk->size = 0;
    if (capacity < 0)
    {
        error.code = NEGATIVECAPACITY;
        return error;
    } 
    else 
    {
        stk->capacity = capacity;
    }

    #ifdef CANARY_DEBUG
        stk->capacity += 2;
        
        error = AllocationStk(stk);

        stk->canaryleft = CANARY;
        stk->canaryright = CANARY;
        stk->data[0] = CANARY;
        stk->data[stk->capacity - 1] = CANARY;
        stk->flag = 0;   
    #else
        error = AllocationStk(stk);

        stk->flag = -1;
    #endif
    
    //INFO
    ONDEBUG(stk->hash = CalcHASH(stk);)

    error = VERIFY(stk);
    //printf("%d\n", error.code);
    if (error.code == 0)
    {
        fprintf(logfile, "Initialisation succses\n");
    }

    return error;
}

StackErr_t StackPush(Stack_t *stk, int value, int line)
{
    ONDEBUG(assert(stk);)

    StackErr_t error = VERIFY(stk);
    CHECKFUNCERROR(error)

    stk->size++;
    CheckSize(stk);

    stk->data[stk->size + stk->flag] = value;
    ONDEBUG(stk->hash = CalcHASH(stk);)

    fprintf(logfile, "line: %d pushed value = %d\n", line, stk->data[stk->size + stk->flag]);
    INFO
    NEWLINE

    error = VERIFY(stk);
    return error;
}

StackErr_t StackPop(Stack_t *stk, StackElement_t *last_input, int line)
{
    ONDEBUG(assert(stk);)
    ONDEBUG(assert(last_input);)

    StackErr_t error = VERIFY(stk);
    CHECKFUNCERROR(error)

    *last_input = stk->data[stk->size + stk->flag];

    if (stk->size > 0)
    {
        #ifdef DEBUG
        stk->data[stk->size + stk->flag] = POISON;
        #else
        stk->data[stk->size + stk->flag] = 0;
        #endif

        stk->size--;

        ONDEBUG(stk->hash = CalcHASH(stk);)
        fprintf(logfile, "line: %d popped value = %d\n", line,  *last_input);
    }
    else
    {
        fprintf(logfile, "can't pop: empty stack\n");
        error.code = SIZELIMIT;
        *last_input = 0;
        error = StackOk(error);
        return error;

    }
    INFO  
    NEWLINE
    

    error = VERIFY(stk);
    return error;
}

StackErr_t StackDestroy(Stack_t *stk, int line)
{
    ONDEBUG(assert(stk);)
    StackErr_t error = VERIFY(stk);
    CHECKFUNCERROR(error)

    NEWLINE
    fprintf(logfile, "info about stack before death:\n");
    INFO

    free(stk->data);
    return error;
}

StackErr_t StackVerify(Stack_t *stk, int line)
{  
    ONDEBUG(assert(stk);)

    StackErr_t err = {};
    err.line = line;
    err.type = NOERROR;

    #ifdef DEBUG
        if (stk == NULL)
        {
            err.code |= NULLSTKPTR;
            printf("stkptr err.code = %d\n", err.code);
        }
        if (stk->canaryleft != CANARY)
        {
            err.code |= STRUCTATTACKLEFT;
            printf("left err.code = %d\n", err.code);
            err = StackOk(err);
            return err;
        }
        if (stk->canaryright != CANARY)
        {
            err.code |= STRUCTATTACKRIGHT; 
            printf("right err.code = %d\n", err.code);
            err = StackOk(err);
            return err; 
        }
        if (stk->data == NULL)
        {
            err.code |= NULLDATAPTR;
            printf("dataptr err.code = %d\n", err.code);
            err = StackOk(err);
            return err;
        }

        else
        {
            if (stk->capacity < 2)
            {
                err.code |= NEGATIVECAPACITY;
                printf("cap err.code = %d\n", err.code);
            }
            for (Ssize_t i = 0; i <= stk->size; i++)
            {
                if (stk->data[i] == POISON)
                {
                    err.code |= POISONING;
                    fprintf(logfile, "Stack POISONing from %ld position\n", i); 
                    break;
                }
            }
            if (stk->hash != CalcHASH(stk))
            {
                err.code |= HASHTRASH;
            }
            if ( (stk->size > (stk->capacity - 2)))
            {
                err.code |= SIZELIMIT;
            }
        }  
          
    #endif

    #ifdef CANARY_DEBUG
        if (stk->data != NULL)
        {
            if (stk->data[0] != CANARY)
            {
                err.code |= STKATTACKLEFT;
            }
            if (stk->data[stk->capacity - 1] != CANARY)
            {
                err.code |= STKATTACKRIGHT;   
            }
        }  
    #endif

    err = StackOk(err);
    //printf("verify err.code = %d\n err.type = %d\n", err.code, err.type);
    return err;
}

StackErr_t StackDump(Stack_t *stk, int line, const char *filename)
{
    ONDEBUG(assert(stk);)
    StackErr_t error = StackVerify(stk, line);
    CHECKFUNCERROR(error)

    //printf("dump error.code = %d\n", error.code);
    fprintf(logfile, "StackDump called from %s: %d\n", filename, line);
    
    if (error.type != ERRORPOINT)
    {
        INFO
        NEWLINE

        for (Ssize_t i = 0; i < stk->capacity; i++)
        {
            #ifdef DEBUG
                if (stk->data[i] == POISON) 
                {
                    fprintf(logfile, "*[%ld] = %d POISON\n", i, stk->data[i]);
                }
                else if (stk->data[i] == CANARY)
                {
                    fprintf(logfile, "*[%ld] = %d CANARY\n", i, stk->data[i]);
                } 
                else //
                {
                    fprintf(logfile, "[%ld] = %d\n", i, stk->data[i]);
                }
            #else
                fprintf(logfile, "[%ld] = %d\n", i, stk->data[i]);
            #endif    
        }
    }

    error = StackVerify(stk, line);
    return error;
}

Ssize_t CalcHASH(Stack_t *stk)
{   
    assert(stk);

    Ssize_t hash = stk->capacity + stk->size CANDEBUG(+ stk->canaryleft + stk->canaryright);
    for (Ssize_t i = 0; i < stk->capacity; i++)
    {
        hash += stk->data[i] * (i + 1);
    }

    return hash;
}    

void CheckSize(Stack_t *stk)
{
    ONDEBUG(assert(stk);)

    Ssize_t last = stk->capacity - 1;

    if (stk->size >= (stk->capacity CANDEBUG(-2)))
    {
        stk->capacity *= 2;
        stk->data = (StackElement_t*) realloc(stk->data, stk->capacity * sizeof(StackElement_t));
        assert(stk->data != NULL);

        for (Ssize_t i = last; i < stk->capacity - 1; i++)
        {
            #ifdef DEBUG
                stk->data[i] = POISON;
            #else
                stk->data[i] = 0;
            #endif
        }
        

        fprintf(logfile, "succesful reallocation\n"); 

        CANDEBUG(stk->data[stk->capacity - 1] = CANARY;)
        ONDEBUG(stk->hash = CalcHASH(stk);)
    }
}

StackErr_t AllocationStk(Stack_t *stk)
{
    ONDEBUG(assert(stk);)
    
    StackErr_t err = {};
    StackElement_t *alloc_ptr = (StackElement_t*) calloc(stk->capacity, sizeof(StackElement_t));

    if (alloc_ptr == NULL)
    {
        fprintf(logfile, "allocation error\n"); 
        err.code = NULLDATAPTR;  
    }
    else
    {
        stk->data = alloc_ptr;
        err.code = 0;
    }

    return err;
}

StackErr_t StackOk(StackErr_t err)
{
    do
    {
        if ((err.code & NULLSTKPTR) == NULLSTKPTR)
        {
            fprintf(logfile, "Error: Stack pointer = NULL\n");      
        }
        if ((err.code & NULLDATAPTR) == NULLDATAPTR)
        {
            fprintf(logfile, "Error: Data pointer = NULL\n");     
        }
        if ((err.code & NEGATIVECAPACITY) == NEGATIVECAPACITY)
        {
            fprintf(logfile, "Error: Capacity too small!\n");  
        }
        if ((err.code & STKATTACKLEFT) == STKATTACKLEFT)
        {
            fprintf(logfile, "Error: Stack attack from the left\n");      
        }
        if ((err.code & STKATTACKRIGHT) == STKATTACKRIGHT)
        {
            fprintf(logfile, "Error: Stack attack from the right\n");      
        }
        if ((err.code & STRUCTATTACKRIGHT) == STRUCTATTACKRIGHT)
        {
            fprintf(logfile, "Error: Struct attack from the right\n");      
        }
        if ((err.code & STRUCTATTACKLEFT) == STRUCTATTACKLEFT)
        {
            fprintf(logfile, "Error: Struct attack from the left\n");     
        }
        if ((err.code & SIZELIMIT) == SIZELIMIT)
        {
            fprintf(logfile, "Warning: Size limit errors\n");      
        }
        if ((err.code & POISONING) == POISONING)
        {
            fprintf(logfile, "Warning: Stack poisoning\n");      
        }
        if ((err.code & HASHTRASH) == HASHTRASH)
        {
            fprintf(logfile, "Error: hash violation\n");
        }
    } while(0);
    
    err.type = ErrorType(err);
    //ERROR

    return err;
}

int ErrorType(StackErr_t err)
{
    err.type = NOERROR;

    if (err.code > ERRORS_END)
    {
        err.type = WARNINGPOINT;
    }
    else if(err.code > 0 && err.code < ERRORS_END)
    {
        err.type = ERRORPOINT;
    }

    return err.type;
}