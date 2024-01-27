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
#include "mcc_generated_files/adc1.h"
#include <stdio.h>
#include "xc.h" 

/*
                         Main application
 */
int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    ADC1_Initialize();
    ADC1_Enable();
    ADC1_ChannelSelect(channel_AN8);
    ADC1_SoftwareTriggerEnable();

//    app_commModuleInit();               //inicjalizacja modulu komunikacyjnego (mqtt))
//    app_updateTemperature(0.5);         //ustawienie wartosci temoratury do wyslania
    float light;
    char lighton;

    while (1)
    {
        // Add your application code
        DELAY_milliseconds(40);         //male opoznienie dla uC (nie musimy sie spieszyc))
        app_mqttScheduler();            //scheduler mqtt - obsluguje cala komunikacje, wysyla dane raz na sekunde
      
        ADC_ReadLightDensity(); // try read light density.
        
        
        lighton = LightCheck(light);
        /* Przyklad uzycia z przyciskami */
        /* wcisniecie przycisku powoduje zapalenie odpowiedniej 
         * diody LED i aktualizuje stan do wyslania przez mqtt*/
        if(!SW1_GetValue())
        {
            app_updateKey1State(1);
            LED_YELLOW_SetLow();
        }
        else
        {
            app_updateKey1State(0);
            LED_YELLOW_SetHigh();
        }
        
        if(!SW2_GetValue())
        {
            app_updateKey2State(1);
            LED_BLUE_SetLow();
        }
        else
        {
            app_updateKey2State(0);
            LED_BLUE_SetHigh();
        }
        
        
        
        LED_RED_Toggle();               //mruganie dioda LED - jak dioda mruga to program dziala w glownej petli (nie zablokowal sie gdzies glebiej)
    }

    return 1;
}

/**
 End of File
*/

