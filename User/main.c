/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
//Huawei LiteOS head files
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "los_base.h"
#include "los_config.h"
#include "los_typedef.h"
#include "los_hwi.h"
#include "los_task.ph"
#include "los_sem.h"
#include "los_event.h"
#include "los_memory.h"
#include "los_queue.ph"

//user files
#include "platform.h"
#include "toollib.h"
#include "device.h"
#include "dprintf.h"

//debug Switch
#define DebugON 1

//creat task api
UINT32 create_task(CHAR*taskName,UINT32*taskHandle,UINT16 taskPrio,TSK_ENTRY_FUNC pFun);

extern void hwi_setup(void);
//hardware init
void hardware_init(void){
    env_init();//init the env
    hwi_setup();//register irq service
}

UINT32 create_SensorTask(void);
UINT32 create_SmartConfigTask(void);
UINT32 create_wifiTask(void);

//user entry function
UINT32 osAppInit(void){
    UINT32 uwRet = LOS_OK;
    hardware_init();
    //uwRet = create_task1();
    uwRet = create_SensorTask();
    if(LOS_OK != uwRet)
        return uwRet;
    #if 0
    uwRet = create_task2();
    if(LOS_OK != uwRet)
        return uwRet;
    #endif
    return LOS_OK;
}

//global task handle variable
UINT32 g_TskHandle2;
UINT32 g_SensorTaskHandle;
UINT32 g_WifiTaskHandle;
UINT32 g_SmartConfigTaskHandle;

CHAR* task2Name="task2";
CHAR* SensorTaskName="SensorTask";
CHAR* SmartConfigTaskName = "SmartConfigTask";
CHAR* wifiTaskName="wifiTask";

void task2(void);
//bussiness task
void sensor_task(void);//for collecting sensor's data
void smartConfig_task(void);
void wifi_task(void);

UINT32 create_task2(){
    return create_task(task2Name,&g_TskHandle2,3,(TSK_ENTRY_FUNC)task2);
}

UINT32 create_SensorTask(){
    return create_task(SensorTaskName,&g_SensorTaskHandle,3,(TSK_ENTRY_FUNC)sensor_task);
}

UINT32 creat_wifiTask(){
    return create_task(wifiTaskName,&g_WifiTaskHandle,3,(TSK_ENTRY_FUNC)wifi_task);
}

UINT32 create_SmartConfigTask(void){
    return create_task(SmartConfigTaskName,&g_SmartConfigTaskHandle,\
        1,(TSK_ENTRY_FUNC)smartConfig_task);
}


//task function
void task2(void){
    UINT32 uwRet = LOS_OK;
    UINT32 count = 0;
    while(1){
        count++;
        dprintfln("1");
        uwRet = LOS_TaskDelay(1000);
        if(LOS_OK != uwRet){
            return;
        }
    }
}

void sensor_task(void){
    UINT32 uwRet = LOS_OK;
	char cStrTemp[10];
	char cStrHumi[10];
    char cStrPm25[10];

    //sersor's value
    FLOAT pm25Value = 0.0;
    DHT11_Data_TypeDef  DHT11_Data;
    //function return
    UINT8 res=255;
    while(1){
        //get DHT11's data
        if(DHT11_Read_TempAndHumidity(&DHT11_Data)==SUCCESS){
            sprintf(cStrTemp,"%d.%d C",DHT11_Data.temp_int,DHT11_Data.temp_deci);
			sprintf(cStrHumi,"%d.%d %%RH",DHT11_Data.humi_int,DHT11_Data.humi_deci);
            
            #if DebugON
            dprintf("temp:");
            dprintfln(cStrTemp);
            dprintf("humi:");
            dprintfln(cStrHumi);
            #endif
        }else{
            #if DebugON
            dprintfln("DHT11 ERROR!\r\n");
            #endif
        }
        //get PM2.5's data
        if(getPM25Index(&pm25Value)){
            sprintf(cStrPm25,"%f",pm25Value);
            #if DebugON
            dprintf("pm25:");
            dprintfln(cStrPm25);
            #endif
        }
        //delay
        uwRet = LOS_TaskDelay(2000);
		if(uwRet !=LOS_OK)
			return;
    }
}

void smartConfig_task(void){
    
}

void wifi_task(void){
    
}


#if 0
//config task parameter and creat task
UINT32 creat_task1(){
    UINT32 uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;//struct of task parameter
    task_init_param.usTaskPrio = 0;
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task1;
    task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
    task_init_param.pcName = "task1";

    uwRet = LOS_TaskCreate(&g_TestTskHandle,&task_init_param);
    if(LOS_OK != uwRet)
        return uwRet;
    return uwRet;
}
#endif

UINT32 create_task(CHAR*taskName,UINT32*taskHandle,UINT16 taskPrio,TSK_ENTRY_FUNC pFun){
    UINT32 uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;//struct of task parameter
    task_init_param.usTaskPrio = taskPrio;
    task_init_param.pfnTaskEntry = pFun;
    task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
    task_init_param.pcName = taskName;

    uwRet = LOS_TaskCreate(taskHandle,&task_init_param);
    if(LOS_OK != uwRet)
        return uwRet;
    return uwRet;
}


