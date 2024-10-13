#include "../h_files/spu.h"
#include "../h_files/runCode.h"
#include "../h_files/commands.h"

#include "../../workWithStack/h_files/stressTests.h"
#include "../../workWithStack/h_files/stackConstructor.h"
#include "../../workWithStack/h_files/stackPushPop.h"
#include "../../workWithStack/h_files/errorNames.h"
#include "../../workWithStack/h_files/stackInitDestroy.h"
#include "../../workWithStack/h_files/stackDump.h"
#include "../../workWithStack/h_files/macros.h"

short executeCurrentCommand(const progCommands cmd, spu_t* spu);

#define STOP_RUN 0
#define CONTINUE_RUN 1

void runCode(spu_t* spu){

    while (1){
        progCommands cmd = (progCommands)atoi(spu->code[spu->ip]);

        if (!executeCurrentCommand(cmd, spu))
            break;
    }

}

short executeCurrentCommand(const progCommands cmd, spu_t* spu){
    StackElem_t firstNum = 0;
    StackElem_t secondNum = 0;
    StackElem_t poppedNum = 0;

    int clearBuffer = 0;

    //stackDump(&spu->stack);

    switch (cmd)
    {
    case COMMAND_PUSH:
        stackPush(&(spu->stack), atof(spu->code[spu->ip+1]));
        spu->ip += 2;
        break;

    case COMMAND_PUSH_REGISTER:
        firstNum = spu->regData[atoi(spu->code[spu->ip + 1])];

        stackPush(&(spu->stack), firstNum);

        spu->ip += 2;

        break;

    case COMMAND_POP:
        firstNum = stackPop(&(spu->stack));
        spu->regData[atoi(spu->code[spu->ip + 1])] = firstNum;
        spu->ip += 2;
        break;

    case COMMAND_ADD:
        firstNum = stackPop(&(spu->stack));
        secondNum = stackPop(&(spu->stack));

        stackPush(&(spu->stack), firstNum + secondNum);

        spu->ip += 1;
        break;

    case COMMAND_SUB:
        firstNum = stackPop(&(spu->stack));
        secondNum = stackPop(&(spu->stack));

        stackPush(&(spu->stack), secondNum - firstNum);

        spu->ip += 1;
        break;

    case COMMAND_MUL:
        firstNum = stackPop(&(spu->stack));
        secondNum = stackPop(&(spu->stack));

        stackPush(&(spu->stack), secondNum*firstNum);

        spu->ip += 1;
        break;

    case COMMAND_DIV:
        firstNum = stackPop(&(spu->stack));
        secondNum = stackPop(&(spu->stack));

        stackPush(&(spu->stack), secondNum/firstNum);
        
        spu->ip += 1;
        break;

    case COMMAND_OUT:
        poppedNum = stackPop(&(spu->stack));

        printf("%lg\n", poppedNum);

        // stackPush(&(spu->stack), atof(spu->code[spu->ip+1]));

        spu->ip += 1;
        break;

    case COMMAND_IN:
        printf("Enter a number: ");

        while (scanf("%lg", &firstNum) != 1) { while((clearBuffer = getchar() != '\n')); printf("Try again: "); }; // FIXME  func + error handling

        stackPush(&(spu->stack), firstNum);

        spu->ip += 1;
        break;

    case COMMAND_HLT:
        return STOP_RUN;
        break;

    default:
        printf("something go wrong...\n");
        return STOP_RUN;
        break;
    }
    return CONTINUE_RUN;
}