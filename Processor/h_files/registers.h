#ifndef REGISTERS
#define REGISTERS

const short NUMBER_OF_REGISTERS = 5;

enum progRegisters {
    AX = 0,
    BX = 1,
    CX = 2, 
    DX = 3,
    FS = 4, /// function start (used for function stack frame)
    NOT_REGISTER = -1
};

#endif