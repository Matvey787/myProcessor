#include "../h_files/spu.h"
#include "../h_files/commands.h"
#include "../h_files/stepByStep.h"
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include "../../workWithStack/h_files/stackPushPop.h"
#include "../../workWithStack/h_files/macros.h"
#include "../../workWithStack/h_files/stackDump.h"

void printwCurrentCommand(spu_t* spu, int yCoord_stack);
void printwOut(spu_t* spu, int yCoord_out);
void printwStak(spu_t* spu, int yCoord_stack);
void printRegisterName(progRegisters indexOfRegister);
void printwRegisters(spu_t* spu, int yCoord_Registers);

static int yCoord_stack_globVar = 0;

int stepByStep(spu_t* spu){
    curs_set(0);

    // get x size of cmd screen
    int xCmdLineSize = getmaxx(stdscr);

    int max_column = xCmdLineSize / 11;

    int row = (int)spu->ip / max_column;
    int col = (int)spu->ip % max_column;

    row = row * 3 + 2; // FIXME make consts
    col = col * 11;

    move(row, col);
    printw("^");
    printwCurrentCommand(spu, yCoord_stack_globVar);

    refresh();
    int c = 0;
    noecho();

    while((c = getch()) != 'q');
    if (c == 'q'){ move(row, col); clrtoeol();};

    row = (int)(spu->ip + 1) / max_column;
    col = (int)(spu->ip +1) % max_column;
    row = row * 3 + 2;
    col = col * 11;

    return 1;
}

void writeCode(spu_t* spu, size_t numberOfCommands){

    // get size of cmd screen
    int xCmdLineSize = getmaxx(stdscr);
    

    //printw("%d %d", yCmdLineSize, xCmdLineSize);
    int writedInfo = 0;

    int xCoord_indexes = 0;
    int yCoord_indexes = 0;
    int xCoord_arguments = 0;
    int yCoord_arguments = 1;

    move(yCoord_indexes, xCoord_indexes);
    printw("|");
    writedInfo += 1;

    for (size_t i = 0; i < numberOfCommands; i++){

        if (writedInfo + 11 > xCmdLineSize){
            yCoord_indexes += 3;
            move(yCoord_indexes, 0);
            printw("|");
            writedInfo = 1;
        }
            
        if (i > 9) 
            printw("   [%lu]   |", i);
        else
            printw("   [0%lu]   |", i);
        writedInfo += 11;
    }

    // -------------------------------------------- write arguments above indexes ---------------------------------------------------------
    move(yCoord_arguments, xCoord_arguments);
    printw("|");
    writedInfo = 1;

    for (size_t i = 0; i < numberOfCommands; i++){
        
        if (writedInfo + 11 > xCmdLineSize){
            yCoord_arguments += 3;
            move(yCoord_arguments, 0);
            printw("|");
            writedInfo = 1;
        }

        char codeSellStr[40];
        if (spu->code[i].int_num >= 0){
            int codeCell = spu->code[i].int_num;
            printw(" %d", codeCell);
            sprintf(codeSellStr, "%d", codeCell);
            for (size_t j = 0; j < (9 - strlen(codeSellStr)); j++)
                printw(" ");
            printw("|");

        } else {
            double codeCell = spu->code[i].dbl_num;
            printw(" %lg", codeCell);
            sprintf(codeSellStr, "%lg", codeCell);
            for (size_t j = 0; j < (9 - strlen(codeSellStr)); j++)
                printw(" ");
            printw("|");
        }

        writedInfo += 11;
    }
    yCoord_stack_globVar = yCoord_arguments+2; // global variable need for y coord of stack

    move(yCoord_stack_globVar, 0);
    printw("Stack:");

    // + 1 becauze out go after registers
    move(yCoord_stack_globVar + 1, 0);
    printw("Registers:");

    // + 2 becauze out go after registers
    move(yCoord_stack_globVar + 2, 0);
    printw("Out:");

    refresh();
}

void printwCurrentCommand(spu_t* spu, int yCoord_stack){
    progCommands cmd = NOT_COMMAND;

    cmd = (progCommands)spu->code[spu->ip].int_num;
    printwStak(spu, yCoord_stack);
    printwRegisters(spu, yCoord_stack + 1);
    
    switch (cmd)
    {
    case COMMAND_PUSH:
        printw("(push)--mod----^");
        break;

    case COMMAND_POP:
        printw("(pop)--mod----^");
        break;

    case COMMAND_ADD:
        printw("(add)");
        break;

    case COMMAND_SUB:
        printw("(sub)");
        break;

    case COMMAND_MUL:
        printw("(mul)");
        break;

    case COMMAND_DIV:
        printw("(div)");
        break;

    case COMMAND_OUT:
        printw("(out)");
        printwOut(spu, yCoord_stack + 2);
        break;

    case COMMAND_IN:
        printw("(in)");
        break;

    case COMMAND_JA:
        printw("(ja)------^");
        break;

    case COMMAND_JE:
        printw("(je)------^");
        break;
    
    case COMMAND_JMP:
        printw("(jmp)-----^");
        break;
    
    case COMMAND_JAE:
        printw("(jae)-----^");
        break;
    
    case COMMAND_HLT:
        printw("(hlt)");
        break;
    case NOT_COMMAND:
    default:
        printw("(err)");
        break;
    }
}

void printwOut(spu_t* spu, int yCoord_out){

    int old_y = 0;
    int old_x = 0;
    getyx(stdscr, old_y, old_x);

    static int indentForOut = 0;

    StackElem_t poppedNum = stackPop(&(spu->stack));

    move(yCoord_out, 5 + indentForOut);
    printw("%lg", poppedNum);
    refresh();

    move(old_y, old_x);

    indentForOut += 4;
}

void printwStak(spu_t* spu, int yCoord_stack){
    int old_y = 0;
    int old_x = 0;
    getyx(stdscr, old_y, old_x);

    move(yCoord_stack, 7);
    for (size_t i = 0; i < spu->stack.size; i++){

        #ifdef TURN_ON_CANARIES
        printw("%lg ", spu->stack.data[i + 1]);
        #else
        printw("%lg ", spu->stack.data[i]);
        #endif

    }
    clrtoeol();
    refresh();

    move(old_y, old_x);
}

void printwRegisters(spu_t* spu, int yCoord_Registers){
    int old_y = 0;
    int old_x = 0;
    getyx(stdscr, old_y, old_x);

    move(yCoord_Registers, 11);

    for (int i = 0; i < NUMBER_OF_REGISTERS; i++){
        printRegisterName((progRegisters)i);
        printw(":= %lg; ", spu->regData[i]);
    }
    clrtoeol();
    refresh();

    move(old_y, old_x);
}

void printRegisterName(progRegisters indexOfRegister){
    switch (indexOfRegister)
    {
    case AX:
        printw("AX");
        break;
    case BX:
        printw("BX");
        break;
    case CX:
        printw("CX");
        break;
    case DX:
        printw("DX");
        break;

    case NOT_REGISTER:
    default:
        break;
    }

}