#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "global.h"
#include "workwithfile.h"
#include "stack.h"
#include "spufuncs.h"
#include "stackstruct.h"
#include "spustruct.h"

void InitSPU    (Processor_t *spu);
void Processing (Processor_t *spu);
void DestroySPU (Processor_t *spu);

#endif
