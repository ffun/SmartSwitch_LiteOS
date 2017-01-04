#include "device.h"

void device_init(void){
    //PM25_init();
    //relay_init();
    USART_Config();
    DHT11_Init();
    //TIM2_init();
}


