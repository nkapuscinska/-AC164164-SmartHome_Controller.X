/**
  Section: Included Files
*/
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/pin_manager.h"
#include "comm_module.h"
#include "mcc_generated_files/uart1.h"
#include "mcc_generated_files/delay.h"
#include "mcc_generated_files/drivers/timeout.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/adc1.h"
#include "buttons.h"
#include <stdio.h>
#include "xc.h" 
#include "mcc_generated_files/MCP9808Drivers/MCP9808.h"
#include "mcc_generated_files/i2c2.h"

/*
                         Main application
 */
   

#define SWITCHES_NUMBER 6

/* ------- Application Functions -------- */
void app_buttonsScheduler(void);                //Funckja obslugujaca akcje przyciskow SW1 i SW2

/* -------------------------------------- */



/* ---------- Global Variables ---------- */
uint8_t switchesStates[SWITCHES_NUMBER] = {0, 0, 0, 0, 0, 0};

extern eState MyState;
eState ButtonsState;

extern uint16_t temperatureReady;
/* -------------------------------------- */




/* ---------- Main Application ---------- */
int main(void)
{
    ADC1_Initialize();
    ADC1_Enable();
    ADC1_ChannelSelect(channel_AN8);
    ADC1_SoftwareTriggerEnable();
    MCP9808_i2c_params i2c_params;
    MCP9600_alert_flags alert_flags;
    i2c_params.i2cAddress = 0x18;
    i2c_params.i2cBusID = 2;
    
    MCP9808_MODULE_Initialize(i2c_params);
    MCP9808_SetResolutionValue(i2c_params, 0.125);
  
  
  
  
    printf("Application Start ............\n\r");
    SYSTEM_Initialize();
    
    printf("Buttons module init ............\n\r");
    BtnStateInit();
    
    printf("Communication module init ............\n\r");
    app_commModuleInit();               //inicjalizacja modulu komunikacyjnego (mqtt)
    
       
    
    while(1)
    {
        app_buttonsScheduler();
        app_mqttScheduler();
        
        //DELAY_milliseconds(200);
    }
    
    return 1;
}


void app_buttonsScheduler(void)
{
    ButtonsState = AskForState();            //Pobranie informacji o stanie przyciskow
    
    printf("Buttons state   -->   ");
    switch(ButtonsState)
    {
        case Released:
            printf("Released \n\r");
            break;
            
        case SW1Press:
            switchesStates[0] ^= 1;
            printf("SW1Press \n\r");
            break;
            
        case SW1Hold:
            switchesStates[1] ^= 1;
            printf("SW1Hold \n\r");
            break;
            
        case SW1Double:
            switchesStates[2] ^= 1;
            printf("SW1Double");
            break;
            
        case SW2Press:
            switchesStates[3] ^= 1;
            printf("SW2Press \n\r");
            break;
            
        case SW2Hold:
            switchesStates[4] ^= 1;
            printf("SW2Hold \n\r");
            break;
            
        case SW2Double:
            switchesStates[5] ^= 1;
            printf("SW2Double \n\r");
            break;
            
        default:
            printf("Default \n\r");
            break;
    }
}




////{
//        // Add your application code
//        DELAY_milliseconds(40);         //male opoznienie dla uC (nie musimy sie spieszyc))
//        app_mqttScheduler();            //scheduler mqtt - obsluguje cala komunikacje, wysyla dane raz na sekunde
//        
//        
//        /* Przyklad uzycia z przyciskami */
//        /* wcisniecie przycisku powoduje zapalenie odpowiedniej 
//         * diody LED i aktualizuje stan do wyslania przez mqtt*/
//        if(!SW1_GetValue())
//        {
//            app_updateKey1State(1);
//            LED_YELLOW_SetLow();
//        }
//        else
//        {
//            app_updateKey1State(0);
//            LED_YELLOW_SetHigh();
//        }
//        
//        if(!SW2_GetValue())
//        {
//            app_updateKey2State(1);
//            LED_BLUE_SetLow();
//        }
//        else
//        {
//            app_updateKey2State(0);
//            LED_BLUE_SetHigh();
//        }
//        
//        
//        
//        LED_RED_Toggle();               //mruganie dioda LED - jak dioda mruga to program dziala w glownej petli (nie zablokowal sie gdzies glebiej)
//    }


/**
 * 
 * 
 End of File
*/

