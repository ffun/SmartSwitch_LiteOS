/*
**************************
file info:
	author:fang.junpeng
	start time:2016-05-09
	email:tfzsll@126.com
	usartConfig 
***************************
*/

#include "baseTypeDef.h" 

#ifndef SERVERACTION_H
#define SERVERACTION_H

u8 isClientGetResponse(void);

void Client_sendConfirmCode(void);

void Client_sendDeviceID(u8* id);

void Client_sendCompleteCode(void);
void Client_sendCharging(void);

void Client_send(u8* str);

u8* getServerResponse(void);

u8 Client_getChargeTimer(u8*response);

u8 Client_getServerConfirmCode(u8*response);

//keep connected
void Client_keepConnected(void);

#endif



