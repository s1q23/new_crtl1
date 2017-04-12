#include "bsp_front_wheel.h"
#include "bsp_gpi.h"
#include "bsp_gpo.h"

static char front_dir=DIR_STOP;

/*���ò��������ʹ�ܡ�������������*/
void Config_Control_Front_Wheel()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;	
	TIM_OCInitTypeDef TIM_OCInitStructure; 

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM9,ENABLE );
	//��������� ʹ�� ��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = PIN_FRONT_EN;
	GPIO_Init( GPIO_FRONT_EN, &GPIO_InitStructure );
	FRONT_EN_OFF();
	//��������� ������������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//��©+���� �͹���
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = PIN_FRONT_DIR;
	GPIO_Init( GPIO_FRONT_DIR, &GPIO_InitStructure );
	//��������� pwm��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//��©+���� �͹��� ��Ӱ��pwm���
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//��Ӱ��pwm���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Pin = PIN_FRONT_PWM;
	GPIO_Init( GPIO_FRONT_PWM, &GPIO_InitStructure );
	GPIO_PinAFConfig( GPIO_FRONT_PWM, PINSOURCE_FRONT_PWM, GPIO_AF_FRONT_PWM );

	/******************PWM ���Ƶ��  1KHz******************/
	TIM_DeInit( TIMx_FRONT_PWM );
	TIM_TimeBaseInitStructure.TIM_Prescaler = 167;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;   //�ظ�������ָ�ظ�������ٴβŽ��ж�
	TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;       //�Զ���װ��0-65536��ͬ����  void TIM_SetAutoreload(TIM_TypeDef* TIMx, uint32_t Autoreload)
	TIM_TimeBaseInit( TIMx_FRONT_PWM, &TIM_TimeBaseInitStructure );

	//TIM Output Compare Init
	TIM_OCStructInit( &TIM_OCInitStructure );
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 500;                           //�Ƚ�ֵ0-65536 ͬ ����void TIM_SetCompare2(TIM_TypeDef* TIMx, uint32_t Compare2)
	TIM_OC2Init( TIMx_FRONT_PWM, &TIM_OCInitStructure );
	TIM_OC2PreloadConfig( TIMx_FRONT_PWM, TIM_OCPreload_Enable );  //ʹ��Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig( TIMx_FRONT_PWM, ENABLE );                //ʹ���Զ���װ�ع���
	TIM_Cmd( TIMx_FRONT_PWM, ENABLE );                             //ʹ��TIM9
}

/************������ʱ��Tim3���ã���ʱ����Ƕȴ�������Ϣ***********/
//��ʱ�������ȼ�������ߣ��ڽ��������ʹ��
//��ת��ﵽ���ֵʱ������һ��Ҫͣ����
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
	TIM_TimeBaseInitStructure.TIM_Period = 4999;     //   50ms һ���ж�
	TIM_TimeBaseInit( TIM3, &TIM_TimeBaseInitStructure );
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);//ʹ�ܶ�ʱ�������ն�
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;//��ʱ��3�ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init( &NVIC_InitStructure );
	
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	TIM_ARRPreloadConfig(TIM3, DISABLE);    //�ر��Զ���װ�ع���
	TIM_Cmd(TIM3, ENABLE);                  //ʹ��TIM3
}

void front_wheel_turn(unsigned char dir,unsigned short speed,float angel)
{
	/*0.�Ϸ��Լ�飺��������Ѿ�������ߣ���Ҫ���������ֱ�ӷ��أ�
		��������Ѿ������ң���Ҫ��������ң�ֱ�ӷ���*/
	float current_angel=get_front_angel();
	if(current_angel>Max_Angel&& dir ==DIR_LEFT)  return;
	if(current_angel<Min_Angel&& dir ==DIR_RIGHT) return;
	
	/*1.����PWM Ƶ��*/
	TIM9->ARR  = speed;
	/*2.����ռ�ձ�*/
	TIM9->CCR2 = (speed>>1);
	/*3.�����˶�����*/
	if(dir) 
		FRONT_DIR_LEFT();
	else 
		FRONT_DIR_RIGHT();
	/*4.����������ʹ��*/
	 FRONT_EN_ON();
	/*5.������*/
	TIM9->CCMR1 |=0x6000;
	/*6.������ʱ��3�����Ƕ���λ����*/
	TIM3->CNT=0;
	TIM_Cmd(TIM3, ENABLE); 
	/*7.�޸�״̬����*/
	front_dir=dir;
}

/**************************ǰ��ֹͣ����**********************/
void front_wheel_stop()
{
	/*0.�ر�����*/
	TIM9->CCMR1 &=0x8FFF;
	/*1.�ر�ʹ��*/
	FRONT_EN_OFF();
	/*2.�رն�ʱ��3*/
	TIM_Cmd(TIM3, DISABLE); 
	/*3.�޸�״̬����*/
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
		TIM3->SR = ~TIM_FLAG_Update;          //���ж�
	}
}
