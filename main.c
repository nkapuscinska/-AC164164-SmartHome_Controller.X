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
#include "mcc_generated_files/MCP9808Drivers/MCP9808.h"
#include "mcc_generated_files/i2c2.h"

/*
                         Main application
 */
extern uint16_t temperatureReady;

int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
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
    
    TMR1_Start();

    
//    app_commModuleInit();               //inicjalizacja modulu komunikacyjnego (mqtt))
//    app_updateTemperature(0.5);         //ustawienie wartosci temoratury do wyslania
  
    while (1)
    {
        // Add your application code
        DELAY_milliseconds(40);         //male opoznienie dla uC (nie musimy sie spieszyc))
        app_mqttScheduler();            //scheduler mqtt - obsluguje cala komunikacje, wysyla dane raz na sekunde
        readAndDisplayTemperature();
     
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

