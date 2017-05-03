#ifndef _BSP_ADC_H_
#define _BSP_ADC_H_

#include "sys.h"
#include "stm32f4xx.h"                              // Device header
#include "com_libs.h"
#include "mj_lib.h"

#define SIZE_OF_FILTER	              10            // ÿ��ͨ���ɼ����ٴ�
#define NUMBER_OF_ADC                 14            // ADC ͨ����

typedef struct adc_v_{
	volatile u16 M;
	volatile u16 L1;
	volatile u16 L2;
	volatile u16 R1;
	volatile u16 R2;
	volatile u16 CO;                //һ����̼������
	volatile u16 BAT_V;             //��ص�ѹ
	volatile u16 TEMP;              //�¶ȴ�����
	volatile u16 HUMI;              //ʪ�ȴ�����
	volatile u16 BAT_C;             //��ص���
	volatile u16 ANGLE;             //�Ƕȴ�����
	volatile u16 WATER_LEVEL;       //ˮλ������
	volatile u16 FOG;               //��������
	volatile u16 vref;              //�ο���ѹ
}adc_value_t;

extern volatile adc_value_t adc_value;

void Config_ADC1_DMA(void);
void DMA2_Stream0_IRQHandler_ADC1(void);
#endif
