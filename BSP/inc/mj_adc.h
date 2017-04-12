#ifndef _MJ_ADC_H_
#define _MJ_ADC_H_

#include "stm32f4xx.h"                  // Device header
#include "sys.h"                  // Device header


#define MJ_ADC_SAMPLE_TIME ADC_SampleTime_71Cycles5
void mj_adc_init(void);

#endif
