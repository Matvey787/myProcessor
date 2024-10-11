#
#include "../h_files/runCommands.h"
#include "../h_files/commands.h"

#include "../workWithStack/h_files/stressTests.h"
#include "../workWithStack/h_files/stackConstructor.h"
#include "../workWithStack/h_files/stackPushPop.h"
#include "../workWithStack/h_files/errorNames.h"
#include "../workWithStack/h_files/stackInitDestroy.h"
#include "../workWithStack/h_files/stackDump.h"
#include "../workWithStack/h_files/macros.h"

char executeCurrentCommand(const progCommands cmd, size_t* index_commands, double* commands, stack_t* stack);

void runCommands(stack_t* stack, double* commands){
    size_t index_commands = 0;
    char end = 0;
    progCommands cmd;
    while (1){
        cmd = (progCommands)commands[index_commands];
        end = executeCurrentCommand(cmd, &index_commands, commands, stack);
        if (end) break;
    }

}

char executeCurrentCommand(const progCommands cmd, size_t* index_commands, double* commands, stack_t* stack){
    StackElem_t firstNum = 0;
    StackElem_t secondNum = 0;
    StackElem_t poppedNum = 0;

    int clearBuffer = 0;

    switch (cmd)
    {
    case COMMAND_PUSH:
        stackPush(stack, commands[*index_commands + 1]);
        *index_commands += 2;
        break;

    case COMMAND_POP:
        stackPop(stack);
        *index_commands += 1;
        break;

    case COMMAND_ADD:
        firstNum = stackPop(stack);
        secondNum = stackPop(stack);
        stackPush(stack, firstNum+secondNum);
        *index_commands += 1;
        break;

    case COMMAND_SUB:
        firstNum = stackPop(stack);
        secondNum = stackPop(stack);
        stackPush(stack, secondNum-firstNum);
        *index_commands += 1;
        break;

    case COMMAND_MUL:
        firstNum = stackPop(stack);
        secondNum = stackPop(stack);
        stackPush(stack, secondNum*firstNum);
        *index_commands += 1;
        break;

    case COMMAND_DIV:
        firstNum = stackPop(stack);
        secondNum = stackPop(stack);
        stackPush(stack, secondNum/firstNum);
        *index_commands += 1;
        break;

    case COMMAND_OUT:
        poppedNum = stackPop(stack);
        printf("%lg\n", poppedNum);
        stackPush(stack, poppedNum);
        *index_commands += 1;
        break;

    case COMMAND_IN:
        printf("Enter a number: ");
        while (scanf("%lg", &firstNum) != 1) { while((clearBuffer = getchar() != '\n')); printf("Try again: "); };
        stackPush(stack, firstNum);
        *index_commands += 1;
        break;

    case COMMAND_HLT:
        return 1;
        break;

    default:
        return 1;
        break;
    }
    return 0;
}