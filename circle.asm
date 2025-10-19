PUSH 35 ;BY DY RDY RAY RBY - free
POPR RAX ;#
PUSH 46
POPR RBX ;.
PUSH 900
POPR RCX ;ramsize
PUSH 30
POPR RCY ;ramstep
IN
POPR RDX ;input R
IN
POPR AX ;input x0
IN
POPR AY ;input y0
PUSH 0
POPR BX ;ram index
PUSH 0
POPR CX ;x
PUSH 0
POPR CY ;y
DUMP

:9 ;do while ram_index < ramsize
PUSHR RCX ;ramsize
PUSHR BX ;index
JB :6 ;if ram_index < ramsize
DRAW ;get circle
HLT

:1 ;(x-xo)^2
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

:2 ;(y-yo)^2
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

:3 ;R^2 - r^2
PUSHR RDX ;R
PUSHR RDX
MUL
CALL :2 ;(y-yo)^2
CALL :1 ;(x-xo)^2
ADD
SUB
RET

:4 ;get x y
PUSHR BX ;index
PUSHR RCY ;ram step
DIV
POPR CY ;get y
PUSHR BX ;index
PUSHR CY ;y
PUSHR RCY ;ram step
MUL
SUB
POPR CX ;get x
RET

:5 ;step
PUSHR BX ;ram_index
PUSH 1
ADD
POPR BX
RET ;ram_index + 1

:6 ;fill ram
CALL :4 ;get x y
CALL :3 ;R^2 - r^2
PUSH 0
JBE :7 ;R^2 - r^2 < 0
PUSHR RBX ;fill [.]
POPM [BX]
:8 ;end of fill
CALL :5 ;next step
JMP :9

:7
PUSHR RAX ;fill [#]
POPM [BX]
JMP :8