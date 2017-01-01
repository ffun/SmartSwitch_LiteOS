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

//stm32 head files
#include "stm32f10x.h"

//global variable
UINT32 g_TestTskHandle;

//hardware init
void hardware_init(void){
}


//task function
void task1(void){
    UINT32 uwRet = LOS_OK;
    UINT32 count = 0;
    while(1){
        count++;
        uwRet = LOS_TaskDelay(1000);
        if(LOS_OK != uwRet){
            return;
        }
    }
}

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

//user entry function
UINT32 osAPPInit(void){
    UINT32 uwRet = LOS_OK;
    hardware_init();
    uwRet = creat_task1();
    if(LOS_OK != uwRet)
        return uwRet;
    return LOS_OK;
}

