#include <ncurses.h>
#include <stdio.h>
#include <math.h>

#include "../h_files/spu.h"
#include "../h_files/runCode.h"
#include "../h_files/commands.h"
#include "../h_files/stepByStep.h"
#include "../h_files/macros.h"
#include "../h_files/getNumber.h"

#include "../../workWithStack/h_files/stressTests.h"
#include "../../workWithStack/h_files/stackConstructor.h"
#include "../../workWithStack/h_files/stackPushPop.h"
#include "../../workWithStack/h_files/errorNames.h"
#include "../../workWithStack/h_files/stackInitDestroy.h"
#include "../../workWithStack/h_files/stackDump.h"
#include "../../workWithStack/h_files/macros.h"

short executeCurrentCommand(const progCommands cmd, spu_t* spu);
double getArg(spu_t* spu);
double* getAddress(spu_t* spu);

#define STOP_RUN 0
#define CONTINUE_RUN 1

#define MACRO_CASE_JUMP(...) firstNum = stackPop(&(spu->stack)); \
                            secondNum = stackPop(&(spu->stack)); \
                            __VA_ARGS__ \
                            stackPush(&(spu->stack), secondNum); \
                            stackPush(&(spu->stack), firstNum);

const double c_compareZero = 0.001;

void runCode(spu_t* spu MYSBS(, size_t numberOfCommands)){

    MYSBS(initscr();)
    MYSBS(writeCode(spu, numberOfCommands);)
    MYSBS(endwin();)

    while (1){
        progCommands cmd = spu->code[spu->ip].int_num >= 0 ? (progCommands)spu->code[spu->ip].int_num : 
                                                            (progCommands)spu->code[spu->ip].dbl_num;
        
        MYSBS(stepByStep(spu);)
        spu->ip++;

        if (!executeCurrentCommand(cmd, spu)){
            break;
        }
    }
}

short executeCurrentCommand(const progCommands cmd, spu_t* spu){
    StackElem_t firstNum = 0;
    StackElem_t secondNum = 0;
    StackElem_t poppedNum = 0;


    //stackDump(&spu->stack);

    switch (cmd)
    {
    // ----------------------------------------------------------------------------  PUSH  ------------------------------------------------------------------
    case COMMAND_PUSH:
        stackPush(&(spu->stack), getArg(spu));

        break;

    // ----------------------------------------------------------------------------  POP  ------------------------------------------------------------------
    case COMMAND_POP:

        poppedNum = stackPop(&(spu->stack));

        *(getAddress(spu)) = poppedNum;

        break;

    // ----------------------------------------------------------------------------  ADD  ------------------------------------------------------------------
    case COMMAND_ADD:
        firstNum = stackPop(&(spu->stack));
        secondNum = stackPop(&(spu->stack));

        stackPush(&(spu->stack), firstNum + secondNum);

        break;
    // ----------------------------------------------------------------------------  SUB  ------------------------------------------------------------------
    case COMMAND_SUB:
        firstNum = stackPop(&(spu->stack));
        secondNum = stackPop(&(spu->stack));

        stackPush(&(spu->stack), secondNum - firstNum);

        break;
    // ----------------------------------------------------------------------------  MUL  ------------------------------------------------------------------
    case COMMAND_MUL:
        firstNum = stackPop(&(spu->stack));
        secondNum = stackPop(&(spu->stack));

        stackPush(&(spu->stack), secondNum*firstNum);

        break;

    // ----------------------------------------------------------------------------  DIV  ------------------------------------------------------------------
    case COMMAND_DIV:
        firstNum = stackPop(&(spu->stack));
        secondNum = stackPop(&(spu->stack));
        if (firstNum != 0)
            stackPush(&(spu->stack), secondNum/firstNum);
        else{
            printf("division by zero");
            return STOP_RUN;
        }
        break;

    // ----------------------------------------------------------------------------  SQRT  ------------------------------------------------------------------
    case COMMAND_SQRT:
        firstNum = stackPop(&(spu->stack));
        if (firstNum >= 0){
            stackPush(&(spu->stack), sqrt(firstNum));
            
        } else {
            printf("root of a negative number");
            return STOP_RUN;
        }
        break;

    // ----------------------------------------------------------------------------  OUT  ------------------------------------------------------------------
    case COMMAND_OUT:
#ifndef STEPBYSTEP
        poppedNum = stackPop(&(spu->stack));
        printf("Out: %lg\n", poppedNum);
#endif
        
        break;

    // ----------------------------------------------------------------------------  IN  ------------------------------------------------------------------
    case COMMAND_IN:
        
        getCoeff("Enter a number: ", &firstNum);

        stackPush(&(spu->stack), firstNum);

        break;

    // ----------------------------------------------------------------------------  JA  ------------------------------------------------------------------
    case COMMAND_JA:
        MACRO_CASE_JUMP(
            if (secondNum > firstNum){

            spu->ip = (size_t)((spu->code)[spu->ip].dbl_num);
        } else 
            spu->ip += 1;
        )
        break;

    // ----------------------------------------------------------------------------  JE  ------------------------------------------------------------------
    case COMMAND_JE:
        MACRO_CASE_JUMP(
            if (fabs(secondNum - firstNum) <= c_compareZero)
            spu->ip = (size_t)(spu->code)[spu->ip].dbl_num;
        else 
            spu->ip += 1;
        )
        break;

    // ----------------------------------------------------------------------------  JAE  ------------------------------------------------------------------
    case COMMAND_JAE:
        MACRO_CASE_JUMP(
        if (secondNum >= firstNum)
            spu->ip = (size_t)(spu->code)[spu->ip].dbl_num;
        else 
            spu->ip += 1;
        )
        break;

    // ----------------------------------------------------------------------------  JMP  ------------------------------------------------------------------
    case COMMAND_JMP:
        spu->ip = (size_t)(spu->code)[spu->ip].dbl_num;

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

double getArg(spu_t* spu){

    int modType = spu->code[spu->ip++].int_num;
    double result = 0;

    if (modType & 1) result = spu->code[(spu->ip)++].dbl_num;

    if (modType & 2) {result += (spu->regData)[spu->code[(spu->ip)++].int_num];}

    if (modType & 4) {
        int addr = (int)result;
        result = (spu->RAM)[addr];
    }
    return result;
}



double* getAddress(spu_t* spu){

    int modType = spu->code[spu->ip++].int_num;

    int ptr_arr = 0;

    if ((modType & 1) && (modType & 4)){
        ptr_arr = (int)( spu->code[(spu->ip)++].dbl_num );
        return &((spu->RAM)[ptr_arr]);

    } else if ((modType & 2) && (modType & 4)){
        ptr_arr = (int)( spu->code[(spu->ip)++].int_num );
        return &((spu->RAM)[ptr_arr]);

    } else if ((modType & 2)){
        ptr_arr = (int)( spu->code[(spu->ip)++].int_num );
        return &(spu->regData[ptr_arr]);
    } 

    return nullptr;
}