#ifndef COMMANDS
#define COMMANDS

#include "registers.h"

enum progCommands {
    COMMAND_PUSH = 1,
    COMMAND_POP = 2,

    COMMAND_ADD = 3,
    COMMAND_SUB = 4,
    COMMAND_MUL = 5,
    COMMAND_DIV = 6,
    COMMAND_SIN = 7,
    COMMAND_COS = 8,
    COMMAND_LOG = 9,
    COMMAND_POW = 10,
    
    COMMAND_SQRT = 11,

    COMMAND_OUT = 12,
    COMMAND_IN = 13,

    COMMAND_JA = 14,
    COMMAND_JAE = 15,
    COMMAND_JE = 16,
    COMMAND_JMP = 17,
    COMMAND_JNE = 18,
    COMMAND_JBE = 19,
    COMMAND_JB = 20,

    COMMAND_CALL = 21,
    COMMAND_RET = 22,
    
    COMMAND_HLT = 0,
    NOT_COMMAND = -1
};

enum modifiers{
    NUMBER_MOD = 1,
    REGISTER_MOD = 2,
    RAM_MOD = 4,
};

#endif