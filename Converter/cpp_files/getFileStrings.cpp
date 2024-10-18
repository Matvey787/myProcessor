#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#include "../h_files/getFileStrings.h"

size_t getFileStrings(char** buffer, const char* file_name){

    // open file for read

    // FIXME CHECK ERRORS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1

    FILE* rFile = fopen(file_name, "rb");

    // find size of file

    fseek(rFile, 0, SEEK_END);
    size_t size = (size_t)ftell(rFile);
    fseek(rFile, 0, SEEK_SET);

    // read text from file

    *buffer = (char*)calloc(size, sizeof(char));
    fread(*buffer, sizeof(char), size, rFile);

    // find number of strings
    size_t numberOfStrings = 0;
    for (size_t i = 0; i < size; i++) if ((*buffer)[i] == '\n') ++numberOfStrings; // TODO count last line without \n

    // close file 

    fclose(rFile);

    return numberOfStrings;
}