#ifndef _TASK_CRASH_STRIP_COM_H
#define _TASK_CRASH_STRIP_COM_H

#include "includes.h"
#include "mj_lib.h"

/*crash strip task 相关配置,处理防撞条异常信息*/
#define CRASH_STRIP_STK_SIZE 		128              //任务堆栈大小	
extern OS_TCB CrashStripTaskTCB;                         //任务控制块
extern CPU_STK CRASH_STRIP_TASK_STK[CRASH_STRIP_STK_SIZE];    //任务堆栈	

void crash_strip_task(void *p_arg);               //任务函数  

#endif
