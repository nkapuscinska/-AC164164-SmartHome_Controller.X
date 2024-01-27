/*
 * File:   Buttons.c
 * Author: Natalia
 *
 * Created on 27 stycznia 2024, 13:10
 */


#include "xc.h"
#include "buttons.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/tmr2.h"

volatile uint16_t Counter = 0;
uint16_t HighCounter = 0;
uint16_t LowCounter = 0;
uint16_t HighCounter2 = 0;
uint16_t LowCounter2 = 0;
uint8_t Flag1 = 0;
uint8_t Flag2 = 0;


#define AKWISITION 200
#define MAXTIME 90
#define MINTIME 10

void CallBack(void){
    Flag1 = SW1_GetValue();
    Flag2 = SW2_GetValue();
    Counter++;
}


void BtnStateInit(void){
    
    TMR2_SetInterruptHandler(&CallBack);
    TMR2_Initialize();
}



typedef enum BtnState {Nothing, Pressed, Hold, Double}BtnState;
typedef enum InState{Idle, Read1, Read2}InState;
BtnState eBtnState;
InState eInState;
eState MyState;



eState ReadBtnState(uint8_t Flag, uint8_t Button){
    uint8_t adder = 0;
    if(Button = 1){
        adder = 0;
    }
    else{
        adder = 4;
    }
                if((Flag == 1)&&(Counter < AKWISITION)){
                    HighCounter++;
                }
                else if ((Flag == 0)&&(Counter < AKWISITION)){
                    LowCounter++;
                }
                else {
                    if(HighCounter > MAXTIME){
                        return SW1Hold + adder;
                    }
                    else if((HighCounter < MINTIME)&& (LowCounter > MAXTIME)){
                        return SW1Press + adder;
                    }
                    else if (LowCounter >= AKWISITION){
                        return Released;
                    }
                    else {
                        return SW1Double + adder;
                    }
                }
                return Released;
}


eState AskForState(void){
    eState InState;
    switch(eInState){
            case Idle:
                if((Flag1 == 1)&&(Flag2 == 0)){
                    eInState = Read1;
                }
                else if((Flag1 == 0)&& (Flag2 == 1)){
                    eInState = Read2;
                }
                else {
                    eInState = Idle;
                }
                return Wait;
                break;
            
            case Read1:
                if(Counter > AKWISITION){
                    ReadBtnState(Flag1, 1);
                    eInState = Read1;
                    return Wait;
                }
                else{
                    Counter = 0;
                    eInState = Idle;
                    InState = ReadBtnState(Flag1, 1);
                    return InState;
                }
            case Read2:
                if(Counter > AKWISITION){
                    ReadBtnState(Flag1, 1);
                    InState = ReadBtnState(Flag2, 2);
                    return Wait;
                }
                else{
                    InState = ReadBtnState(Flag2, 2);
                    Counter = 0;
                    eInState = Idle;
                    return InState;
                    
                }
                break;
        default:
            return Wait;
            break;
    }
}


