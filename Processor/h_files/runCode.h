#ifndef RUNCOMMANDS
#define RUNCOMMANDS

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#include "spu.h"
#include "macros.h"

void runCode(spu_t* spu MYSBS(, size_t numberOfCommands));

#endif