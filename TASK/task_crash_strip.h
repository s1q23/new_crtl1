#ifndef _TASK_CRASH_STRIP_COM_H
#define _TASK_CRASH_STRIP_COM_H

#include "includes.h"
#include "mj_lib.h"

/*crash strip task �������,�����ײ���쳣��Ϣ*/
#define CRASH_STRIP_STK_SIZE 		128              //�����ջ��С	
extern OS_TCB CrashStripTaskTCB;                         //������ƿ�
extern CPU_STK CRASH_STRIP_TASK_STK[CRASH_STRIP_STK_SIZE];    //�����ջ	

void crash_strip_task(void *p_arg);               //������  

#endif
