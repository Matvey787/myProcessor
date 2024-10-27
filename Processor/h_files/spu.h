#ifndef SPU
#define SPU

#include <stdio.h>
#include "../../workWithStack/h_files/stackConstructor.h"
#include "../h_files/registers.h"

struct number_t{
    double dbl_num;
    int int_num;
};

struct spu_t{
    number_t* code;
    size_t codeLength;
    size_t ip;
    stack_t stack;
    StackElem_t regData[NUMBER_OF_REGISTERS];
    StackElem_t* RAM;
};

#endif