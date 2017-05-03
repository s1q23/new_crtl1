#ifndef _BSP_ADC_H_
#define _BSP_ADC_H_

#include "sys.h"
#include "stm32f4xx.h"                              // Device header
#include "com_libs.h"
#include "mj_lib.h"

#define SIZE_OF_FILTER	              10            // 每个通道采集多少次
#define NUMBER_OF_ADC                 14            // ADC 通道数

typedef struct adc_v_{
	volatile u16 M;
	volatile u16 L1;
	volatile u16 L2;
	volatile u16 R1;
	volatile u16 R2;
	volatile u16 CO;                //一氧化碳传感器
	volatile u16 BAT_V;             //电池电压
	volatile u16 TEMP;              //温度传感器
	volatile u16 HUMI;              //湿度传感器
	volatile u16 BAT_C;             //电池电流
	volatile u16 ANGLE;             //角度传感器
	volatile u16 WATER_LEVEL;       //水位传感器
	volatile u16 FOG;               //烟雾传感器
	volatile u16 vref;              //参考电压
}adc_value_t;

extern volatile adc_value_t adc_value;

void Config_ADC1_DMA(void);
void DMA2_Stream0_IRQHandler_ADC1(void);
#endif
