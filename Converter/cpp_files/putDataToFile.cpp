#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../h_files/putDataToFile.h"

static const size_t maxSizeOfDouble = 40;

void putDataToFile(command_t* commands, size_t length, const char* file_name){

    FILE* wFile = fopen(file_name, "w");
    char double_to_str[maxSizeOfDouble];

    time_t t;   // not a primitive datatype
    time(&t);

    fputs("MGP\nCompilation time: ", wFile);
    fputs(ctime(&t), wFile);
    fputs("---------------------------------------------------------------------------------------------------------------\n", wFile);

    for (size_t i = 0; i < length; i++){
        
        if (commands[i].com >= 0){
            snprintf(double_to_str, maxSizeOfDouble, "%d", commands[i].com);
            fputs(double_to_str, wFile);
            fputs("\n", wFile);
        }

        if (commands[i].reg >= 0){
            snprintf(double_to_str, maxSizeOfDouble, "%d", commands[i].reg);
            fputs(double_to_str, wFile);
            fputs("\n", wFile);
        }

        if (commands[i].num >= 0){
            snprintf(double_to_str, maxSizeOfDouble, "%lg", commands[i].num);
            fputs(double_to_str, wFile);
            fputs("\n", wFile);
        }
        
    }
    fclose(wFile);
}