#ifndef _TASK_CAN_COM_H_
#define _TASK_CAN_COM_H_

#include "includes.h"
#include "mj_lib.h"

#define  SEND_TO_NAVI              1

#define CANRx_STK_SIZE 		         512                  //Can �����ջ�Ĵ�С
extern OS_TCB CanRx0TaskTCB;                            //������ƿ�
extern OS_TCB CanTxTaskTCB;                            //������ƿ�
extern CPU_STK CANRx0_TASK_STK[CANRx_STK_SIZE];         //�����ջ	
extern CPU_STK CANTx_TASK_STK[CANRx_STK_SIZE];         //�����ջ	

void task_canRx_FIFO0(void *p_arg);
void task_canTx(void *p_arg);

#endif
