#include "task_crash_strip.h"

OS_TCB CrashStripTaskTCB;                         //任务控制块
CPU_STK CRASH_STRIP_TASK_STK[CRASH_STRIP_STK_SIZE];
void crash_strip_task(void *p_arg)
{
	OS_ERR err;
	static const short Max_Cnt=70;
	short cnt=0;
	while(1)
	{
		/*0.等待外部中断发生*/
		OSFlagPend(&IRQ_EVENTs,EVENTS_CRASH_STRIP,0,OS_OPT_PEND_FLAG_SET_ALL|OS_OPT_PEND_FLAG_CONSUME,0,&err);
		/*1.连续查询70次，确认是否真的发生碰撞*/
		while(cnt<Max_Cnt)
		{
			OSTimeDlyHMSM(0,0,0,1,OS_OPT_TIME_PERIODIC,&err);
		}
		/*真的发生碰撞，停车处理*/
		if(cnt>=Max_Cnt)
		{
			
		}
	}
}
