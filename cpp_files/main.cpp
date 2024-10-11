#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#include "../h_files/getFileStrings.h"
#include "../h_files/convertAsmToCommands.h"


int main(){

    // read commmands from assembler
    char* buffer = {};
    char assemblerFileName[30] = "PROGRAM.ASM";
    size_t numberOfStrings = getFileStrings(&buffer, assemblerFileName);

    // convert buffer to commands
    double* commands = (double*)calloc(sizeof(double), 20);
    size_t numberOfCommands = convertAsmToCommands(&commands, buffer, numberOfStrings, assemblerFileName);


    for (size_t i = 0; i < numberOfCommands; i++) printf("%lg \n", commands[i]);
    //printf("%s", buffer);
    
    

    free(commands);
    free(buffer);

    return 0;
}

