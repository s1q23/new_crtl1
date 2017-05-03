#include "bsp_adc.h"

uint16_t adc_buff[SIZE_OF_FILTER][NUMBER_OF_ADC];//ǰ��adc����   ����ȡ������
volatile adc_value_t adc_value = {0}; 

//ADC1 DMA   DMA2-stream0-channel0
void Config_ADC1_DMA()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2|RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
		//DMAͨ������
	DMA_DeInit(DMA2_Stream0);	
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	//����DMA�ڴ���ʱ�������ĳ��� 
	DMA_InitStructure.DMA_BufferSize = NUMBER_OF_ADC * SIZE_OF_FILTER;
	//dma���䷽��
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	
	//�ڴ��ַ 
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)adc_buff;
	 //�����ַ  
	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)(&ADC1->DR);//(uint32_t)(USART2_BASE + 0x04);//���ַ�ʽ������
	//����DMA���ڴ����ģʽ 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//����DMA���������ģʽ��һ������
	DMA_InitStructure.DMA_PeripheralInc = DMA_MemoryInc_Disable;
	//�ڴ������ֳ� 
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;
	//���������ֳ�
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	//����DMA�Ĵ���ģʽ 
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//DMA_Mode_Normal  DMA_Mode_Circular
	
	
	//����DMA�����ȼ���
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	//ָ�����FIFOģʽ��ֱ��ģʽ������ָ������ �� ��ʹ��FIFOģʽ    
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;      
	//ָ����FIFO��ֵˮƽ  
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;          
	//ָ����Burstת�������ڴ洫��   
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;         
	//ָ����Burstת��������Χת�� */    
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; 
	
	DMA_Init(DMA2_Stream0,&DMA_InitStructure);
	
	//DMA2 Stream 0 global Interrupt 
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	DMA_ITConfig( DMA2_Stream0, DMA_IT_TC, ENABLE );
	
	DMA_Cmd(DMA2_Stream0,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//GPIO Analog Mode
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//GPIO Analog Mode
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//GPIO Analog Mode
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	 /* ADC Common Init **********************************************************/
	//��adc��ת��ʱ�����������������ʹ������ͨ������µ�ת�����ȸ��ߣ�̫���ʹǰ���ת��Ӱ�쵽����ģ�Ŀǰת��������һ��ѭ����6ms
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//ֻ��һ��adc
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//�ڶ�adcģʽ�»��õ�ʹ�����ģʽ
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;  	//����Ϊ12λ           
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;              //ɨ��ת��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  	    //����ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //�����ⲿ�������������ת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //�����Ҷ��룬���ֽڶ���
	ADC_InitStructure.ADC_NbrOfConversion = NUMBER_OF_ADC;    	//�涨��˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_480Cycles);//ADC,ͨ��0ת��,��ѡͨ���Ĳ�������
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_480Cycles);//ADC,ͨ��1ת��,��ѡͨ���Ĳ�������
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 3, ADC_SampleTime_480Cycles);//ADC,ͨ��4ת��,��ѡͨ���Ĳ�������
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 4, ADC_SampleTime_480Cycles);//ADC,ͨ��5ת��,��ѡͨ���Ĳ�������
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 5, ADC_SampleTime_480Cycles);//ADC,ͨ��6ת��,��ѡͨ���Ĳ�������
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 6, ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 7, ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 8, ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 9, ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 10, ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 11, ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 12, ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 13, ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_17, 14, ADC_SampleTime_480Cycles);//ADC,ͨ��17ת��,��ѡͨ���Ĳ�������Vrefint 
	
	ADC_TempSensorVrefintCmd( ENABLE );                                          //�ڲ��ο���ѹ���� 
	ADC_DMARequestAfterLastTransferCmd( ADC1, ENABLE );//�����������ֻ����һ��DMA�ж�
	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	/* Start ADC1 Software Conversion */ 
	ADC_SoftwareStartConv(ADC1);   //��������ⲿ��������������ʼת��
}

// adc filter,��λֵƽ���˲�
void DMA2_Stream0_IRQHandler_ADC1() //�洢��ʱ����������  a[0][1]   a[0][2]    a[0][3]
{
	u32 adc_no, i, sum;
//	u16 tmp;u32 j;

	for ( adc_no = 0; adc_no < NUMBER_OF_ADC; adc_no++) 
	{
	/*������ȥ����������ݵĶ����ϴ��ټ������򣬽�ʡ������ʱ��*/	
//		for (i = 0; i < SIZE_OF_FILTER; i++) 
//		{
//			for ( j = 0; j < SIZE_OF_FILTER - i ; j++) 
//			{
//				if ( adc_buff[j][adc_no] > adc_buff[j+1][adc_no]) 
//				{
//					tmp = adc_buff[j][adc_no];
//					adc_buff[j][adc_no] = adc_buff[j+1][adc_no];
//					adc_buff[j+1][adc_no] = tmp;
//				}
//			}
//		}
		// ��ƽ��ֵ
		for ( i = 0, sum = 0 ; i < SIZE_OF_FILTER; i++) {  //ȥ���м�����
			sum += adc_buff[i][adc_no];
		}
		sum = sum / (SIZE_OF_FILTER);//ȡ�м�İ˸���ֵ��ȥ��ͷβ
		if (adc_no == 0)
			adc_value.M = sum;
		else if (adc_no == 1)
			adc_value.L1 = sum;
		else if (adc_no == 2)
			adc_value.L2 = sum;
		else if (adc_no == 3)
			adc_value.R1 = sum;
		else if (adc_no == 4)
			adc_value.R2 = sum;
		else if (adc_no == 5)
			adc_value.CO = sum;
		else if (adc_no == 6)
			adc_value.BAT_V = sum;
		else if (adc_no == 7)
			adc_value.TEMP = sum;
		else if (adc_no == 8)
			adc_value.HUMI = sum;
		else if (adc_no == 9)
			adc_value.BAT_C = sum;
		else if (adc_no == 10)
			adc_value.ANGLE = sum;
		else if (adc_no == 11)
			adc_value.WATER_LEVEL = sum;
		else if (adc_no == 12)
			adc_value.FOG = sum;
		else if (adc_no == 13)
			adc_value.vref = sum;
	}
}

void DMA2_Stream0_IRQHandler(void)  //adc  dma������ƽ��ֵ�˲�
{
	static OS_ERR err;
	OSIntEnter(); 
	if(DMA_GetITStatus( DMA2_Stream0, DMA_IT_TCIF0 ) != RESET) 
	{
		OSFlagPost(&IRQ_EVENTs,EVENTS_DMA_CVTOK,OS_OPT_POST_FLAG_SET,&err);	
		DMA_ClearITPendingBit( DMA2_Stream0, DMA_IT_TCIF0 );
	}
	OSIntExit(); 
}
