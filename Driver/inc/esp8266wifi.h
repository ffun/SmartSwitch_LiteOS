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

//wifi connected status
typedef enum{Connected=0,Disconnected = !Connected}WiFiStatus_t;


/***************esp8266wifi config****************/


u8 esp8266Config(void);

void esp8266_reset(void);

void setWiFiSTAMode(void);

void setTouChuan(void);

void startTransmit(void);

void stopTouChuan(void);

WiFiStatus_t getWiFiStatus(void);

u8 isResponseOK(void);

void reConnectWiFi(void);

void esp8266_SmartConfig(void);


/***********esp8266wifi send and receive*****************/
/*
function:create a tcp connection to the server
return:1---success;0---failed
*/
u8 esp8266WiFi_TcpConnect(char* ip,u16 port);
void esp8266WiFi_WriteData(u8* data,u8 length);
void esp8266WiFi_Write(u8* str);
void esp8266WiFi_WriteLine(u8* str);

/***************HooK operation***************/
void HookOfEsp8266WiFi(u8 data);

#endif

