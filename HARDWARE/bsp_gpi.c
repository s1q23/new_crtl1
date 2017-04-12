#include "bsp_gpi.h"
#include "delay.h" 

//0-1数字量信号输入管脚配置，这里如果这种信号多的话可以考虑使用DMA
void Config_GPIO_Input()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//震动传感器 玻璃破碎传感器
	RCC_AHB1PeriphClockCmd( GPIO_SHAKE_CLOCK | GPIO_SLASS_CLOCK, ENABLE);
	//震动传感器管脚配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//根据传感器的输出类型而定
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = PIN_SHAKE;
	
	GPIO_Init(GPIO_SHAKE,&GPIO_InitStructure);
	//玻璃破碎传感器管脚配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//根据传感器的输出类型而定
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


/*获取前驱的角度信息*/
float get_front_angel()
{
	uint16_t f_encoder_gray,f_encoder_binary;
	float angel_value;
	/*0.读取IO的引脚原始信息*/
	f_encoder_gray = ~GPIO_ReadInputData( GPIOG );
	f_encoder_gray = f_encoder_gray & 0x1FF;
	/*1.将格雷码转换为BCD码*/
	f_encoder_binary = GraytoDecimal( (unsigned int )f_encoder_gray ) & 0x1FF;
	/*2.正负号处理*/
	if(f_encoder_binary >= 256)
		f_encoder_binary = f_encoder_binary - 512;
	/*3.得到角度信息*/
	angel_value=f_encoder_binary * RESOLUTION_512;
	return angel_value;
}

//外部中断管脚配置
void Config_GPIO_EXTI()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_AHB1PeriphClockCmd( GPIO_BUMPER_CLOCK | GPIO_SCRAM_BUTTON_CLOCK | GPIO_MECHANICAL_ZERO_CLOCK, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_SYSCFG, ENABLE);
	
	//防撞条外部中断管脚配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
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
}

void EXTI9_5_IRQHandler(void)  //防撞条中断
{
  if(SET == EXTI_GetITStatus(EXTI_LINE_BUMPER))
  {    
    EXTI_ClearFlag(EXTI_LINE_BUMPER);
    /*
			处理防撞条信息,直接写在这个函数里，不要另开函数
			Your Codes
		*/
    EXTI_ClearITPendingBit(EXTI_LINE_BUMPER);
  }  
}
