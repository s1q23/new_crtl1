#ifndef _TASK_RF433_H
#define _TASK_RF433_H

#include "includes.h"
#include "mj_lib.h"

/*pc com rx task 相关配置,处理从PC发下来的指令*/
#define RF433_STK_SIZE 		                   128   //任务堆栈大小	
extern OS_TCB Rf433TaskTCB;                        //任务控制块
extern CPU_STK RF_433_TASK_STK[RF433_STK_SIZE];    //任务堆栈	
void rf_433_task(void *p_arg);                     //任务函数  

#endif
