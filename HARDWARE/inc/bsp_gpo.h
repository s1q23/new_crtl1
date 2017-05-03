#ifndef _BSP_GPO_H
#define _BSP_GPO_H
#include "sys.h"
#include "mj_lib.h"

/******************�����ϵ�LEDָʾ��IO����********************************/
#define LED1                                     PIout(4)
#define LED2                                     PIout(5)
#define LED3                                     PIout(6)
#define LED4                                     PIout(7)


/**************************************************************************
�̵�����relay�����ƹܽ�����
**************************************************************************/
//PG9 ǿ���
#define GPIO_BRINGHTLIGHT							GPIOG
#define GPIO_BRINGHTLIGHT_CLOCK				RCC_AHB1Periph_GPIOG
#define PIN_BRINGHTLIGHT								GPIO_Pin_9
#define BRINGHTLIGHT_ON								GPIO_SetBits(GPIO_BRINGHTLIGHT, PIN_BRINGHTLIGHT);
#define BRINGHTLIGHT_OFF								GPIO_ResetBits(GPIO_BRINGHTLIGHT, PIN_BRINGHTLIGHT);

//PD5 ��������
#define GPIO_SPEAKERMUTE							GPIOD
#define GPIO_SPEAKERMUTE_CLOCK				RCC_AHB1Periph_GPIOD
#define PIN_SPEAKERMUTE								GPIO_Pin_5
#define SPEAKERMUTE_ON								GPIO_SetBits(GPIO_SPEAKERMUTE, PIN_SPEAKERMUTE);
#define SPEAKERMUTE_OFF								GPIO_ResetBits(GPIO_SPEAKERMUTE, PIN_SPEAKERMUTE);

//PD6 ������
#define GPIO_NAVIBRIGHT							GPIOD
#define GPIO_NAVIBRIGHT_CLOCK				RCC_AHB1Periph_GPIOD
#define PIN_NAVIBRIGHT							GPIO_Pin_6
#define NAVIBRIGHT_ON								GPIO_SetBits(GPIO_NAVIBRIGHT, PIN_NAVIBRIGHT);
#define NAVIBRIGHT_OFF							GPIO_ResetBits(GPIO_NAVIBRIGHT, PIN_NAVIBRIGHT);

//PD7 �����������ؿ���
#define GPIO_SMOGSENSOR							GPIOD
#define GPIO_SMOGSENSOR_CLOCK				RCC_AHB1Periph_GPIOD
#define PIN_SMOGSENSOR							GPIO_Pin_7
#define SMOGSENSOR_ON								GPIO_SetBits(GPIO_SMOGSENSOR, PIN_SMOGSENSOR);
#define SMOGSENSOR_OFF							GPIO_ResetBits(GPIO_SMOGSENSOR, PIN_SMOGSENSOR);

//���������ƣ�PD4 ultrasonic	
#define GPIO_ULTRASONIC							GPIOD
#define GPIO_ULTRASONIC_CLOCK				RCC_AHB1Periph_GPIOD
#define PIN_ULTRASONIC							GPIO_Pin_4
#define ULTRASONIC_ON								GPIO_SetBits(GPIO_ULTRASONIC, PIN_ULTRASONIC);
#define ULTRASONIC_OFF							GPIO_ResetBits(GPIO_ULTRASONIC, PIN_ULTRASONIC);

//���ɲ���ƣ�PD3 electromagentic brake
#define GPIO_ELECTR_BRAKE						GPIOD
#define GPIO_ELECTR_BRAKE_CLOCK			RCC_AHB1Periph_GPIOD
#define PIN_ELECTR_BRAKE						GPIO_Pin_3
#define ELECTR_BRAKE_ON							GPIO_SetBits(GPIO_ELECTR_BRAKE, PIN_ELECTR_BRAKE);
#define ELECTR_BRAKE_OFF						GPIO_ResetBits(GPIO_ELECTR_BRAKE, PIN_ELECTR_BRAKE);

//ǰ�ƿ��ƣ�PD1 headlight
#define GPIO_HEADLIGHT							GPIOD
#define GPIO_HEADLIGHT_CLOCK				RCC_AHB1Periph_GPIOD
#define PIN_Headlight								GPIO_Pin_1
#define Headlight_ON								GPIO_SetBits(GPIO_HEADLIGHT, PIN_Headlight);
#define Headlight_OFF								GPIO_ResetBits(GPIO_HEADLIGHT, PIN_Headlight);
#define HeadLight_TOGGLE            GPIO_ToggleBits(GPIO_HEADLIGHT, PIN_Headlight);

//β�ƿ��ƣ�PD1 taillight
#define GPIO_TAILLIGHT							GPIOD
#define GPIO_TAILLIGHT_CLOCK				RCC_AHB1Periph_GPIOD
#define PIN_TAILLIGHT								GPIO_Pin_0
#define TAILLIGHT_ON								GPIO_SetBits(GPIO_TAILLIGHT, PIN_TAILLIGHT);
#define TAILLIGHT_OFF								GPIO_ResetBits(GPIO_TAILLIGHT, PIN_TAILLIGHT);

//��ת�ƿ��ƣ�PG10 leftlight
#define GPIO_LEFTLIGHT							GPIOG
#define GPIO_LEFTLIGHT_CLOCK				RCC_AHB1Periph_GPIOG
#define PIN_LEFTLIGHT								GPIO_Pin_10
#define LEFTLIGHT_ON								GPIO_SetBits(GPIO_LEFTLIGHT, PIN_LEFTLIGHT);
#define LEFTLIGHT_OFF								GPIO_ResetBits(GPIO_LEFTLIGHT, PIN_LEFTLIGHT);
#define LEFTLIGHT_TOGGLE            GPIO_ToggleBits(GPIO_LEFTLIGHT, PIN_LEFTLIGHT);

//��ת�ƿ��ƣ�PG11 rightlight
#define GPIO_RIGHTLIGHT							GPIOG
#define GPIO_RIGHTLIGHT_CLOCK				RCC_AHB1Periph_GPIOG
#define PIN_RIGHTLIGHT							GPIO_Pin_11
#define RIGHTLIGHT_ON								GPIO_SetBits(GPIO_RIGHTLIGHT, PIN_RIGHTLIGHT);
#define RIGHTLIGHT_OFF							GPIO_ResetBits(GPIO_RIGHTLIGHT, PIN_RIGHTLIGHT);
#define RIGHTLIGHT_TOGGLE           GPIO_ToggleBits(GPIO_RIGHTLIGHT, PIN_RIGHTLIGHT);

//ɲ���ƿ��ƣ�PG12 stoplight
#define GPIO_STOPLIGHT							GPIOG
#define GPIO_STOPLIGHT_CLOCK				RCC_AHB1Periph_GPIOG
#define PIN_STOPLIGHT								GPIO_Pin_12
#define STOPLIGHT_ON								GPIO_SetBits(GPIO_STOPLIGHT, PIN_STOPLIGHT);
#define STOPLIGHT_OFF								GPIO_ResetBits(GPIO_STOPLIGHT, PIN_STOPLIGHT);

//���ƿ��ƣ�PG13 alarm lamp
#define GPIO_ALARM_LAMP							GPIOG
#define GPIO_ALARM_LAMP_CLOCK				RCC_AHB1Periph_GPIOG
#define PIN_ALARM_LAMP							GPIO_Pin_13
#define ALARM_LAMP_ON								GPIO_SetBits(GPIO_ALARM_LAMP, PIN_ALARM_LAMP);
#define ALARM_LAMP_OFF							GPIO_ResetBits(GPIO_ALARM_LAMP, PIN_ALARM_LAMP);

//̽�յƿ��ƣ�PG14 INFRAREDLIGHT
#define GPIO_INFRAREDLIGHT						GPIOG
#define GPIO_INFRAREDLIGHT_CLOCK			RCC_AHB1Periph_GPIOG
#define PIN_INFRAREDLIGHT							GPIO_Pin_14
#define INFRAREDLIGHT_ON							GPIO_SetBits(GPIO_INFRAREDLIGHT, PIN_INFRAREDLIGHT);
#define INFRAREDLIGHT_OFF							GPIO_ResetBits(GPIO_INFRAREDLIGHT, PIN_INFRAREDLIGHT);

//��Դָʾ�ƿ��ƣ�PG15 power light
#define GPIO_POWER_LIGHT						GPIOG
#define GPIO_POWER_LIGHT_CLOCK			RCC_AHB1Periph_GPIOG
#define PIN_POWER_LIGHT							GPIO_Pin_15
#define POWER_LIGHT_ON							GPIO_SetBits(GPIO_POWER_LIGHT, PIN_POWER_LIGHT);
#define POWER_LIGHT_OFF							GPIO_ResetBits(GPIO_POWER_LIGHT, PIN_POWER_LIGHT);

//�������ȿ��ƣ�PB3 tweeter
#define GPIO_TWEETER								GPIOB
#define GPIO_TWEETER_CLOCK					RCC_AHB1Periph_GPIOB
#define PIN_TWEETER									GPIO_Pin_3
#define TWEETER_ON									GPIO_SetBits(GPIO_TWEETER, PIN_TWEETER);
#define TWEETER_OFF									GPIO_ResetBits(GPIO_TWEETER, PIN_TWEETER);

//�Խ�ģ����ƣ�PB4 talkback
#define GPIO_TALKBACK								GPIOB
#define GPIO_TALKBACK_CLOCK					RCC_AHB1Periph_GPIOB
#define PIN_TALKBACK								GPIO_Pin_4
#define TALKBACK_ON									GPIO_SetBits(GPIO_TALKBACK, PIN_TALKBACK);
#define TALKBACK_OFF								GPIO_ResetBits(GPIO_TALKBACK, PIN_TALKBACK);

//��Դָʾ���к�������״��,��������ָʾ��
#define GPIO_POWERLED								GPIOD
#define GPIO_POWERLED_CLOCK					RCC_AHB1Periph_GPIOD
#define PIN_POWERLED								(GPIO_Pin_10|GPIO_Pin_11)
#define POWERLED_GREEN							GPIO_SetBits(GPIO_POWERLED, PIN_TALKBACK);
#define POWERLED_RED								GPIO_ResetBits(GPIO_POWERLED, PIN_TALKBACK);
#define pPowerLedGreen              PDout(10)
#define pPowerLedRed                PDout(11)

void LED_Init(void);
void Config_GPIO_Control_Relay(void);
void Config_GPIO_Control_485(void);
void Config_GPIO_Control_enc28j60_res(void);
//��ת�Ƽ̵������� 0 �أ�1 ����2����˸
void Relay_leftlight( u8 f_set, u8 f_priority );
//��ת�Ƽ̵������� 0 �أ�1 ����2����˸
void Relay_rightlight( u8 f_set, u8 f_priority );
void Relay_frontlight( u8 f_set, u8 f_priority );
void Relay_backlight( u8 f_set, u8 f_priority );
void Relay_highspeaker( u8 f_set, u8 f_priority );
void Relay_alarmlight( u8 f_set, u8 f_priority );
void Set_Tracking( u8 f_set, u8 f_priority );
void Relay_brakelight( u8 f_set, u8 f_priority );
void Relay_infraredlight( u8 f_set, u8 f_priority );
void Relay_talkback_module( u8 f_set, u8 f_priority );
void Relay_brightlight( u8 f_set, u8 f_priority );
void Relay_infraredlight( u8 f_set, u8 f_priority );
void Relay_speakermute( u8 f_set, u8 f_priority );
void Relay_navibright( u8 f_set, u8 f_priority );
void Set_PowerLED(unsigned char color);
#endif
