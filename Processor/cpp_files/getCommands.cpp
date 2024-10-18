#include "../h_files/getFileStrings.h"
#include "../h_files/getCommands.h"

static void addZeroTerminator_splitLineIntoWords(char* buffer, const size_t numberOfStrings);

size_t getCommands(char*** code, char* buffer, const size_t numberOfStrings){

    *code = (char**)calloc(sizeof(char**), numberOfStrings - FIRST_LINES_INFO_OF_FILE);
    
    addZeroTerminator_splitLineIntoWords(buffer, numberOfStrings);

    size_t passedLines = 0;
    size_t buff_i = 0;

    char command[200] = {}; // FIXME remove copy

    while(1){

        sscanf(buffer + buff_i, "%s", command);

        if (passedLines < FIRST_LINES_INFO_OF_FILE){
            
            buff_i += strlen(command) + 1;
            passedLines += 1;
            continue;

        } else {
            (*code)[passedLines - FIRST_LINES_INFO_OF_FILE] = buffer + buff_i;
        }

        passedLines += 1;

        if (passedLines == numberOfStrings)
            break;

        buff_i += strlen(command) + 1;
    }
    
    return COMMANDS_WAS_GETTED_SUCCSESSFULLY;

}

static void addZeroTerminator_splitLineIntoWords(char* buffer, const size_t numberOfStrings){

    size_t buffer_i = 0;
    //printf("%d\n", numberOfStrings);

    for (size_t j = 0; j < numberOfStrings;){
        
        if (buffer[buffer_i] == '\n'){
            buffer[buffer_i] = '\0';
            j++;
        }   else if (isspace(buffer[buffer_i])){
            buffer[buffer_i] = '_';
        }
        //printf("%c", buffer[buffer_i]);
        ++buffer_i;
    }

}