#ifndef _TASK_ADC_CALC_H_
#define _TASK_ADC_CALC_H_

#include "includes.h"
#include "mj_lib.h"

/*Adc ���������ݴ�������ִ���˲�����*/
#define ADC_CALC_STK_SIZE 		                   256   //�����ջ��С	
extern OS_TCB AdcCalcTaskTCB;                          //������ƿ�
extern CPU_STK ADC_CALC_TASK_STK[ADC_CALC_STK_SIZE];   //�����ջ	
void adc_calc_task(void *p_arg);                       //������  



#endif
