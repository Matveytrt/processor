#include <stdio.h>
#include <stdlib.h>
#include <sys\types.h>
#include <string.h>

#include "stackstruct.h"
#include "stack.h"
#include "logfile.h"

/*#ifdef ISEMPTY
    if (stk->size == 0)
    {
        printf("empty stack...\n");
    }
#endif*/

void Calculator(Stack_t *stk);