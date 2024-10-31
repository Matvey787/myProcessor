#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#include "../h_files/getFileStrings.h"

size_t getFileStrings(char** buffer, const char* file_name){

    assert(buffer && "buffer is nullptr in getFileStrs");
    assert(buffer && "filename is nullptr in getFileStrs");

    // open file for read

    FILE* rFile = fopen(file_name, "rb");

    if (rFile == nullptr) {
        printf("couldn't open read file");
        return 0;
    }

    // find size of file

    fseek(rFile, 0, SEEK_END);
    long int size = ftell(rFile);
    if (size == -1L) return 0;
    fseek(rFile, 0, SEEK_SET);

    // read text from file

    *buffer = (char*)calloc((size_t)(size + 1), sizeof(char));
    if (*buffer == nullptr){
        printf("couldn't allocate memory for buffer");
        return 0;
    }

    fread(*buffer, sizeof(char), (size_t)size, rFile);

    // find number of strings
    size_t numberOfStrings = 0;
    short isLineWithoutLineTransfer = 0;

    for (size_t i = 0; i < (size_t)size; i++)
        if ((*buffer)[i] == '\n') {

            isLineWithoutLineTransfer = 0;
            ++numberOfStrings;

        } else {
            isLineWithoutLineTransfer = 1;
        };

    if (isLineWithoutLineTransfer){
        ++numberOfStrings;
        (*buffer)[size] = '\n';
    }

    // close file 

    fclose(rFile);
    rFile = nullptr;
    
    return numberOfStrings;
}