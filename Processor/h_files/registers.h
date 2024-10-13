#ifndef REGISTERS
#define REGISTERS

const short NUMBER_OF_REGISTERS = 4;

enum progRegisters {
    AX = 0,
    BX = 1,
    CX = 2, 
    DX = 3,
    NOT_REGISTER = -1
};

#endif