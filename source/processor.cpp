#include "processor.h"

FILE *logfile = fopen("logfile.log", "w");

const char *GetName();

int main()
{
    Processor cpu = {};    

    size_t buff_size = 0;
    StackElement_t regs_data[NREGS] = {};
    cpu.bytecode = (StackElement_t*) ReadBuffResult(GetName(), "rb", &buff_size, sizeof(StackElement_t));
    //printf("%d\n", cpu.bytecode[2]);
    assert(cpu.bytecode);
    Executor(cpu.bytecode, regs_data);


    free(cpu.bytecode);
}

const char *GetName()
{
    const char *file_name = "bytecodefile2.bin";
    assert(file_name);

    return file_name;    
}

void Executor(StackElement_t data[], StackElement_t regs_data[])
{
    assert(data);

    StackErr_t error = {};
    Stack_t stk = {};
    size_t ip = 0;
    
    INIT(stk, STARTCAPACITY);
    
    int command = data[ip];
    int val1, val2;
    int value;

    while (command != HLT_C)
    {
        //printf("%d\n", command);
        switch (command)
        {
        case PUSH_C:
            PUSH(stk, data[ip + 1]);
            
            ip += 2;
            break;
        case POP_C:
            POP(stk, value);
            
            ip++;
            break;
        case ADD_C:
            POP(stk, val1);
            POP(stk, val2);
            PUSH(stk, val1 + val2);
            
            ip++;
            break;
        case SUB_C:
            POP(stk, val1);
            POP(stk, val2);
            PUSH(stk, val2 - val1);

            ip++;
            break;
        case MUL_C:
            POP(stk, val1);
            POP(stk, val2);
            PUSH(stk, val1 * val2);

            ip++;
            break;
        case DIV_C:
            POP(stk, val1);
            POP(stk, val2);
            PUSH(stk, val2 / val1);

            ip++;
            break;
        case SQRT_C:
            POP(stk, value);
            PUSH(stk, (StackElement_t)sqrt(value));

            ip++;
            break;
        case IN_C:
            scanf("%d", &value);
            PUSH(stk, value);

            ip++;
            break;
        case DUMP_C:
            DUMP(stk);

            ip++;
            break;
        case CLEAR_C:
            while(stk.size > 0)
            {   
                POP(stk, value);
            }

            ip++;
            break; 
        case INIT_C:
            DESTROY(stk);
            INIT(stk, data[ip + 1]);

            ip += 2;
            break;
        case PUSHR_C:
            PUSH(stk, regs_data[data[ip + 1]]);

            ip += 2;
            break;
        case POPR_C:
            POP(stk, regs_data[data[ip + 1]]);

            ip += 2;
            break;
        case JB_C:
            POP(stk, val1);
            POP(stk, val2);
            if (val1 < val2)
            {
                ip = data[ip + 1];
            }
            else
            {
                ip += 2;
            }

            break;
        case JBE_C:
            POP(stk, val1);
            POP(stk, val2);
            if (val1 <= val2)
            {
                ip = data[ip + 1];
            }
            else
            {
                ip += 2;
            }

            break;
        case JA_C:
            POP(stk, val1);
            POP(stk, val2);
            if (val1 > val2)
            {
                ip = data[ip + 1];
            }
            else
            {
                ip += 2;
            }

            break;
        case JAE_C:
            POP(stk, val1);
            POP(stk, val2);
            if (val1 >= val2)
            {
                ip = data[ip + 1];
            }
            else
            {
                ip += 2;
            }

            break;
        case JE_C:
            POP(stk, val1);
            POP(stk, val2);
            if (val1 == val2)
            {
                ip = data[ip + 1];
            }
            else
            {
                ip += 2;
            }

            break;
        case JNE_C:
            POP(stk, val1);
            POP(stk, val2);
            if (val1 != val2)
            {
                ip = data[ip + 1];
            }
            else
            {
                ip += 2;
            }

            break;
        case JMP_C:
            
            ip = data[ip + 1];
            break;    
        default:
            break;
        }

        command = data[ip];  
    }
    DESTROY(stk);
}