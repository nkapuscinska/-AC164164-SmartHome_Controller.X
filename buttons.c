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


#define AKWISITION 2000
#define PRESSTIME 10
#define MAXPRESSTIME 200
#define HOLDTIME 500
#define DOUBLETIMEMAX 500
#define DOUBLETIMEMIN 200

typedef enum BtnState {Nothing, Pressed, Hold, Double}BtnState;
typedef enum InState{Idle, Read1, Read2}InState;
static BtnState eBtnState;
static InState eInState;
static eState MyState;

void CallBack(void){
    Flag1 = SW1_GetValue();
    Flag2 = SW2_GetValue();
    Counter++;
    switch(eInState){
        case Idle:

            if((Flag1 == 1)&&(Flag2 == 0)){
                eInState = Read2;
            }
            else if((Flag1 == 0)&& (Flag2 == 1)){
                eInState = Read1;
            }
            else {
                eInState = Idle;
            }
            MyState = Wait;
            break;
        case Read1:
             if((Flag1 == 0)&&(Counter < AKWISITION)){
                HighCounter++;
                eInState = Read1;
            }
            else if ((Flag1 == 1)&&(Counter < AKWISITION)){
                LowCounter++;
                eInState = Read1;
            }
            else {
                Counter = 0;
                if(HighCounter >= HOLDTIME){
                    MyState = SW1Hold;
                    printf("SW1Hold \n\r");
                }
                else if((HighCounter >= PRESSTIME)&&(HighCounter <= MAXPRESSTIME)){
                    MyState = SW1Press;
                    printf("SW1Press \n\r");
                }
                else if((HighCounter >= DOUBLETIMEMIN)&&(HighCounter < DOUBLETIMEMAX)){
                    MyState = SW1Double;
                    printf("SW1Double \n\r");
                }
                else {
                    MyState = Released;
                    printf("Released \n\r");
                }
                Counter = 0;
                HighCounter = 0;
                LowCounter = 0;
                eInState = Idle;
            }
            break;
        case Read2:
            if((Flag2 == 0)&&(Counter < AKWISITION)){
                HighCounter++;
                eInState = Read2;
            }
            else if ((Flag2 == 1)&&(Counter < AKWISITION)){
                LowCounter++;
                eInState = Read2;
            }
            else {
                Counter = 0;
                if(HighCounter >= HOLDTIME){
                    MyState = SW2Hold;
                }
                else if((HighCounter >= PRESSTIME)&&(HighCounter <= MAXPRESSTIME)){
                    MyState = SW2Press;
                }
                else if((HighCounter >= DOUBLETIMEMIN)&&(HighCounter < DOUBLETIMEMAX)){
                    MyState = SW2Double;
                }
                else {
                    MyState = Released;
                }
                Counter = 0;
                HighCounter = 0;
                LowCounter = 0;
                eInState = Idle;
                printf("State %d", MyState);
            }
            break;
        }
            
    }
    

void BtnStateInit(void){
    
    TMR2_SetInterruptHandler(&CallBack);
    TMR2_Initialize();
    TMR2_Start();
}
////
//eState AskForState(void){
//    return 0;
//
eState AskForState(void){
    eState PreviousState = Wait;
    if(PreviousState == MyState){
        //printf(" to samo %d \n\r", MyState);
        return Wait;
    }
    else{
       //printf("%d \n\r", MyState);
       PreviousState = MyState; 
       return MyState;
       
    }
}
