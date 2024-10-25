#include "../h_files/getData_of_BinaryFile.h"
#include "../h_files/getCommands.h"
#include "../h_files/spu.h"
#include "../h_files/commands.h"
#include "math.h"

getComStatuses getCommands(spu_t* spu, char* buffer){

    spu->code = (number_t*)calloc(sizeof(number_t), 1000);

    for (int i = 0; i < 1000; i++){
        spu->code[i].dbl_num = NAN;
        spu->code[i].int_num = -1;
    }

    size_t buff_i = 0;
    size_t codeLength = 0;
    int indexOfCurrentCommand = 0;

    while(1){
        memcpy(&((spu->code)[codeLength++].int_num), buffer + buff_i, sizeof(int));
        buff_i += sizeof(int);
        indexOfCurrentCommand = (spu->code)[codeLength - 1].int_num;
        
        if ( indexOfCurrentCommand == COMMAND_HLT)
            break;

        if (indexOfCurrentCommand == COMMAND_POP){
            memcpy(&(spu->code)[codeLength++].int_num, buffer + buff_i, sizeof(int));
            buff_i += sizeof(int);
            int mode = (spu->code)[codeLength - 1].int_num;


            switch (mode)
            {
            case REGISTER_MOD + RAM_MOD:
            case REGISTER_MOD:
                memcpy(&(spu->code)[codeLength++].int_num, buffer + buff_i, sizeof(int));
                buff_i += sizeof(int);
                break;

            case NUMBER_MOD + RAM_MOD:
                memcpy(&(spu->code)[codeLength++].dbl_num, buffer + buff_i, sizeof(double));
                buff_i += sizeof(double);
                break;

            default:
                return SOMETHING_GO_WRONG_WITH_GETTING_COMMANDS;
                break;
            }
            

        } else if ( ((COMMAND_JA <= indexOfCurrentCommand) && (indexOfCurrentCommand <= COMMAND_JMP))){
            memcpy(&(spu->code)[codeLength++].dbl_num, buffer + buff_i, sizeof(double));
            buff_i += sizeof(double);

        } else if (indexOfCurrentCommand == COMMAND_PUSH){
            memcpy(&(spu->code)[codeLength++].int_num, buffer + buff_i, sizeof(int));
            buff_i += sizeof(int);
            int mode = (spu->code)[codeLength - 1].int_num;


            switch (mode)
            {
            case (REGISTER_MOD + NUMBER_MOD + RAM_MOD):
                memcpy(&(spu->code)[codeLength++].dbl_num, buffer + buff_i, sizeof(double));
                buff_i += sizeof(double);
                memcpy(&(spu->code)[codeLength++].int_num, buffer + buff_i, sizeof(int));
                buff_i += sizeof(int);
                break;
            
            case (REGISTER_MOD + RAM_MOD):
                memcpy(&(spu->code)[codeLength++].int_num, buffer + buff_i, sizeof(int));
                buff_i += sizeof(int);
                break;

            case (NUMBER_MOD + RAM_MOD):
            case NUMBER_MOD:
                memcpy(&(spu->code)[codeLength++].dbl_num, buffer + buff_i, sizeof(double));
                buff_i += sizeof(double);
                break;

            case REGISTER_MOD:
                memcpy(&(spu->code)[codeLength++].int_num, buffer + buff_i, sizeof(int));
                buff_i += sizeof(int);
                break;
            default:
                return SOMETHING_GO_WRONG_WITH_GETTING_COMMANDS;
                break;
            }


        }
    }

    spu->codeLength = codeLength;
    
    return COMMANDS_WAS_GETTED_SUCCSESSFULLY;

}

