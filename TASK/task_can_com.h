#ifndef _TASK_CAN_COM_H_
#define _TASK_CAN_COM_H_

#include "includes.h"
#include "mj_lib.h"

#define CANRx_STK_SIZE 		         256                  //Can 任务堆栈的大小
extern OS_TCB CanRx0TaskTCB;                            //任务控制块
extern OS_TCB CanRx1TaskTCB;                            //任务控制块
extern CPU_STK CANRx0_TASK_STK[CANRx_STK_SIZE];         //任务堆栈	
extern CPU_STK CANRx1_TASK_STK[CANRx_STK_SIZE];         //任务堆栈	

void task_canRx_FIFO0(void *p_arg);
void task_canRx_FIFO1(void *p_arg);

#endif
