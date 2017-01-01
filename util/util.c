#include "util.h"
#include "timeControl.h"

#define ITOABUFFSIZE 10
static u8 itoaBuffer[ITOABUFFSIZE]={0};

void byteCopy(u8* dst,u8* src,s16 Length){
    u8 i=0;
	if(0 == dst || 0 == src)//check pointer
		return;
	while(i<Length){//start copy
		dst[i] = src[i];
        i++;
	}
}

void BufferSimpleCopy(Buffer_t*dst,Buffer_t*src){
    if(0 == dst || 0 == src)//check pointer
		return;
    dst->cnt = src->cnt;
    dst->enable = src->enable;
    dst->index = src->index;
    byteCopy(dst->buffer,src->buffer,src->cnt);
}

void BufferCopy2U8(u8* dst,Buffer_t*src){
    if(0 == dst || 0 == src)//check pointer
		return;
    byteCopy(dst,&(src->buffer[src->index]),src->cnt - src->index);
    dst[src->cnt-src->index]=0;
}


u16 stringLength(u8* str){
	u16 length=0;
	if(0 != str){//check str pointer
		while(0 != *str++)
			length++;
	}
	return length;
}

void memorySet(u8* mem,u8 byteValue,s16 length){
	if(0 == mem)
		return;
	while(length>0){
		mem[--length]=byteValue;
	}
}

u8 stringEqual(u8* src,u8* dst){
	if(0 == dst || 0 == src)
		return 0;
	while(*src!=0 && *dst!=0){//the first char is not 0
		if(*src != *dst)
			return 0;
		src++;dst++;
		if(*src ==0 && *dst == 0)//if two string end with 0 at the same time
			return 1;
		if(*src ==0 || *dst == 0)//if the one end,but another is not
			break;
	}
	return 0;
}

u8 stringEqualIgnoreCase(u8* src,u8* dst){
	if(0 == dst || 0 == src)
		return 0;
	while(*src!=0 && *dst!=0){//the first char is not 0
		if(*src != *dst || 32!=Abs_U8(*src,*dst))//ignore case
			return 0;
		src++;dst++;
		if(*src ==0 && *dst == 0)//if two string end with 0 at the same time
			return 1;
		if(*src ==0 || *dst == 0)//if the one end,but another is not
			break;
	}
	return 0;
}

u8 Abs_U8(u8 data1,u8 data2){
	u8 temp=0;
	if(data1>=data2)
		temp = data1 - data2;
	else
		temp = data2 - data1;
	return temp;
}

u8* u8toString(u8 num){
	u8 i=0,j=0,temp[ITOABUFFSIZE]={0};
	if(num == 0)
		temp[i++] = '0';
	while(num != 0){
		temp[i++]='0'+(num%10);
		num/=10;
	}
	//reverse temp,saving to itoBuffer
	for(j=0;i>0;j++){
		itoaBuffer[j]=temp[--i];
	}
	itoaBuffer[j]='\0';
	return itoaBuffer;
}

//
Timer_t delay_timer;
void delay_ms(u16 n)
{
	initTimerWithSeciton(&delay_timer,n);
	while(delay_timer.end != getSystemTime());
}

void delay_s(u16 n){
	while(n>0)
	{
		delay_ms(1000);
		n--;
	}
}


