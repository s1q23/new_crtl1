#include "bsp_usart_us.h"
#include "string.h"  

static usart_us_buffer tx_us_head_buf;
static usart_us_buffer rx_us_head_buf;

static usart_us_buffer tx_us_tail_buf;
static usart_us_buffer rx_us_tail_buf;

void usart_us_head_init(uint32_t baud) 
{
  USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	/*1.����ʱ��*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_DMA1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
 
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
  /*DMA1 ch4 strem4 �� stream2*/  
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream2_IRQn;// ����
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream4_IRQn;// ����
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/*��������*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);

	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART4, &USART_InitStructure);
    
/***********************DMA���ճ���************************/   
  DMA_DeInit(DMA1_Stream2);  
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART4->DR);  
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)tx_us_head_buf.data;  
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;  
	DMA_InitStructure.DMA_BufferSize = 128;  
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;  
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;  
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;      
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;          
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;         
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;   		
	  		
	DMA_Init(DMA1_Stream2, &DMA_InitStructure);    
	DMA_Cmd(DMA1_Stream2,ENABLE);  	
	
/***************************DMA����************************/    
  DMA_DeInit(DMA1_Stream4);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART4->DR);  
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)rx_us_head_buf.data;  
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;  
	DMA_InitStructure.DMA_BufferSize = 128;  
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;  
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;  
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;      
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;          
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;         
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;   		
	
	DMA_Init(DMA1_Stream4, &DMA_InitStructure);    
	DMA_Cmd(DMA1_Stream4,ENABLE);  	
  
	DMA_ITConfig(DMA1_Stream4,DMA_IT_TC,ENABLE);
	
	USART_ITConfig(UART4,USART_IT_IDLE, ENABLE);
	USART_ITConfig(UART4,USART_IT_RXNE,DISABLE);  
	USART_ITConfig(UART4,USART_IT_TXE,DISABLE); 	
	
	USART_DMACmd(UART4,USART_DMAReq_Tx,ENABLE);  
  USART_DMACmd(UART4,USART_DMAReq_Rx,ENABLE); 
	USART_GetFlagStatus(UART4, USART_FLAG_TC);	
	USART_Cmd(UART4, ENABLE);
}

void usart_us_tail_init(uint32_t baud) 
{
  USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	/*1.����ʱ��*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_DMA1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
 
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
  /*DMA1 ch4 strem4 �� stream2*/  
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream5_IRQn;// ����
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;// ����
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/*��������*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
    
/***********************DMA���ճ���************************/   
  DMA_DeInit(DMA1_Stream5);  
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DR);  
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)tx_us_tail_buf.data;  
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;  
	DMA_InitStructure.DMA_BufferSize = 128;  
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;  
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;  
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;      
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;          
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;         
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;   		
	  		
	DMA_Init(DMA1_Stream5, &DMA_InitStructure);    
	DMA_Cmd(DMA1_Stream5,ENABLE);  	
	
/***************************DMA����************************/    
  DMA_DeInit(DMA1_Stream6);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DR);  
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)tx_us_tail_buf.data;  
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;  
	DMA_InitStructure.DMA_BufferSize = 128;  
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;  
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;  
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;      
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;          
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;         
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;   		
	  		
	DMA_Init(DMA1_Stream6, &DMA_InitStructure);    
	DMA_Cmd(DMA1_Stream6,ENABLE);  	
  
	DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);
	
	USART_ITConfig(USART2,USART_IT_IDLE, ENABLE);
	USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);  
	USART_ITConfig(USART2,USART_IT_TXE,DISABLE); 	
	
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);  
  USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE); 
	USART_GetFlagStatus(USART2, USART_FLAG_TC);	
	USART_Cmd(USART2, ENABLE);
}

volatile static char headsendDoneFlag=1;
void usart_us_head_send(char* buffer)
{
	short length=strlen(buffer);
	while(0==headsendDoneFlag);
	headsendDoneFlag = 0;
	memcpy(tx_us_head_buf.data,buffer,length);
	DMA1_Stream4->M0AR = (uint32_t)tx_us_head_buf.data;
	DMA1_Stream4->NDTR = length;
	DMA_Cmd(DMA1_Stream4, ENABLE);
}

volatile static char tailsendDoneFlag=1;
void usart_us_tail_send(char* buffer)
{
	short length=strlen(buffer);
	while(0==tailsendDoneFlag);
	tailsendDoneFlag = 0;
	memcpy(tx_us_tail_buf.data,buffer,length);
	DMA1_Stream6->M0AR = (uint32_t)tx_us_tail_buf.data;
	DMA1_Stream6->NDTR = length;
	DMA_Cmd(DMA1_Stream6, ENABLE);
}
/****************************** ���ڽ�������ж�********************************/
void UART4_IRQHandler(void)
{
	OSIntEnter();    
	if(USART_GetITStatus(UART4, USART_IT_IDLE) != RESET)
	{
		USART_ClearITPendingBit(UART4, USART_IT_IDLE);//����
		USART_ReceiveData(UART4);                     //�ն���״̬
		DMA_Cmd(DMA1_Stream2,DISABLE);
		rx_us_head_buf.len=PACKET_US_MAX_LEN-DMA1_Stream2->NDTR;
		/*�����ź�������������*/
		
//		osEventFlagsSet(IRQs,USART1_DONE);	
		DMA1_Stream2->M0AR = (uint32_t)rx_us_head_buf.data;
		DMA1_Stream2->NDTR=PACKET_US_MAX_LEN;
		DMA_Cmd(DMA1_Stream2,ENABLE);                  //�򿪽���
	}
	OSIntExit();    	//�˳��ж�
}

void USART2_IRQHandler(void)
{
	OSIntEnter();    
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
	{
		USART_ClearITPendingBit(USART2, USART_IT_IDLE);
		USART_ReceiveData(USART2);                     //�ն���״̬
		DMA_Cmd(DMA1_Stream5,DISABLE);
		rx_us_head_buf.len=PACKET_US_MAX_LEN-DMA1_Stream5->NDTR;
		/*�����ź�������������*/

//		osEventFlagsSet(IRQs,USART1_DONE);	
		DMA1_Stream5->M0AR = (uint32_t)rx_us_tail_buf.data;
		DMA1_Stream5->NDTR=PACKET_US_MAX_LEN;
		DMA_Cmd(DMA1_Stream5,ENABLE);                  //�򿪽���
	}
	OSIntExit();    	//�˳��ж�
}
/****************************����4DMA��������ж�************************************/
void DMA1_Stream4_IRQHandler(void)
{
	OSIntEnter();  
	if(DMA_GetITStatus(DMA1_Stream4,DMA_IT_TCIF4)==SET)
	{
		DMA_Cmd(DMA1_Stream4,DISABLE); 
		DMA_ClearITPendingBit(DMA1_Stream4,DMA_IT_TCIF4);
		headsendDoneFlag = 1;
	}
	OSIntExit();
}

void DMA1_Stream6_IRQHandler(void)
{
	OSIntEnter();  
	if(DMA_GetITStatus(DMA1_Stream6,DMA_IT_TCIF6)==SET)
	{
		DMA_Cmd(DMA1_Stream4,DISABLE); 
		DMA_ClearITPendingBit(DMA1_Stream6,DMA_IT_TCIF6);
		tailsendDoneFlag = 1;
	}
	OSIntExit();
}