#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#include "../h_files/getFileStrings.h"
#include "../h_files/convertAsmToCommands.h"
#include "../h_files/runCommands.h"

#include "../h_files/commands.h"

#include "../workWithStack/h_files/stressTests.h"
#include "../workWithStack/h_files/stackConstructor.h"
#include "../workWithStack/h_files/stackPushPop.h"
#include "../workWithStack/h_files/errorNames.h"
#include "../workWithStack/h_files/stackInitDestroy.h"
#include "../workWithStack/h_files/stackDump.h"
#include "../workWithStack/h_files/macros.h"

int main(){

    // read commmands from assembler
    char* buffer = {};
    char assemblerFileName[30] = "PROGRAM.ASM";
    size_t numberOfStrings = getFileStrings(&buffer, assemblerFileName);

    // convert buffer to commands
    double* commands = (double*)calloc(sizeof(double), 20);
    convertAsmToCommands(&commands, buffer, numberOfStrings, assemblerFileName);


    //for (size_t i = 0; i < numberOfCommands; i++) printf("%lg \n", commands[i]);
    
    // start work with stack
    stack_t stack;
    MACRO_stackInit(&stack);

    runCommands(&stack, commands);

    free(commands);
    free(buffer);
    stackDestroy(&stack);

    return 0;
}
