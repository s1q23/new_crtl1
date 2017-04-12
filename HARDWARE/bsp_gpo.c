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
}

//继电器控制管脚配置函数
void Config_GPIO_Control_Relay()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//预留
	RCC_AHB1PeriphClockCmd(GPIO_RESERVE_1_CLOCK | GPIO_RESERVE_2_CLOCK | GPIO_RESERVE_3_CLOCK | GPIO_RESERVE_4_CLOCK, ENABLE);
	//超声波  电磁刹 高音喇叭 对讲模块
	RCC_AHB1PeriphClockCmd(GPIO_ULTRASONIC_CLOCK | GPIO_ELECTR_BRAKE_CLOCK | GPIO_TWEETER_CLOCK | GPIO_TALKBACK_CLOCK, ENABLE);
	//前灯 尾灯 左转灯 右转灯 
	RCC_AHB1PeriphClockCmd(GPIO_HEADLIGHT_CLOCK | GPIO_TAILLIGHT_CLOCK | GPIO_LEFTLIGHT_CLOCK | GPIO_RIGHTLIGHT_CLOCK, ENABLE);
	//刹车灯 警灯 探照灯 电源指示灯
	RCC_AHB1PeriphClockCmd(GPIO_STOPLIGHT_CLOCK | GPIO_ALARM_LAMP_CLOCK | GPIO_SEARCHLIGHT_CLOCK | GPIO_POWER_LIGHT_CLOCK, ENABLE);
	//管脚配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//GPIO_OType_OD;//开漏+上拉 低功耗
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//预留
	GPIO_InitStructure.GPIO_Pin = PIN_RESERVE_1;
	GPIO_Init(GPIO_RESERVE_1,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_RESERVE_2;
	GPIO_Init(GPIO_RESERVE_2,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_RESERVE_3;
	GPIO_Init(GPIO_RESERVE_3,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_RESERVE_4;
	GPIO_Init(GPIO_RESERVE_4,&GPIO_InitStructure);
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
	
	GPIO_InitStructure.GPIO_Pin = PIN_SEARCHLIGHT;
	GPIO_Init(GPIO_SEARCHLIGHT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_POWER_LIGHT;
	GPIO_Init(GPIO_POWER_LIGHT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
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


//电磁刹继电器控制
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
	if( f_set == RELAY_OFF )
	{
		Headlight_OFF;
	}
	else
	{
		Headlight_ON;
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
}

//刹车灯继电器控制
void Relay_breaklight( u8 f_set, u8 f_priority )
{
	if( f_set == RELAY_OFF )
	{
		STOPLIGHT_OFF;
	}
	else
	{
		STOPLIGHT_ON;
	}
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
}
//探照灯继电器控制
void Relay_searchlight( u8 f_set, u8 f_priority )
{
	if( f_set == RELAY_OFF )
	{
		SEARCHLIGHT_OFF;
	}
	else
	{
		SEARCHLIGHT_ON;
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
	if( f_set == RELAY_OFF )
	{
		OSTmrStop(&TmrLeftLight,OS_OPT_TMR_NONE,0,&err);
		LEFTLIGHT_OFF;
	}
	else if( f_set == RELAY_ON )
	{
		OSTmrStop(&TmrLeftLight,OS_OPT_TMR_NONE,0,&err);
		LEFTLIGHT_ON;
	}
	else
	{
		LEFTLIGHT_ON;
		OSTmrStart(&TmrLeftLight,&err);
	}		
}
//右转灯继电器控制 0 关；1 开；2；闪烁
void Relay_rightlight( u8 f_set, u8 f_priority )
{
	static OS_ERR err;
	if( f_set == RELAY_OFF )
	{
		OSTmrStop(&TmrRightLight,OS_OPT_TMR_NONE,0,&err);
		RIGHTLIGHT_OFF;
	}
	else if( f_set == RELAY_ON )
	{
		OSTmrStop(&TmrRightLight,OS_OPT_TMR_NONE,0,&err);
		RIGHTLIGHT_ON;
	}
	else
	{
		RIGHTLIGHT_ON;
		OSTmrStart(&TmrRightLight,&err);
	}		
}

//寻迹功能的开启和关闭
void Set_Tracking( u8 f_set, u8 f_priority )
{

}
