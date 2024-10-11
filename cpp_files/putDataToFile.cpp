#include <stdio.h>
#include <stdlib.h>

#include "../h_files/putDataToFile.h"

void putDataToFile(double* array, size_t length, const char* file_name){

    FILE* wFile = fopen(file_name, "w");
    char double_to_str[50];

    for (size_t i = 0; i < length; i++){

        snprintf(double_to_str, 50, "%f", array[i]);

        fputs(double_to_str, wFile);
        fputs("\n", wFile);
    }
    fclose(wFile);
}