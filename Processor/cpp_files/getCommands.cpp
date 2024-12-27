#include "../h_files/getData_of_BinaryFile.h"
#include "../h_files/getCommands.h"
#include "../h_files/spu.h"
#include "../h_files/commands.h"
#include "math.h"
const size_t c_numOfCommmands = 1000;

static getComStatuses getArgs_POP(spu_t* spu, size_t* codeLength, char* buffer, size_t* buff_i);
static getComStatuses getArgs_JMPS(spu_t* spu, size_t* codeLength, char* buffer, size_t* buff_i);
static getComStatuses getArgs_PUSH(spu_t* spu, size_t*codeLength, char* buffer, size_t* buff_i);

getComStatuses getCommands(spu_t* spu, char* buffer, const size_t sizeOfBuffer){
    assert(buffer != nullptr);
    spu->code = (number_t*)calloc(sizeof(number_t), c_numOfCommmands);

    for (size_t i = 0; i < c_numOfCommmands; i++){
        spu->code[i].dbl_num = NAN;
        spu->code[i].int_num = -1;
    }
    char* endOfBuffer = buffer + sizeOfBuffer;
    size_t buff_i = 0;
    size_t codeLength = 0;
    int indexOfCurrentCommand = 0;

    while(1){
        //printf("%p\n", buffer + buff_i);
        if (buffer + buff_i == endOfBuffer)
            break;
        memcpy(&((spu->code)[codeLength++].int_num), buffer + buff_i, sizeof(int));
        //printf("ffffff %d\n", (spu->code)[codeLength-1].int_num);
        buff_i += sizeof(int);
        indexOfCurrentCommand = (spu->code)[codeLength - 1].int_num;
        
        /* if (indexOfCurrentCommand == COMMAND_HLT)
            break; */

        if (indexOfCurrentCommand == COMMAND_POP){
            if (getArgs_POP(spu, &codeLength, buffer, &buff_i) == SOMETHING_GO_WRONG_WITH_GETTING_COMMANDS)
                return SOMETHING_GO_WRONG_WITH_GETTING_COMMANDS;

        } else if ( (COMMAND_JA <= indexOfCurrentCommand && indexOfCurrentCommand <= COMMAND_JB) ||
         indexOfCurrentCommand == COMMAND_CALL /* because call function is the same as jump */){
            if (getArgs_JMPS(spu, &codeLength, buffer, &buff_i) == SOMETHING_GO_WRONG_WITH_GETTING_COMMANDS)
                return SOMETHING_GO_WRONG_WITH_GETTING_COMMANDS;

        } else if (indexOfCurrentCommand == COMMAND_PUSH){
             if (getArgs_PUSH(spu, &codeLength, buffer, &buff_i) == SOMETHING_GO_WRONG_WITH_GETTING_COMMANDS)
                return SOMETHING_GO_WRONG_WITH_GETTING_COMMANDS;
        }
    }

    spu->codeLength = codeLength;

    return COMMANDS_WAS_GETTED_SUCCSESSFULLY;

}

static getComStatuses getArgs_POP(spu_t* spu, size_t* codeLength, char* buffer, size_t* buff_i){
    memcpy(&(spu->code)[(*codeLength)++].int_num, buffer + *buff_i, sizeof(int));
    *buff_i += sizeof(int);
    int mode = (spu->code)[(*codeLength) - 1].int_num;

    switch (mode)
    {
    case (REGISTER_MOD + NUMBER_MOD + RAM_MOD):
        memcpy(&(spu->code)[(*codeLength)++].dbl_num, buffer + *buff_i, sizeof(double));
        *buff_i += sizeof(double);
        memcpy(&(spu->code)[(*codeLength)++].int_num, buffer + *buff_i, sizeof(int));
        *buff_i += sizeof(int);
        break;
    
    case (REGISTER_MOD + RAM_MOD):
        memcpy(&(spu->code)[(*codeLength)++].int_num, buffer + *buff_i, sizeof(int));
        *buff_i += sizeof(int);
        break;

    case (NUMBER_MOD + RAM_MOD):
        memcpy(&(spu->code)[(*codeLength)++].dbl_num, buffer + *buff_i, sizeof(double));
        *buff_i += sizeof(double);
        break;

    case REGISTER_MOD:
        memcpy(&(spu->code)[(*codeLength)++].int_num, buffer + *buff_i, sizeof(int));
        *buff_i += sizeof(int);
        break;
    default:
        return SOMETHING_GO_WRONG_WITH_GETTING_COMMANDS;
        break;
    }
    return COMMANDS_WAS_GETTED_SUCCSESSFULLY;
}

static getComStatuses getArgs_JMPS(spu_t* spu, size_t* codeLength, char* buffer, size_t* buff_i){
    memcpy(&(spu->code)[(*codeLength)++].dbl_num, buffer + *buff_i, sizeof(double));
    *buff_i += sizeof(double);
    return COMMANDS_WAS_GETTED_SUCCSESSFULLY;
}

static getComStatuses getArgs_PUSH(spu_t* spu, size_t*codeLength, char* buffer, size_t* buff_i){
    memcpy(&(spu->code)[(*codeLength)++].int_num, buffer + *buff_i, sizeof(int));
    *buff_i += sizeof(int);
    int mode = (spu->code)[(*codeLength) - 1].int_num;

    switch (mode)
    {
    case (REGISTER_MOD + NUMBER_MOD + RAM_MOD):
        memcpy(&(spu->code)[(*codeLength)++].dbl_num, buffer + *buff_i, sizeof(double));
        *buff_i += sizeof(double);
        memcpy(&(spu->code)[(*codeLength)++].int_num, buffer + *buff_i, sizeof(int));
        *buff_i += sizeof(int);
        break;
    
    case (REGISTER_MOD + RAM_MOD):
        memcpy(&(spu->code)[(*codeLength)++].int_num, buffer + *buff_i, sizeof(int));
        *buff_i += sizeof(int);
        break;

    case (NUMBER_MOD + RAM_MOD):
    case NUMBER_MOD:
        memcpy(&(spu->code)[(*codeLength)++].dbl_num, buffer + *buff_i, sizeof(double));
        *buff_i += sizeof(double);
        break;

    case REGISTER_MOD:
        memcpy(&(spu->code)[(*codeLength)++].int_num, buffer + *buff_i, sizeof(int));
        *buff_i += sizeof(int);
        break;
    default:
        return SOMETHING_GO_WRONG_WITH_GETTING_COMMANDS;
        break;
    }
    return COMMANDS_WAS_GETTED_SUCCSESSFULLY;
}