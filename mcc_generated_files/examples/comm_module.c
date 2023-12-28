/*
    � [2020] Microchip Technology Inc. and its subsidiaries. 
    
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


#include "../mcc.h"
#include "../winc/m2m/m2m_wifi.h"
#include "../winc/socket/socket.h"
#include "../winc/common/winc_defines.h"
#include "../winc/driver/winc_adapter.h"
#include "../mqtt/mqtt_core/mqtt_core.h"
#include "../winc/m2m/m2m_types.h"
#include "../config/mqtt_config.h"
#include "../drivers/timeout.h"

#include "../clock.h"
#include "../delay.h"

#define CONN_SSID CFG_MAIN_WLAN_SSID
#define CONN_AUTH CFG_MAIN_WLAN_AUTH
#define CONN_PASSWORD CFG_MAIN_WLAN_PSK


typedef enum
{
    APP_STATE_INIT,
    APP_STATE_STA_CONNECTING,
    APP_STATE_STA_CONNECTED,
    APP_STATE_WAIT_FOR_DNS,
    APP_STATE_TLS_START,
    APP_STATE_TLS_CONNECTING,
    APP_STATE_TLS_CONNECTED,
    APP_STATE_ERROR,
    APP_STATE_STOPPED
} appStates_e;


static const char mqttPublishTopic[] = CFG_PUBTOPIC;
const char mqttPublishMsg[] = "Hello MQTT :)";
static const char mqttSubscribeTopicsList[NUM_TOPICS_SUBSCRIBE][SUBSCRIBE_TOPIC_SIZE] = {{CFG_SUBTOPIC}};
static appStates_e appState = APP_STATE_INIT;
static uint32_t serverIPAddress = 0;
static uint8_t recvBuffer[256];
static bool timeReceived = false;
static bool appMQTTPublishTimeoutOccured = false;


static void app_buildMQTTConnectPacket(void);
static void app_buildPublishPacket(char pubTopic[], char pubMsg[]);
static uint32_t appCheckMQTTPublishTimeout();
timerStruct_t appMQTTPublishTimer = {appCheckMQTTPublishTimeout, NULL};


static uint32_t appCheckMQTTPublishTimeout()
{
    appMQTTPublishTimeoutOccured = true; // Mark that timer has executed
    return ((CFG_MQTT_CONN_TIMEOUT - 1) * SECONDS);
}

static void app_buildMQTTConnectPacket(void)
{
    mqttConnectPacket appConnectPacket;

    memset(&appConnectPacket, 0, sizeof(mqttConnectPacket));

    appConnectPacket.connectVariableHeader.connectFlagsByte.All = 0x02;
    // Packets need to be sent to the server every 10s.
    appConnectPacket.connectVariableHeader.keepAliveTimer = CFG_MQTT_CONN_TIMEOUT;
    appConnectPacket.clientID = (uint8_t *)"Microchip_ac164164";

    MQTT_CreateConnectPacket(&appConnectPacket);
}


static void app_buildPublishPacket(char pubTopic[], char pubMsg[])
{
    mqttPublishPacket appPublishPacket;
    
    memset(&appPublishPacket, 0, sizeof(mqttPublishPacket));   
    //static const char mqttPublishTopic[] = CFG_PUBTOPIC;
    //const char mqttPublishMsg[] = "Hello MQTT :)";
    appPublishPacket.topic = (uint8_t *)pubTopic;
    appPublishPacket.payload = (uint8_t *)pubMsg;

    // Fixed header
    appPublishPacket.publishHeaderFlags.duplicate = 0;
    appPublishPacket.publishHeaderFlags.qos = CFG_QOS;
    appPublishPacket.publishHeaderFlags.retain = 0;
    
    // Variable header
    appPublishPacket.packetIdentifierLSB = 10;
    appPublishPacket.packetIdentifierMSB = 0;

    appPublishPacket.payloadLength = strlen((const char*)appPublishPacket.payload);

    MQTT_CreatePublishPacket(&appPublishPacket);
}

static void changeState(appStates_e newState)
{
    if (newState != appState)
    {
        appState = newState;
    }
}

static void wifiHandler(uint8_t u8MsgType, void *pvMsg)
{
    tstrM2mWifiStateChanged *pstrWifiState = (tstrM2mWifiStateChanged *)pvMsg;
    
    switch (u8MsgType)
    {
        case M2M_WIFI_RESP_CON_STATE_CHANGED:
            if (pstrWifiState->u8CurrState == M2M_WIFI_CONNECTED) 
            {
                // Add your custom indication for successful AP connection
                printf("Connected to AP");
            }
            break;
            
        case M2M_WIFI_REQ_DHCP_CONF:
            if (appState == APP_STATE_STA_CONNECTING)
            {
                changeState(APP_STATE_STA_CONNECTED);
            }
            break;

        case M2M_WIFI_RESP_GET_SYS_TIME:
            timeReceived = true;
            break;

        default:
            break;
    }
}

static void icmpReplyHandler(uint32_t u32IPAddr, uint32_t u32RTT, uint8_t u8ErrorCode)
{
    // Add your custom ICMP data handler
}

static void dnsHandler(uint8_t *pu8DomainName, uint32_t u32ServerIP)
{
    if (u32ServerIP)
    {
        if (appState == APP_STATE_WAIT_FOR_DNS)
        {
            changeState(APP_STATE_TLS_START);
            m2m_ping_req(u32ServerIP, 0, icmpReplyHandler);
        }
    }

    serverIPAddress = u32ServerIP;
}


static void socketHandler(SOCKET sock, uint8_t u8Msg, void *pvMsg)
{
    switch (u8Msg)
    {
        case SOCKET_MSG_CONNECT:
        {
            mqttContext* mqttConnnectionInfo = MQTT_GetClientConnectionInfo();
            tstrSocketConnectMsg *pstrConnect = (tstrSocketConnectMsg *)pvMsg;

            if (pstrConnect && pstrConnect->s8Error >= 0)
            {
                changeState(APP_STATE_TLS_CONNECTED);

                recv(sock, recvBuffer, sizeof(recvBuffer), 0);
            }
            else
            {
                changeState(APP_STATE_ERROR);

                *mqttConnnectionInfo->tcpClientSocket = -1;
                close(sock);
            }

            break;
        }

        case SOCKET_MSG_RECV:
        {
            mqttContext* mqttConnnectionInfo = MQTT_GetClientConnectionInfo();
            tstrSocketRecvMsg *pstrRecv = (tstrSocketRecvMsg *)pvMsg;

            if (pstrRecv && pstrRecv->s16BufferSize > 0)
            {                
                MQTT_GetReceivedData(pstrRecv->pu8Buffer, pstrRecv->s16BufferSize);
            }
            else
            {
                changeState(APP_STATE_ERROR);
                
                *mqttConnnectionInfo->tcpClientSocket = -1;
                close(sock);
            }
            break;
        }

        case SOCKET_MSG_SEND:
        {
            break;
        }
    }
}


void app_commModuleInit(void)
{
    tstrWifiInitParam param;
    
    winc_adapter_init();
    
    param.pfAppWifiCb = wifiHandler;
    if (M2M_SUCCESS != m2m_wifi_init(&param))
    {
        printf("WiFi Error");
    }
    
    MQTT_ClientInitialise();
    app_buildMQTTConnectPacket();   
    timeout_create(&appMQTTPublishTimer, ((CFG_MQTT_CONN_TIMEOUT - 1) * SECONDS));
    
    m2m_wifi_set_sleep_mode(M2M_PS_DEEP_AUTOMATIC, 1);
}

void app_mqttScheduler(void)
{
    mqttContext* mqttConnnectionInfo = MQTT_GetClientConnectionInfo();
    
    timeout_callNextCallback();
    
    switch(appState)
    {
        case APP_STATE_INIT:
        {
            tstrNetworkId   strNetworkId;
            tstrAuthPsk     strAuthPsk;

            strNetworkId.pu8Bssid   = NULL;
            strNetworkId.pu8Ssid    = (uint8_t*)CONN_SSID;
            strNetworkId.u8SsidLen  = strlen(CONN_SSID);
            strNetworkId.enuChannel = M2M_WIFI_CH_ALL;

            strAuthPsk.pu8Psk          = NULL;
            strAuthPsk.pu8Passphrase   = (uint8_t*)CONN_PASSWORD;
            strAuthPsk.u8PassphraseLen = strlen((const char*)CONN_PASSWORD);


            if(M2M_SUCCESS != m2m_wifi_connect((char *)CONN_SSID, sizeof(CONN_SSID), CONN_AUTH, (void *)CONN_PASSWORD, M2M_WIFI_CH_ALL))
            {
                changeState(APP_STATE_ERROR);
                break;
            }

            changeState(APP_STATE_STA_CONNECTING);
            break;
        }

        case APP_STATE_STA_CONNECTING:
        {
            break;
        }

        case APP_STATE_STA_CONNECTED:
        {
            socketInit();
            registerSocketCallback(socketHandler, dnsHandler);
            changeState(APP_STATE_TLS_START);

            break;
        }

        case APP_STATE_WAIT_FOR_DNS:
        {
            break;
        }
        
        case APP_STATE_TLS_START:
        {
            struct sockaddr_in addr;

            if (*mqttConnnectionInfo->tcpClientSocket != -1)
            {
                close(*mqttConnnectionInfo->tcpClientSocket);
            }

            if(*mqttConnnectionInfo->tcpClientSocket = socket(AF_INET, SOCK_STREAM, 0))
            {
                changeState(APP_STATE_ERROR);
                break;
            }
            addr.sin_family      = AF_INET;
            addr.sin_port        = _htons(CFG_MQTT_PORT);
            addr.sin_addr.s_addr = _htonl(CFG_MQTT_SERVERIPv4_HEX);

            if (connect(*mqttConnnectionInfo->tcpClientSocket, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) < 0)
            {
                close(*mqttConnnectionInfo->tcpClientSocket);
                *mqttConnnectionInfo->tcpClientSocket = -1;
                changeState(APP_STATE_ERROR);

                break;
            }
            changeState(APP_STATE_TLS_CONNECTING);

            break;
        }

        case APP_STATE_TLS_CONNECTING:
        {
            break;
        }

        case APP_STATE_TLS_CONNECTED:
        {
            if(appMQTTPublishTimeoutOccured == true)
            {
                appMQTTPublishTimeoutOccured = false;
                app_buildPublishPacket(mqttPublishTopic, "hello");    
            }
            mqttCurrentState state1 = MQTT_GetConnectionState();
            mqttHandlerState_t state2 = MQTT_GetLastHandlerState();
            
            printf("Connection state_Current: %d \n\r", state1);
            printf("Connection state_Handler: %d \n\r\n\r", state2);
            
            
            MQTT_ReceptionHandler(mqttConnnectionInfo);
            MQTT_TransmissionHandler(mqttConnnectionInfo);
        }
        break;

        case APP_STATE_ERROR:
        {
            m2m_wifi_deinit(NULL);
            timeout_delete(&appMQTTPublishTimer);
            changeState(APP_STATE_STOPPED);

            break;
        }

        case APP_STATE_STOPPED:
        {
            changeState(APP_STATE_INIT);
            break;
        }
    }

    m2m_wifi_handle_events(NULL);
}

