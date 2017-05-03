#include "bsp_gpi.h"
#include "bsp_gpo.h"
#include "delay.h"
#include "bsp_back_wheel.h"
#include "bsp_front_wheel.h"

//0-1�������ź�����ܽ����ã�������������źŶ�Ļ����Կ���ʹ��DMA
void Config_GPIO_Input()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//�𶯴����� �������鴫����
	RCC_AHB1PeriphClockCmd( GPIO_SHAKE_CLOCK|GPIO_GLASS_CLOCK|GPIO_SMOG_CLOCK|GPIO_EMGBTN_CLOCK|GPIO_WATER_CLOCK, ENABLE);
	//�𶯴������ܽ�����  PH6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//���ݴ�������������Ͷ���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = PIN_SHAKE;
	GPIO_Init(GPIO_SHAKE,&GPIO_InitStructure);

	//�������鴫�����ܽ����� PH8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//���ݴ�������������Ͷ���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = PIN_GLASS;
	GPIO_Init(GPIO_GLASS,&GPIO_InitStructure);
	
	//��������PD8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//���ݴ�������������Ͷ���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = PIN_SMOG;
	GPIO_Init(GPIO_SMOG,&GPIO_InitStructure);	
	//��ͣ��ť PD9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//���ݴ�������������Ͷ���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = PIN_EMGBTN;
	GPIO_Init(GPIO_SMOG,&GPIO_InitStructure);		
	//ˮλ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//���ݴ�������������Ͷ���
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

/*��ȡǰ���ĽǶ���Ϣ*/
float get_front_angel()
{
/********************************************************************
// �Ƕȴ�����ֱ�ӽ���MCU��,ʵ��ʹ��ʱʹ����ݴ���
********************************************************************/
#if 1	
	int16_t f_encoder_gray,f_encoder_binary;
	float angel_value;
	/*0.��ȡIO������ԭʼ��Ϣ*/
	f_encoder_gray = ~GPIO_ReadInputData(GPIOG);
	f_encoder_gray = f_encoder_gray&0x1FF;
	/*1.��������ת��ΪBCD��*/
	f_encoder_binary = GraytoDecimal((unsigned int)f_encoder_gray ) & 0x1FF;
	/*2.�����Ŵ���*/
	if(f_encoder_binary >= 256)
		f_encoder_binary = f_encoder_binary - 512;
	/*3.�õ��Ƕ���Ϣ*/
	angel_value=f_encoder_binary * RESOLUTION_512;
	return angel_value;
/*******************************************************************
�Ƕȴ�����������Сϵͳ���ϣ�ͨ��can���Ƕ���Ϣ���ݹ���������ʱ��ʱʹ��
ȫ�ֱ�����task_can_com.c�ļ��б��޸�	
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
	TIM_TimeBaseInitStructure.TIM_Period = 49;     //   0.5ms һ���ж�
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseInitStructure );
		
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;//��ʱ��3�ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure );
	
	TIM_ClearFlag(TIM7, TIM_FLAG_Update);
	TIM_ARRPreloadConfig(TIM7, DISABLE);             //�ر��Զ���װ�ع���
	TIM_Cmd(TIM7, DISABLE);                          //ʹ��TIM7
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);       //ʹ�ܶ�ʱ�������ն�	
}

//�ⲿ�жϹܽ����ã���ײ��
void Config_GPIO_EXTI_BUMPER()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_AHB1PeriphClockCmd( GPIO_BUMPER_CLOCK | GPIO_SCRAM_BUTTON_CLOCK | GPIO_MECHANICAL_ZERO_CLOCK, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_SYSCFG, ENABLE);
	
	//��ײ���ⲿ�жϹܽ�����PH7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;     //�ⲿ�жϺ���Ҫ���ó�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;   //��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = PIN_BUMPER;
	GPIO_Init(GPIO_BUMPER,&GPIO_InitStructure);
	//�ⲿ�ж�����
	SYSCFG_EXTILineConfig( EXTI_PORTSOURCE_BUMPER, EXTI_PINSOURCE_BUMPER );
	EXTI_ClearITPendingBit(EXTI_LINE_BUMPER);
	EXTI_InitStructure.EXTI_Line = EXTI_LINE_BUMPER;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_Init(&EXTI_InitStructure);
	//��ײ���ж�����
	NVIC_InitStructure.NVIC_IRQChannel = EXTI_BUMPER_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EXTI_BUMPER_PreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = EXTI_BUMPER_SubPriority;
	NVIC_Init(&NVIC_InitStructure);
	TIM7_init();
}

void EXTI9_5_IRQHandler(void)  //��ײ���ж�
{
	OSIntEnter();
  if(SET == EXTI_GetITStatus(EXTI_LINE_BUMPER))
  {    
		/*�ⲿ�жϱ������κ����飬���˲���������ʱ��*/
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
			/*0.����ֹͣ����*/
			back_wheel_set_speed(0);
			/*1.ǰ��ֹͣ*/
			front_wheel_stop();
			/*2.����С���������κ��˶����ƶ�������ִ��*/
			elecmbile_status.crash_strip=true;
			/*3.3���Ӧ�ý������*/
			Idle_Cnt_Crash=0;
			LED4=0;
		}
		if(uncrash_cnt)
		{
			TIM_Cmd(TIM7,DISABLE);
		}
		TIM7->SR = ~TIM_FLAG_Update;          //���ж�
	}	
	OSIntExit();
}
