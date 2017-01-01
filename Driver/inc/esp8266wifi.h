/*
**************************
file info:
	author:fang.junpeng
	start time:2016-05-03
	email:tfzsll@126.com
	esp8266wifi 
***************************
*/

#include "baseTypeDef.h"
#include "sysTypeDef.h"

#ifndef ESP8266WIFI_H
#define ESP8266WIFI_H

typedef enum{
	eConnetRouterError,eConnectServerError,eStartPassThroughError,eWiFiConfigSuccess
}WiFiCfgStatus;
//wifi connected status
typedef enum{Connected=0,Disconnected = !Connected}WiFiStatus;

/********************buffer operation************************/

u8 isWiFiBufferEnable(void);
void resetWiFiBuffer(void);
Buffer_t* getWiFiBuffer(void);

/********************esp8266wifi operation************************/

WiFiCfgStatus esp8266Config(void);
void esp8266WiFi_WriteData(u8* data,u8 length);
void esp8266WiFi_Write(u8* str);
void esp8266WiFi_WriteLine(u8* str);

u8 getWiFiConnectedStatus(void);

void stopPassThroughMode(void);

u8 isResponseOK(void);

void reConnectWiFi(void);

/********************HooK operation************************/
void HookOfEsp8266WiFi(u8 data);

#endif

