#include "bsp_gpi.h"
#include "bsp_gpo.h"
#include "delay.h"
#include "bsp_back_wheel.h"
#include "bsp_front_wheel.h"

//0-1数字量信号输入管脚配置，这里如果这种信号多的话可以考虑使用DMA
void Config_GPIO_Input()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//震动传感器 玻璃破碎传感器
	RCC_AHB1PeriphClockCmd( GPIO_SHAKE_CLOCK|GPIO_GLASS_CLOCK|GPIO_SMOG_CLOCK|GPIO_EMGBTN_CLOCK|GPIO_WATER_CLOCK, ENABLE);
	//震动传感器管脚配置  PH6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//根据传感器的输出类型而定
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = PIN_SHAKE;
	GPIO_Init(GPIO_SHAKE,&GPIO_InitStructure);

	//玻璃破碎传感器管脚配置 PH8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//根据传感器的输出类型而定
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = PIN_GLASS;
	GPIO_Init(GPIO_GLASS,&GPIO_InitStructure);
	
	//烟雾传感器PD8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//根据传感器的输出类型而定
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = PIN_SMOG;
	GPIO_Init(GPIO_SMOG,&GPIO_InitStructure);	
	//急停按钮 PD9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//根据传感器的输出类型而定
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = PIN_EMGBTN;
	GPIO_Init(GPIO_SMOG,&GPIO_InitStructure);		
	//水位
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//根据传感器的输出类型而定
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin =  PIN_WATER;
	GPIO_Init(GPIO_WATER,&GPIO_InitStructure);			
}

void Config_GPIO_Encoder_TRD_MA()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	//GPIOG:
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOG,&GPIO_InitStructure);
}

/*获取前驱的角度信息*/
float get_front_angel()
{
/********************************************************************
// 角度传感器直接接在MCU上,实际使用时使用这份代码
********************************************************************/
#if 1	
	int16_t f_encoder_gray,f_encoder_binary;
	float angel_value;
	/*0.读取IO的引脚原始信息*/
	f_encoder_gray = ~GPIO_ReadInputData(GPIOG);
	f_encoder_gray = f_encoder_gray&0x1FF;
	/*1.将格雷码转换为BCD码*/
	f_encoder_binary = GraytoDecimal((unsigned int)f_encoder_gray ) & 0x1FF;
	/*2.正负号处理*/
	if(f_encoder_binary >= 256)
		f_encoder_binary = f_encoder_binary - 512;
	/*3.得到角度信息*/
	angel_value=f_encoder_binary * RESOLUTION_512;
	return angel_value;
/*******************************************************************
角度传感器接在最小系统板上，通过can将角度信息传递过来，测试时临时使用
全局变量在task_can_com.c文件中被修改	
********************************************************************/	
#else  
	return front_angel_by_can;
#endif	
}

static void TIM7_init()
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM7,ENABLE );
	
	TIM_TimeBaseInitStructure.TIM_Prescaler = 839;    //84MHz---->100KHz
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInitStructure.TIM_Period = 49;     //   0.5ms 一次中断
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseInitStructure );
		
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;//定时器3中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure );
	
	TIM_ClearFlag(TIM7, TIM_FLAG_Update);
	TIM_ARRPreloadConfig(TIM7, DISABLE);             //关闭自动重装载功能
	TIM_Cmd(TIM7, DISABLE);                          //使能TIM7
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);       //使能定时器更新终端	
}

//外部中断管脚配置，防撞条
void Config_GPIO_EXTI_BUMPER()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_AHB1PeriphClockCmd( GPIO_BUMPER_CLOCK | GPIO_SCRAM_BUTTON_CLOCK | GPIO_MECHANICAL_ZERO_CLOCK, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_SYSCFG, ENABLE);
	
	//防撞条外部中断管脚配置PH7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;     //外部中断好像要配置成输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;   //下拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = PIN_BUMPER;
	GPIO_Init(GPIO_BUMPER,&GPIO_InitStructure);
	//外部中断配置
	SYSCFG_EXTILineConfig( EXTI_PORTSOURCE_BUMPER, EXTI_PINSOURCE_BUMPER );
	EXTI_ClearITPendingBit(EXTI_LINE_BUMPER);
	EXTI_InitStructure.EXTI_Line = EXTI_LINE_BUMPER;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_Init(&EXTI_InitStructure);
	//防撞条中断配置
	NVIC_InitStructure.NVIC_IRQChannel = EXTI_BUMPER_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EXTI_BUMPER_PreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = EXTI_BUMPER_SubPriority;
	NVIC_Init(&NVIC_InitStructure);
	TIM7_init();
}

void EXTI9_5_IRQHandler(void)  //防撞条中断
{
	OSIntEnter();
  if(SET == EXTI_GetITStatus(EXTI_LINE_BUMPER))
  {    
		/*外部中断本身不做任何事情，将滤波处理交给定时器*/
		TIM7->CNT=0;
		TIM_Cmd(TIM7,ENABLE);
    EXTI_ClearITPendingBit(EXTI_LINE_BUMPER);
  }
	OSIntExit();
}

void TIM7_IRQHandler()
{
	volatile static int crash_cnt=0;
	volatile static int uncrash_cnt=0;
	OSIntEnter();
	if(TIM_GetITStatus(TIM7,TIM_IT_Update)==SET)
	{
		if(Crash_State) 
		{
			crash_cnt++;
			uncrash_cnt=0;
		}
		else 
		{
			crash_cnt=0;
			uncrash_cnt++;
		}
		if(crash_cnt>150)
		{
			/*0.紧急停止后驱*/
			back_wheel_set_speed(0);
			/*1.前驱停止*/
			front_wheel_stop();
			/*2.锁定小车，其他任何运动控制都将不被执行*/
			elecmbile_status.crash_strip=true;
			/*3.3秒后应该解除锁定*/
			Idle_Cnt_Crash=0;
			LED4=0;
		}
		if(uncrash_cnt)
		{
			TIM_Cmd(TIM7,DISABLE);
		}
		TIM7->SR = ~TIM_FLAG_Update;          //清中断
	}	
	OSIntExit();
}
