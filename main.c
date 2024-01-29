/**
  Section: Included Files
*/
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/examples/comm_module.h"
#include "mcc_generated_files/uart1.h"
#include "mcc_generated_files/delay.h"
#include "mcc_generated_files/drivers/timeout.h"
#include "mcc_generated_files/pin_manager.h"
#include "buttons.h"
#include <stdio.h>

/*
                         Main application
 */
extern eState MyState;
int main(void)
{
    //initialize the device
    SYSTEM_Initialize();
    printf("Start");
    BtnStateInit();
    eState ReturnedState;

    app_commModuleInit();               //inicjalizacja modulu komunikacyjnego (mqtt))
    app_updateTemperature(0.5);         //ustawienie wartosci temoratury do wyslania
    
    while(1){
        ReturnedState = AskForState();
        switch(ReturnedState){
        case SW1Press:
            printf("SW1Press \n\r");
            break;
        case SW1Hold:
            printf("SW1Hold \n\r");
            break;
        case SW1Double:
            printf("SW1Double");
            break;
        case SW2Press:
            printf("SW2Press \n\r");
            break;
        case SW2Hold:
            printf("SW2Hold \n\r");
            break;
        case SW2Double:
            printf("SW2Double");
            break;
        default:
            printf("default");
            break;
        }
    }
    return 1;
}
//        switch(MyState){
//            case Released:
//                   
//                break;
//                    
//            case Released2:
//                
//                break;
//            
//            case SW1Press:
//                LED_YELLOW_SetLow();
//                LED_BLUE_SetHigh();
//                LED_RED_SetHigh();
//                LED_GREEN_SetHigh();
//                break;
//              
//            case SW1Hold:
//                LED_YELLOW_SetHigh();
//                LED_BLUE_SetHigh();
//                LED_RED_SetHigh();
//                LED_GREEN_SetHigh();
//                break;
//            case SW1Double:
//                LED_GREEN_SetLow();
//                LED_YELLOW_SetHigh();
//                LED_BLUE_SetHigh();
//                LED_RED_SetHigh();
//                break;
//            default:
//                LED_RED_SetLow();
//                LED_YELLOW_SetHigh();
//                LED_BLUE_SetHigh();
//                LED_GREEN_SetHigh();
//                break;
//                    
//        }                
//    }
//    return 1;



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

