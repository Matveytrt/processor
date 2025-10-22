#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "global.h"
#include "workwithfile.h"
#include "stack.h"
#include "functions.h"
#include "stackstruct.h"
#include "spustruct.h"

#define DEBUGSPU

void InitSPU(Processor_t *spu);
void Processing(Processor_t *spu);
void DestroySPU(Processor_t *spu);

#endif

#ifdef DEBUGSPU
    #define ONDEBUGSPU(func) func
#else
    #define ONDEBUGSPU(func) 
#endif