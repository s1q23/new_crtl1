#ifndef _BSP_ADC_H_
#define _BSP_ADC_H_

#include "sys.h"
#include "stm32f4xx.h"                              // Device header
#include "com_libs.h"
#include "mj_lib.h"

#define SIZE_OF_FILTER	              10            // 每个通道采集多少次
#define NUMBER_OF_ADC                 14            // ADC 通道数

typedef struct adc_v_{
	u16 M;
	u16 L1;
	u16 L2;
	u16 R1;
	u16 R2;
	u16 CO;                //一氧化碳传感器
	u16 BAT_V;             //电池电压
	u16 TEMP;              //温度传感器
	u16 HUMI;              //湿度传感器
	u16 BAT_C;             //电池电流
	u16 ANGLE;             //角度传感器
	u16 WATER_LEVEL;       //水位传感器
	u16 FOG;               //烟雾传感器
	u16 vref;              //参考电压
}adc_value_t;

extern volatile adc_value_t adc_value;

void Config_ADC1_DMA(void);

#endif
