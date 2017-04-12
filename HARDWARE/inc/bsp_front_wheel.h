#ifndef _BSP_FRONT_WHEEL_H_
#define _BSP_FRONT_WHEEL_H_
#include "mj_lib.h"

#define SPEED_TO_SPECIFIED_ANGLE   10
#define ANGLE_TO_CODE              1000
#define RS433_TO_ANGLE             0
#define CLOCK_FREQUENCY_PWM        1000000       //PWM所用时钟的频率为1MHZ

#define DIR_LEFT                   1             // 往左的方向 
#define DIR_RIGHT                  0             // 往右的方向
#define DIR_STOP                   2
/**************************************************************************
前轮驱动器相关配置
**************************************************************************/
//方向：PE5
#define GPIO_FRONT_DIR							GPIOE
#define GPIO_FRONT_DIR_CLOCK				RCC_AHB1Periph_GPIOE
#define PIN_FRONT_DIR								GPIO_Pin_5
#define FRONT_DIR_LEFT()						GPIO_SetBits(GPIO_FRONT_DIR, PIN_FRONT_DIR)
#define FRONT_DIR_RIGHT()						GPIO_ResetBits(GPIO_FRONT_DIR, PIN_FRONT_DIR)
//使能：PE4
#define GPIO_FRONT_EN								GPIOE
#define GPIO_FRONT_EN_CLOCK					RCC_AHB1Periph_GPIOE
#define PIN_FRONT_EN								GPIO_Pin_4
#define FRONT_EN_ON()								GPIO_SetBits(GPIO_FRONT_EN, PIN_FRONT_EN)
#define FRONT_EN_OFF()							GPIO_ResetBits(GPIO_FRONT_EN, PIN_FRONT_EN)
//PWM：PE6
#define GPIO_FRONT_PWM							GPIOE
#define GPIO_FRONT_PWM_CLOCK				RCC_AHB1Periph_GPIOE
#define PIN_FRONT_PWM								GPIO_Pin_6
#define	PINSOURCE_FRONT_PWM					GPIO_PinSource6
#define TIMx_FRONT_PWM							TIM9
#define GPIO_AF_FRONT_PWM						GPIO_AF_TIM9

void Config_Control_Front_Wheel(void);
void Config_Tim3(void);
void front_wheel_stop(void);
void front_auto_mid(void);
void front_wheel_turn(unsigned char dir,unsigned short speed,float angel);

//void IPC_Set_Front_Speed( s16 f_front_speed_IPC );//前轮轮的速度转换 度/10s 转到PWM的值
//void RS433_Set_Front_Speed( s16 f_front_speed_433 );//前轮轮的速度转换 度/s 转到PWM的值
//void Front_Speed_Switch(void);//前轮轮的速度转换 度/s 转到PWM的值
//void Set_Front_Speed_ang( s16 f_front_speed_ang, u8 f_priority );
//void Set_Front_Speed_pwm( s32 f_front_speed_pwm );
//s16 Get_Front_Speed_ang(void);
//s32 Get_Front_Speed_pwm(void);

//void Front_Wheel_Control(void);
//void IPC_Set_Specified_Angle( s16 f_front_sa_IPC );//前轮转到指定角度
//void Set_Front_Specified_Angle( s16 f_sa, u8 f_priority );
//s8 Get_Front_Specified_Angle(void);
//s8 Turn_To_Specified_Angle( void );

#endif
