#ifndef _BSP_FRONT_WHEEL_H_
#define _BSP_FRONT_WHEEL_H_
#include "mj_lib.h"

#define SPEED_TO_SPECIFIED_ANGLE   10
#define ANGLE_TO_CODE              1000
#define RS433_TO_ANGLE             0
#define CLOCK_FREQUENCY_PWM        1000000       //PWM所用时钟的频率为1MHZ

/**************************************************************************
前轮驱动器相关配置
**************************************************************************/
//方向：PE5
#define GPIO_FRONT_DIR							GPIOE
#define GPIO_FRONT_DIR_CLOCK				RCC_AHB1Periph_GPIOE
#define PIN_FRONT_DIR								GPIO_Pin_5
#define FRONT_DIR_LEFT()						GPIO_ResetBits(GPIO_FRONT_DIR, PIN_FRONT_DIR)
#define FRONT_DIR_RIGHT()						GPIO_SetBits(GPIO_FRONT_DIR, PIN_FRONT_DIR)
//使能：PE4
#define GPIO_FRONT_EN								GPIOE
#define GPIO_FRONT_EN_CLOCK					RCC_AHB1Periph_GPIOE
#define PIN_FRONT_EN								GPIO_Pin_4
#define FRONT_EN_ON()								GPIO_ResetBits(GPIO_FRONT_EN, PIN_FRONT_EN)
#define FRONT_EN_OFF()							GPIO_SetBits(GPIO_FRONT_EN, PIN_FRONT_EN)
//PWM：PE6
#define GPIO_FRONT_PWM							GPIOE
#define GPIO_FRONT_PWM_CLOCK				RCC_AHB1Periph_GPIOE
#define PIN_FRONT_PWM								GPIO_Pin_6
#define	PINSOURCE_FRONT_PWM					GPIO_PinSource6
#define TIMx_FRONT_PWM							TIM9
#define GPIO_AF_FRONT_PWM						GPIO_AF_TIM9

extern volatile char front_dir;
void Config_Control_Front_Wheel(void);
void Config_Tim3(void);
char front_angel_to_gear(float in_angel);
float front_gear_to_angel(int8_t in_gear);
void front_wheel_stop(void);
void front_auto_mid(void);
void front_wheel_turn(unsigned char dir,unsigned short speed,float angel);

#endif
