#ifndef _TASK_PM_COM_H
#define _TASK_PM_COM_H

#include "includes.h"
#include "mj_lib.h"

#define PM_COMRx_STK_SIZE 		128

typedef struct _pm_pack
{
	volatile unsigned char head;
	volatile unsigned char cmd;
	volatile unsigned char pm25L;
	volatile unsigned char pm25H;
	volatile unsigned char pm10L;
	volatile unsigned char pm10H;
	volatile unsigned char ID1;
	volatile unsigned char ID2;
	volatile unsigned char acc;
	volatile unsigned char end;
}pm_pack;

extern OS_TCB  PmComRxTaskTCB;                        
extern CPU_STK PM_COMRx_TASK_STK[PM_COMRx_STK_SIZE];

void pm_comRx_task(void *p_arg);

#endif
