#include "asmfuncs.h"

void   NameReg       (const char *regname, StackElement_t *bytecode);
int    CheckLabel    (const char *tag_name, Assembler_t *assembler);

#define Bytecode_step       assembler->bytecode++;                 \
                            (*size)++;                             \
                            buff += offset;

#define CheckHash(data, size)                       \
    for (int i = 0; i < size; i++)                  \
    {                                               \
        if (data[i].hash == 0)                      \
        {                                           \
            data[i].hash = CalcHash(data[i].name);  \
        }                                           \
        else                                        \
        {                                           \
            break;                                  \
        }                                           \
    }

char *GetSingle(Assembler_t *assembler, char *buff, size_t *size)
{
    assert(assembler);
    assert(buff);
    assert(size);

    ONDEBUGCMD(fprintf(Logfile, "\n"));

    return buff;
}

char *GetDouble(Assembler_t *assembler, char *buff, size_t *size)
{
    assert(assembler);
    assert(buff);
    assert(size);

    int offset = 0;
    sscanf(buff, " %d%n", assembler->bytecode, &offset);

    ONDEBUGCMD(fprintf(Logfile, "argument: %d\n", *assembler->bytecode));

    Bytecode_step
    return buff;
}

char *GetRegistr(Assembler_t *assembler, char *buff, size_t *size)
{
    assert(assembler);
    assert(buff);
    assert(size);

    int offset = 0;
    char regname[StrSize] = "";
    sscanf(buff, " %s%n", regname, &offset);

    NameReg(regname, assembler->bytecode);

    ONDEBUGCMD(fprintf(Logfile, "registr: %s\n", regname));

    Bytecode_step
    return buff;
}

char *GetLabel(Assembler_t *assembler, char *buff, size_t *size)
{
    assert(assembler);
    assert(buff);
    assert(size);

    char tag_name[StrSize] = "";
    int offset = 0;

    sscanf(buff, " :%s%n", tag_name, &offset);

    int address = CheckLabel(tag_name, assembler);

    if (address != NOLABELPTR)
    {
        *assembler->bytecode = address;
        ONDEBUGLBL(fprintf(Logfile, "label[:%s]: %d\n", tag_name, address));
    }

    else 
    {
        *assembler->bytecode = NOLABELPTR;
        ONDEBUGLBL(fprintf(Logfile, "label[:%s]: NoLabel\n", tag_name));
    }

    Bytecode_step
    return buff;
}

char *GetRam(Assembler_t *assembler, char *buff, size_t *size)
{
    assert(assembler);
    assert(buff);
    assert(size);

    char ram_reg[StrSize] = "";
    int offset = 0;
    sscanf(buff, " [%[^]]%n", ram_reg, &offset);

    NameReg(ram_reg, assembler->bytecode);
    offset++;

    ONDEBUGASM(fprintf(Logfile, "ram_reg: %s\n", ram_reg));
    
    Bytecode_step
    return buff;
}

size_t CalcHash(const char *str)
{
    assert(str);

    size_t hash = 0;
    size_t lenstr = strlen(str);
    size_t k = AlphabetSize;

    for (size_t i = 0; i < lenstr; i++)
    {
        hash += ((size_t) *(str + i));
        hash *= k;
    }

    return hash;
}

void NameReg(const char *regname, StackElement_t *bytecode)
{
    assert(regname);
    assert(bytecode);
    
    static Registr_t regs[Nregs] =   {
                                    {RAX_R,   0,   "RAX"},
                                    {RBX_R,   0,   "RBX"},
                                    {RCX_R,   0,   "RCX"}, 
                                    {RDX_R,   0,   "RDX"}, 
                                    {AX_R,    0,   "AX"}, 
                                    {BX_R,    0,   "BX"}, 
                                    {CX_R,    0,   "CX"}, 
                                    {DX_R,    0,   "DX"},
                                    {RAY_R,   0,   "RAY"},
                                    {RBY_R,   0,   "RBY"},
                                    {RCY_R,   0,   "RCY"}, 
                                    {RDY_R,   0,   "RDY"}, 
                                    {AY_R,    0,   "AY"}, 
                                    {BY_R,    0,   "BY"}, 
                                    {CY_R,    0,   "CY"}, 
                                    {DY_R,    0,   "DY"},
                                    };

    CheckHash(regs, Nregs);
    
    for (int i = 0; i < Nregs; i++)
    {
        if (CalcHash(regname) == regs[i].hash)
        {
            *bytecode = regs[i].number;
            break;    
        }
    } 
}

int CheckLabel(const char *tag_name, Assembler_t *assembler)
{
    assert(tag_name);
    assert(assembler);

    size_t tag_hash = CalcHash(tag_name);

    for (int i = 0; i < assembler->nlabels; i++)
    {
        if (tag_hash == assembler->labels[i].hash)
        {
            return assembler->labels[i].address;
        }
    }

    return NOLABELPTR;
}