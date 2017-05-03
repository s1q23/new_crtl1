#include "bsp_gpo.h"
#include "task_timer.h"

//LED IO��ʼ��
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI,ENABLE); //ʹ��GPIOD��ʱ��
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;      //���
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;     //�������
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;  //�������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz; //����GPIO
	GPIO_Init(GPIOI,&GPIO_InitStructure);
	LED1=1;LED2=1;LED3=1;LED4=1;
}

//�̵������ƹܽ����ú���
void Config_GPIO_Control_Relay()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//ǿ���   �㲥����  ����̽�յ� 
	RCC_AHB1PeriphClockCmd(GPIO_BRINGHTLIGHT_CLOCK | GPIO_SPEAKERMUTE_CLOCK | GPIO_NAVIBRIGHT_CLOCK | GPIO_SMOGSENSOR_CLOCK, ENABLE);
	//������  ���ɲ �������� �Խ�ģ��
	RCC_AHB1PeriphClockCmd(GPIO_ULTRASONIC_CLOCK | GPIO_ELECTR_BRAKE_CLOCK | GPIO_TWEETER_CLOCK | GPIO_TALKBACK_CLOCK, ENABLE);
	//ǰ�� β�� ��ת�� ��ת�� 
	RCC_AHB1PeriphClockCmd(GPIO_HEADLIGHT_CLOCK | GPIO_TAILLIGHT_CLOCK | GPIO_LEFTLIGHT_CLOCK | GPIO_RIGHTLIGHT_CLOCK, ENABLE);
	//ɲ���� ���� ���ⲹ��� ��Դָʾ��
	RCC_AHB1PeriphClockCmd(GPIO_STOPLIGHT_CLOCK | GPIO_ALARM_LAMP_CLOCK | GPIO_INFRAREDLIGHT_CLOCK | GPIO_POWER_LIGHT_CLOCK|GPIO_POWERLED_CLOCK, ENABLE);
	//�ܽ�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//GPIO_OType_OD;//��©+���� �͹���
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//Ԥ��
	GPIO_InitStructure.GPIO_Pin = PIN_BRINGHTLIGHT;
	GPIO_Init(GPIO_BRINGHTLIGHT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_SPEAKERMUTE;
	GPIO_Init(GPIO_SPEAKERMUTE,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_NAVIBRIGHT;
	GPIO_Init(GPIO_NAVIBRIGHT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_SMOGSENSOR;
	GPIO_Init(GPIO_SMOGSENSOR,&GPIO_InitStructure);
	//������  ���ɲ �������� �Խ�ģ��
	GPIO_InitStructure.GPIO_Pin = PIN_ULTRASONIC;
	GPIO_Init(GPIO_ULTRASONIC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_ELECTR_BRAKE;
	GPIO_Init(GPIO_ELECTR_BRAKE,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_TWEETER;
	GPIO_Init(GPIO_TWEETER,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_TALKBACK;
	GPIO_Init(GPIO_TALKBACK,&GPIO_InitStructure);
	//ǰ�� β�� ��ת�� ��ת�� 
	GPIO_InitStructure.GPIO_Pin = PIN_Headlight;
	GPIO_Init(GPIO_HEADLIGHT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_TAILLIGHT;
	GPIO_Init(GPIO_TAILLIGHT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_LEFTLIGHT;
	GPIO_Init(GPIO_LEFTLIGHT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_RIGHTLIGHT;
	GPIO_Init(GPIO_RIGHTLIGHT,&GPIO_InitStructure);
	//ɲ���� ���� ̽�յ� ��Դָʾ��
	GPIO_InitStructure.GPIO_Pin = PIN_STOPLIGHT;
	GPIO_Init(GPIO_STOPLIGHT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_ALARM_LAMP;
	GPIO_Init(GPIO_ALARM_LAMP,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_INFRAREDLIGHT;
	GPIO_Init(GPIO_INFRAREDLIGHT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_POWER_LIGHT;
	GPIO_Init(GPIO_POWER_LIGHT,&GPIO_InitStructure);
	//PB9 ��ʲô�õģ�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_POWERLED;
	GPIO_Init(GPIO_POWERLED,&GPIO_InitStructure);
	
	pPowerLedGreen=1;
	pPowerLedRed=0;
	SMOGSENSOR_ON;
}

void Config_GPIO_Control_485()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//PC8 485��DIR
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC, ENABLE);
	//�ܽ�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//Ԥ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}
void Config_GPIO_Control_enc28j60_res()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOF, ENABLE);

	//�ܽ�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//Ԥ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOF,&GPIO_InitStructure);
}


//���ɲ�̵�������,���ɲ�Ѿ����ü̵���������
void Relay_embreak( u8 f_set, u8 f_priority )  //���ȼ�����ܴ󡣡����������Ժ�ø�
{
	if( f_set == RELAY_OFF )
	{
		ELECTR_BRAKE_OFF;
	}
	else
	{
		ELECTR_BRAKE_ON;
	}
}
//ǰ�Ƽ̵�������
void Relay_frontlight( u8 f_set, u8 f_priority )
{
	static OS_ERR err;
	if(f_set==RELAY_OFF)
	{
		OSTmrStop(&TmrFrontLight,OS_OPT_TMR_NONE,0,&err);
		Headlight_OFF;
		rms_state.front_light=RELAY_OFF;
	}
	else if(f_set==RELAY_ON)
	{
		OSTmrStop(&TmrFrontLight,OS_OPT_TMR_NONE,0,&err);
		Headlight_ON;
		rms_state.front_light=RELAY_ON;
	}
	else
	{
		LEFTLIGHT_ON;
		OSTmrStart(&TmrFrontLight,&err);
		rms_state.front_light=RELAY_FLASH;
	}
}
//��Ƽ̵�������
void Relay_backlight( u8 f_set, u8 f_priority )
{
	if( f_set == RELAY_OFF )
	{
		TAILLIGHT_OFF;
	}
	else
	{
		TAILLIGHT_ON;
	}
	rms_state.back_light=f_set;
}

//ɲ���Ƽ̵�������
void Relay_brakelight( u8 f_set, u8 f_priority )
{
	if( f_set == RELAY_OFF )
	{
		STOPLIGHT_OFF;
		elecmbile_status.brakelight_status=0;
	}
	else
	{
		STOPLIGHT_ON;
		elecmbile_status.brakelight_status=1;
	}
	rms_state.brake_light=f_set;
}

//���Ƽ̵�������
void Relay_alarmlight( u8 f_set, u8 f_priority )
{
	if( f_set == RELAY_OFF )
	{
		ALARM_LAMP_OFF;
	}
	else
	{
		ALARM_LAMP_ON;
	}
	rms_state.light_alarm=f_set;
}
//̽�յƼ̵�������
void Relay_infraredlight( u8 f_set, u8 f_priority )
{
	if( f_set == RELAY_OFF )
	{
		INFRAREDLIGHT_OFF;
	}
	else
	{
		INFRAREDLIGHT_ON;
	}
}
//��Դָʾ�Ƽ̵�������
void Relay_powerlight( u8 f_set, u8 f_priority )
{
	if( f_set == RELAY_OFF )
	{
		POWER_LIGHT_OFF;
	}
	else
	{
		POWER_LIGHT_ON;
	}
}
//�Խ�ģ��̵�������
void Relay_talkback_module(u8 f_set,u8 f_priority)
{
	if(f_set==RELAY_OFF)
	{
		TALKBACK_OFF;
	}
	else
	{
		TALKBACK_ON;
	}
}

//ǿ��Ƽ̵�������
void Relay_brightlight( u8 f_set, u8 f_priority )
{
	if( f_set == RELAY_OFF )
	{
		BRINGHTLIGHT_OFF;
	}
	else
	{
		BRINGHTLIGHT_ON;
	}
}

//�㲥�����̵�������
void Relay_speakermute( u8 f_set, u8 f_priority )
{
	if( f_set == RELAY_OFF )
	{
		SPEAKERMUTE_OFF;
	}
	else
	{
		SPEAKERMUTE_ON;
	}
}

//����̽�յƼ̵�������
void Relay_navibright( u8 f_set, u8 f_priority )
{
	if(f_set == RELAY_OFF)
	{
		NAVIBRIGHT_OFF;
	}
	else
	{
		NAVIBRIGHT_ON;
	}
}
//�������ȼ̵�������
void Relay_highspeaker( u8 f_set, u8 f_priority )
{
	if( f_set == RELAY_OFF )
	{
		TWEETER_OFF;
	}
	else
	{
		TWEETER_ON;
	}
}

//��ת�Ƽ̵������� 0 �أ�1 ����2����˸
void Relay_leftlight( u8 f_set, u8 f_priority )
{
	static OS_ERR err;
	if(f_set == RELAY_OFF)
	{
		OSTmrStop(&TmrLeftLight,OS_OPT_TMR_NONE,0,&err);
		LEFTLIGHT_OFF;
		elecmbile_status.L_light_state=RELAY_OFF;
	}
	else if(f_set == RELAY_ON)
	{
		LEFTLIGHT_ON;
		OSTmrStart(&TmrLeftLight,&err);
		elecmbile_status.L_light_state=RELAY_ON;
	}
	rms_state.left_light=f_set;
}

//��ת�Ƽ̵������� 0 �أ�1 ����2����˸
void Relay_rightlight( u8 f_set, u8 f_priority )
{
	static OS_ERR err;
	if(f_set == RELAY_OFF)
	{
		OSTmrStop(&TmrRightLight,OS_OPT_TMR_NONE,0,&err);
		RIGHTLIGHT_OFF;
		elecmbile_status.R_light_state=RELAY_OFF;
	}
	else if(f_set==RELAY_ON)
	{
		RIGHTLIGHT_ON;
		OSTmrStart(&TmrRightLight,&err);
		elecmbile_status.R_light_state=RELAY_ON;
	}
	rms_state.right_light=f_set;
}

/*��Դָʾ�Ƶ���ɫ*/
void Set_PowerLED(unsigned char color)
{
	unsigned char green=color&0x01;
	unsigned char red  =(color&0x02)>>1;
	pPowerLedGreen=green;
	pPowerLedRed  =red;
}

//Ѱ�����ܵĿ����͹ر�
void Set_Tracking( u8 f_set, u8 f_priority )
{
	// TODO
}
