#ifndef GETCOMMANDS
#define GETCOMMANDS

#include <stdio.h>

const short FIRST_LINES_INFO_OF_FILE = 3;

enum getComStatuses{
    COMMANDS_WAS_GETTED_SUCCSESSFULLY = 1,
    SOMETHING_GO_WRONG_WITH_GETTING_COMMANDS = 0
};

size_t getCommands(char*** code, char* buffer, const size_t numberOfStrings);

#endif