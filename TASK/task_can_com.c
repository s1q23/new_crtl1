#include "task_can_com.h"
#include "bsp_can.h"
#include "bsp_gpo.h"

OS_TCB CanRx0TaskTCB;                           //任务控制块
OS_TCB CanRx1TaskTCB;                           //任务控制块
CPU_STK CANRx0_TASK_STK[CANRx_STK_SIZE];         //任务堆栈	
CPU_STK CANRx1_TASK_STK[CANRx_STK_SIZE];         //任务堆栈	

void task_canRx_FIFO0(void *p_arg)
{
	OS_ERR err;
	CanRxMsg *p;
	while(1)
	{
		OSFlagPend(&IRQ_EVENTs,EVENTS_CANRX_FIFO0,0,MJ_FLAG_OPT,0,&err);
		p=can_get_msg_fifo0();
		/*对得到的数据包处理*/
		LED3=!LED3;
	}
}

void task_canRx_FIFO1(void *p_arg)
{
	OS_ERR err;
	CanRxMsg *p;
	while(1)
	{
		OSFlagPend(&IRQ_EVENTs,EVENTS_CANRX_FIFO0,0,MJ_FLAG_OPT,0,&err);
		p=can_get_msg_fifo1();
		/*对得到的数据包处理*/
		LED4=!LED4;
	}
}