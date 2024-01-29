/*
    © [2020] Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any derivatives 
    exclusively with Microchip products. You're responsible for complying with 3rd party license terms 
    applicable to your use of 3rd party software (including open source software) that may accompany 
    Microchip software. SOFTWARE IS "AS IS." NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR 
    STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON- 
    INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR 
    CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED 
    TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE 
    POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY 
    LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE. 
*/

#ifndef COMM_MODULE_H
#define	COMM_MODULE_H

#define MQTT_PARAMS_NUM 3
#define PAYLOAD_LENGHT 16
#define TOPIC_LENGHT 16

#define PUB_TOPIC "home/data"

// Nazwy parametrów przeznaczonych do wysslnia za pomoca MQTT, nie modyfikowac domyslnej kolejnosci(numeracji) wartosci enuma.
typedef enum MQTT_parameter {TEMPERATURE, KEY1_STATE, KEY2_STATE} MQTT_parameter;


/*    FUNKCJE NIE UZYWANE    */
void MQTT_setParameterTopic(MQTT_parameter param, char * topic);
/* ------------------------- */

void MQTT_setParameterPayload(MQTT_parameter param, char * payload );

void app_updateTemperature(float temp);
void app_updateKey1State(uint8_t state);
void app_updateKey2State(uint8_t state);

void app_commModuleInit(void);
void app_mqttScheduler(void);

#endif	/* COMM_MODULE_H */