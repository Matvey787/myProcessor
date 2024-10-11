#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#include "../h_files/getFileStrings.h"
#include "../h_files/putDataToFile.h"
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
    //files
    char assembler_FileName[30] = "PROGRAM.ASM";
    char programCode_FileName[30] = "program_code.txt";

    // read commmands from assembler
    char* buffer = {};
    size_t numberOfStrings = getFileStrings(&buffer, assembler_FileName);
    
    // convert buffer to commands
    double* commands = (double*)calloc(sizeof(double), 50);

    size_t lengthOfCommands = 0;

    if ((lengthOfCommands = convertAsmToCommands(&commands, buffer, numberOfStrings, assembler_FileName)) != 0){
        
        putDataToFile(commands, lengthOfCommands, programCode_FileName);

        //for (size_t i = 0; i < lengthOfCommands; i++) printf("%lg \n", commands[i]);
        
        // start work with stack
        stack_t stack;
        MACRO_stackInit(&stack);

        runCommands(&stack, commands);
        stackDestroy(&stack);

    }

    free(commands);
    free(buffer);

    return 0;
}
