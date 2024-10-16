#ifndef STEPBYSTEPHEADER
#define STEPBYSTEPHEADER

#include "spu.h"

int stepByStep(spu_t* spu, int commandWithArg);
void writeCode(spu_t* spu, size_t numberOfCommands);

#endif