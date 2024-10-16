#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "../h_files/putDataToFile.h"

static const size_t maxSizeOfDouble = 40;
static const double compareZero = 0.0001;

void putDataToFile(command_t* commands, size_t length, const char* file_name){

    FILE* wFile = fopen(file_name, "w");
    char double_to_str[maxSizeOfDouble];

    time_t t;   // not a primitive datatype
    time(&t);

    fputs("MGP\nCompilation time: ", wFile);
    fputs(ctime(&t), wFile);
    fputs("---------------------------------------------------------------------------------------------------------------\n", wFile);

    for (size_t i = 0; i < length; i++){
        
        //printf("%d %d %lg\n", commands[i].com, commands[i].reg, commands[i].num);

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

        if (fabs(commands[i].num - commands[i].num) < compareZero){
            snprintf(double_to_str, maxSizeOfDouble, "%lg", commands[i].num);
            fputs(double_to_str, wFile);
            fputs("\n", wFile);
        }
        
    }
    fclose(wFile);
}