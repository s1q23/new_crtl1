#ifndef _TASK_RF433_H
#define _TASK_RF433_H

#include "includes.h"
#include "mj_lib.h"

/*pc com rx task �������,�����PC��������ָ��*/
#define RF433_STK_SIZE 		                   128   //�����ջ��С	
extern OS_TCB Rf433TaskTCB;                        //������ƿ�
extern CPU_STK RF_433_TASK_STK[RF433_STK_SIZE];    //�����ջ	
void rf_433_task(void *p_arg);                     //������  

#endif
