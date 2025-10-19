#include "assembler.h"

FILE *Logfile = fopen("assembler.log", "w");

void RegName(char *regname, StackElement_t *bytecode);
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

    assembler->labels = (int*) calloc(LABELSIZE, sizeof(int));
    assert(assembler->labels);
    memset(assembler->labels, NOLABELPTR, sizeof(int));
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
    
    size_t ncommands = 0;
    size_t size = 0;
    int offset = 0, argtype = 0;

    const Command_t cmd[Ncommands] = {  {HLT_C,     "HLT",       END_T},
                                        {INIT_C,    "INIT",      DOUBLE_T}, 
                                        {PUSH_C,    "PUSH",      DOUBLE_T},
                                        {POP_C,     "POP",       SINGLE_T}, 
                                        {ADD_C,     "ADD",       SINGLE_T}, 
                                        {SUB_C,     "SUB",       SINGLE_T}, 
                                        {MUL_C,     "MUL",       SINGLE_T}, 
                                        {DIV_C,     "DIV",       SINGLE_T}, 
                                        {SQRT_C,    "SQRT",      SINGLE_T},
                                        {IN_C,      "IN",        SINGLE_T}, 
                                        {DUMP_C,    "DUMP",      SINGLE_T}, 
                                        {CLEAR_C,   "CLEAR",     SINGLE_T}, 
                                        {DRAW_C,    "DRAW",      SINGLE_T},
                                        {PUSHR_C,   "PUSHR",     REGISTR_T}, 
                                        {POPR_C,    "POPR",      REGISTR_T}, 
                                        {JB_C,      "JB",        LBL_T}, 
                                        {JBE_C,     "JBE",       LBL_T}, 
                                        {JA_C,      "JA",        LBL_T}, 
                                        {JAE_C,     "JAE",       LBL_T}, 
                                        {JE_C,      "JE",        LBL_T}, 
                                        {JNE_C,     "JNE",       LBL_T}, 
                                        {JMP_C,     "JMP",       LBL_T},
                                        {CALL_C,    "CALL",      LBL_T},
                                        {RET_C,     "RET",       RET_T},
                                        {PUSHM_C,   "PUSHM",     RAM_T},
                                        {POPM_C,    "POPM",      RAM_T},
                                     };

    while (1)
    {
        char str[StrSize] = "";
        int value = sscanf(buff, " %s%n", str, &offset);
        int tag_num = NOLABELPTR;

        for (int i = 0; i < Ncommands; i++)
        {
            if (strcmp(cmd[i].name, str) == 0)
            {
                *(assembler->bytecode) = cmd[i].code;
                argtype = cmd[i].argtype;
                ncommands++;

                ONDEBUGCMD(fprintf(Logfile, "%s command #%ld\n", str, ncommands));
                //ONDEBUGCMD(printf("%s command #%ld\n", str, ncommands));
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
                    //ONDEBUGCMD(printf("cmd reg name = %s\n", regname));
                    RegName(regname, assembler->bytecode);

                    Bytecode_step
                }
                else if (argtype == LBL_T)
                {
                    //printf("%s\n", cmd[i].name);
                    value = sscanf(buff, " :%d%n", &tag_num, &offset);
                    if (assembler->labels[tag_num] != NOLABELPTR)
                    {
                        *assembler->bytecode = assembler->labels[tag_num];
                        ONDEBUGLBL(fprintf(Logfile, "label[:%d] = %d\n", tag_num, assembler->labels[tag_num]));
                    }
                    else 
                    {
                        *assembler->bytecode = NOLABELPTR;
                        //printf("nolable\n");
                    }

                    Bytecode_step
                }
                else if (argtype == RAM_T)
                {
                    char ram_reg[StrSize] = "";
                    value = sscanf(buff, " [%[^]]%n", ram_reg, &offset);
                    ONDEBUGASM(fprintf(Logfile, "ram_regname = %s\n", ram_reg));

                    offset += Brackets;
                    RegName(ram_reg, assembler->bytecode);

                    Bytecode_step
                }
                break;
            }
        }

        if (*str == ';')
        {
            //printf("comment = %s\n", str);
            buff = SkipComment(buff);   
        }

        if (*str == ':')
        {
            buff += offset;

            value = sscanf(str, ":%d", &tag_num);
            ONDEBUGLBL(fprintf(Logfile, "defined tag_num = :%d\n", tag_num));

            assembler->labels[tag_num] = (int) size;
        }

        if (*str == '\0')
        {
            ONDEBUGASM(fprintf(Logfile, "info about assembling:\nsize = %ld, ncommands = %ld\n\n\n", size, ncommands));
            break;
        }
    }
    assembler->bytecode -= size;

    fwrite(assembler->bytecode, sizeof(int), size, bytecodefile);

    fclose(bytecodefile);
    return ncommands;
}

void RegName(char *regname, StackElement_t *bytecode)
{
    assert(regname);
    assert(bytecode);
    
    const Registr_t reg[Nregs] =    {
                                    {RAX,    "RAX"},
                                    {RBX,    "RBX"},
                                    {RCX,    "RCX"}, 
                                    {RDX,    "RDX"}, 
                                    {AX,     "AX"}, 
                                    {BX,     "BX"}, 
                                    {CX,     "CX"}, 
                                    {DX,     "DX"},
                                    {RAY,    "RAY"},
                                    {RBY,    "RBY"},
                                    {RCY,    "RCY"}, 
                                    {RDY,    "RDY"}, 
                                    {AY,     "AY"}, 
                                    {BY,     "BY"}, 
                                    {CY,     "CY"}, 
                                    {DY,     "DY"},
                                    };
    
    for (int i = 0; i < Nregs; i++)
    {
        if (strcmp(reg[i].name, regname) == 0)
        {
            *bytecode = reg[i].number;
            break;    
        }
    } 
    
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

    free(assembler->bytecode);
    free(assembler->labels);
}