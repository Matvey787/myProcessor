#ifndef SPU
#define SPU

#include <stdio.h>
#include "../../workWithStack/h_files/stackConstructor.h"
#include "../h_files/registers.h"

struct spu_t{
    char** code;
    size_t ip;
    stack_t stack;
    StackElem_t regData[NUMBER_OF_REGISTERS];
};

#endif