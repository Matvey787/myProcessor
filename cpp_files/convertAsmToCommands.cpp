#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#include "../h_files/convertAsmToCommands.h"
#include "../h_files/commands.h"

enum errors {
    NO_ERROR = 0,
    INCORRECT_COMMAND = 2
};

static errors writeCommand(const char* command, double** commands, size_t indexOfCommand);
static void printTypeOfError(errors error, const char* asmFileName, const size_t line);

size_t convertAsmToCommands(double** commands, const char* buffer, const size_t numberOfStrings, const char* asmFileName){

    size_t indexOfCommand_in_Commands = 0;

    errors error = NO_ERROR;

    char symbol = 0;
    int readed_symbols = 0;

    double number = 0;
    char f_startNumber = 0;

    char command[20] = {};
    int command_i = 0;

    for (size_t line = 0; line < numberOfStrings;){

        symbol = buffer[readed_symbols++];
        
        if (symbol == '\n'){
            if (f_startNumber){

                (*commands)[indexOfCommand_in_Commands] = number;
                ++indexOfCommand_in_Commands;

            } else {
                command[command_i] = '\0';
                if ((error = writeCommand(command, commands, indexOfCommand_in_Commands)) != NO_ERROR){
                    printTypeOfError(error, asmFileName, line+1);
                    break;
                }

                ++indexOfCommand_in_Commands;
            }
                
            line += 1;
            f_startNumber = 0;
            number = 0;
            command_i = 0;
        }

        else if (isspace(symbol))
        {
            command[command_i] = '\0';
            if ((error = writeCommand(command, commands, indexOfCommand_in_Commands)) != NO_ERROR){
                    printTypeOfError(error, asmFileName, line+1);
                    break;
                }
            ++indexOfCommand_in_Commands;
            
            command_i = 0;
            f_startNumber = 1;
        }

        else if (isdigit(symbol))
        {
            number = ((symbol-'0') + 10*number);
        }

        else if (isalpha(symbol))
        {
            if (f_startNumber) {printf("incorrect command: "); break;}
            command[command_i++] = symbol;
        }
    }
    return indexOfCommand_in_Commands + 1;
}

static errors writeCommand(const char* command, double** commands, size_t indexOfCommand){

    double* commands_address = *commands;
    if (strcmp(command, "PUSH") == 0)
        commands_address[indexOfCommand] = COMMAND_PUSH;

    else if (strcmp(command, "POP") == 0)
        commands_address[indexOfCommand] = COMMAND_POP;

    else if (strcmp(command, "ADD") == 0)
        commands_address[indexOfCommand] = COMMAND_ADD;

    else if (strcmp(command, "SUB") == 0)
        commands_address[indexOfCommand] = COMMAND_SUB;

    else if (strcmp(command, "MUL") == 0)
        commands_address[indexOfCommand] = COMMAND_MUL;

    else if (strcmp(command, "DIV") == 0)
        commands_address[indexOfCommand] = COMMAND_DIV;
    
    else if (strcmp(command, "OUT") == 0)
        commands_address[indexOfCommand] = COMMAND_OUT;
    
    else if (strcmp(command, "HLT") == 0)
        commands_address[indexOfCommand] = COMMAND_HLT;
    else
        return INCORRECT_COMMAND;

    return NO_ERROR;
}

static void printTypeOfError(errors error, const char* asmFileName, const size_t line){

    fprintf(stderr, "Incorrect line in .asm file: %s:%lu\n", asmFileName, line);
    fprintf(stderr, "Error: [");

    switch (error)
    {
    case INCORRECT_COMMAND:
        fprintf(stderr, "INCORRECT_COMMAND");
        break;

    case NO_ERROR:
        break;

    default:
        break;
    }

    fprintf(stderr, "]\n");
}