#include <ncurses.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

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
                            __VA_ARGS__

const double c_compareZero = 0.001;

void runCode(spu_t* spu MYSBS(, size_t numberOfCommands)){
    assert((spu != nullptr) && "spu is nullptr in runCode");
    MYSBS(initscr();)
    MYSBS(writeCode(spu, numberOfCommands);)
    MYSBS(endwin();)

    while (1){
        progCommands cmd = spu->code[spu->ip].int_num >= 0 ? (progCommands)spu->code[spu->ip].int_num : 
                                                            (progCommands)spu->code[spu->ip].dbl_num;
        //printf("------------- %d\n", cmd);
        MYSBS(stepByStep(spu);)
        spu->ip++;

        if (!executeCurrentCommand(cmd, spu)){
            break;
        }
    }
}

short executeCurrentCommand(const progCommands cmd, spu_t* spu){
    assert((spu != nullptr) && "spu is nullptr in executeCurrentCommand");
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
        
        if (fabs(firstNum) > c_compareZero)
            stackPush(&(spu->stack), secondNum/firstNum);
        else{
            stackPush(&(spu->stack), DBL_MAX);
            printf("division by zero");
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
    
    // ----------------------------------------------------------------------------  SIN  ------------------------------------------------------------------
    case COMMAND_SIN:
        firstNum = stackPop(&(spu->stack));
        if (firstNum >= 0){
            stackPush(&(spu->stack), sin(firstNum));
            
        } else {
            printf("root of a negative number");
            return STOP_RUN;
        }
        break;
    
    // ----------------------------------------------------------------------------  COS  ------------------------------------------------------------------
    case COMMAND_COS:
        firstNum = stackPop(&(spu->stack));
        if (firstNum >= 0){
            stackPush(&(spu->stack), cos(firstNum));
            
        } else {
            printf("root of a negative number");
            return STOP_RUN;
        }
        break;
    
    // ----------------------------------------------------------------------------  LOG  ------------------------------------------------------------------
    case COMMAND_LOG:
    {
        firstNum = stackPop(&(spu->stack)); // base
        secondNum = stackPop(&(spu->stack)); // argument of log
        double ans = log(secondNum) / log(firstNum);
        if (firstNum >= 0){
            stackPush(&(spu->stack), ans);
            
        } else {
            printf("root of a negative number");
            return STOP_RUN;
        }
        break;
    }
        
    // ----------------------------------------------------------------------------  POW  ------------------------------------------------------------------
    case COMMAND_POW:
    {
        firstNum = stackPop(&(spu->stack)); // power
        secondNum = stackPop(&(spu->stack)); // argument of pow
        double ans = pow(secondNum, firstNum);
        if (firstNum >= 0){
            stackPush(&(spu->stack), ans);
            
        } else {
            printf("root of a negative number");
            return STOP_RUN;
        }
        break;
    }

    // ----------------------------------------------------------------------------  OUT  ------------------------------------------------------------------
    case COMMAND_OUT:
#ifndef STEPBYSTEP
        poppedNum = stackPop(&(spu->stack));
        if (fabs(poppedNum - DBL_MAX) < c_compareZero)
            printf("inf\n");
        else
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

    // ----------------------------------------------------------------------------  JB  ------------------------------------------------------------------
    case COMMAND_JB:
        MACRO_CASE_JUMP(
            if (secondNum < firstNum){

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

    // ----------------------------------------------------------------------------  JNE  ------------------------------------------------------------------
    case COMMAND_JNE:
        MACRO_CASE_JUMP(
            if (fabs(secondNum - firstNum) > c_compareZero)
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

    // ----------------------------------------------------------------------------  JBE  ------------------------------------------------------------------
    case COMMAND_JBE:
        MACRO_CASE_JUMP(
        if (secondNum <= firstNum)
            spu->ip = (size_t)(spu->code)[spu->ip].dbl_num;
        else 
            spu->ip += 1;
        )
        break;

    // ----------------------------------------------------------------------------  JMP  ------------------------------------------------------------------
    case COMMAND_JMP:
        spu->ip = (size_t)(spu->code)[spu->ip].dbl_num;

        break;
    
    // ----------------------------------------------------------------------------  CALL  ------------------------------------------------------------------
    case COMMAND_CALL:

        stackPush(&(spu->stack), (StackElem_t)(spu->ip + 1));
        spu->ip = (size_t)(spu->code)[spu->ip].dbl_num;

        break;
    // ----------------------------------------------------------------------------  RET  ------------------------------------------------------------------
    case COMMAND_RET:
    {
        size_t ipToGoTo = (size_t)stackPop(&(spu->stack));
        spu->ip = ipToGoTo;

        break;
    }

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

    if (modType & 2) { result += (spu->regData)[spu->code[(spu->ip)++].int_num]; }

    if (modType & 4)
    {
        int addr = (int)result;
        result = (spu->RAM)[addr];
    }
    return result;
}



double* getAddress(spu_t* spu){
   
    int modType = spu->code[spu->ip++].int_num;
    //printf("get addres called %d\n", modType);
    int ptr_arr = 0;
    if ((modType & 1) && (modType & 2) && (modType & 4)) {
        ptr_arr = (int)( spu->code[spu->ip].dbl_num );
        spu->ip++;
        ptr_arr += (int)( (spu->regData)[spu->code[(spu->ip)].int_num] ) ;
        spu->ip++;
        //printf("ptr addres in ram: %d\n", ptr_arr);
        return &((spu->RAM)[ptr_arr]);
    }
    else if ((modType & 1) && (modType & 4)){
        ptr_arr = (int)( spu->code[(spu->ip)++].dbl_num );
        return &((spu->RAM)[ptr_arr]);

    } else if ((modType & 2) && (modType & 4)){
        ptr_arr = (int)( (spu->regData)[spu->code[(spu->ip)++].int_num] );
        return &((spu->RAM)[ptr_arr]);

    } else if ((modType & 2)){
        ptr_arr = (int)( spu->code[(spu->ip)++].int_num );
        return &(spu->regData[ptr_arr]);
    } 

    return nullptr;
}
