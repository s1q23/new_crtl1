#ifndef __KEY_H
#define __KEY_H	 

#include "stm32f4xx.h"                  // Device header
#include "sys.h" 
#include "mj_lib.h" 
/**************************************************************************
外部中断管脚配置
**************************************************************************/
//防撞条：PH7 bumper 
#define GPIO_BUMPER																GPIOH
#define GPIO_BUMPER_CLOCK													RCC_AHB1Periph_GPIOH
#define PIN_BUMPER																GPIO_Pin_7
#define	EXTI_LINE_BUMPER													EXTI_Line7
#define EXTI_PORTSOURCE_BUMPER										EXTI_PortSourceGPIOH
#define EXTI_PINSOURCE_BUMPER											EXTI_PinSource7
#define EXTI_BUMPER_IRQn													EXTI9_5_IRQn
#define EXTI_BUMPER_PreemptionPriority						0
#define EXTI_BUMPER_SubPriority										0
#define EXTI_BUMPER_GET_PIN_VALUE									(GPIO_BUMPER->IDR & PIN_BUMPER)
#define Crash_State                               PHin(7)

//急停开关：PXx scram button 
#define GPIO_SCRAM_BUTTON													GPIOX
#define GPIO_SCRAM_BUTTON_CLOCK										RCC_AHB1Periph_GPIOH
#define PIN_SCRAM_BUTTON													GPIO_Pin_X
#define EXTI_PORTSOURCE_SCRAM_BUTTON							EXTI_PortSourceGPIOE
#define EXTI_PINSOURCE_SCRAM_BUTTON								EXTI_PinSource7
#define	EXTI_LINE_SCRAM_BUTTON										EXTI_Linex
#define EXTI_SCRAM_BUTTON_IRQn										EXTI9_5_IRQn
#define EXTI_SCRAM_BUTTON_PreemptionPriority			1
#define EXTI_SCRAM_BUTTON_SubPriority							0

//编码器机械零点信号：PXx mechanical zero
#define GPIO_MECHANICAL_ZERO											GPIOX
#define GPIO_MECHANICAL_ZERO_CLOCK								RCC_AHB1Periph_GPIOH
#define PIN_MECHANICAL_ZERO												GPIO_Pin_X
#define EXTI_PORTSOURCE_MECHANICAL_ZERO						EXTI_PortSourceGPIOE
#define EXTI_PINSOURCE_MECHANICAL_ZERO						EXTI_PinSource7
#define	EXTI_LINE_MECHANICAL_ZERO									EXTI_Linex
#define EXTI_MECHANICAL_ZERO_IRQn									EXTI9_5_IRQn
#define EXTI_MECHANICAL_ZERO_PreemptionPriority		0
#define EXTI_MECHANICAL_ZERO_SubPriority					1

/**************************************************************************
信号量输入管脚配置
**************************************************************************/
//震动：PH6 shake 
#define GPIO_SHAKE							GPIOH
#define GPIO_SHAKE_CLOCK				RCC_AHB1Periph_GPIOH
#define PIN_SHAKE								GPIO_Pin_6

//防撞条：PH7 Crash
#define GPIO_CRASH							GPIOH
#define GPIO_CRASH_CLOCK				RCC_AHB1Periph_GPIOH
#define PIN_CRASH								GPIO_Pin_7
//玻璃破碎：PH8 glass 
#define GPIO_GLASS							GPIOH
#define GPIO_GLASS_CLOCK				RCC_AHB1Periph_GPIOH
#define PIN_GLASS								GPIO_Pin_8
//烟雾报警：PD8
#define GPIO_SMOG               GPIOD
#define GPIO_SMOG_CLOCK         RCC_AHB1Periph_GPIOD
#define PIN_SMOG                GPIO_Pin_8
//急停按钮
#define GPIO_EMGBTN              GPIOD
#define GPIO_EMGBTN_CLOCK        RCC_AHB1Periph_GPIOD
#define PIN_EMGBTN               GPIO_Pin_9

//水位传感器
#define GPIO_WATER              GPIOD
#define GPIO_WATER_CLOCK        RCC_AHB1Periph_GPIOD
#define PIN_WATER               GPIO_Pin_12


void Config_GPIO_Encoder_TRD_MA(void);
float get_front_angel(void);
void Config_GPIO_Input(void);
void Config_GPIO_EXTI_BUMPER(void);

#endif
