#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "workwithfile.h"
#include "spustruct.h"
#include "stackstruct.h"
#include "stack.h"
#include "functions.h"

void InitSPU(Processor_t *spu);
void Processing(Processor_t *spu);
void DestroySPU(Processor_t *spu);

#endif