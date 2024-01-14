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

#define MQTT_PARAMS_NUM 2
#define PAYLOAD_LENGHT 16
#define TOPIC_LENGHT 16

// Nazwy parametrów przeznaczonych do wys??nia za pomoc? MQTT, nie modyfikowa? domy?lnej kolejno?ci warto?ci enuma.
typedef enum MQTT_parameter {PARAM1, PARAM2} MQTT_parameter;


void MQTT_setParameterPayload(MQTT_parameter param, char * payload );
void MQTT_setParameterTopic(MQTT_parameter param, char * topic);

void app_commModuleInit(void);
void app_mqttScheduler(void);

#endif	/* COMM_MODULE_H */