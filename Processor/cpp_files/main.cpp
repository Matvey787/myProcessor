#include <stdio.h>
#include "../h_files/getFileStrings.h"
#include "../h_files/getCommands.h"
#include "../h_files/spu.h"
#include "../h_files/runCode.h"
#include "../h_files/macros.h"

#include "../../workWithStack/h_files/stackConstructor.h"
#include "../../workWithStack/h_files/stackInitDestroy.h"
#include "../../workWithStack/h_files/macros.h"

void initSpu(spu_t* spu);
void destroySpu(spu_t* spu);

int main(){

    //files
    char programCode_FileName[] = "../program_code.txt";

    // read commmands from assembler
    char* buffer = nullptr;
    size_t numberOfStrings = getFileStrings(&buffer, programCode_FileName);
    
    // create soft processing unit
    spu_t spu = {};
    initSpu(&spu);

    getCommands(&spu.code, buffer, numberOfStrings);

    //for(size_t i = 0; i< numberOfStrings - FIRST_LINES_INFO_OF_FILE; i++) printf("%s \n", spu.code[i]);
    
    runCode(&spu MYSBS(, numberOfStrings - FIRST_LINES_INFO_OF_FILE));


    destroySpu(&spu);
    free(buffer);
    return 0;
}

void initSpu(spu_t* spu){
    spu->ip = 0;
    MACRO_stackInit(&spu->stack);
    spu->code = nullptr;
}

void destroySpu(spu_t* spu){

    free(spu->code);
    stackDestroy(&spu->stack);
    spu->ip = 0;

    for (int i = 0; i < NUMBER_OF_REGISTERS; i++)
        spu->regData[i] = 0;
}