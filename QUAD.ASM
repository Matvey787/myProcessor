PUSH 1
POP [0]
PUSH 100
POP [1]
PUSH 4
POP [2]
    PUSH [1]
    PUSH [1]
    MUL
    PUSH 4
    PUSH [0]
    PUSH [2]
    MUL
    MUL
    SUB
    POP DX
PUSH -1
PUSH [1]
MUL
POP [4]
    PUSH 2
    PUSH [0]
    MUL
    POP [5]
PUSH DX
PUSH 0
JA AZ:
JE EZ:
JMP SPR:
AZ:
JMP FS:
EZ:
JMP SS:
FS:
    PUSH DX
    SQRT
    PUSH [4]
    ADD
    PUSH [5]
    DIV
    OUT
    JMP SS:
SS:
    PUSH [4]
    PUSH DX
    SQRT
    SUB
    PUSH [5]
    DIV
    OUT
    JMP SPR:
SPR:
HLT
