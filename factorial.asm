IN
POPR AX ;n
PUSH 0
POPR BX ;result n!


    PUSHR AX
    :cycle
        PUSH 1
        PUSHR AX
        JA :Factorial
        POPR BX   
    PUSHR BX
    PUSH 0
    JE :IsZero

    PUSHR BX
    :end
    OUT
    HLT

:Factorial
    CALL :StepDown
    PUSHR AX
    MUL
    JMP :cycle

:StepDown ;n -= 1
    PUSHR AX
    PUSH 1
    SUB
    POPR AX
    RET

:IsZero
    PUSH 1
    JMP :end