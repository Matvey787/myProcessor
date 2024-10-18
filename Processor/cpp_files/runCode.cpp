#include <ncurses.h>
#include <stdio.h>
#include <math.h>

#include "../h_files/spu.h"
#include "../h_files/runCode.h"
#include "../h_files/commands.h"
#include "../h_files/stepByStep.h"
#include "../h_files/macros.h"

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

const double compareZero = 0.001;

void runCode(spu_t* spu MYSBS(, size_t numberOfCommands)){

    MYSBS(initscr();)
    MYSBS(writeCode(spu, numberOfCommands);)
    MYSBS(endwin();)

    while (1){
        progCommands cmd = (progCommands)atoi(spu->code[spu->ip]);

        MYSBS(stepByStep(spu, 0);)

        if (!executeCurrentCommand(cmd, spu)){
            break;
        }
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
    // ----------------------------------------------------------------------------  PUSH  ------------------------------------------------------------------
    case COMMAND_PUSH:
        stackPush(&(spu->stack), atof(spu->code[++spu->ip]));

        MYSBS(stepByStep(spu, 1);)

        ++spu->ip;
        break;
    // ----------------------------------------------------------------------------  PUSHR  ------------------------------------------------------------------
    case COMMAND_PUSH_REGISTER:
        firstNum = spu->regData[atoi(spu->code[++spu->ip])];

        stackPush(&(spu->stack), firstNum);

        MYSBS(stepByStep(spu, 1);)

        ++spu->ip;

        break;

    // ----------------------------------------------------------------------------  POP  ------------------------------------------------------------------
    case COMMAND_POP:
        poppedNum = stackPop(&(spu->stack));
        spu->regData[atoi(spu->code[++spu->ip])] = poppedNum;

        MYSBS(stepByStep(spu, 1);)

        ++spu->ip;
        break;

    // ----------------------------------------------------------------------------  ADD  ------------------------------------------------------------------
    case COMMAND_ADD:
        firstNum = stackPop(&(spu->stack));
        secondNum = stackPop(&(spu->stack));

        stackPush(&(spu->stack), firstNum + secondNum);

        ++spu->ip;
        break;
    // ----------------------------------------------------------------------------  SUB  ------------------------------------------------------------------
    case COMMAND_SUB:
        firstNum = stackPop(&(spu->stack));
        secondNum = stackPop(&(spu->stack));

        stackPush(&(spu->stack), secondNum - firstNum);

        ++spu->ip;
        break;
    // ----------------------------------------------------------------------------  MUL  ------------------------------------------------------------------
    case COMMAND_MUL:
        firstNum = stackPop(&(spu->stack));
        secondNum = stackPop(&(spu->stack));

        stackPush(&(spu->stack), secondNum*firstNum);

        ++spu->ip;
        break;

    // ----------------------------------------------------------------------------  DIV  ------------------------------------------------------------------
    case COMMAND_DIV:
        firstNum = stackPop(&(spu->stack));
        secondNum = stackPop(&(spu->stack));

        stackPush(&(spu->stack), secondNum/firstNum); // TODO check a / 0

        ++spu->ip;
        break;

    // ----------------------------------------------------------------------------  OUT  ------------------------------------------------------------------
    case COMMAND_OUT:

        #ifndef STEPBYSTEP
        poppedNum = stackPop(&(spu->stack));
        printf("Out: %lg\n", poppedNum);
        #endif
        
        ++spu->ip;
        break;

    // ----------------------------------------------------------------------------  IN  ------------------------------------------------------------------
    case COMMAND_IN:
        printf("Enter a number: ");

        while (scanf("%lg", &firstNum) != 1) { while((clearBuffer = getchar() != '\n')); printf("Try again: "); }; // FIXME  func + error handling

        stackPush(&(spu->stack), firstNum);

        ++spu->ip;
        break;

    // ----------------------------------------------------------------------------  JA  ------------------------------------------------------------------
    case COMMAND_JA: // TODO remove copypaste
        firstNum = stackPop(&(spu->stack));
        secondNum = stackPop(&(spu->stack));
        if (secondNum > firstNum){
            //MYSBS(stepByStep(spu, 1);)
            spu->ip = (size_t)atoi(spu->code[++spu->ip]);
        } else 
            spu->ip += 2;

        stackPush(&(spu->stack), secondNum);
        stackPush(&(spu->stack), firstNum);
        break;

    // ----------------------------------------------------------------------------  JE  ------------------------------------------------------------------
    case COMMAND_JE:
        firstNum = stackPop(&(spu->stack));
        secondNum = stackPop(&(spu->stack));
        if (fabs(secondNum - firstNum) <= compareZero)
            spu->ip = (size_t)atoi(spu->code[++spu->ip]);
        else 
            spu->ip += 2;

        stackPush(&(spu->stack), secondNum);
        stackPush(&(spu->stack), firstNum);

        break;

    // ----------------------------------------------------------------------------  JAE  ------------------------------------------------------------------
    case COMMAND_JAE:
        firstNum = stackPop(&(spu->stack));
        secondNum = stackPop(&(spu->stack));
        if (secondNum >= firstNum)
            spu->ip = (size_t)atoi(spu->code[++spu->ip]);
        else 
            spu->ip += 2;
            
        stackPush(&(spu->stack), secondNum);
        stackPush(&(spu->stack), firstNum);
        break;

    // ----------------------------------------------------------------------------  JMP  ------------------------------------------------------------------
    case COMMAND_JMP:
        spu->ip = (size_t)atoi(spu->code[++spu->ip]);

        ++spu->ip;
        break;

    // ----------------------------------------------------------------------------  HLT  ------------------------------------------------------------------
    case COMMAND_HLT:

        MYSBS(
        endwin();
        setvbuf(stdout, NULL, _IOLBF, 0);
        setvbuf(stderr, NULL, _IONBF, 0);
        )

        printf("Programm halt\n");

        return STOP_RUN;
        break;

    case NOT_COMMAND:
    default:
        printf("something go wrong...\n");
        return STOP_RUN;
        break;
    }
    return CONTINUE_RUN;
}