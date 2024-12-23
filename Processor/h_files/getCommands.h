#ifndef GETCOMMANDS
#define GETCOMMANDS

#include <stdio.h>
#include "spu.h"

const short FIRST_LINES_INFO_OF_FILE = 3;

enum getComStatuses{
    COMMANDS_WAS_GETTED_SUCCSESSFULLY = 1,
    SOMETHING_GO_WRONG_WITH_GETTING_COMMANDS = 0
};

getComStatuses getCommands(spu_t* spu, char* buffer, const size_t sizeOfBuffer);

#endif