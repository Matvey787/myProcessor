#ifndef CONVERTASMTOCOMMANDS
#define CONVERTASMTOCOMMANDS

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "commands.h"

enum convertationStatuses{
    CONVERTATION_SUCCESS = 1,
    CONVERTATION_FAIL = 0
};

convertationStatuses convertAsmToCommands(command_t* commands, char* buffer, const size_t numberOfStrings, const char* asmFileName);

#endif