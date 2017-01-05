/*
**************************
file info:
	author:fang.junpeng
	start time:2016-01-05
	server action
***************************
*/

#include "baseTypeDef.h" 

#ifndef SERVERACTION_H
#define SERVERACTION_H

/*****server buffer operation*****/
void ServerBuf_init(void);

//get the data buf 
u8* getServerBuf(void);

//get the data buf and disable the buffer
u8* getServerBufDisable(void);

u8 isServerBufferEabled(void);

void ServerBuffer_clear(void);

u8 ServerBuf_dataLength(void);

/*******client-server bussiness*******/
typedef enum{
    CMD_close = 0,CMD_open,CMD_common,CMD_unknow
}ServerMsg_t;

ServerMsg_t getServerMsg(void);

/********************HooK operation************************/
void HookOfServerAction(u8 data);

#endif



