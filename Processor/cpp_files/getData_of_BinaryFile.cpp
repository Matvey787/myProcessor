#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#include "../h_files/getData_of_BinaryFile.h"

size_t getData_of_BinaryFile(char** buffer, const char* file_name){

    // open file for read

    FILE* rFile = fopen(file_name, "rb");

    // find size of file

    fseek(rFile, 0, SEEK_END);
    size_t size = (size_t)ftell(rFile);
    fseek(rFile, 0, SEEK_SET);

    // read text from file

    *buffer = (char*)calloc(size, sizeof(char));
    fread(*buffer, sizeof(char), size, rFile);

    // close file 

    fclose(rFile);

    return size;
}