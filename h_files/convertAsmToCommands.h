#ifndef CONVERTASMTOCOMMANDS
#define CONVERTASMTOCOMMANDS

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

size_t convertAsmToCommands(double** commands, const char* buffer, const size_t numberOfStrings, const char* asmFileName);

#endif