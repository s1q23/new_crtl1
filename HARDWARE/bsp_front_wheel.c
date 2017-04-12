#include "bsp_front_wheel.h"
#include "bsp_gpi.h"
#include "bsp_gpo.h"

static char front_dir=DIR_STOP;

/*配置步进电机的使能、方向、脉冲引脚*/
void Config_Control_Front_Wheel()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;	
	TIM_OCInitTypeDef TIM_OCInitStructure; 

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM9,ENABLE );
	//步进电机的 使能 引脚配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = PIN_FRONT_EN;
	GPIO_Init( GPIO_FRONT_EN, &GPIO_InitStructure );
	FRONT_EN_OFF();
	//步进电机的 方向引脚配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//开漏+上拉 低功耗
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = PIN_FRONT_DIR;
	GPIO_Init( GPIO_FRONT_DIR, &GPIO_InitStructure );
	//步进电机的 pwm引脚配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//开漏+上拉 低功耗 不影响pwm输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//不影响pwm输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Pin = PIN_FRONT_PWM;
	GPIO_Init( GPIO_FRONT_PWM, &GPIO_InitStructure );
	GPIO_PinAFConfig( GPIO_FRONT_PWM, PINSOURCE_FRONT_PWM, GPIO_AF_FRONT_PWM );

	/******************PWM 输出频率  1KHz******************/
	TIM_DeInit( TIMx_FRONT_PWM );
	TIM_TimeBaseInitStructure.TIM_Prescaler = 167;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;   //重复次数，指重复溢出多少次才进中断
	TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;       //自动重装载0-65536，同函数  void TIM_SetAutoreload(TIM_TypeDef* TIMx, uint32_t Autoreload)
	TIM_TimeBaseInit( TIMx_FRONT_PWM, &TIM_TimeBaseInitStructure );

	//TIM Output Compare Init
	TIM_OCStructInit( &TIM_OCInitStructure );
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 500;                           //比较值0-65536 同 函数void TIM_SetCompare2(TIM_TypeDef* TIMx, uint32_t Compare2)
	TIM_OC2Init( TIMx_FRONT_PWM, &TIM_OCInitStructure );
	TIM_OC2PreloadConfig( TIMx_FRONT_PWM, TIM_OCPreload_Enable );  //使能预装载寄存器
	TIM_ARRPreloadConfig( TIMx_FRONT_PWM, ENABLE );                //使能自动重装载功能
	TIM_Cmd( TIMx_FRONT_PWM, ENABLE );                             //使能TIM9
}

/************基本定时器Tim3配置，定时处理角度传感器信息***********/
//定时器的优先级比任务高，在紧急情况下使用
//当转向达到最大值时，车子一定要停下来
/****************************************************************/
void Config_Tim3()
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3,ENABLE );
	
	TIM_TimeBaseInitStructure.TIM_Prescaler = 839;    //84MHz---->100KHz
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInitStructure.TIM_Period = 4999;     //   50ms 一次中断
	TIM_TimeBaseInit( TIM3, &TIM_TimeBaseInitStructure );
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);//使能定时器更新终端
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;//定时器3中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init( &NVIC_InitStructure );
	
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	TIM_ARRPreloadConfig(TIM3, DISABLE);    //关闭自动重装载功能
	TIM_Cmd(TIM3, ENABLE);                  //使能TIM3
}

void front_wheel_turn(unsigned char dir,unsigned short speed,float angel)
{
	/*0.合法性检查：如果车子已经在最左边，还要求继续往左，直接返回；
		如果车子已经在最右，还要求继续往右，直接返回*/
	float current_angel=get_front_angel();
	if(current_angel>Max_Angel&& dir ==DIR_LEFT)  return;
	if(current_angel<Min_Angel&& dir ==DIR_RIGHT) return;
	
	/*1.设置PWM 频率*/
	TIM9->ARR  = speed;
	/*2.设置占空比*/
	TIM9->CCR2 = (speed>>1);
	/*3.设置运动方向*/
	if(dir) 
		FRONT_DIR_LEFT();
	else 
		FRONT_DIR_RIGHT();
	/*4.打开驱动器的使能*/
	 FRONT_EN_ON();
	/*5.打开脉冲*/
	TIM9->CCMR1 |=0x6000;
	/*6.开启定时器3，做角度限位处理*/
	TIM3->CNT=0;
	TIM_Cmd(TIM3, ENABLE); 
	/*7.修改状态参数*/
	front_dir=dir;
}

/**************************前驱停止运行**********************/
void front_wheel_stop()
{
	/*0.关闭脉冲*/
	TIM9->CCMR1 &=0x8FFF;
	/*1.关闭使能*/
	FRONT_EN_OFF();
	/*2.关闭定时器3*/
	TIM_Cmd(TIM3, DISABLE); 
	/*3.修改状态参数*/
	front_dir=DIR_STOP;
}

void front_auto_mid()
{
	float angel=get_front_angel();
	if(fabs(angel-Mid_Angel)<Err_Angel) return;
	if(angel>0) front_wheel_turn(DIR_RIGHT,500,0);
	else front_wheel_turn(DIR_LEFT,500,0);
}

void TIM3_IRQHandler()
{
	static float current_angel=0;
	if( TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		current_angel = get_front_angel();
		if(current_angel>Max_Angel-Err_Angel&&front_dir==DIR_LEFT)
			front_wheel_stop();
		if(current_angel<Min_Angel+Err_Angel&&front_dir==DIR_RIGHT)
			front_wheel_stop();
		TIM3->SR = ~TIM_FLAG_Update;          //清中断
	}
}
