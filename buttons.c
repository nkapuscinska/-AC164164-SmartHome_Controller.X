/*
 * File:   Buttons.c
 * Author: Natalia
 *
 * Created on 27 stycznia 2024, 13:10
 */


#include "xc.h"
#include"buttons.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/tmr2.h"

uint16_t Counter = 0;
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

TMR2_SetInterruptHandler(* CallBack);

TMR2_Initialize();



enum BtnState {Nothing, Pressed, Hold, Double} eBtnState;
//enum MyState{Released, SW1Press, SW1Hold, SW1Double, Released2, SW2Press, SW2Hold, SW2Double }eState;
enum InState{Idle, Read1, Read2} eInState;


enum eState AskForState(void){
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
                break;
            
            case Read1:
                if(Counter > AKWISITION){
                    ReadBtnState(Flag1);
                    eInState = Read1;
                }
                else{
                    Counter = 0;
                    return eBtnState;
                    eInState = Idle;
                }
            case Read2:
                if(Counter > AKWISITION){
                    ReadBtnState(Flag2);
                    eInState = Read2;
                }
                else{
                    Counter = 0;
                    return eBtnState + 4;
                    eInState = Idle;
                }
                break;
    }
};

enum eBtnState ReadBtnState(uint8_t Flag){
                if((Flag == 1)&&(Counter < AKWISITION)){
                    HighCounter++;
                }
                else if ((Flag == 0)&&(Counter < AKWISITION)){
                    LowCounter++;
                }
                else {
                    if(HighCounter > MAXTIME){
                        return Hold;
                    }
                    else if((HighCounter < MINTIME)&& (LowCounter > MAXTIME)){
                        return Pressed;
                    }
                    else if (LowCounter >= AKWISITION){
                        return Nothing;
                    }
                    else {
                        return Double;
                    }
                }
}


