#include "serverAction.h"
#include "util.h"
#include "sysTypeDef.h"

//server buffer
static Buffer_t sServerBuf = {{0},0};

//client-server interaction command
static u8 OpenCmd[]="open";static const u8 OpenCmdLength = 4;
static u8 CloseCmd[]="close";static const u8 CloseCmdLength = 5;
static u8 ComResponse[]="%%%";static const u8 ComResLength = 3;

/*******client-server bussiness*******/
ServerMsg_t getServerMsg(void){
    ServerMsg_t MsgType = CMD_unknow;
    switch(ServerBuf_dataLength()){
        case OpenCmdLength:
            if(byteCompare(getServerBuf(),OpenCmd,OpenCmdLength))
                MsgType = CMD_open;
            break;
        case CloseCmdLength:
            if(byteCompare(getServerBuf(),CloseCmd,CloseCmdLength))
                MsgType = CMD_close;
            break;
        case ComResLength:
            if(byteCompare(getServerBuf(),ComResponse,ComResLength))
                MsgType = CMD_close;
            break;
        default:
            break; 
    }
    return MsgType;
}


/*****server buffer operation*****/
void ServerBuf_init(void){
    Buffer_init(&sServerBuf);
}

u8* getServerBuf(void){
    return getBuffer(&sServerBuf);
}

u8 isServerBufferEabled(void){
    return isBufferEabled(&sServerBuf);
}

void ServerBuffer_clear(void){
    Buffer_clear(&sServerBuf);
}

u8 ServerBuf_dataLength(void){
    return Buffer_dataLength(&sServerBuf);
}

/*
the server data is like "@xxxxx$"
we should store the content "xxxxx" between '@' and '$'
*/
void HookOfServerAction(u8 data){
    static u8 mState = 0;
    switch(data){
        case '@':
            mState = 1;break;
        case '$':
            mState = 2;
            Buffer_enable(&sServerBuf);
            break;
        default:
            //save the data into buffer
            if(1 == mState && '@' != data){
                Buffer_add(&sServerBuf,data);
            }
    }
}



