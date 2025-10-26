PUSH 255
POPR RAX ;Red
PUSH 255
POPR RBX ;Green
PUSH 255
POPR RAY ;Blue

PUSH 270000
POPR RCX ;ramsize
PUSH 300
POPR RCY ;ramstep

IN
POPR RDX ;input R
IN
POPR AX ;input x0
IN
POPR AY ;input y0
IN
POPR DX ;flat coef x (probably 5)
IN
POPR DY ;flat coef y (probably 6)

PUSH 0
POPR BX ;ram index
PUSH 0
POPR CX ;start x
PUSH 0
POPR CY ;start y

DUMP ;check

:cycle
    PUSHR RCX ;ramsize
    PUSHR BX ;index
    JB :Fill_ram
    DRAW

    HLT

:(x-xo)^2
    PUSHR AX
    PUSHR CX
    SUB
    PUSH 1
    ADD
    PUSHR AX
    PUSHR CX
    SUB
    PUSH 1
    ADD
    MUL
    RET

:(y-yo)^2
    PUSHR AY
    PUSHR CY
    SUB
    PUSH 1
    ADD
    PUSHR AY
    PUSHR CY
    SUB
    PUSH 1
    ADD
    MUL
    RET

:(R^2-r^2)
    PUSHR RDX ;R
    PUSHR RDX
    MUL
    PUSHR DX
    PUSHR DY
    MUL
    MUL

    CALL :(y-yo)^2
    PUSHR DY
    PUSHR DY
    MUL
    MUL

    CALL :(x-xo)^2
    PUSHR DX
    PUSHR DX
    MUL
    MUL

    ADD
    
    SUB
    RET

:Get_xy
    PUSHR BX ;index
    PUSH 3 ;RGB
    DIV
    PUSHR RCY ;ram step
    DIV
    POPR CY ;get y

    PUSHR BX ;index
    PUSH 3
    DIV
    PUSHR CY ;y
    PUSHR RCY ;ram step
    MUL
    SUB
    POPR CX ;get x

    RET

:Next_step
    PUSHR BX ;ram_index
    PUSH 3
    ADD
    POPR BX
    RET ;ram_index += 3
:Next_colour
    PUSHR BX
    PUSH 1
    ADD
    POPR BX
    RET

:Fill_ram
    CALL :Get_xy
    CALL :(R^2-r^2)
    PUSH 0
    JBE :(R^2-r^2<0)

    PUSHR RAX ;red
    POPM [BX]

    PUSHR RBX ;green
    CALL :Next_colour
    POPM [BX]

    PUSH 0 ;blue
    CALL :Next_colour
    POPM [BX]

    :fill_cycle
    CALL :Next_colour
    JMP :cycle

:(R^2-r^2<0)
    PUSHR RAX ;red
    POPM [BX]
    CALL :Next_colour

    PUSH 0 ;green
    POPM [BX]
    CALL :Next_colour

    PUSH 0 ;blue
    POPM [BX]

    JMP :fill_cycle