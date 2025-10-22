IN
POPR AX
IN
POPR BX
IN
POPR CX

PUSHR AX
PUSH 0
JE :IsZeroA
JMP :IsZeroDiscr
:2roots
    PUSHR BX
    PUSH -1
    MUL
    POPR RBX

    PUSHR AX
    PUSH 2
    MUL
    POPR RAX

    CALL :FirstRoot
    CALL :SecondRoot
    HLT ;2roots

:IsZeroA
    PUSHR BX
    PUSH 0
    JE :IsZeroB

    PUSHR CX
    PUSHR BX
    DIV
    OUT
    HLT ;1root
    
:IsZeroB
    PUSHR CX
    PUSH 0
    JE :IsZeroC

    PUSH -1
    OUT
    HLT ;NO roots

:IsZeroC
    PUSH 8 
    OUT
    HLT ;infinity roots

:IsZeroDiscr
    CALL :Discr
    PUSH 0
    JNE :2roots
    PUSHR BX
    PUSHR AX
    DIV
    PUSH -2
    DIV
    OUT
    HLT ;1root

:FirstRoot
    PUSHR RBX
    CALL :Discr
    SUB
    PUSHR RAX
    DIV
    OUT
    RET

:SecondRoot
    PUSHR RBX
    CALL :Discr
    ADD
    PUSHR RAX
    DIV
    OUT
    RET

:Discr
    PUSHR BX
    PUSHR BX
    MUL
    PUSH -4
    PUSHR AX
    PUSHR CX
    MUL
    MUL
    ADD
    RET