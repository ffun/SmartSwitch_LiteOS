#include "serverAction.h"
#include "esp8266wifi.h"
//#include "jsonLib.h"
#include "util.h"
#include "sysTypeDef.h"
#if 0
static u8 deviceIDKey[]="deviceID";
static u8 deviceIDValue[]="123456";
static u8 confirmCodeSTM32Key[] = "confirmCodeSTM32";
static u8 completeCodeSTM32Key[] = "completeCodeSTM32";
static u8 onfirmCodeServerKey[] ="onfirmCodeServer";
#endif

//static Buffer_t ClientBuffer={{0},0};
static u8 cbuffer[BUFFSIZE]={0};
static u8 bClientGetResponse = 0;
#if 0
static u8 ConfirmCodeData = 32;
static u8 CompleteCodeData = 56;
#endif

static u8 deviceID[]="deviceID,hdu123456";
static u8 confirmCode[]="confirmCode,32";
static u8 completeCode[]="completeCode,56";
//charge code
static u8 chargeCode1[]="charge1";
static u8 chargeCode5[]="charge5";
static u8 chargeCode15[]="charge15";
static u8 chargeCode30[]="charge30";
static u8 chargeCode45[]="charge45";
static u8 chargeCode60[]="charge60";

//
static u8 confirmCodeServer[]="confirmCode33";
static u8 strCharging[]="charging,State_OK";
#if 0
//alive code
static u32 ClientAliveCode = 1;
static u8 AliveCode[]="HDU";
#endif

static void setClientGetResponse(void){
    bClientGetResponse = 1;
}

u8 isClientGetResponse(void){
    if(1 == bClientGetResponse){
        bClientGetResponse = 0;
        return 1;
    }
    return 0;
}

void Client_sendConfirmCode(void)
{
#if 0
	u8* ccode=0,*ccodeStr=0;
	ccode = u8toString(ConfirmCode);
	ccodeStr = jsonPackageKV(confirmCodeSTM32Key,ccode);
	esp8266WiFi_WriteLine(ccodeStr);
#endif
	esp8266WiFi_WriteLine(confirmCode);
}

void Client_sendDeviceID(u8* id)
{
#if 0
	u8* deviceIDJson = jsonPackageKV(deviceIDKey,deviceIDValue);
	esp8266WiFi_WriteLine(deviceIDJson);
#endif
    if(0 == id)
    	esp8266WiFi_WriteLine(deviceID);
	else{
		esp8266WiFi_Write("deviceID,");
		esp8266WiFi_WriteLine(id);
	}
}

void Client_sendCompleteCode(void)
{
#if 0
	u8 *ccode=0,*ccodeStr=0;
	ccode = u8toString(CompleteCode);
	ccodeStr = jsonPackageKV(completeCodeSTM32Key,ccode);
	esp8266WiFi_WriteLine(ccodeStr);
#endif
	esp8266WiFi_WriteLine(completeCode);
}

void Client_sendCharging(void){
	esp8266WiFi_WriteLine(strCharging);
}


u8* getServerResponse(void){
    Buffer_t* pBuf=0;
	if(0 != isWiFiBufferEnable()){
		//BufferSimpleCopy(&ClientBuffer,getWiFiBuffer());
		pBuf = getWiFiBuffer();
		BufferCopy2U8(cbuffer,pBuf);
        resetWiFiBuffer();//reset wifi buffer
        setClientGetResponse();//
		return cbuffer;
	}
	else
		return 0;
}

u8 Client_getChargeTimer(u8*response){
	if(0 == response)
		return 0;
	if(stringEqual(response,chargeCode1))
		return 1;
	else if(stringEqual(response,chargeCode5))
		return 5;
	else if(stringEqual(response,chargeCode15))
		return 15;
	else if(stringEqual(response,chargeCode30))
		return 30;
	else if(stringEqual(response,chargeCode45))
		return 45;
	else if(stringEqual(response,chargeCode60))
		return 60;
	return 0;
}

u8 Client_getServerConfirmCode(u8*response){
	if(0 == response)
		return 0;
	if(stringEqual(response,confirmCodeServer))
		return 1;
	return 0;
}

void Client_keepConnected(void){
	//Timer_t keepConnect_timer;
	
}

void Client_send(u8* str){
    esp8266WiFi_Write(str);
}



