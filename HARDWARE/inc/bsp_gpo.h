#ifndef _BSP_GPO_H
#define _BSP_GPO_H
#include "sys.h"
#include "mj_lib.h"

/******************板子上的LED指示灯IO定义********************************/
#define LED1                                     PIout(4)
#define LED2                                     PIout(5)
#define LED3                                     PIout(6)
#define LED4                                     PIout(7)

/**************************************************************************
继电器（relay）控制管脚配置
**************************************************************************/
//预留：PG9,PD7,PD6,PD5 reserve
#define GPIO_RESERVE_1							GPIOG
#define GPIO_RESERVE_1_CLOCK				RCC_AHB1Periph_GPIOG
#define PIN_RESERVE_1								GPIO_Pin_9
#define RESERVE_1_ON								GPIO_SetBits(GPIO_RESERVE_1, PIN_RESERVE_1);
#define RESERVE_1_OFF								GPIO_ResetBits(GPIO_RESERVE_1, PIN_RESERVE_1);

#define GPIO_RESERVE_2							GPIOD
#define GPIO_RESERVE_2_CLOCK				RCC_AHB1Periph_GPIOD
#define PIN_RESERVE_2								GPIO_Pin_5
#define RESERVE_2_ON								GPIO_SetBits(GPIO_RESERVE_2, PIN_RESERVE_2);
#define RESERVE_2_OFF								GPIO_ResetBits(GPIO_RESERVE_2, PIN_RESERVE_2);

#define GPIO_RESERVE_3							GPIOD
#define GPIO_RESERVE_3_CLOCK				RCC_AHB1Periph_GPIOD
#define PIN_RESERVE_3								GPIO_Pin_6
#define RESERVE_3_ON								GPIO_SetBits(GPIO_RESERVE_3, PIN_RESERVE_3);
#define RESERVE_3_OFF								GPIO_ResetBits(GPIO_RESERVE_3, PIN_RESERVE_3);

#define GPIO_RESERVE_4							GPIOD
#define GPIO_RESERVE_4_CLOCK				RCC_AHB1Periph_GPIOD
#define PIN_RESERVE_4								GPIO_Pin_7
#define RESERVE_4_ON								GPIO_SetBits(GPIO_RESERVE_4, PIN_RESERVE_4);
#define RESERVE_4_OFF								GPIO_ResetBits(GPIO_RESERVE_4, PIN_RESERVE_4);

//超声波控制：PD4 ultrasonic	
#define GPIO_ULTRASONIC							GPIOD
#define GPIO_ULTRASONIC_CLOCK				RCC_AHB1Periph_GPIOD
#define PIN_ULTRASONIC							GPIO_Pin_4
#define ULTRASONIC_ON								GPIO_SetBits(GPIO_ULTRASONIC, PIN_ULTRASONIC);
#define ULTRASONIC_OFF							GPIO_ResetBits(GPIO_ULTRASONIC, PIN_ULTRASONIC);

//电磁刹控制：PD3 electromagentic brake
#define GPIO_ELECTR_BRAKE						GPIOD
#define GPIO_ELECTR_BRAKE_CLOCK			RCC_AHB1Periph_GPIOD
#define PIN_ELECTR_BRAKE						GPIO_Pin_3
#define ELECTR_BRAKE_ON							GPIO_SetBits(GPIO_ELECTR_BRAKE, PIN_ELECTR_BRAKE);
#define ELECTR_BRAKE_OFF						GPIO_ResetBits(GPIO_ELECTR_BRAKE, PIN_ELECTR_BRAKE);

//前灯控制：PD1 headlight
#define GPIO_HEADLIGHT							GPIOD
#define GPIO_HEADLIGHT_CLOCK				RCC_AHB1Periph_GPIOD
#define PIN_Headlight								GPIO_Pin_1
#define Headlight_ON								GPIO_SetBits(GPIO_HEADLIGHT, PIN_Headlight);
#define Headlight_OFF								GPIO_ResetBits(GPIO_HEADLIGHT, PIN_Headlight);

//尾灯控制：PD1 taillight
#define GPIO_TAILLIGHT							GPIOD
#define GPIO_TAILLIGHT_CLOCK				RCC_AHB1Periph_GPIOD
#define PIN_TAILLIGHT								GPIO_Pin_0
#define TAILLIGHT_ON								GPIO_SetBits(GPIO_TAILLIGHT, PIN_TAILLIGHT);
#define TAILLIGHT_OFF								GPIO_ResetBits(GPIO_TAILLIGHT, PIN_TAILLIGHT);

//左转灯控制：PG10 leftlight
#define GPIO_LEFTLIGHT							GPIOG
#define GPIO_LEFTLIGHT_CLOCK				RCC_AHB1Periph_GPIOG
#define PIN_LEFTLIGHT								GPIO_Pin_10
#define LEFTLIGHT_ON								GPIO_SetBits(GPIO_LEFTLIGHT, PIN_LEFTLIGHT);
#define LEFTLIGHT_OFF								GPIO_ResetBits(GPIO_LEFTLIGHT, PIN_LEFTLIGHT);
#define LEFTLIGHT_TOGGLE            GPIO_ToggleBits(GPIO_LEFTLIGHT, PIN_LEFTLIGHT);

//右转灯控制：PG11 rightlight
#define GPIO_RIGHTLIGHT							GPIOG
#define GPIO_RIGHTLIGHT_CLOCK				RCC_AHB1Periph_GPIOG
#define PIN_RIGHTLIGHT							GPIO_Pin_11
#define RIGHTLIGHT_ON								GPIO_SetBits(GPIO_RIGHTLIGHT, PIN_RIGHTLIGHT);
#define RIGHTLIGHT_OFF							GPIO_ResetBits(GPIO_RIGHTLIGHT, PIN_RIGHTLIGHT);
#define RIGHTLIGHT_TOGGLE           GPIO_ToggleBits(GPIO_RIGHTLIGHT, PIN_RIGHTLIGHT);

//刹车灯控制：PG12 stoplight
#define GPIO_STOPLIGHT							GPIOG
#define GPIO_STOPLIGHT_CLOCK				RCC_AHB1Periph_GPIOG
#define PIN_STOPLIGHT								GPIO_Pin_12
#define STOPLIGHT_ON								GPIO_SetBits(GPIO_STOPLIGHT, PIN_STOPLIGHT);
#define STOPLIGHT_OFF								GPIO_ResetBits(GPIO_STOPLIGHT, PIN_STOPLIGHT);

//警灯控制：PG13 alarm lamp
#define GPIO_ALARM_LAMP							GPIOG
#define GPIO_ALARM_LAMP_CLOCK				RCC_AHB1Periph_GPIOG
#define PIN_ALARM_LAMP							GPIO_Pin_13
#define ALARM_LAMP_ON								GPIO_SetBits(GPIO_ALARM_LAMP, PIN_ALARM_LAMP);
#define ALARM_LAMP_OFF							GPIO_ResetBits(GPIO_ALARM_LAMP, PIN_ALARM_LAMP);

//探照灯控制：PG14 searchlight
#define GPIO_SEARCHLIGHT						GPIOG
#define GPIO_SEARCHLIGHT_CLOCK			RCC_AHB1Periph_GPIOG
#define PIN_SEARCHLIGHT							GPIO_Pin_14
#define SEARCHLIGHT_ON							GPIO_SetBits(GPIO_SEARCHLIGHT, PIN_SEARCHLIGHT);
#define SEARCHLIGHT_OFF							GPIO_ResetBits(GPIO_SEARCHLIGHT, PIN_SEARCHLIGHT);

//电源指示灯控制：PG15 power light
#define GPIO_POWER_LIGHT						GPIOG
#define GPIO_POWER_LIGHT_CLOCK			RCC_AHB1Periph_GPIOG
#define PIN_POWER_LIGHT							GPIO_Pin_15
#define POWER_LIGHT_ON							GPIO_SetBits(GPIO_POWER_LIGHT, PIN_POWER_LIGHT);
#define POWER_LIGHT_OFF							GPIO_ResetBits(GPIO_POWER_LIGHT, PIN_POWER_LIGHT);

//高音喇叭控制：PB3 tweeter
#define GPIO_TWEETER								GPIOB
#define GPIO_TWEETER_CLOCK					RCC_AHB1Periph_GPIOB
#define PIN_TWEETER									GPIO_Pin_3
#define TWEETER_ON									GPIO_SetBits(GPIO_TWEETER, PIN_TWEETER);
#define TWEETER_OFF									GPIO_ResetBits(GPIO_TWEETER, PIN_TWEETER);

//对讲模块控制：PB4 talkback
#define GPIO_TALKBACK								GPIOB
#define GPIO_TALKBACK_CLOCK					RCC_AHB1Periph_GPIOB
#define PIN_TALKBACK								GPIO_Pin_4
#define TALKBACK_ON									GPIO_SetBits(GPIO_TALKBACK, PIN_TALKBACK);
#define TALKBACK_OFF								GPIO_ResetBits(GPIO_TALKBACK, PIN_TALKBACK);


void LED_Init(void);
void Config_GPIO_Control_Relay(void);
void Config_GPIO_Control_485(void);
void Config_GPIO_Control_enc28j60_res(void);
//左转灯继电器控制 0 关；1 开；2；闪烁
void Relay_leftlight( u8 f_set, u8 f_priority );
//右转灯继电器控制 0 关；1 开；2；闪烁
void Relay_rightlight( u8 f_set, u8 f_priority );
void Relay_frontlight( u8 f_set, u8 f_priority );
void Relay_backlight( u8 f_set, u8 f_priority );
void Relay_highspeaker( u8 f_set, u8 f_priority );
void Relay_alarmlight( u8 f_set, u8 f_priority );
void Set_Tracking( u8 f_set, u8 f_priority );
void Relay_breaklight( u8 f_set, u8 f_priority );
void Relay_searchlight( u8 f_set, u8 f_priority );

#endif
