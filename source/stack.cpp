#include "stack.h"

StackErr_t StackInit(Stack_t *stk, int capacity, int line)
{ 
    assert(stk);
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
    if (error.code == NOERROR)
    {
        fprintf(Logfile, "Initialisation succses\n");
    }

    return error;
}

StackErr_t StackPush(Stack_t *stk, int value, int line)
{
    assert(stk);

    StackErr_t error = VERIFY(stk);
    CHECKFUNCERROR(error)

    stk->size++;
    CheckSize(stk);

    stk->data[stk->size + stk->flag] = value;
    ONDEBUG(stk->hash = CalcHASH(stk);)

    fprintf(Logfile, "line: %d pushed value = %d\n", line, stk->data[stk->size + stk->flag]);
    INFO
    NEWLINE

    error = VERIFY(stk);
    return error;
}

StackErr_t StackPop(Stack_t *stk, StackElement_t *last_input, int line)
{
    assert(stk);
    assert(last_input);

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
        fprintf(Logfile, "line: %d popped value = %d\n", line,  *last_input);
    }
    else
    {
        fprintf(Logfile, "can't pop: empty stack\n");
        error.code = SIZELIMIT;
        *last_input = 0;
        error = StackOk(error, line);
        return error;

    }
    INFO  
    NEWLINE
    

    error = VERIFY(stk);
    return error;
}

StackErr_t StackDestroy(Stack_t *stk, int line)
{
    assert(stk);
    StackErr_t error = VERIFY(stk);
    CHECKFUNCERROR(error)

    NEWLINE
    fprintf(Logfile, "info about stack before death:\n");
    INFO

    free(stk->data);
    return error;
}

StackErr_t StackVerify(Stack_t *stk, int line)
{  
    assert(stk);

    StackErr_t err = {};
    err.line = line;
    err.type = NOERROR;

    #ifdef DEBUG
        if (stk == NULL)
        {
            err.code |= NULLSTKPTR;
            printf("stkptr err.code = %d\n", err.code);
        }
        #ifdef CANARY_DEBUG
            if (stk->canaryleft != CANARY)
            {
                err.code |= STRUCTATTACKLEFT;
                printf("left err.code = %d\n", err.code);
                err = StackOk(err, line);
                return err;
            }
            if (stk->canaryright != CANARY)
            {
                err.code |= STRUCTATTACKRIGHT; 
                printf("right err.code = %d\n", err.code);
                err = StackOk(err, line);
                return err; 
            }
        #endif
        if (stk->data == NULL)
        {
            err.code |= NULLDATAPTR;
            printf("dataptr err.code = %d\n", err.code);
            err = StackOk(err, line);
            return err;
        }

        else
        {
            if (stk->capacity < 0)
            {
                err.code |= NEGATIVECAPACITY;
                printf("cap err.code = %d\n", err.code);
            }
            for (Ssize_t i = 0; i <= stk->size + stk->flag; i++)
            {
                if (stk->data[i] == POISON)
                {
                    err.code |= POISONING;
                    fprintf(Logfile, "Stack POISONing from %ld position\n", i); 
                    break;
                }
            }
            if (stk->hash != CalcHASH(stk))
            {
                err.code |= HASHTRASH;
            }
            if ( stk->size > (stk->capacity CANDEBUG(-2)))
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

    err = StackOk(err, line);
    //printf("verify err.code = %d\n err.type = %d\n", err.code, err.type);
    return err;
}

StackErr_t StackDump(Stack_t *stk, int line, const char *filename, const char *stkname)
{
    assert(stk);
    assert(filename);
    StackErr_t error = StackVerify(stk, line);
    CHECKFUNCERROR(error)

    //printf("dump error.code = %d\n", error.code);
    NEWLINE
    fprintf(Logfile, "stack name: %s\nStackDump called from %s: %d\n", stkname, filename, line);
    
    if (error.type != ERRORPOINT)
    {
        INFO
        NEWLINE

        for (Ssize_t i = 0; i < stk->capacity; i++)
        {
            #ifdef DEBUG
                if (stk->data[i] == POISON) 
                {
                    fprintf(Logfile, "*[%ld] = %d POISON\n", i, stk->data[i]);
                }
                else if (stk->data[i] == CANARY)
                {
                    fprintf(Logfile, "*[%ld] = %d CANARY\n", i, stk->data[i]);
                } 
                else //
                {
                    fprintf(Logfile, "[%ld] = %d\n", i, stk->data[i]);
                }
            #else
                fprintf(Logfile, "[%ld] = %d\n", i, stk->data[i]);
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
    assert(stk);

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
        

        fprintf(Logfile, "succesful reallocation\n"); 

        CANDEBUG(stk->data[stk->capacity - 1] = CANARY;)
        ONDEBUG(stk->hash = CalcHASH(stk);)
    }
}

StackErr_t AllocationStk(Stack_t *stk)
{
    assert(stk);
    
    StackErr_t err = {};
    StackElement_t *alloc_ptr = (StackElement_t*) calloc((size_t) stk->capacity, sizeof(StackElement_t));

    if (alloc_ptr == NULL)
    {
        fprintf(Logfile, "allocation error\n"); 
        err.code = NULLDATAPTR;  
    }
    else
    {
        stk->data = alloc_ptr;
        err.code = NOERROR;
    }

    return err;
}

StackErr_t StackOk(StackErr_t err, int line)
{
    if ((err.code & NULLSTKPTR))
    {
        fprintf(Logfile, "line: %d Error: Stack pointer = NULL\n", line);      
    }
    if (err.code & NULLDATAPTR)
    {
        fprintf(Logfile, "line: %d Error: Data pointer = NULL\n", line);     
    }
    if (err.code & NEGATIVECAPACITY)
    {
        fprintf(Logfile, "line: %d Error: Capacity too small!\n", line);  
    }
    if (err.code & STKATTACKLEFT)
    {
        fprintf(Logfile, "line: %d Error: Stack attack from the left\n", line);      
    }
    if (err.code & STKATTACKRIGHT)
    {
        fprintf(Logfile, "line: %d Error: Stack attack from the right\n", line);      
    }
    if (err.code & STRUCTATTACKRIGHT)
    {
        fprintf(Logfile, "line: %d Error: Struct attack from the right\n", line);      
    }
    if (err.code & STRUCTATTACKLEFT)
    {
        fprintf(Logfile, "line: %d Error: Struct attack from the left\n", line);     
    }
    if (err.code & SIZELIMIT)
    {
        fprintf(Logfile, "line: %d Warning: Size limit errors\n", line);      
    }
    if (err.code & POISONING)
    {
        fprintf(Logfile, "line: %d Warning: Stack poisoning\n", line);      
    }
    if (err.code & HASHTRASH)
    {
        fprintf(Logfile, "line: %d Error: hash violation\n", line);
    }
    
    err.type = ErrorType(err);
    //ERROR

    return err;
}

int ErrorType(StackErr_t err)
{
    if (err.code == 0)
    {
        return NOERROR;
    }
    else if (err.code < ERRORS_END)
    {
        return ERRORPOINT;
    }
    else
    {
        return WARNINGPOINT;
    }
}