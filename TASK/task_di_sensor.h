#ifndef _TASK_DI_SENSOR_H
#define _TASK_DI_SENSOR_H

#include "includes.h"
#include "mj_lib.h"

#define FILTER_TIMES          2         //滤波次数，连续读到多少次才认为真的触发
/*******************数字输入引脚的定义******************/
#define pShakeState           PHin(6)   //振动传感器
#define pBrokenGlassState     PHin(8)   //玻璃破碎
#define pSomkeSensor          PDin(8)   //烟雾传感器检测
#define pEmgBtnState          PDin(9)   //急停按钮检测
#define pWaterState           PDin(12)  //水位传感器检测

#define DI_STK_SIZE 		      128

extern OS_TCB  DITaskTCB;                
extern CPU_STK DI_TASK_STK[DI_STK_SIZE];

void task_di_sensor(void *p_arg);

#endif
