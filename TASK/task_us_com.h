#ifndef _TASK_US_COM_H_
#define _TASK_US_COM_H_
#include "includes.h"
#include "mj_lib.h"

#define US_COMRx_STK_SIZE 		128
extern OS_TCB  UsHeadTaskTCB;                
extern CPU_STK US_HEAD_TASK_STK[US_COMRx_STK_SIZE];
extern OS_TCB  UsTailTaskTCB;                
extern CPU_STK US_TAIL_TASK_STK[US_COMRx_STK_SIZE];

typedef struct _pack_utlr
{
  volatile uint8_t dir;
/*������ϢΪǰʱ��ʹ�������ĸ�����*/	
  volatile uint8_t dist_A;
  volatile uint8_t dist_B;
  volatile uint8_t dist_C;
  volatile uint8_t dist_D;
/*������ϢΪ��ʱ��ʹ�������ĸ�����*/
  volatile uint8_t dist_E;
  volatile uint8_t dist_F;
  volatile uint8_t dist_G;
  volatile uint8_t dist_H;
}pack_utlr;

void us_Head_task(void *p_arg);
void us_Tail_task(void *p_arg);

#endif
