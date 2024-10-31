#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "../h_files/putDataToFile.h"

static const double compareZero = 0.0001;

void putDataToFileCode(command_t* commands, size_t length, const char* file_name){
    assert(commands != nullptr && "commands is nullptr in putDataToFileCode");

    FILE* wFile = fopen(file_name, "w");

    time_t t = {};   // not a primitive datatype
    time(&t);

    fputs("MGP\nCompilation time: ", wFile);
    fputs(ctime(&t), wFile);
    fputs("---------------------------------------------------------------------------------------------------------------\n", wFile);

    for (size_t i = 0; i < length; i++){
        
        //printf("%d %d %lg\n", commands[i].com, commands[i].reg, commands[i].num);

        if (commands[i].com >= 0){
            fprintf(wFile, "%d\n", commands[i].com);
        }
        
        if (commands[i].mode > 0) {
            fprintf(wFile, "%d\n", commands[i].mode);
        }

        if (!isnan(commands[i].num)) {
            fprintf(wFile, "%lg\n", commands[i].num);
        }

        if (commands[i].reg >= 0){
            fprintf(wFile, "%d\n", commands[i].reg);
        }
        
    }
    fclose(wFile);
}

void putDataToFileBinaryCode(command_t* commands, size_t length, const char* file_name){
    assert(commands != nullptr && "commands is nullptr in putDataToFileBinaryCode");

    FILE* wFile = fopen(file_name, "wb");

    for (size_t i = 0; i < length; i++){
        
        //printf("%d %d %d %lg\n", commands[i].com, commands[i].mode, commands[i].reg, commands[i].num);

        if (commands[i].com >= 0){
            fwrite(&commands[i].com, sizeof(int), 1, wFile);
        }

        if (commands[i].mode > 0) {
            fwrite(&commands[i].mode, sizeof(int), 1, wFile);
        }
        
        if (!isnan(commands[i].num)) {
            fwrite(&commands[i].num, sizeof(double), 1, wFile);
        }

        if (commands[i].reg >= 0){
            fwrite(&commands[i].reg, sizeof(int), 1, wFile);
        }

    }
    fclose(wFile);
}