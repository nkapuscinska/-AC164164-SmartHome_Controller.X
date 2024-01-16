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
#include <stdio.h>

/*
                         Main application
 */
int main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    //app_mqttExampleInit();
    app_commModuleInit();
    
    app_updateTemperature(0.5);


    while (1)
    {
        // Add your application code
        DELAY_milliseconds(40);
        app_mqttScheduler();
        
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
        
        
        
        LED_RED_Toggle();
    }

    return 1;
}

/**
 End of File
*/

