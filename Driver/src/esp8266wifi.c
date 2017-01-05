#include "esp8266wifi.h"
#include "uartConfig.h"
#include "util.h"
#include "stdio.h"

#define Esp8266WiFi_UART eUart1

static Buffer_t WiFiBuffer = {{0},0};//wifi buffer


//wifi status
static WiFiStatus sWifiConnectedStatus = Disconnected;

/*************************wifi control code****************************/
//static u8 strWiFiMode[]="AT+CWMODE=3";//return OK
//static u8 strRestart[]="AT+RST";//return OK
static u8 strConnect2Router[] = "AT+CWJAP=\"whz\",\"1234567890\"";//return OK
static u8 strConnect2Server[] = "AT+CIPSTART=\"TCP\",\"192.168.1.101\",8989";//return OK
static u8 strStartPassthroughMode[]="AT+CIPMODE=1";
static u8 strStartTransmit[]="AT+CIPSEND";
static u8 strStopPassThrough[]="+++";

#if 0
//wifi response
static u8 strOK[]="OK";
static u8 strPassThroughOK[]=">";
#endif

/***************buffer operation*****************/

static void setWiFiBufferIndex(u8 index){
    WiFiBuffer.index = index;
}

//make buffer disable
static void disAbleWiFiBuffer(void){
	WiFiBuffer.enable = 0;
}

u8 isWiFiBufferEnable(void){
	return WiFiBuffer.enable;
}

//make buffer able
static void enableWiFiBuffer(void){
	WiFiBuffer.enable = 1;
}

void resetWiFiBuffer(void){
	WiFiBuffer.enable = WiFiBuffer.cnt = WiFiBuffer.index = 0;
	WiFiBuffer.buffer[0]=0;
}

//the function will disable the buffer,if you want to use data of the buffer,please enableWiFiBuffer()
static void appendWiFiBuffer(u8 ch){
	disAbleWiFiBuffer();
	if(WiFiBuffer.cnt >= BUFFSIZE)
		WiFiBuffer.cnt = 0;
	WiFiBuffer.buffer[WiFiBuffer.cnt++]=ch;
}

static u8* getWiFiBufferDataPointer(void){
	//return &((WiFiBuffer.buffer)[WiFiBuffer.index]);
	return WiFiBuffer.buffer;
}

static u8 getWiFiBufferDataLength(void){
    return WiFiBuffer.cnt;
}

Buffer_t* getWiFiBuffer(void){
    return &WiFiBuffer;
}


/**********esp8266wifi control*************/
WiFiCfgStatus esp8266Config(void)
{
	u8 cnt=0;
	stopPassThroughMode();
#if 0
	//1.set wifi mode
	esp8266WiFi_WriteLine(strWiFiMode);
	delay_ms(200);
	//2.restart to be valid
	esp8266WiFi_WriteLine(strRestart);
	delay_ms(1000);
#endif

	//3.connect to router
	esp8266WiFi_WriteLine(strConnect2Router);
	do{
		delay_ms(1000);
		if(0 != isResponseOK())
			break;
		if(cnt == 10)
			esp8266WiFi_WriteLine(strConnect2Router);
		cnt++;
	}while(cnt<20);
	if(cnt == 10)
		return eConnetRouterError;
	cnt = 0;
	USARTSendLine("join router success",eUart2);
	//4.connect to server
	esp8266WiFi_WriteLine(strConnect2Server);
	do{
		delay_ms(200);
		if(0 != isResponseOK())
			break;
		cnt++;
	}while(cnt<10);
	if(cnt == 10)
		return eConnectServerError;
	cnt = 0;
	USARTSendLine("connect server success",eUart2);
	//
	esp8266WiFi_WriteLine(strStartPassthroughMode);
	do{
		delay_ms(200);
		if(0 != isResponseOK())
			break;
		cnt++;
	}while(cnt<10);
	if(cnt == 10)
		return eStartPassThroughError;
	cnt = 0;
	USARTSendLine("set passthrough success",eUart2);
	//
	esp8266WiFi_WriteLine(strStartTransmit);
	do{
		delay_ms(200);
		if(0 != isResponseOK())
			break;
		cnt++;
	}while(cnt<10);
	if(cnt == 10)
		return eStartPassThroughError;
	cnt = 0;
	USARTSendLine("start transmit",eUart2);
#if 0
	response = getWifiBufferData();
	while(0 == isWiFiBufferEnable());
	if(0 != stringEqual(strServerOK,response))
		return eConnectServerError;
#endif
	//set wifi status
	sWifiConnectedStatus = Connected;

	return eWiFiConfigSuccess;
}

u8 esp8266WiFi_TcpConnect(char* ip,u16 port){
    //result code
    u8 result = 0;
    //check
    if(0 == ip)
        return result;
    //AT cmd of Tcp connection
    char* sTcpConHead[] = "AT+CIPSTART=\"TCP\",\"";
    char sPort[10];
    //get string of u16's port
    sprintf(sPort,"%d",port);
    //connect
    esp8266WiFi_Write(sTcpConHead);
    esp8266WiFi_Write(ip);
    esp8266WiFi_WriteData("\",",2);
    esp8266WiFi_WriteLine(sPort);
    //check if connect success
    
    
    return result;
}


void esp8266WiFi_WriteData(u8* data,u8 length){
	u8 i=0;
	while(i<length){
		USARTSendData(data[i++],Esp8266WiFi_UART);
	}
}

void esp8266WiFi_Write(u8* str){
	if(0 == str)
		return;
	while(*str){
		USARTSendData(*str++,Esp8266WiFi_UART);
	}
}


void esp8266WiFi_WriteLine(u8* str){
	u8 newline[]="\r\n";
	esp8266WiFi_Write(str);
	esp8266WiFi_Write(newline);
}

void stopPassThroughMode(void){
	esp8266WiFi_Write(strStopPassThrough);
	delay_ms(100);
}

u8 getWiFiConnectedStatus(void){
	return sWifiConnectedStatus;
}

void reConnectWiFi(void){
    //set wifi status
    sWifiConnectedStatus = Disconnected;
    //stop passthrough Mode
	//stopPassThroughMode();
	if(eWiFiConfigSuccess == esp8266Config())
        sWifiConnectedStatus = Connected;
}

static u8 bResponseOK=0;
u8 isResponseOK(void){
	if(bResponseOK == 1){
		bResponseOK = 0;
		return 1;
	}
	return 0;
}


/***************HooK operation*****************/
//check if the data end with "OK"
void HookOfEsp8266WiFi(u8 data){
    static u8 mState = 0;
/***************"OK" flag*******************/
    switch(data){
        case 'O':
            mState = 1;break;
        case 'K':
            if(1 == mState)
                mState = 2;
            else
                mState = 0;
            break;
        default:
            mState = 0;break;
    }
    if(2 == mState)
        bResponseOK = 1;
    }
	
}

