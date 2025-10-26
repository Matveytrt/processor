#ifndef ASMFUNCS_H
#define ASMFUNCS_H

#include "global.h"
#include "asmstruct.h"

char *GetSingle  (Assembler_t *assembler, char *buff, size_t *size);
char *GetDouble  (Assembler_t *assembler, char *buff, size_t *size);
char *GetRegistr (Assembler_t *assembler, char *buff, size_t *size);
char *GetRam     (Assembler_t *assembler, char *buff, size_t *size);
char *GetLabel   (Assembler_t *assembler, char *buff, size_t *size);
size_t CalcHash (const char *str);

#endif