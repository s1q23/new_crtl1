#include "bsp_usart_pm.h"

static usart_pm_buffer tx_pm_buf;
static usart_pm_buffer rx_pm_buf;
static usart_pm_buffer prx_pm_buf;

usart_pm_buffer * usart_pm_get_data()
{
	return &prx_pm_buf;
}

void usart_pm_init(uint32_t baud)
{
  USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	/*1.开启时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_DMA1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
 
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
  /*DMA1 ch4 strem4 和 stream2*/  
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream5_IRQn;// 接收
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;// 发送
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/*引脚配置*/
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
    
/***********************DMA接收程序************************/   
  DMA_DeInit(DMA1_Stream5);  
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DR);  
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)rx_pm_buf.data;  
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
	
/***************************DMA发送************************/    
  DMA_DeInit(DMA1_Stream6);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DR);  
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)tx_pm_buf.data;  
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

volatile static char PmSendDoneFlag=1;
void usart_pm_send(char* buffer)
{
	short length=strlen(buffer);
	while(0==PmSendDoneFlag);
	PmSendDoneFlag = 0;
	memcpy(tx_pm_buf.data,buffer,length);
	DMA1_Stream6->M0AR = (uint32_t)tx_pm_buf.data;
	DMA1_Stream6->NDTR = length;
	DMA_Cmd(DMA1_Stream6, ENABLE);
}

/*********************串口接收完成中断*******************/
void USART2_IRQHandler(void)
{
	static OS_ERR err;
	OSIntEnter();    
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
	{
		USART_ClearITPendingBit(USART2, USART_IT_IDLE);
		USART_ReceiveData(USART2);                     //空读清状态
		DMA_Cmd(DMA1_Stream5,DISABLE);
		DMA_ClearFlag(DMA1_Stream5,DMA_FLAG_TCIF5);
//		rx_pm_buf.len=PACKET_PM_MAX_LEN-DMA1_Stream5->NDTR;
		/*发送信号量给其他任务*/
		memcpy(prx_pm_buf.data,rx_pm_buf.data,PACKET_PM_MAX_LEN);
		memset(rx_pm_buf.data,0,PACKET_PM_MAX_LEN);
		OSFlagPost(&IRQ_EVENTs,EVENTS_PM_DATA,OS_OPT_POST_FLAG_SET,&err);
		DMA1_Stream5->M0AR = (uint32_t)rx_pm_buf.data;
		DMA1_Stream5->NDTR=PACKET_PM_MAX_LEN;
		DMA_Cmd(DMA1_Stream5,ENABLE);                  //打开接收
	}
	OSIntExit();    	//退出中断
}

/*********************串口发送完成中断***********************/
void DMA1_Stream6_IRQHandler(void)
{
	OSIntEnter();  
	if(DMA_GetITStatus(DMA1_Stream6,DMA_IT_TCIF6)==SET)
	{
		DMA_Cmd(DMA1_Stream4,DISABLE); 
		DMA_ClearITPendingBit(DMA1_Stream6,DMA_IT_TCIF6);
		PmSendDoneFlag = 1;
	}
	OSIntExit();
}
