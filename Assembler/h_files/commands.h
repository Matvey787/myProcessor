#ifndef COMMANDS
#define COMMANDS

#include "registers.h"

#define DEF_CMD_(name, num)\
    COMMAND_##name = num,

enum progCommands {
    #include "defCommands.h"
    NOT_COMMAND = -1
};
#undef DEF_CMD_

enum modifiers{
    NUMBER_MOD = 1,
    REGISTER_MOD = 2,
    RAM_MOD = 4,
};
struct command_t {
    progCommands com;
    int mode;
    progRegisters reg;
    double num;
};

#endif