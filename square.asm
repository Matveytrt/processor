IN
POPR AX ;a
IN
POPR BX ;b
IN
POPR CX ;c

PUSHR AX
PUSH 0
JE :IsZeroA
JMP :IsZeroDiscr

:two_roots
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

    HLT ;2 roots

:IsZeroA
    PUSHR BX
    PUSH 0
    JE :IsZeroB

    PUSHR CX
    PUSHR BX
    DIV
    PUSH -1
    MUL
    OUT

    HLT ;1 root
    
:IsZeroB
    PUSHR CX
    PUSH 0
    JE :IsZeroC

    :no_roots
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
    JB :two_roots
    CALL :Discr
    PUSH 0
    JA :no_roots
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
    SQRT
    SUB
    PUSHR RAX
    DIV
    OUT
    RET

:SecondRoot
    PUSHR RBX
    CALL :Discr
    SQRT
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