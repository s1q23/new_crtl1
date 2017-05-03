#ifndef _TASK_ADC_CALC_H_
#define _TASK_ADC_CALC_H_

#include "includes.h"
#include "mj_lib.h"

/*Adc 采样的数据处理任务，执行滤波任务*/
#define ADC_CALC_STK_SIZE 		                   256   //任务堆栈大小	
extern OS_TCB AdcCalcTaskTCB;                          //任务控制块
extern CPU_STK ADC_CALC_TASK_STK[ADC_CALC_STK_SIZE];   //任务堆栈	
void adc_calc_task(void *p_arg);                       //任务函数  



#endif
