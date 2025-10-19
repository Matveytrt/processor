#include "processor.h"

FILE *Logfile = fopen("proc.log", "w");
const char *GetName();

int main()
{
    Processor_t spu = {};  
 
    InitSPU(&spu);

    Processing(&spu);

    DestroySPU(&spu);
    //printf("ram[5] = %d, ram[3] = %d\n", spu.ram[5], spu.ram[3]);
    fclose(Logfile);
}

void InitSPU(Processor_t *spu)
{
    assert(spu);

    size_t buff_size = 0;
    spu->ip = 0;

    spu->bytecode = (StackElement_t*) ReadBuffResult(GetName(), "rb", &buff_size, sizeof(StackElement_t));
    assert(spu->bytecode);

    spu->regs = (StackElement_t*) calloc(Nregs, sizeof(StackElement_t));
    assert(spu->regs);

    spu->ram = (StackElement_t*) calloc(RamSize, sizeof(StackElement_t));
    assert(spu->ram);
    
    INIT(spu->retaddr, RetSize);

}

void Processing(Processor_t *spu)
{
    assert(spu);
    
    const CmdFunc_t cmd[Nfuncs] = {  
                                        {PUSH_C,    Push},
                                        {POP_C,     Pop}, 
                                        {ADD_C,     Add}, 
                                        {SUB_C,     Sub}, 
                                        {MUL_C,     Mul}, 
                                        {DIV_C,     Div}, 
                                        {SQRT_C,    Sqrt},
                                        {IN_C,      In}, 
                                        {DUMP_C,    Dump}, 
                                        {CLEAR_C,   Clear}, 
                                        {INIT_C,    Init}, 
                                        {PUSHR_C,   PushR}, 
                                        {POPR_C,    PopR}, 
                                        {JB_C,      JB}, 
                                        {JBE_C,     JBE}, 
                                        {JA_C,      JA}, 
                                        {JAE_C,     JAE}, 
                                        {JE_C,      JE}, 
                                        {JNE_C,     JNE}, 
                                        {JMP_C,     JUMP},
                                        {CALL_C,    CALL},
                                        {RET_C,     RET},
                                        {PUSHM_C,   PUSHM},
                                        {POPM_C,    POPM},
                                        {DRAW_C,    DRAW},
                                     };
    int command = spu->bytecode[spu->ip];
    INIT(spu->stk, STARTCAPACITY);
    
    while (command != HLT_C)
    {
        for(int i = 0; i < Nfuncs; i++)
        {
            if (command == cmd[i].code)
            {
                cmd[i].ptr_func(spu);
                //printf("command = %d, ip = %d\n", cmd[i].code, spu->ip);
                break;
            }
        }
        command = spu->bytecode[spu->ip];
        //printf("new command = %d, ip = %d\n", command, spu->ip);
    }

    DUMP(spu->stk);
    DESTROY(spu->stk);
}

void DestroySPU(Processor_t *spu)
{
    assert(spu);

    DESTROY(spu->retaddr);
    free(spu->ram);
    free(spu->regs);
    free(spu->bytecode);
}

const char *GetName()
{
    const char *file_name = "bytecodefile2.txt";
    assert(file_name);

    return file_name;    
}