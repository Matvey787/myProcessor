#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "../h_files/convertAsmToCommands.h"
#include "../h_files/commands.h"
#include "../h_files/registers.h"

enum errors {
    NO_ERROR = 0,
    INCORRECT_COMMAND = 2,
    NO_CORRECT_NUMBER_OR_REGISTER_AFTER_COMMAND = 4,
    NO_CORRECT_REGISTER_AFTER_COMMAND = 8,
    REDEFENITION_OF_LABEL = 16,
    NO_MEMORY_FOR_NEW_LABEL = 32
};

static errors writeCommandWithoutArg(const char* command, command_t* commands, size_t indexOfCommand);
static void printTypeOfError(errors error, const char* asmFileName, const size_t line);
static void addZeroTerminator_splitLineIntoWords(char* buffer, const size_t numberOfStrings);
static progRegisters getRegisterAddress(const char* command);
static errors writeArgument(const char* arg, command_t* commands, size_t indexOfCommand, 
                            progCommands cmdBeforeArg, labelsData_t* lData);
int findValueOfLabel(labelsData_t* lData, const char* nameOfLabel);

errors initLabel(labelsData_t* lData, const char* nameOfLabel, size_t addedCommands);
errors addLabel(labelsData_t* lData, const char* nameOfLabel);

convertationStatuses convertAsmToCommands(command_t* commands, char* buffer, const size_t numberOfStrings, 
                                          const char* asmFileName, labelsData_t* lData){
    assert(commands != nullptr && "commmands is null pointer");
    assert(buffer != nullptr && "buffer is null pointer");
    assert(asmFileName != nullptr && "assembler file name is null pointer");

    addZeroTerminator_splitLineIntoWords(buffer, numberOfStrings);
    /* for (int i = 0; i < 10; i++){
        printf("%s\n", buffer + i);
    } */

    size_t addedCommands = 0;
    errors error = NO_ERROR;
    char* command = nullptr;
    char* arg = nullptr;
    
    size_t buff_i = 0;

    for (size_t line = 0; line < numberOfStrings;){
        command = buffer + buff_i;

        error = writeCommandWithoutArg(command, commands, line);

        if (error == NO_ERROR) ++addedCommands;

        if (error != NO_ERROR){

            if (command[strlen(command) - 1] == ':' && strlen(command) > 1)
                error = initLabel(lData, command, addedCommands);
        }

        buff_i += strlen(command)+1;
        
        if (strcmp(command, "PUSH") == 0){
            arg = buffer + buff_i;

            error = writeArgument(arg, commands, line, COMMAND_PUSH, lData);
            if (error == NO_ERROR) ++addedCommands;

            buff_i += strlen(arg)+1;

        } else if (strcmp(command, "POP") == 0){
            arg = buffer + buff_i;
            error = writeArgument(arg, commands, line, COMMAND_POP, lData);
            if (error == NO_ERROR) ++addedCommands;
            buff_i += strlen(arg)+1;
        
        // TODO remove strcmp and add enum comparison
        } else if (strcmp(command, "JMP") == 0 || strcmp(command, "JA") == 0 || strcmp(command, "JAE") == 0 || strcmp(command, "JE") == 0){
            arg = buffer + buff_i;
            error = writeArgument(arg, commands, line, COMMAND_JMP, lData);
            if (error == NO_ERROR) ++addedCommands;
            buff_i += strlen(arg)+1;
        }
        
        if (error != NO_ERROR){
            printTypeOfError(error, asmFileName, line+1);
            return CONVERTATION_FAIL; 
        }

        line++;
    }
  
    return CONVERTATION_SUCCESS;
}
errors initLabel(labelsData_t* lData, const char* nameOfLabel, size_t addedCommands){
    for (size_t label_index = 0; label_index < lData->size; label_index++){

        if (strcmp((lData->labels)[label_index].name, "empty\0") == 0){
            strcpy((lData->labels)[label_index].name, nameOfLabel);
            (lData->labels)[label_index].value = (int)addedCommands;

        } else if (strcmp((lData->labels)[label_index].name, nameOfLabel) == 0){
            if ((lData->labels)[label_index].value != -1) return REDEFENITION_OF_LABEL;

            (lData->labels)[label_index].value = (int)addedCommands;
        }
    }
    return NO_ERROR;
}

errors addLabel(labelsData_t* lData, const char* nameOfLabel){

    for (size_t label_index = 0; label_index < lData->size; label_index++){

        if (strcmp((lData->labels)[label_index].name, "empty\0") == 0){
            strcpy((lData->labels)[label_index].name, nameOfLabel);
            (lData->labels)[label_index].value = -1;
            return NO_ERROR;
        }
    }
    return NO_MEMORY_FOR_NEW_LABEL;
}

int findValueOfLabel(labelsData_t* lData, const char* nameOfLabel){

    for (size_t label_index = 0; label_index < lData->size; label_index++)
        if (strcmp((lData->labels)[label_index].name, nameOfLabel) == 0)
            return (lData->labels)[label_index].value;

    assert(addLabel(lData, nameOfLabel) != NO_MEMORY_FOR_NEW_LABEL);
    return -1;
}

static errors writeCommandWithoutArg(const char* command, command_t* commands, size_t indexOfCommand){

    progCommands* command_address = &(commands[indexOfCommand].com);

    if (strcmp(command, "PUSH") == 0)
        *command_address = COMMAND_PUSH;

    else if (strcmp(command, "POP") == 0)
        *command_address = COMMAND_POP;

    else if (strcmp(command, "ADD") == 0)
        *command_address = COMMAND_ADD;

    else if (strcmp(command, "SUB") == 0)
        *command_address = COMMAND_SUB;

    else if (strcmp(command, "MUL") == 0)
        *command_address = COMMAND_MUL;

    else if (strcmp(command, "DIV") == 0)
        *command_address = COMMAND_DIV;
    
    else if (strcmp(command, "OUT") == 0)
        *command_address = COMMAND_OUT;
        
    else if (strcmp(command, "IN") == 0)
        *command_address = COMMAND_IN;

    else if (strcmp(command, "JA") == 0)
        *command_address = COMMAND_JA;

    else if (strcmp(command, "JAE") == 0)
        *command_address = COMMAND_JAE;

    else if (strcmp(command, "JE") == 0)
        *command_address = COMMAND_JE;

    else if (strcmp(command, "JMP") == 0)
        *command_address = COMMAND_JMP;

    else if (strcmp(command, "HLT") == 0)
        *command_address = COMMAND_HLT;
    else
        return INCORRECT_COMMAND;

    return NO_ERROR;
}

static errors writeArgument(const char* arg, command_t* commands, size_t indexOfCommand,
                            progCommands cmdBeforeArg, labelsData_t* lData){
    double number = 0;
    char reg[20];
    //---------------------------------------------------command push----------------------------------------------------------
    if (cmdBeforeArg == COMMAND_PUSH){
        if (sscanf(arg, "[%lg+%2s]", &number, reg) == 2){
            printf("%s\n", reg);
            commands[indexOfCommand].mod = NUMBER_MOD + REGISTER_MOD + RAM_MOD;
            commands[indexOfCommand].num = number;
            commands[indexOfCommand].reg = getRegisterAddress(reg);


        } else if (sscanf(arg, "[%lg]", &number) == 1){
            commands[indexOfCommand].mod = NUMBER_MOD + RAM_MOD;
            commands[indexOfCommand].num = number;


        } else if (sscanf(arg, "[%s]", reg) == 1){
            commands[indexOfCommand].mod = REGISTER_MOD + RAM_MOD;
            commands[indexOfCommand].reg = getRegisterAddress(reg);


        } else if (sscanf(arg, "%lg", &number) == 1){
            commands[indexOfCommand].mod = NUMBER_MOD;
            commands[indexOfCommand].num = number;


        } else if (sscanf(arg, "%s", reg) == 1){
            commands[indexOfCommand].mod = REGISTER_MOD;
            commands[indexOfCommand].reg = getRegisterAddress(reg);


        } else {
            printf("%s %d\n", arg, commands[indexOfCommand].com);
            return NO_CORRECT_NUMBER_OR_REGISTER_AFTER_COMMAND;
        }
    }
        
    //---------------------------------------------------command pop----------------------------------------------------------

    else if (cmdBeforeArg == COMMAND_POP){
        if (sscanf(arg, "[%lg]", &number) == 1){
            commands[indexOfCommand].mod = NUMBER_MOD + RAM_MOD;
            commands[indexOfCommand].num = number;
            
        } else if (sscanf(arg, "[%2s]", reg) == 1){
            commands[indexOfCommand].mod = REGISTER_MOD + RAM_MOD;
            commands[indexOfCommand].reg = getRegisterAddress(reg);
        
        }else if (sscanf(arg, "%2s", reg) == 1){
            commands[indexOfCommand].mod = REGISTER_MOD;
            commands[indexOfCommand].reg = getRegisterAddress(reg);

        }
        
        
        else {
            return NO_CORRECT_REGISTER_AFTER_COMMAND;
        }
            

    //---------------------------------------------------commands family of jumps----------------------------------------------------------

    } else if (cmdBeforeArg == COMMAND_JMP || cmdBeforeArg == COMMAND_JE || cmdBeforeArg == COMMAND_JAE || 
                cmdBeforeArg == COMMAND_JA){

                    //sscanf(arg, "%lg", &number);
                    commands[indexOfCommand].num = findValueOfLabel(lData, arg);
                }
    return NO_ERROR;

}

static progRegisters getRegisterAddress(const char* regName){


    if (strcmp(regName, "AX") == 0){
        return AX;
    }
    else if (strcmp(regName, "BX") == 0){
        return BX;
    }
    else if (strcmp(regName, "CX") == 0){
        return CX;
    }
    else if (strcmp(regName, "DX") == 0){
        return DX;
    }
    else
        return NOT_REGISTER;

}

static void printTypeOfError(errors error, const char* asmFileName, const size_t line){

    fprintf(stderr, "Incorrect line in .asm file: %s:%lu\n", asmFileName, line);
    fprintf(stderr, "Error: [");

    switch (error)
    {
    case INCORRECT_COMMAND:
        fprintf(stderr, "INCORRECT_COMMAND");
        break;
    case NO_CORRECT_NUMBER_OR_REGISTER_AFTER_COMMAND:
        fprintf(stderr, "NO_CORRECT_NUMBER_OR_REGISTER_AFTER_COMMAND");
        break;

    case NO_CORRECT_REGISTER_AFTER_COMMAND:
        fprintf(stderr, "NO_CORRECT_REGISTER_AFTER_COMMAND");
        break;
    case REDEFENITION_OF_LABEL:
        fprintf(stderr, "REDEFENITION_OF_LABEL");
        break;

    case NO_MEMORY_FOR_NEW_LABEL:
        fprintf(stderr, "NO_MEMORY_FOR_NEW_LABEL");
        break;

    case NO_ERROR:
        break;

    default:
        break;
    }

    fprintf(stderr, "]\n");
}

static void addZeroTerminator_splitLineIntoWords(char* buffer, const size_t numberOfStrings){

    size_t buffer_i = 0;
    //printf("%d\n", numberOfStrings);

    for (size_t strings_cnt = 0; strings_cnt < numberOfStrings;){
        //printf("%c", buffer[buffer_i]);
        assert(buffer[buffer_i] != '\0');

        if (buffer[buffer_i] == '\n'){
            buffer[buffer_i] = '\0';
            strings_cnt++;
        }
        else if (isspace(buffer[buffer_i])){
            //printf("space ");
            buffer[buffer_i] = '\0';
        }

        
        ++buffer_i;
    }

}