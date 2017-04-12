#include "task_can_com.h"
#include "bsp_can.h"
#include "bsp_gpo.h"

OS_TCB CanRx0TaskTCB;                           //������ƿ�
OS_TCB CanRx1TaskTCB;                           //������ƿ�
CPU_STK CANRx0_TASK_STK[CANRx_STK_SIZE];         //�����ջ	
CPU_STK CANRx1_TASK_STK[CANRx_STK_SIZE];         //�����ջ	

void task_canRx_FIFO0(void *p_arg)
{
	OS_ERR err;
	CanRxMsg *p;
	while(1)
	{
		OSFlagPend(&IRQ_EVENTs,EVENTS_CANRX_FIFO0,0,MJ_FLAG_OPT,0,&err);
		p=can_get_msg_fifo0();
		/*�Եõ������ݰ�����*/
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
		/*�Եõ������ݰ�����*/
		LED4=!LED4;
	}
}