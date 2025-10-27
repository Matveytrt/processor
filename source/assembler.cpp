#include "assembler.h"

FILE *Logfile = fopen("filelisting.log", "w");

void SortHashCmd (Command_t *cmd);
int BinSearchHash(size_t str_hash, Command_t *cmd, int size);
int Cmp_int(const void *par1, const void *par2);
char *SkipComment (char *buff);

#define STARTSIZE info.buff_size / 2

int main(int argc, char *argv[])
{
    assert(argc == 2);
    assert(argv[1]);

    Assembler_t assembler = {};
    File_Info_t info = {};

    char *filename = argv[1];
    assert(filename);

    StrArrayCtor(filename, "r", &info);
    assert(info.str_data); 

    InitASM(&assembler, STARTSIZE);

    Assembling(&info, &assembler);
    DumpASM(assembler);

    ONDEBUGASM(fprintf(Logfile, "\n\nSecond going\n")); 
    Assembling(&info, &assembler);
    DumpASM(assembler);

    StrArrayDestroy(&info);
    DestroyASM(&assembler);

    return 0;
}

void InitASM(Assembler_t *assembler, size_t size)
{
    assert(assembler);

    assembler->bytecode_size = size;

    assembler->bytecode = (StackElement_t*) calloc(assembler->bytecode_size, sizeof(StackElement_t));
    assert(assembler->bytecode); 
    
    assembler->npasses = 0;

    assembler->labels = (Label_t*) calloc(LABELSIZE, sizeof(Label_t));
    assert(assembler->labels);

    for (int i = 0; i < LABELSIZE; i++)
    {
        assembler->labels[i].name = (char*) calloc(StrSize, sizeof(char));
        assert(assembler->labels[i].name);    
    }

    assembler->nlabels = 0;

    ONDEBUGASM(fprintf(Logfile, "Initialisation struct ASM success!\n"));
}

void Assembling(File_Info_t *info, Assembler_t *assembler)
{
    assert(info);
    assert(assembler);
    assert(assembler->bytecode);
    assert(assembler->labels);
    
    FILE *bytecodefile = fopen("bytecodefile.bin", "wb");                                                       // TODO: задавать файл через аргументы командной строки
    assert(bytecodefile);
    
    static Command_t cmd[Ncommands] = {
                                        {HLT_C,     "HLT",    0,   GetSingle},
                                        {POP_C,     "POP",    0,   GetSingle}, 
                                        {RET_C,     "RET",    0,   GetSingle},            
                                        {ADD_C,     "ADD",    0,   GetSingle}, 
                                        {SUB_C,     "SUB",    0,   GetSingle}, 
                                        {MUL_C,     "MUL",    0,   GetSingle}, 
                                        {DIV_C,     "DIV",    0,   GetSingle}, 
                                        {SQRT_C,    "SQRT",   0,   GetSingle},
                                        {IN_C,      "IN",     0,   GetSingle}, 
                                        {DUMP_C,    "DUMP",   0,   GetSingle}, 
                                        {CLEAR_C,   "CLEAR",  0,   GetSingle}, 
                                        {DRAW_C,    "DRAW",   0,   GetSingle},
                                        {OUT_C,     "OUT",    0,   GetSingle},
                                        {INIT_C,    "INIT",   0,   GetDouble},
                                        {PUSH_C,    "PUSH",   0,   GetDouble},
                                        {PUSHR_C,   "PUSHR",  0,   GetRegistr}, 
                                        {POPR_C,    "POPR",   0,   GetRegistr}, 
                                        {JB_C,      "JB",     0,   GetLabel}, 
                                        {JBE_C,     "JBE",    0,   GetLabel}, 
                                        {JA_C,      "JA",     0,   GetLabel}, 
                                        {JAE_C,     "JAE",    0,   GetLabel}, 
                                        {JE_C,      "JE",     0,   GetLabel}, 
                                        {JNE_C,     "JNE",    0,   GetLabel}, 
                                        {JMP_C,     "JMP",    0,   GetLabel},
                                        {CALL_C,    "CALL",   0,   GetLabel},
                                        {PUSHM_C,   "PUSHM",  0,   GetRam},
                                        {POPM_C,    "POPM",   0,   GetRam},
                                      };

    SortHashCmd(cmd);

    int ncommands = 0;
    size_t size = 0;
    int offset = 0;
    int label_index = 0;

    assembler->npasses++;
    ONDEBUGCMD(fprintf(Logfile, "cmd    || ncommand     || size      || cmd hash:              ||\n"));

    for (size_t nline = 0; nline < info->nlines; nline++) //nline
    {
        char str[StrSize] = "";

        char *cmd_str = info->str_data[nline].str_ptr;
        sscanf(cmd_str, " %s%n", str, &offset);

        size_t str_hash = CalcHash(str);
        
        if (*str != ';' && *str != ':' && *str != '\0')
        {
            int index = BinSearchHash(str_hash, cmd, Ncommands);

            if (index != NotCmd)
            {
                *assembler->bytecode = cmd[index].code;
                ncommands++;
                assembler->bytecode++;
                size++;
                cmd_str += offset;
                
                #ifdef DEBUGCMD
                fprintf(Logfile, "------------------------------------------------------------------------------------------------\n");
                fprintf(Logfile, "%-6s || command 0x%-3d || size 0x%-3ld || cmd hash: %-12ld || ", str, ncommands, size, cmd[index].hash);
                #endif
                
                cmd_str = cmd[index].ptr_func(assembler, cmd_str, &size);

                //break;
            }
        }
        
        else if (*str == ':')
        {
            cmd_str += offset;
            
            sscanf(str, ":%s", assembler->labels[label_index].name);
            
            assembler->labels[label_index].hash = CalcHash(assembler->labels[label_index].name);
            assembler->labels[label_index].address = (int) size;
            
            label_index++;
            if (assembler->npasses == 1) assembler->nlabels = label_index;
        }  
    }

    assembler->bytecode -= size;

    assembler->bytecode_size = size;
    assembler->ncommands = ncommands;

    fwrite(assembler->bytecode, sizeof(int), size, bytecodefile);
    fclose(bytecodefile);
}

void SortHashCmd (Command_t *cmd)
{
    assert(cmd);

    for (int i = 0; i < Ncommands; i++)                  
    {                                               
        if (cmd[i].hash == 0)                      
        {                                           
            cmd[i].hash = CalcHash(cmd[i].name);  
        }

        else                                        
        {                                           
            break;                                  
        }                                           
    }

    qsort(cmd, Ncommands, sizeof(cmd[0]), &Cmp_int);
}

int BinSearchHash(size_t str_hash, Command_t *cmd, int size)
{
    assert(cmd);

    int head = 0;
    int tail = size;
    while (head <= tail)
    {
        int mid = (tail + head) / 2;

        if (cmd[mid].hash == str_hash)
        {
            return mid;
        }

        else if (cmd[mid].hash > str_hash)
        {
            tail = mid - 1;    
        }

        else
        {
            head = mid + 1;
        }
        
    }

    return -1;
}

char* SkipComment(char* buff) 
{
    assert(buff);

    while (*buff != '\n' && *buff != '\0') buff++;

    return buff;
}

int Cmp_int(const void *par1, const void *par2)
{
    assert(par1);
    assert(par2);

    Command_t cmd_1 = *(const Command_t *) par1;
    Command_t cmd_2 = *(const Command_t *) par2;
    size_t hash_1 = cmd_1.hash;
    size_t hash_2 = cmd_2.hash;

    if (hash_1 > hash_2)
    {
        return 1;
    }

    else if (hash_1 == hash_2)
    {
        return 0;
    }

    else
    {
        return -1;
    }
}

void DumpASM (Assembler_t assembler)
{
    fprintf(Logfile, "\n-------------------------------------------------------------------------------------------------------\n");
    fprintf(Logfile, "info about assembling:\nsize = %zu, ncommands = %d\n", assembler.bytecode_size, assembler.ncommands);

    #ifdef DEBUGLBL
        fprintf(Logfile, "init %d labels\n", assembler.nlabels);

        for (int i = 0; i < assembler.nlabels; i++)
        {
            fprintf(Logfile, "label[%2d]: %-20s || hash = %-20ld || ip = %-d\n", i, assembler.labels[i].name, assembler.labels[i].hash, assembler.labels[i].address);
            fprintf(Logfile, "-------------------------------------------------------------------------------------------------------\n");    
        }
    #endif

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
    fclose(Logfile);
}