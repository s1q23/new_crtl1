#ifndef _BSP_ADC_H_
#define _BSP_ADC_H_

#include "sys.h"
#include "stm32f4xx.h"                              // Device header
#include "com_libs.h"
#include "mj_lib.h"

#define SIZE_OF_FILTER	              10            // ÿ��ͨ���ɼ����ٴ�
#define NUMBER_OF_ADC                 14            // ADC ͨ����

typedef struct adc_v_{
	u16 M;
	u16 L1;
	u16 L2;
	u16 R1;
	u16 R2;
	u16 CO;                //һ����̼������
	u16 BAT_V;             //��ص�ѹ
	u16 TEMP;              //�¶ȴ�����
	u16 HUMI;              //ʪ�ȴ�����
	u16 BAT_C;             //��ص���
	u16 ANGLE;             //�Ƕȴ�����
	u16 WATER_LEVEL;       //ˮλ������
	u16 FOG;               //��������
	u16 vref;              //�ο���ѹ
}adc_value_t;

extern volatile adc_value_t adc_value;

void Config_ADC1_DMA(void);

#endif
