#ifndef _TASK_DI_SENSOR_H
#define _TASK_DI_SENSOR_H

#include "includes.h"
#include "mj_lib.h"

#define FILTER_TIMES          2         //�˲������������������ٴβ���Ϊ��Ĵ���
/*******************�����������ŵĶ���******************/
#define pShakeState           PHin(6)   //�񶯴�����
#define pBrokenGlassState     PHin(8)   //��������
#define pSomkeSensor          PDin(8)   //�����������
#define pEmgBtnState          PDin(9)   //��ͣ��ť���
#define pWaterState           PDin(12)  //ˮλ���������

#define DI_STK_SIZE 		      128

extern OS_TCB  DITaskTCB;                
extern CPU_STK DI_TASK_STK[DI_STK_SIZE];

void task_di_sensor(void *p_arg);

#endif
