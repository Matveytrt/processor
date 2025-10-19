#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <math.h>

#include "stack.h"
#include "stackstruct.h"
#include "spustruct.h"

void Push  (Processor_t *spu);
void Pop   (Processor_t *spu);
void Add   (Processor_t *spu);
void Sub   (Processor_t *spu);
void Mul   (Processor_t *spu);
void Div   (Processor_t *spu);
void Sqrt  (Processor_t *spu);
void In    (Processor_t *spu);
void Clear (Processor_t *spu);
void Dump  (Processor_t *spu);
void Init  (Processor_t *spu);
void PushR (Processor_t *spu);
void PopR  (Processor_t *spu);
void JB    (Processor_t *spu);
void JBE   (Processor_t *spu);
void JA    (Processor_t *spu);
void JAE   (Processor_t *spu);
void JE    (Processor_t *spu);
void JNE   (Processor_t *spu);
void JUMP  (Processor_t *spu);
void CALL  (Processor_t *spu);
void RET   (Processor_t *spu);
void PUSHM (Processor_t *spu);
void POPM  (Processor_t *spu);
void DRAW  (Processor_t *spu);

#define Nfuncs 25

#endif