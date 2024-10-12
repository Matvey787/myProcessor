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
    NO_CORRECT_REGISTER_AFTER_COMMAND = 8
};

static errors writeCommand(const char* command, command_t* commands, size_t indexOfCommand);
static void printTypeOfError(errors error, const char* asmFileName, const size_t line);
static void addZeroTerminator_splitLineIntoWords(char* buffer, const size_t numberOfStrings);
static progRegisters writeRegisterAddress(const char* command, command_t* commands,  size_t indexOfCommand);

convertationStatuses convertAsmToCommands(command_t* commands, char* buffer, const size_t numberOfStrings, const char* asmFileName){
    assert(commands != nullptr && "commmands is null pointer");
    assert(buffer != nullptr && "buffer is null pointer");
    assert(asmFileName != nullptr && "assembler file name is null pointer");

    addZeroTerminator_splitLineIntoWords(buffer, numberOfStrings);
    /* for (int i = 0; i < 10; i++){
        printf("%s\n", buffer + i);
    } */

    errors error = NO_ERROR;
    char command[20];
    double number = 0;
    size_t buff_i = 0;

    for (size_t line = 0; line < numberOfStrings;){
        sscanf(buffer + buff_i, "%s", command);

        if (writeCommand(command, commands, line) == INCORRECT_COMMAND)
            error = INCORRECT_COMMAND;

        buff_i += strlen(command)+1;
        
        //---------------------------------------------------command push----------------------------------------------------------

        if (strcmp(command, "PUSH") == 0){

            sscanf(buffer + buff_i, "%s", command);

            if (sscanf(command, "%lg", &number) == 1)
                commands[line].num = number;

            else if (writeRegisterAddress(command, commands, line) != NOT_REGISTER){

                // change code of push because of register was fpunded after push
                commands[line].com = COMMAND_PUSH_REGISTER;
                
            } else 
                error = NO_CORRECT_NUMBER_OR_REGISTER_AFTER_COMMAND;
            
            buff_i += strlen(command)+1;
        }
        
        //---------------------------------------------------command pop----------------------------------------------------------

        if (strcmp(command, "POP") == 0){
            
            sscanf(buffer + buff_i, "%s", command);

            if (writeRegisterAddress(command, commands, line) == NOT_REGISTER)
                error = NO_CORRECT_REGISTER_AFTER_COMMAND;

            buff_i += strlen(command)+1;
        }

        if (error != NO_ERROR){
            printTypeOfError(error, asmFileName, line+1);
            return CONVERTATION_FAIL; 
        }

        //---------------------------------------------------command jump----------------------------------------------------------
        if (strcmp(command, "JMP") == 0 || strcmp(command, "JA") == 0 || strcmp(command, "JAE") == 0 || strcmp(command, "JE") == 0){
            
            sscanf(buffer + buff_i, "%s", command);
            sscanf(command, "%lg", &number);
            commands[line].num = number;

            buff_i += strlen(command)+1;
        }

        if (error != NO_ERROR){
            printTypeOfError(error, asmFileName, line+1);
            return CONVERTATION_FAIL; 
        }

        line++;
    }
  
    return CONVERTATION_SUCCESS;
}

static errors writeCommand(const char* command, command_t* commands, size_t indexOfCommand){

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

static progRegisters writeRegisterAddress(const char* command, command_t* commands, size_t indexOfCommand){

    progRegisters* register_address = &(commands[indexOfCommand].reg);

    if (strcmp(command, "AX") == 0){
        *register_address = AX;
        return AX;
    }
    else if (strcmp(command, "BX") == 0){
        *register_address = BX;
        return BX;
    }

    else if (strcmp(command, "CX") == 0){
        *register_address = CX;
        return CX;
    }

    else if (strcmp(command, "DX") == 0){
        *register_address = DX;
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

    for (size_t j = 0; j < numberOfStrings;){
        //printf("%c", buffer[buffer_i]);

        if (buffer[buffer_i] == '\n'){
            buffer[buffer_i] = '\0';
            j++;
        }
        else if (isspace(buffer[buffer_i])){
            //printf("space ");
            buffer[buffer_i] = '\0';
        }

        
        ++buffer_i;
    }

}