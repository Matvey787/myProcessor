#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "../h_files/getFileStrings.h"
#include "../h_files/putDataToFile.h"
#include "../h_files/convertAsmToCommands.h"

#include "../h_files/commands.h"

/* #include "../workWithStack/h_files/stressTests.h"
#include "../workWithStack/h_files/stackConstructor.h"
#include "../workWithStack/h_files/stackPushPop.h"
#include "../workWithStack/h_files/errorNames.h"
#include "../workWithStack/h_files/stackInitDestroy.h"
#include "../workWithStack/h_files/stackDump.h"
#include "../workWithStack/h_files/macros.h" */

void myCallocOfCommands(command_t** commands, size_t numberOfStrings);

int main(){
    
    //files
    char assembler_FileName[] = "../PROGRAM.ASM";
    char programCode_FileName[] = "../program_code.txt";

    // read commmands from assembler
    char* buffer = nullptr;
    size_t numberOfStrings = getFileStrings(&buffer, assembler_FileName);
    
    // convert buffer to commands
    command_t* commands = nullptr;
    myCallocOfCommands(&commands, numberOfStrings); // creation and initialization of commands 

    if (convertAsmToCommands(commands, buffer, numberOfStrings, assembler_FileName)){
        
        putDataToFile(commands, numberOfStrings, programCode_FileName);

    } else {

        printf("CONVERTATION FAILED\n");
    }

    free(commands);
    free(buffer);

    return 0;
}

void myCallocOfCommands(command_t** commands, size_t numberOfStrings){

    *commands = (command_t*)calloc(sizeof(command_t), numberOfStrings);

    for (size_t i = 0; i < numberOfStrings; i++){
        (*commands)[i].com = NOT_COMMAND;
        (*commands)[i].reg = NOT_REGISTER;
        (*commands)[i].num = NAN;
    }
}
