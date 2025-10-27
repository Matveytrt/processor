#include "spufuncs.h"

#define DO(sign)                                \
POP(spu->stk, val1)                             \
POP(spu->stk, val2)                             \
/*printf("ip = %d\n val1 = %d\n val2 = %d\n", spu->ip, val1, val2);*/ \
PUSH(spu->stk, val2 sign val1)                  \
spu->ip++; 

#define JMP(sign)                           \
int val1 = 0, val2 = 0;                     \
POP(spu->stk, val1)                         \
POP(spu->stk, val2)                         \
if (val1 sign val2)                         \
{                                           \
    spu->ip = spu->bytecode[spu->ip + 1];   \
}                                           \
else                                        \
{                                           \
    spu->ip += 2;                           \
}


void Push(Processor_t *spu)
{
    assert(spu);

    PUSH(spu->stk, spu->bytecode[spu->ip + 1]);

    spu->ip += 2;
}

void Pop(Processor_t *spu)
{
    assert(spu);

    int value = 0;
    POP(spu->stk, value);

    spu->ip++;
}

void Add(Processor_t *spu)
{
    assert(spu);

    int val1 = 0, val2 = 0;
    DO(+);
}

void Sub(Processor_t *spu)
{
    assert(spu);

    int val1 = 0, val2 = 0;
    DO(-);
}

void Mul(Processor_t *spu)
{
    assert(spu);

    int val1 = 0, val2 = 0;
    DO(*);
}

void Div(Processor_t *spu)
{
    assert(spu);

    int val1 = 0, val2 = 0;
    DO(/);
}

void Sqrt(Processor_t *spu)
{
    assert(spu);

    int value = 0;
    POP(spu->stk, value);
    PUSH(spu->stk, (StackElement_t) sqrt(value));

    spu->ip++;
}

void In(Processor_t *spu)
{
    assert(spu);

    int value = 0;
    scanf(" %d", &value);
    PUSH(spu->stk, value);

    spu->ip++;
}

void Dump(Processor_t *spu)
{
    assert(spu);

    DUMP(spu->stk);

    spu->ip++;    
}

void Clear(Processor_t *spu)
{
    assert(spu);

    int value = 0;
    while((spu->stk).size > 0)
    {  
        POP(spu->stk, value);
    }

    spu->ip++;
}

void Init(Processor_t *spu)
{
    assert(spu);

    DESTROY(spu->stk);
    INIT(spu->stk, spu->bytecode[spu->ip + 1]);

    spu->ip += 2;
}

void PushR(Processor_t *spu)
{
    assert(spu);

    PUSH(spu->stk, spu->regs[spu->bytecode[spu->ip + 1]]);

    spu->ip += 2;
}

void PopR(Processor_t *spu)
{
    assert(spu);

    POP(spu->stk, spu->regs[spu->bytecode[spu->ip + 1]]);

    spu->ip += 2;
}

void JB(Processor_t *spu)
{
    assert(spu);

    JMP(<);    
}

void JBE(Processor_t *spu)
{
    assert(spu);

    JMP(<=);
}

void JA(Processor_t *spu)
{
    assert(spu);

    JMP(>);
}

void JAE(Processor_t *spu)
{
    assert(spu);

    JMP(>=);
}

void JE(Processor_t *spu)
{
    assert(spu);

    JMP(==);
}

void JNE(Processor_t *spu)
{
    assert(spu);

    JMP(!=);
}

void JUMP(Processor_t *spu)
{
    assert(spu);

    spu->ip = spu->bytecode[spu->ip + 1];
}

void CALL(Processor_t *spu)
{
    assert(spu);

    PUSH(spu->retaddr, spu->ip + 2);

    spu->ip = spu->bytecode[spu->ip + 1];
}

void RET(Processor_t *spu)
{
    assert(spu);

    int address = 0;
    POP(spu->retaddr, address);

    spu->ip = address;    
}

void PUSHM(Processor_t *spu)
{
    assert(spu);

    PUSH(spu->stk, spu->ram[spu->regs[spu->bytecode[spu->ip + 1]]]);

    spu->ip += 2;
}

void POPM(Processor_t *spu)
{
    assert(spu);

    POP(spu->stk, spu->ram[spu->regs[spu->bytecode[spu->ip + 1]]]);

    spu->ip += 2;
}

/*void DRAW(Processor_t *spu)
{
    assert(spu);

    for (int i = 0; i < RamSize; i++)
    {
        fprintf(Logfile, "%c ", spu->ram[i]);
        if ((i + 1) % DrawStep == 0)
        {
            fprintf(Logfile, "\n");
        }
    }
    fprintf(Logfile, "\n");
    
    spu->ip++;
}*/

void OUTPUT(Processor_t *spu)
{
    assert(spu);

    int value;
    POP(spu->stk, value);
    printf("%d\n", value);

    spu->ip ++;
}

void DRAW(Processor_t *spu)
{
    assert(spu);
    //printf("x = %lg, y = %lg\n", x, y);
    //fprintf(Logfile, "first = %d, second = %d, third = %d\n", spu->ram[i], spu->ram[i + 1], spu->ram[i + 2]);
    txCreateWindow(DrawStep, DrawStep, false);

    for (int i = 0; i < RamSize; i += 3)
    {
        double y = (i / 3) / DrawStep;
        double x = (i / 3) % DrawStep;
        
        txSetPixel(x, y, RGB(spu->ram[i], spu->ram[i + 1], spu->ram[i + 2]));
    }
    //txSleep(100); 
    
    spu->ip++;
}
