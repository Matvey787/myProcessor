#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "../h_files/convertAsmToCommands.h"
#include "../h_files/commands.h"
#include "../h_files/registers.h"

#define MACRO_WR_ARG(name)  writeArgument(arg, commands, line, COMMAND_##name, lData, &addedCommands); \
                            buff_i += strlen(arg)+1; \
                            break;
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

static errors writeArgument(char* arg, command_t* commands, size_t indexOfCommand, 
                            progCommands cmdBeforeArg, labelsData_t* lData, size_t* addedCommands);

static void writeToStruct(command_t* commands, size_t indexOfCommand, int mode, double number, char* reg);

static int findValueOfLabel(labelsData_t* lData, char* nameOfLabel);

static errors wrLabelAddr(labelsData_t* lData, char* nameOfLabel, size_t addedCommands);

static errors addLabel(labelsData_t* lData, char* nameOfLabel);


convertationStatuses convertAsmToCommands(command_t* commands, char* buffer, const size_t numberOfStrings, 
                                          const char* asmFileName, labelsData_t* lData, short numPass){
    assert(commands != nullptr && "commmands is null pointer");
    assert(buffer != nullptr && "buffer is null pointer");
    assert(asmFileName != nullptr && "assembler file name is null pointer");

    if (numPass == 1) addZeroTerminator_splitLineIntoWords(buffer, numberOfStrings);
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
        
        if (strlen(command) == 0) { buff_i += 1; continue; }

        // check could it be a label
        if ((command[strlen(command) - 1] == ':') && (strlen(command) > 1)){
            if (numPass == 1)
            {
                error = wrLabelAddr(lData, command, addedCommands);
            }
                

            buff_i += strlen(command)+1;
            ++line;
            continue;
        }

        error = writeCommandWithoutArg(command, commands, line);
        
        if (error == NO_ERROR) {
            ++addedCommands;
            buff_i += strlen(command)+1;
            arg = buffer + buff_i;

            switch (commands[line].com)
            {
                case COMMAND_PUSH:
                    
                    error = MACRO_WR_ARG(PUSH)
                    
                case COMMAND_POP:

                    error = MACRO_WR_ARG(POP)

                case COMMAND_CALL:
                    error = MACRO_WR_ARG(CALL)
                    /* error = writeArgument(arg, commands, line, COMMAND_POP, lData, &addedCommands);
                    buff_i += strlen(arg)+1;
                    break; */

                case COMMAND_JA: // TODO jne, jb, jbe
                case COMMAND_JAE:
                case COMMAND_JE:
                case COMMAND_JBE:
                case COMMAND_JB:
                case COMMAND_JNE:
                case COMMAND_JMP:
                    error = MACRO_WR_ARG(JMP)

                /*  error = writeArgument(arg, commands, line, COMMAND_JMP, lData, &addedCommands);
                    buff_i += strlen(arg)+1;
                    break; */
                case COMMAND_ADD:
                case COMMAND_SIN:
                case COMMAND_COS:
                case COMMAND_LOG:
                case COMMAND_POW:
                case COMMAND_SUB:
                case COMMAND_DIV:
                case COMMAND_MUL:
                case COMMAND_IN:
                case COMMAND_OUT:
                case COMMAND_HLT:
                case NOT_COMMAND:
                case COMMAND_SQRT:
                case COMMAND_RET:
                default:
                    break;
            }
        }

        if (error != NO_ERROR){
            printTypeOfError(error, asmFileName, line);
            return CONVERTATION_FAIL; 
        }
        line++;
    }
  
    return CONVERTATION_SUCCESS;
}
static errors wrLabelAddr(labelsData_t* lData, char* nameOfLabel, size_t addedCommands){ 
    assert(lData != nullptr && "label data is null pointer");
    assert(nameOfLabel != nullptr && "name of label is null pointer");

    addLabel(lData, nameOfLabel);

    for (size_t label_index = 0; label_index < lData->size; label_index++){

        if ((lData->labels)[label_index].name != nullptr)
            if (strcmp((lData->labels)[label_index].name, nameOfLabel) == 0){

                if ((lData->labels)[label_index].value != -1) return REDEFENITION_OF_LABEL;
                (lData->labels)[label_index].value = (int)addedCommands;
            }
    }

    return NO_ERROR;
}

static errors addLabel(labelsData_t* lData, char* nameOfLabel){
    assert(lData != nullptr && "label data is null pointer in addLabel");
    assert(nameOfLabel != nullptr && "name of label is null pointer in addLabel");

    for (size_t label_index = 0; label_index < lData->size; label_index++){

        if ((lData->labels)[label_index].name == nullptr){
            (lData->labels)[label_index].name = nameOfLabel;
            (lData->labels)[label_index].value = -1;
            return NO_ERROR;
        }
    }
    return NO_MEMORY_FOR_NEW_LABEL;
}

static int findValueOfLabel(labelsData_t* lData, char* nameOfLabel){
    assert(lData != nullptr && "label data is null pointer in findValueOfLabel");
    assert(nameOfLabel != nullptr && "name of label is null pointer in findValueOfLabel");
    
    for (size_t label_index = 0; label_index < lData->size; label_index++)
        if ((lData->labels)[label_index].name != nullptr)
            if (strcmp((lData->labels)[label_index].name, nameOfLabel) == 0)
                return (lData->labels)[label_index].value;

    errors error = addLabel(lData, nameOfLabel);
    assert(error != NO_MEMORY_FOR_NEW_LABEL); // FIXME remove and check return value

    if (error == NO_MEMORY_FOR_NEW_LABEL) printf("All memory for labels is exhausted.\n \
                                                  Please, reduce number of labels.");
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
    
    else if (strcmp(command, "SQRT") == 0)
        *command_address = COMMAND_SQRT;
    
    else if (strcmp(command, "SIN") == 0)
        *command_address = COMMAND_SIN;

    else if (strcmp(command, "COS") == 0)
        *command_address = COMMAND_COS;
    
    else if (strcmp(command, "LOG") == 0)
        *command_address = COMMAND_LOG;

    else if (strcmp(command, "POW") == 0)
        *command_address = COMMAND_POW;

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

    else if (strcmp(command, "JBE") == 0)
        *command_address = COMMAND_JBE;

    else if (strcmp(command, "JB") == 0)
        *command_address = COMMAND_JB;

    else if (strcmp(command, "JNE") == 0)
        *command_address = COMMAND_JNE;

    else if (strcmp(command, "CALL") == 0)
        *command_address = COMMAND_CALL;

    else if (strcmp(command, "RET") == 0)
        *command_address = COMMAND_RET;

    else if (strcmp(command, "HLT") == 0)
        *command_address = COMMAND_HLT;
    else
        return INCORRECT_COMMAND;

    return NO_ERROR;
}

static errors writeArgument(char* arg, command_t* commands, size_t indexOfCommand,
                            progCommands cmdBeforeArg, labelsData_t* lData, size_t* addedCommands){
    double number = NAN;
    char reg[20] = "NOT_REGISTER";
    
    //---------------------------------------------------command push----------------------------------------------------------
    if (cmdBeforeArg == COMMAND_PUSH){
        if (sscanf(arg, "[%lg+%2s]", &number, reg) == 2){
            writeToStruct(commands, indexOfCommand, REGISTER_MOD | NUMBER_MOD | RAM_MOD, number, reg);
            *addedCommands += 3;

        } else if (sscanf(arg, "[%2s+%lg]", reg, &number) == 2){
            writeToStruct(commands, indexOfCommand, REGISTER_MOD | NUMBER_MOD | RAM_MOD, number, reg);
            *addedCommands += 3;
        
        /* } else if (sscanf(arg, "%2s+%lg", reg, &number) == 2){
            writeToStruct(commands, indexOfCommand, REGISTER_MOD | NUMBER_MOD, number, reg);
            *addedCommands += 3; */

        } else if (sscanf(arg, "[%lg]", &number) == 1){
            writeToStruct(commands, indexOfCommand, NUMBER_MOD | RAM_MOD, number, reg);
            *addedCommands += 2;

        } else if (sscanf(arg, "[%s]", reg) == 1){
            writeToStruct(commands, indexOfCommand, REGISTER_MOD | RAM_MOD, number, reg);
            *addedCommands += 2;

        } else if (sscanf(arg, "%lg", &number) == 1){
            writeToStruct(commands, indexOfCommand, NUMBER_MOD, number, reg);
            *addedCommands += 2;

        } else if (sscanf(arg, "%s", reg) == 1){
            writeToStruct(commands, indexOfCommand, REGISTER_MOD, number, reg);
            *addedCommands += 2;

        } else {
            //printf("%s %d\n", arg, commands[indexOfCommand].com);
            return NO_CORRECT_NUMBER_OR_REGISTER_AFTER_COMMAND;
        }
    }
        
    //---------------------------------------------------command pop----------------------------------------------------------

    else if (cmdBeforeArg == COMMAND_POP){
        //printf("++++\n");
        if (sscanf(arg, "[%lg+%2s]", &number, reg) == 2){
            writeToStruct(commands, indexOfCommand, REGISTER_MOD | NUMBER_MOD | RAM_MOD, number, reg);
            *addedCommands += 3;

        } else if (sscanf(arg, "[%2s+%lg]", reg, &number) == 2){
            writeToStruct(commands, indexOfCommand, REGISTER_MOD | NUMBER_MOD | RAM_MOD, number, reg);
            //printf("I see pop with all mode %c%c %d\n", reg[0], reg[1], *addedCommands);
            *addedCommands += 3;

        } else if (sscanf(arg, "[%lg]", &number) == 1){
            writeToStruct(commands, indexOfCommand, NUMBER_MOD | RAM_MOD, number, reg);
            *addedCommands += 2;

        } else if (sscanf(arg, "[%2s]", reg) == 1){
            writeToStruct(commands, indexOfCommand, REGISTER_MOD | RAM_MOD, number, reg);
            *addedCommands += 2;

        } else if (sscanf(arg, "%s", reg) == 1){
            writeToStruct(commands, indexOfCommand, REGISTER_MOD, number, reg);
            *addedCommands += 2;

        } else {
            //printf("%s %d\n", arg, commands[indexOfCommand].com);
            return NO_CORRECT_NUMBER_OR_REGISTER_AFTER_COMMAND;
        }
        //printf("++++\n");

    //---------------------------------------------------commands family of jumps----------------------------------------------------------

    } else if (cmdBeforeArg == COMMAND_JMP || cmdBeforeArg == COMMAND_JE || cmdBeforeArg == COMMAND_JAE || cmdBeforeArg == COMMAND_JA ||
               cmdBeforeArg == COMMAND_JB || cmdBeforeArg == COMMAND_JBE || cmdBeforeArg == COMMAND_JNE){

        commands[indexOfCommand].num = findValueOfLabel(lData, arg);
        *addedCommands += 1;
    }
    //---------------------------------------------------call function command----------------------------------------------------------
    else if (cmdBeforeArg == COMMAND_CALL)
    {
        commands[indexOfCommand].num = findValueOfLabel(lData, arg);
        *addedCommands += 1;
    }
    return NO_ERROR;

}

static void writeToStruct(command_t* commands, size_t indexOfCommand, int mode, double number, char* reg){
    commands[indexOfCommand].mode = mode;
    commands[indexOfCommand].num = number;
    commands[indexOfCommand].reg = getRegisterAddress(reg);
}

static progRegisters getRegisterAddress(const char* regName){

    printf("get register %s\n", regName);
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
    else if (strcmp(regName, "FS") == 0){
        return FS;
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