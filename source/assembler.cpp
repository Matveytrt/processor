#include "assembler.h"

const char *GetName();
void RegCode(char *buff, int *offset, StackElement_t *bytecode);

int main()
{
    Processor cpu  = {};
    size_t buff_size = 0;
    const int labelsize = LABELSIZE; 

    int label[labelsize] = {-1};

    char *buff = (char*) ReadBuffResult(GetName(), "r", &buff_size, sizeof(char));
    assert(buff);

    size_t bytecode_size = STARTSIZE;
    cpu.bytecode = (StackElement_t*) calloc(bytecode_size, sizeof(StackElement_t));
    assert(cpu.bytecode); 

    cpu.ncommands = Assembler(buff, cpu.bytecode, label); 
    Assembler(buff, cpu.bytecode, label);

    free(cpu.bytecode);
    free(buff);
    return 0;
}

size_t Assembler(char *buff, StackElement_t *bytecode, int *label)
{
    assert(buff != NULL);
    assert(bytecode != NULL);
    FILE *bytecodefile = fopen("bytecodefile2.txt", "wb");
    assert(bytecodefile);
    
    size_t ncommands = 0;
    int count = 0, offset = 0, argtype = 0;
    size_t size = 0;

    Command cmd[NCOMMANDS] = {  {HLT_C,     "HLT",       END_T},
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
                                {INIT_C,    "INIT",      DOUBLE_T}, 
                                {PUSHR_C,   "PUSHR",     REGISTR_T}, 
                                {POPR_C,    "POPR",      REGISTR_T}, 
                                {JB_C,      "JB",        JMP_T}, 
                                {JBE_C,     "JBE",       JMP_T}, 
                                {JA_C,      "JA",        JMP_T}, 
                                {JAE_C,     "JAE",       JMP_T}, 
                                {JE_C,      "JE",        JMP_T}, 
                                {JNE_C,     "JNE",       JMP_T}, 
                                {JMP_C,     "JMP",       JMP_T}
                             };
    
    while (1)
    {
        int tag_num = 0;
        char str[10] = "";
        sscanf(buff, " %s%n", str, &offset);
        
        for (int i = 0; i < NCOMMANDS; i++)
        {
            if (strcmp(cmd[i].name, str) == 0)
            {
                *bytecode = cmd[i].code;
                argtype = cmd[i].argtype;
                ncommands++;
                STEP

                if (argtype == DOUBLE_T)
                {
                    count = sscanf(buff, " %d%n", bytecode, &offset);
                    if (count == 0)
                    {
                        bytecode[size - 1] = ERROR;    
                    }
                    STEP
                }
                else if (argtype == REGISTR_T)
                {
                    RegCode(buff, &offset, bytecode);
                    STEP
                }
                else if (argtype == JMP_T)
                {
                    sscanf(buff, " %d%n", &tag_num, &offset);
                    if (label[tag_num] != NOLABELPTR)
                    {
                        *bytecode = label[tag_num];
                    }
                    else 
                    {
                        *bytecode = NOLABELPTR;
                        printf("nolable\n");
                    }
                    STEP
                }
                break;
            }
        }

        if (str[0] == ':')
        {
            //printf("%s\n", str);
            buff += offset;
            sscanf(str, "%d", &tag_num);
            label[tag_num] = (int) size;
        }

        if (argtype == END_T)
        {
            break;
        }
    }
    bytecode -= size;

    fwrite(bytecode, sizeof(int), size, bytecodefile);

    fclose(bytecodefile);
    return ncommands;
}

const char *GetName()
{
    const char *file_name = "command.asm";
    return file_name;    
}

void RegCode(char *buff, int *offset, StackElement_t *bytecode)
{
    char reg[10] = "";
    sscanf(buff, " %s%n", reg, offset);

    if (strcmp(reg, "RAX") == 0)        
    {                                   
        *bytecode = RAX;                
    }                                  
    else if (strcmp(reg, "RBX") == 0)   
    {                                   
        *bytecode = RBX;                
    }                                   
    else if (strcmp(reg, "RCX") == 0)   
    {                                   
        *bytecode = RCX;                
    }                                   
    else if (strcmp(reg, "RDX") == 0)   
    {                                   
        *bytecode = RDX;                
    }                                   
    else if (strcmp(reg, "AX") == 0)    
    {                                   
        *bytecode = AX;                 
    }                                   
    else if (strcmp(reg, "BX") == 0)    
    {                                   
        *bytecode = BX;                 
    }                                   
    else if (strcmp(reg, "CX") == 0)    
    {                                   
        *bytecode = CX;                 
    }                                   
    else if (strcmp(reg, "DX") == 0)    
    {                                   
        *bytecode = DX;                 
    }                                   
    else                                
    {                                   
        *bytecode = ERROR;              
    } 
}