#include "bsp_gpi.h"
#include "delay.h" 

//0-1�������ź�����ܽ����ã�������������źŶ�Ļ����Կ���ʹ��DMA
void Config_GPIO_Input()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//�𶯴����� �������鴫����
	RCC_AHB1PeriphClockCmd( GPIO_SHAKE_CLOCK | GPIO_SLASS_CLOCK, ENABLE);
	//�𶯴������ܽ�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//���ݴ�������������Ͷ���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = PIN_SHAKE;
	
	GPIO_Init(GPIO_SHAKE,&GPIO_InitStructure);
	//�������鴫�����ܽ�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//���ݴ�������������Ͷ���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = PIN_SLASS;
	
	GPIO_Init(GPIO_SLASS,&GPIO_InitStructure);
}

void Config_GPIO_Encoder_TRD_MA()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	//GPIOE:
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
}


/*��ȡǰ���ĽǶ���Ϣ*/
float get_front_angel()
{
	uint16_t f_encoder_gray,f_encoder_binary;
	float angel_value;
	/*0.��ȡIO������ԭʼ��Ϣ*/
	f_encoder_gray = ~GPIO_ReadInputData( GPIOG );
	f_encoder_gray = f_encoder_gray & 0x1FF;
	/*1.��������ת��ΪBCD��*/
	f_encoder_binary = GraytoDecimal( (unsigned int )f_encoder_gray ) & 0x1FF;
	/*2.�����Ŵ���*/
	if(f_encoder_binary >= 256)
		f_encoder_binary = f_encoder_binary - 512;
	/*3.�õ��Ƕ���Ϣ*/
	angel_value=f_encoder_binary * RESOLUTION_512;
	return angel_value;
}

//�ⲿ�жϹܽ�����
void Config_GPIO_EXTI()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_AHB1PeriphClockCmd( GPIO_BUMPER_CLOCK | GPIO_SCRAM_BUTTON_CLOCK | GPIO_MECHANICAL_ZERO_CLOCK, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_SYSCFG, ENABLE);
	
	//��ײ���ⲿ�жϹܽ�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
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
}

void EXTI9_5_IRQHandler(void)  //��ײ���ж�
{
  if(SET == EXTI_GetITStatus(EXTI_LINE_BUMPER))
  {    
    EXTI_ClearFlag(EXTI_LINE_BUMPER);
    /*
			�����ײ����Ϣ,ֱ��д������������Ҫ������
			Your Codes
		*/
    EXTI_ClearITPendingBit(EXTI_LINE_BUMPER);
  }  
}
