#include "mj_usart_pc.h" 
#include "string.h"  

static usart_pc_buffer tx_pc_buf;
static usart_pc_buffer rx_pc_buf;
static usart_pc_buffer prx_pc_buf;


void usart_pc_init(uint32_t baud) 
{
  USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	/*1.开启时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_DMA1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
 
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
  /*DMA1 ch4 strem0 和 stream7*/  
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream0_IRQn;// 接收
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream7_IRQn;// 发送
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/*引脚配置*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);

	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART5, &USART_InitStructure);
    
/***********************DMA接收程序************************/   
  DMA_DeInit(DMA1_Stream0);  
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART5->DR);  
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)rx_pc_buf.data;  
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
	  		
	DMA_Init(DMA1_Stream0, &DMA_InitStructure);    
	DMA_Cmd(DMA1_Stream0,ENABLE);  	
	
/***************************DMA发送************************/    
  DMA_DeInit(DMA1_Stream7);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART5->DR);  
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)tx_pc_buf.data;  
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
	  		
	DMA_Init(DMA1_Stream7, &DMA_InitStructure);    
	DMA_Cmd(DMA1_Stream7,ENABLE);  	
  
	DMA_ITConfig(DMA1_Stream7,DMA_IT_TC,ENABLE);
	
	USART_ITConfig(UART5,USART_IT_IDLE, ENABLE);
	USART_ITConfig(UART5,USART_IT_RXNE,DISABLE);  
	USART_ITConfig(UART5,USART_IT_TXE,DISABLE); 	
	
	USART_DMACmd(UART5,USART_DMAReq_Tx,ENABLE);  
  USART_DMACmd(UART5,USART_DMAReq_Rx,ENABLE); 
	USART_GetFlagStatus(UART5, USART_FLAG_TC);	
	USART_Cmd(UART5, ENABLE);
}


usart_pc_buffer *PcCom_get_data()
{
	return &prx_pc_buf;
}

volatile static char sendDoneFlag=1;
/******************************************************************
//将buffer内容通过DMA发送出去
//如果长度为0的话，会把buffer当做字符串发送，用strlen取其长度
//否则使用形参传递过来的长度
*******************************************************************/
void usart_pc_send(char* buffer,unsigned short length)
{
	if(length==0) length=strlen(buffer);
	while(0==sendDoneFlag);
	sendDoneFlag = 0;
	memcpy(tx_pc_buf.data,buffer,length);
	DMA1_Stream7->M0AR = (uint32_t)tx_pc_buf.data;
	DMA1_Stream7->NDTR = length;
	DMA_Cmd(DMA1_Stream7, ENABLE);
}


/****************************** 串口接收完成中断********************************/
void UART5_IRQHandler(void)
{
	OS_ERR err;
	OSIntEnter();    
	if(USART_GetITStatus(UART5, USART_IT_IDLE) != RESET)
	{
		USART_ClearITPendingBit(UART5, USART_IT_IDLE);//空闲
		USART_ReceiveData(UART5);                     //空读清状态
		DMA_Cmd(DMA1_Stream0,DISABLE);
		DMA_ClearFlag(DMA1_Stream0,DMA_FLAG_TCIF0);   //清除DMA标记位
		/*发送信号量给其他任务*/
		rx_pc_buf.len=PACKET_PC_MAX_LEN-DMA1_Stream0->NDTR;
		memcpy(&prx_pc_buf,&rx_pc_buf.data,PACKET_PC_MAX_LEN);
		memset(rx_pc_buf.data,0,PACKET_PC_MAX_LEN);
		OSFlagPost(&IRQ_EVENTs,EVENTS_PC_CMD,OS_OPT_POST_FLAG_SET,&err);	
		DMA1_Stream0->M0AR = (uint32_t)rx_pc_buf.data;
		DMA1_Stream0->NDTR=PACKET_PC_MAX_LEN;
		DMA_Cmd(DMA1_Stream0,ENABLE);                  //打开接收
	}
	OSIntExit();    	//退出中断
}

/****************************串口5DMA发送完成中断************************************/
void DMA1_Stream7_IRQHandler(void)
{
	OSIntEnter();  
	if(DMA_GetITStatus(DMA1_Stream7,DMA_IT_TCIF7)==SET)
	{
		DMA_Cmd(DMA1_Stream7,DISABLE); 
		DMA_ClearITPendingBit(DMA1_Stream7,DMA_IT_TCIF7);
		sendDoneFlag = 1;
	}
	OSIntExit();
}
