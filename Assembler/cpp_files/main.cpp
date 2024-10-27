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

void myCallocOfCommands(command_t** commands, size_t numberOfStrings);
void myCallocOfLabels(labelsData_t* lData, size_t size);
const char c_default_asm_FileName[] = "../QUAD.ASM";

const short c_numOfLbls = 10;

int main(int argc, char* argv[]){
    char* buffer = nullptr;
    size_t numberOfStrings = 0;

    //files
    const char* fileName = nullptr;
    if (argc > 1){
        fileName = argv[argc - 1];
        
    } else {
        fileName = c_default_asm_FileName;
    }

    numberOfStrings = getFileStrings(&buffer, fileName); 
    char programCode_FileName[] = "../program_code.txt";
    char programBinaryCode_FileName[] = "../program_BinaryCode.txt";

    // read commmands from assembler
    
    

    // create array for commands
    command_t* commands = nullptr;
    myCallocOfCommands(&commands, numberOfStrings); // creation and initialization of commands 

    // create data for labels
    labelsData_t lData = {};
    myCallocOfLabels(&lData, c_numOfLbls);

    convertationStatuses firCompStatus = convertAsmToCommands(commands, buffer, numberOfStrings, c_default_asm_FileName, &lData, 1);
    convertationStatuses secCompStatus = convertAsmToCommands(commands, buffer, numberOfStrings, c_default_asm_FileName, &lData, 2);

    if (firCompStatus && secCompStatus){
        
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
    assert(commands != nullptr && "commands is nullptr in myCallocOfCommands");

    *commands = (command_t*)calloc(sizeof(command_t), numberOfStrings);

    for (size_t i = 0; i < numberOfStrings; i++){
        (*commands)[i].com  = NOT_COMMAND;
        (*commands)[i].reg  = NOT_REGISTER;
        (*commands)[i].num  = NAN;
        (*commands)[i].mode = -1;
    }
}

void myCallocOfLabels(labelsData_t* lData, size_t size){
    assert(lData != nullptr && "lData is nullptr in myCallocOfCommands");

    lData->labels = (label_t*)calloc(size, sizeof(label_t));
    lData->size = size;

    for (size_t i = 0; i < size; i++){
        lData->labels[i].name = nullptr; 
        lData->labels[i].value = -1; 
    }
}   
