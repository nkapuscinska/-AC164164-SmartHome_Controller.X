/**
 *
 * \file
 *
 * \brief WINC1500 Example.
 *
 * Copyright (c) 2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#include "../mcc.h"
#include "../winc/include/winc.h"
#include "../winc/include/winc_legacy.h"


void winc_register_init(void);

void wifi_event_cb(uint8_t u8WiFiEvent, const void *const pvMsg)
{
    printf("WiFi event callback\n\r");
    switch(u8WiFiEvent)
    {
    case M2M_WIFI_RESP_CON_STATE_CHANGED:
        {
            //printf("Conn state changed %d\n\r", ((tuWiFiCbMsgs *)pvMsg)->strStateChanged.u8CurrState);
            break;
        }
        
    case M2M_WIFI_RESP_CONN_INFO:
        {
            //printf("Conn info %s \n\r", ((tuWiFiCbMsgs *)pvMsg)->strConnInfo.acSSID);
            //printf("IP addr: %s \n\r", ((tuWiFiCbMsgs *)pvMsg)->strConnInfo.au8IPAddr);
            break;
        }

    default:
        break;
    }
}

void winc_example(void)
{
    printf("Start Example\n\r");
    winc_register_init();
    winc_adapter_init();
    tstrWifiInitParam   param;
    
    m2m_memset((uint8_t *)&param, 0, sizeof(param));
    param.pfAppWifiCb   = wifi_event_cb;

    int8_t ret = m2m_wifi_init(&param);
    if (M2M_SUCCESS != ret){
        while(1);
    }
    
    ret = m2m_wifi_connect((char *)CFG_MAIN_WLAN_SSID, sizeof(CFG_MAIN_WLAN_SSID), CFG_MAIN_WLAN_AUTH, (void *)CFG_MAIN_WLAN_PSK, M2M_WIFI_CH_ALL);
    
    printf("%d \n\r", m2m_wifi_get_state());
    m2m_wifi_get_connection_info();

    while(1)
    {
        m2m_wifi_handle_events(NULL);
    }
}

void winc_register_init(void)
{
}
