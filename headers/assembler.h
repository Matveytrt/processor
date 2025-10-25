#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "global.h"
#include "workwithfile.h"
#include "strstruct.h"
#include "asmstruct.h"
#include "stackstruct.h"
#include "asmfuncs.h"

void InitASM    (Assembler_t *assembler, size_t size);
void Assembling (File_Info_t *info, Assembler_t *assembler);
void DumpASM    (Assembler_t assembler);
void DestroyASM (Assembler_t *assembler);

#endif

