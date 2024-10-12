#ifndef PUTDATATOFILE
#define PUTDATATOFILE

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "commands.h"

void putDataToFile(command_t* commands, size_t length, const char* file_name);

#endif