#include "bsp_gpo.h"
#include "task_timer.h"

//LED IO初始化
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI,ENABLE); //使能GPIOD的时钟
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;      //输出
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;     //推挽输出
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;  //上拉输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz; //高速GPIO
	GPIO_Init(GPIOI,&GPIO_InitStructure);
	LED1=1;LED2=1;LED3=1;LED4=1;
}

//继电器控制管脚配置函数
void Config_GPIO_Control_Relay()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//强光灯   广播静音  导航探照灯 
	RCC_AHB1PeriphClockCmd(GPIO_BRINGHTLIGHT_CLOCK | GPIO_SPEAKERMUTE_CLOCK | GPIO_NAVIBRIGHT_CLOCK | GPIO_SMOGSENSOR_CLOCK, ENABLE);
	//超声波  电磁刹 高音喇叭 对讲模块
	RCC_AHB1PeriphClockCmd(GPIO_ULTRASONIC_CLOCK | GPIO_ELECTR_BRAKE_CLOCK | GPIO_TWEETER_CLOCK | GPIO_TALKBACK_CLOCK, ENABLE);
	//前灯 尾灯 左转灯 右转灯 
	RCC_AHB1PeriphClockCmd(GPIO_HEADLIGHT_CLOCK | GPIO_TAILLIGHT_CLOCK | GPIO_LEFTLIGHT_CLOCK | GPIO_RIGHTLIGHT_CLOCK, ENABLE);
	//刹车灯 警灯 红外补光灯 电源指示灯
	RCC_AHB1PeriphClockCmd(GPIO_STOPLIGHT_CLOCK | GPIO_ALARM_LAMP_CLOCK | GPIO_INFRAREDLIGHT_CLOCK | GPIO_POWER_LIGHT_CLOCK|GPIO_POWERLED_CLOCK, ENABLE);
	//管脚配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//GPIO_OType_OD;//开漏+上拉 低功耗
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//预留
	GPIO_InitStructure.GPIO_Pin = PIN_BRINGHTLIGHT;
	GPIO_Init(GPIO_BRINGHTLIGHT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_SPEAKERMUTE;
	GPIO_Init(GPIO_SPEAKERMUTE,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_NAVIBRIGHT;
	GPIO_Init(GPIO_NAVIBRIGHT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_SMOGSENSOR;
	GPIO_Init(GPIO_SMOGSENSOR,&GPIO_InitStructure);
	//超声波  电磁刹 高音喇叭 对讲模块
	GPIO_InitStructure.GPIO_Pin = PIN_ULTRASONIC;
	GPIO_Init(GPIO_ULTRASONIC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_ELECTR_BRAKE;
	GPIO_Init(GPIO_ELECTR_BRAKE,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_TWEETER;
	GPIO_Init(GPIO_TWEETER,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_TALKBACK;
	GPIO_Init(GPIO_TALKBACK,&GPIO_InitStructure);
	//前灯 尾灯 左转灯 右转灯 
	GPIO_InitStructure.GPIO_Pin = PIN_Headlight;
	GPIO_Init(GPIO_HEADLIGHT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_TAILLIGHT;
	GPIO_Init(GPIO_TAILLIGHT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_LEFTLIGHT;
	GPIO_Init(GPIO_LEFTLIGHT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_RIGHTLIGHT;
	GPIO_Init(GPIO_RIGHTLIGHT,&GPIO_InitStructure);
	//刹车灯 警灯 探照灯 电源指示灯
	GPIO_InitStructure.GPIO_Pin = PIN_STOPLIGHT;
	GPIO_Init(GPIO_STOPLIGHT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_ALARM_LAMP;
	GPIO_Init(GPIO_ALARM_LAMP,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_INFRAREDLIGHT;
	GPIO_Init(GPIO_INFRAREDLIGHT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_POWER_LIGHT;
	GPIO_Init(GPIO_POWER_LIGHT,&GPIO_InitStructure);
	//PB9 做什么用的？
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
	//PC8 485的DIR
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC, ENABLE);
	//管脚配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//预留
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}
void Config_GPIO_Control_enc28j60_res()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOF, ENABLE);

	//管脚配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//预留
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOF,&GPIO_InitStructure);
}


//电磁刹继电器控制,电磁刹已经不用继电器控制了
void Relay_embreak( u8 f_set, u8 f_priority )  //优先级问题很大。。。。。。以后得改
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
//前灯继电器控制
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
//后灯继电器控制
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

//刹车灯继电器控制
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

//警灯继电器控制
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
//探照灯继电器控制
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
//电源指示灯继电器控制
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
//对讲模块继电器控制
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

//强光灯继电器控制
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

//广播静音继电器控制
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

//导航探照灯继电器控制
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
//高音喇叭继电器控制
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

//左转灯继电器控制 0 关；1 开；2；闪烁
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

//右转灯继电器控制 0 关；1 开；2；闪烁
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

/*电源指示灯的颜色*/
void Set_PowerLED(unsigned char color)
{
	unsigned char green=color&0x01;
	unsigned char red  =(color&0x02)>>1;
	pPowerLedGreen=green;
	pPowerLedRed  =red;
}

//寻迹功能的开启和关闭
void Set_Tracking( u8 f_set, u8 f_priority )
{
	// TODO
}
