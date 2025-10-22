#include "assembler.h"

FILE *Logfile = fopen("assembler.log", "w");

size_t CalcHash(const char *str);
void GetRegName(char *regname, StackElement_t *bytecode);
int GetLabel(char *tag_name, Assembler_t *assembler);
const char *SkipComment(const char *buff);

int main(int argc, char *argv[])
{
    Assembler_t assembler = {};
    size_t buff_size = 0;
    char *filename = argv[1];
    assert(filename);

    char *buff = (char*) ReadBuffResult(filename, "r", &buff_size, sizeof(char));
    assert(buff);

    assembler.bytecode_size = STARTSIZE;
    InitASM(&assembler);

    Assembling(buff, &assembler); 
    Assembling(buff, &assembler);

    DestroyASM(&assembler);

    free(buff);
    fclose(Logfile);
    return 0;
}

void InitASM(Assembler_t *assembler)
{
    assert(assembler);

    assembler->bytecode = (StackElement_t*) calloc(assembler->bytecode_size, sizeof(StackElement_t));
    assert(assembler->bytecode); 
    
    assembler->labels = (Label_t*) calloc(LABELSIZE, sizeof(Label_t));
    assert(assembler->labels);
    for (int i = 0; i < LABELSIZE; i++)
    {
        assembler->labels[i].name = (char*) calloc(StrSize, sizeof(char));
        assert(assembler->labels[i].name);    
    }

    assembler->nlabels = 0;

    printf("Initialisation success!\n");
}

size_t Assembling(const char *buff, Assembler_t *assembler)
{
    assert(assembler);
    assert(assembler->bytecode);
    assert(assembler->labels);
    assert(buff);
    
    FILE *bytecodefile = fopen("bytecodefile2.txt", "wb");                                                       // TODO: задавать файл через аргументы командной строки
    assert(bytecodefile);
    
    static Command_t cmd[Ncommands] = { {HLT_C,     "HLT",    0,   END_T},
                                        {INIT_C,    "INIT",   0,   DOUBLE_T}, 
                                        {PUSH_C,    "PUSH",   0,   DOUBLE_T},
                                        {POP_C,     "POP",    0,   SINGLE_T}, 
                                        {ADD_C,     "ADD",    0,   SINGLE_T}, 
                                        {SUB_C,     "SUB",    0,   SINGLE_T}, 
                                        {MUL_C,     "MUL",    0,   SINGLE_T}, 
                                        {DIV_C,     "DIV",    0,   SINGLE_T}, 
                                        {SQRT_C,    "SQRT",   0,   SINGLE_T},
                                        {IN_C,      "IN",     0,   SINGLE_T}, 
                                        {DUMP_C,    "DUMP",   0,   SINGLE_T}, 
                                        {CLEAR_C,   "CLEAR",  0,   SINGLE_T}, 
                                        {DRAW_C,    "DRAW",   0,   SINGLE_T},
                                        {OUT_C,     "OUT",    0,   SINGLE_T},
                                        {PUSHR_C,   "PUSHR",  0,   REGISTR_T}, 
                                        {POPR_C,    "POPR",   0,   REGISTR_T}, 
                                        {JB_C,      "JB",     0,   LBL_T}, 
                                        {JBE_C,     "JBE",    0,   LBL_T}, 
                                        {JA_C,      "JA",     0,   LBL_T}, 
                                        {JAE_C,     "JAE",    0,   LBL_T}, 
                                        {JE_C,      "JE",     0,   LBL_T}, 
                                        {JNE_C,     "JNE",    0,   LBL_T}, 
                                        {JMP_C,     "JMP",    0,   LBL_T},
                                        {CALL_C,    "CALL",   0,   LBL_T},
                                        {RET_C,     "RET",    0,   RET_T},
                                        {PUSHM_C,   "PUSHM",  0,   RAM_T},
                                        {POPM_C,    "POPM",   0,   RAM_T},
                                      };

    CheckHash(cmd, Ncommands);
    size_t ncommands = 0;
    size_t size = 0;
    int offset = 0, argtype = 0;
    int address = 0;
    

    while (1)
    {
        
        char str[StrSize] = "";
        char tag_name[StrSize] = "";

        int value = sscanf(buff, " %s%n", str, &offset);
        size_t str_hash = CalcHash(str);
        
        
        if (*str != ';' && *str != ':' && *str != '\0')
        {
            for (int i = 0; i < Ncommands; i++)
                {
                    if (cmd[i].hash == str_hash)
                    {
                        *(assembler->bytecode) = cmd[i].code;
                        argtype = cmd[i].argtype;
                        ncommands++;
                        ONDEBUGCMD(fprintf(Logfile, "%s command #%ld\n", str, ncommands));
                        ONDEBUGCMD(fprintf(Logfile, "hash = %ld,\n", cmd[i].hash));
                        Bytecode_step

                        if (argtype == DOUBLE_T)
                        {
                            value = sscanf(buff, " %d%n", assembler->bytecode, &offset);
                            ONDEBUGCMD(fprintf(Logfile, "cmd second value = %d\n", *assembler->bytecode));
                            Bytecode_step
                        }

                        else if (argtype == REGISTR_T)
                        {
                            char regname[StrSize] = "";
                            value = sscanf(buff, " %s%n", regname, &offset);
                            ONDEBUGCMD(fprintf(Logfile, "cmd reg name = %s\n", regname));

                            GetRegName(regname, assembler->bytecode);
                            Bytecode_step
                        }

                        else if (argtype == LBL_T)
                        {
                            value = sscanf(buff, " :%s%n", tag_name, &offset);

                            address = GetLabel(tag_name, assembler);

                            if (address != NOLABELPTR)
                            {
                                *assembler->bytecode = address;
                                ONDEBUGLBL(fprintf(Logfile, "label[:%s] = %d\n", tag_name, address));
                            }

                            else 
                            {
                                *assembler->bytecode = NOLABELPTR;
                                ONDEBUGLBL(fprintf(Logfile, "label[:%s] = %d\n", tag_name, address));
                            }

                            Bytecode_step
                        }

                        else if (argtype == RAM_T)
                        {
                            char ram_reg[StrSize] = "";
                            value = sscanf(buff, " [%[^]]%n", ram_reg, &offset);
                            offset++;
                            ONDEBUGASM(fprintf(Logfile, "ram_regname = %s\n", ram_reg));
                            
                            GetRegName(ram_reg, assembler->bytecode);
                            Bytecode_step
                        }

                        break;
                    }
                }
        }

        else if (*str == ';')
        {
            buff = SkipComment(buff);   
        }
        
        else if (*str == ':')
        {
            buff += offset;
            
            value = sscanf(str, ":%s", assembler->labels[assembler->nlabels].name);
            ONDEBUGLBL(fprintf(Logfile, "defined tag_name = :%s\n", assembler->labels[assembler->nlabels].name));
            
            assembler->labels[assembler->nlabels].hash = CalcHash(assembler->labels[assembler->nlabels].name);
            assembler->labels[assembler->nlabels].address = (int) size;
            assembler->nlabels++;
        }
        
        else if (*str == '\0')
        {
            ONDEBUGASM(fprintf(Logfile, "info about assembling:\nsize = %ld, ncommands = %ld\n\n\n", size, ncommands));
            break;
        }

        else
        {
            ONDEBUGASM(fprintf(Logfile, "read Ncommands = #%ld\n can't reach the end of file, %s\n", ncommands, __FILE__));
            break;
        }
        
        fprintf(Logfile, "\n");
    }

    assembler->bytecode -= size;
    fwrite(assembler->bytecode, sizeof(int), size, bytecodefile);
    
    fclose(bytecodefile);
    return ncommands;  
}

void GetRegName(char *regname, StackElement_t *bytecode)
{
    assert(regname);
    assert(bytecode);
    
    static Registr_t regs[Nregs] =   {
                                    {RAX,   0,  "RAX"},
                                    {RBX,   0,  "RBX"},
                                    {RCX,   0,  "RCX"}, 
                                    {RDX,   0,  "RDX"}, 
                                    {AX,    0,  "AX"}, 
                                    {BX,    0,  "BX"}, 
                                    {CX,    0,  "CX"}, 
                                    {DX,    0,  "DX"},
                                    {RAY,   0,  "RAY"},
                                    {RBY,   0,  "RBY"},
                                    {RCY,   0,  "RCY"}, 
                                    {RDY,   0,  "RDY"}, 
                                    {AY,    0,  "AY"}, 
                                    {BY,    0,  "BY"}, 
                                    {CY,    0,  "CY"}, 
                                    {DY,    0,  "DY"},
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

int GetLabel(char *tag_name, Assembler_t *assembler)
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

size_t CalcHash(const char *str)
{
    assert(str);

    size_t hash = 0;
    int lenstr = strlen(str);
    size_t k = AlphabetSize;

    for (int i = 0; i < lenstr; i++)
    {
        hash += ((size_t)*(str + i));
        hash *= k;
    }

    return hash;
}

const char* SkipComment(const char* buff) 
{
    assert(buff);

    while (*buff != '\n' && *buff != '\0') buff++;

    return buff;
}

void DestroyASM(Assembler_t *assembler)
{
    assert(assembler);

    for (int i = 0; i < LABELSIZE; i++)
    {
        free(assembler->labels[i].name);    
    }
    free(assembler->bytecode);
    free(assembler->labels);
}