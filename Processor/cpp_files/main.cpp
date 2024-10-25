#include <stdio.h>

#include "../h_files/getData_of_BinaryFile.h"
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
    char programCode_FileName[] = "../program_BinaryCode.txt";

    // read commmands from assembler
    char* buffer = nullptr;
    getData_of_BinaryFile(&buffer, programCode_FileName);

    // create soft processing unit
    spu_t spu = {};
    initSpu(&spu);

    getCommands(&spu, buffer);

    //for(size_t i = 0; i< spu.codeLength; i++) printf("%lg %d\n", (spu.code[i].dbl_num), (spu.code[i].int_num));
    
    runCode(&spu MYSBS(, spu.codeLength));

    
    destroySpu(&spu);
    free(buffer);
    return 0; 
}

void initSpu(spu_t* spu){
    assert((spu != nullptr) && "spu is nullptr in initSpu");
    spu->ip = 0;
    MACRO_stackInit(&spu->stack);
    spu->code = nullptr;
    spu->RAM = (StackElem_t*)calloc(100, sizeof(StackElem_t));
}

void destroySpu(spu_t* spu){
    assert((spu != nullptr) && "spu is nullptr in destroySpu");
    free(spu->code);
    free(spu->RAM);
    stackDestroy(&spu->stack);
    spu->ip = 0;

    for (int i = 0; i < NUMBER_OF_REGISTERS; i++)
        spu->regData[i] = 0;
}