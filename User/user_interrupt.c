#include "los_hwi.h"
#include "los_typedef.h"
#define HWI_NUM_INT_UART1 37

//stm32 head Files
#include "stm32f10x_usart.h"
#include "uartConfig.h"



void uart_irqhandle(int irq,void *dev)
{
    u16 data;
    //printf("\n int the func uart_irqhandle \n");
    
	//receive interrupt
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET){
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);//clear receive Interrupt
		//do something
		//USART_ReceiveData(USART1);
		data = (u8)(USART_ReceiveData(USART1));
        #if 0
		if(uart1Hook != 0)
			uart1Hook(data);
		HookOfEsp8266WiFi(data);
        #endif
        //USARTSendByteString("\n int the func uart_irqhandle \n",eUart1);
        USARTSendData(data,eUart1);
	}
}

void hwi_test(){    
    int a = 1;
    UINTPTR uvIntSave;
    uvIntSave = LOS_IntLock();
    LOS_HwiCreate(HWI_NUM_INT_UART1, 0,0,uart_irqhandle,NULL);//create interrupt
    //hal_interrupt_unmask(HWI_NUM_INT50);
    LOS_IntRestore(uvIntSave);
    //hal_interrupt_mask(HWI_NUM_INT50);
}

