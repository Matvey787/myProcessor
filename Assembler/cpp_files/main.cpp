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
#include "../h_files/label.h"
/* #include "../workWithStack/h_files/stressTests.h"
#include "../workWithStack/h_files/stackConstructor.h"
#include "../workWithStack/h_files/stackPushPop.h"
#include "../workWithStack/h_files/errorNames.h"
#include "../workWithStack/h_files/stackInitDestroy.h"
#include "../workWithStack/h_files/stackDump.h"
#include "../workWithStack/h_files/macros.h" */

void myCallocOfCommands(command_t** commands, size_t numberOfStrings);
void myCallocOfLabels(labelsData_t* lData, size_t size);

int main(){
    
    //files
    char assembler_FileName[] = "../PROGRAM.ASM";
    char programCode_FileName[] = "../program_code.txt";
    char programBinaryCode_FileName[] = "../program_BinaryCode.txt";

    // read commmands from assembler
    char* buffer = nullptr;
    size_t numberOfStrings = getFileStrings(&buffer, assembler_FileName);
    //printf("%lu\n", numberOfStrings);

    // convert buffer to commands
    command_t* commands = nullptr;
    labelsData_t lData;

    myCallocOfLabels(&lData, 10);

    myCallocOfCommands(&commands, numberOfStrings); // creation and initialization of commands 

    if (convertAsmToCommands(commands, buffer, numberOfStrings, assembler_FileName, &lData)){
        /* for (size_t i = 0; i < numberOfStrings; i++){
            printf("%d %d %lg\n", commands[i].com, commands[i].reg, commands[i].num);
        } */
        putDataToFileCode(commands, numberOfStrings, programCode_FileName);
        putDataToFileBinaryCode(commands, numberOfStrings, programBinaryCode_FileName);
    } else {

        printf("CONVERTATION FAILED\n");
    }

    free(commands);
    free(buffer);
    free(lData.labels);
    return 0;
}

void myCallocOfCommands(command_t** commands, size_t numberOfStrings){

    *commands = (command_t*)calloc(sizeof(command_t), numberOfStrings);

    for (size_t i = 0; i < numberOfStrings; i++){
        (*commands)[i].com = NOT_COMMAND;
        (*commands)[i].reg = NOT_REGISTER;
        (*commands)[i].num = NAN;
        (*commands)[i].mod = -1;
    }
}

void myCallocOfLabels(labelsData_t* lData, size_t size){

    lData->labels = (label_t*)calloc(size, sizeof(label_t));
    lData->size = size;

    for (size_t i = 0; i < size; i++){
        strcpy(lData->labels[i].name, "empty\0"); 
        lData->labels[i].value = -1; 
    }
}   
