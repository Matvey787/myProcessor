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

    COMMAND_OUT = 7,
    COMMAND_IN = 8,

    COMMAND_JA = 11,
    COMMAND_JAE = 12,
    COMMAND_JE = 13,
    COMMAND_JMP = 14,

    COMMAND_HLT = 0,
    NOT_COMMAND = -1
};

enum modifiers{
    NUMBER_MOD = 1,
    REGISTER_MOD = 2,
    RAM_MOD = 4,
};
struct command_t {
    progCommands com;
    int mod;
    progRegisters reg;
    double num;
};

#endif