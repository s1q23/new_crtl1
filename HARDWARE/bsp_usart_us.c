#include "bsp_usart_us.h"
#include "bsp_gpo.h"
#include "string.h"  

/*****************************************************
	超声波，串口4	
*****************************************************/

static usart_us_buffer tx_us_head_buf;
static usart_us_buffer rx_us_head_buf;
static usart_us_buffer prx_us_head_buf;
//static usart_us_buffer tx_us_tail_buf;
//static usart_us_buffer rx_us_tail_buf;
static usart_us_buffer prx_us_tail_buf;

usart_us_buffer * us_head_get_data()
{
	return &prx_us_head_buf;
}

usart_us_buffer * us_tail_get_data()
{
	return &prx_us_tail_buf;
}

void usart_us_head_init(uint32_t baud) 
{
  USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	DMA_InitTypeDef   DMA_InitStructure;
	
	/*1.开启时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_DMA1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
 
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
  /*DMA1 ch4 strem4 和 stream2*/  
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream2_IRQn;// 接收
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream4_IRQn;// 发送
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/*引脚配置*/
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
    
/***********************DMA接收程序************************/   
  DMA_DeInit(DMA1_Stream2);  
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART4->DR);  
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)rx_us_head_buf.data;  
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;  
	DMA_InitStructure.DMA_BufferSize = PACKET_US_MAX_LEN;  
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
	
/***************************DMA发送************************/    
  DMA_DeInit(DMA1_Stream4);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART4->DR);  
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)tx_us_head_buf.data;  
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;  
	DMA_InitStructure.DMA_BufferSize = PACKET_US_MAX_LEN;  
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
	DMA_Cmd(DMA1_Stream4,DISABLE);  	
  
	DMA_ITConfig(DMA1_Stream4,DMA_IT_TC,ENABLE);
	
	USART_ITConfig(UART4,USART_IT_IDLE, ENABLE);
	USART_ITConfig(UART4,USART_IT_RXNE,DISABLE);  
	USART_ITConfig(UART4,USART_IT_TXE,DISABLE); 	
	
	USART_DMACmd(UART4,USART_DMAReq_Tx,ENABLE);  
  USART_DMACmd(UART4,USART_DMAReq_Rx,ENABLE); 
	USART_GetFlagStatus(UART4, USART_FLAG_TC);	
	USART_Cmd(UART4, ENABLE);
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

/******************************串口接收完成中断********************************/
void UART4_IRQHandler(void)
{
	static OS_ERR err;
	OSIntEnter();    
	if(USART_GetITStatus(UART4, USART_IT_IDLE) != RESET)
	{
		USART_ClearITPendingBit(UART4, USART_IT_IDLE);//空闲
		USART_ReceiveData(UART4);                     //空读清状态
		DMA_Cmd(DMA1_Stream2,DISABLE);
		DMA_ClearFlag(DMA1_Stream2,DMA_FLAG_TCIF2);
		/*发送信号量给其他任务*/
//		rx_us_head_buf.len=PACKET_US_MAX_LEN-DMA1_Stream2->NDTR;
		if(rx_us_head_buf.data[2]==0x5A)
		{
			memcpy(prx_us_head_buf.data,rx_us_head_buf.data,PACKET_US_MAX_LEN);
			memset(&rx_us_head_buf,0,sizeof(rx_us_head_buf));		
			OSFlagPost(&IRQ_EVENTs,EVENTS_US_HEAD_DATA,OS_OPT_POST_FLAG_SET,&err);
		}
		else if(rx_us_head_buf.data[2]==0xA5)	
		{
			memcpy(prx_us_tail_buf.data,rx_us_head_buf.data,PACKET_US_MAX_LEN);
			memset(&rx_us_head_buf,0,sizeof(rx_us_head_buf));		
			OSFlagPost(&IRQ_EVENTs,EVENTS_US_TAIL_DATA,OS_OPT_POST_FLAG_SET,&err);
		}
		DMA1_Stream2->M0AR = (uint32_t)rx_us_head_buf.data;
		DMA1_Stream2->NDTR=PACKET_US_MAX_LEN;
		DMA_Cmd(DMA1_Stream2,ENABLE);                  //打开接收
	}
	OSIntExit();    	//退出中断
}

/****************************串口4DMA发送完成中断************************************/
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
