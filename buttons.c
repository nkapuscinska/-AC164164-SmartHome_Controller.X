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
#include "mcc_generated_files/delay.h"

volatile int Stopper = 0;
volatile uint16_t HighCounter1 = 0;
volatile uint16_t LowCounter = 0;
volatile uint16_t HighCounter2 = 0;
uint16_t LowCounter2 = 0;
uint8_t Flag1 = 0;
uint8_t Flag2 = 0;
uint8_t Buttons = 0;


#define AKWISITION 200
#define PRESSTIME 1
#define MAXPRESSTIME 20
#define HOLDTIME 50
#define DOUBLETIMEMAX 50
#define DOUBLETIMEMIN 20

typedef enum BtnState {Nothing, Pressed, Hold, Double}BtnState;
typedef enum InState{Idle, Read1, Read2}InState;
BtnState eBtnState;
InState eInState;
volatile eState MyState;
eState PreviousState;

void CallBack(void){
    Stopper++;
        if(Stopper <= AKWISITION){
            if((SW1_GetValue() == 1)&&(SW2_GetValue() == 0)){
                //printf("SW1Pressed");
                HighCounter1++;
            }
            else if((SW1_GetValue() == 0)&& (SW2_GetValue() == 1)){
                HighCounter2++;
                //printf("SW2Pressed");
            }
            else {
                LowCounter++;
                //printf("NothinPresed");
            }  
        }
        else{
            HighCounter1 = 0;
            HighCounter2 = 0;
            LowCounter = 0;
            Stopper = 0;
        }
    }
    

void BtnStateInit(void){
    
    TMR2_SetInterruptHandler(&CallBack);
    TMR2_Initialize();
    TMR2_Start();
}


eState BtnStateFunction(void){
    if((HighCounter1 > 0)&&(Stopper >= AKWISITION) ){
        if(HighCounter1 >= HOLDTIME){
            //printf("SW1Hold \n\r");
            return SW1Hold;
        }
        else if((HighCounter1 >= PRESSTIME)&&(HighCounter1 <= MAXPRESSTIME)){
            //printf("SW1Press \n\r");
            return SW1Press;
        }
        else if((HighCounter1 >= DOUBLETIMEMIN)&&(HighCounter1 < DOUBLETIMEMAX)){
            //printf("SW1Double \n\r");
            return SW1Double;
        }
        else{
            //printf("Released \n\r");
            return Released;
        }
    }
    else if ((HighCounter2 > 0)&&(Stopper >= AKWISITION) ){
        if(HighCounter2 >= HOLDTIME){
            //printf("SW2Hold \n\r");
            return SW2Hold;
        }
        else if((HighCounter2 >= PRESSTIME)&&(HighCounter2 <= MAXPRESSTIME)){
            //printf("SW2Press \n\r");
            return SW2Press;
        }
        else if((HighCounter2 >= DOUBLETIMEMIN)&&(HighCounter2 < DOUBLETIMEMAX)){
            //printf("SW2Double \n\r");
            return SW2Double;
        }
        else{
            //printf("Released \n\r");
            return Released;
        }
    }
    else{
        return Released;
        }
    }
    eState AskForState(void){
    MyState = BtnStateFunction();
    if(PreviousState == MyState){
        return Released;
    }
    else{
        PreviousState = MyState;
        return MyState;
    }
}