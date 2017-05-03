#include "bsp_usart_433.h"

static usart_433_buffer tx_433_buf;
static usart_433_buffer rx_433_buf;
static usart_433_buffer prx_433_buf;

//usart3 DMA收发   (DMA1-stream1-channel4,usart3-rx);(DMA1-stream3-channel4,usart3-tx);(PB10,usart3-tx);(PB11,usart3-rx)
void usart_433_init(uint32_t baud) 
{
  USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	/*1.开启时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_DMA1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
 
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
  /*DMA1 ch4 strem1(Rx) 和 stream3(Tx)*/  
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream1_IRQn;// 接收
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream3_IRQn;// 发送
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/*引脚配置*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
    
/***********************DMA接收程序************************/   
  DMA_DeInit(DMA1_Stream1);  
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART3->DR);  
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)rx_433_buf.data;  
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;  
	DMA_InitStructure.DMA_BufferSize = 20;  
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;  
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//DMA_Mode_Circular;// DMA_Mode_Normal;  
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;  
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;      
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;          
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;         
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;   		
	  		
	DMA_Init(DMA1_Stream1, &DMA_InitStructure);    
	DMA_Cmd(DMA1_Stream1,ENABLE);  	
	
/***************************DMA发送************************/    
  DMA_DeInit(DMA1_Stream3);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART3->DR);  
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&tx_433_buf.data[0];  
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
	  		
	DMA_Init(DMA1_Stream3, &DMA_InitStructure);    
	DMA_Cmd(DMA1_Stream3,DISABLE);  	
  
	DMA_ITConfig(DMA1_Stream3,DMA_IT_TC,ENABLE);
	
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
	USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);  
	USART_ITConfig(USART3,USART_IT_TXE,DISABLE); 	
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);  
  USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE); 
	
	USART_GetFlagStatus(USART3, USART_FLAG_TC);	
	USART_Cmd(USART3, ENABLE);
}

usart_433_buffer * Rf433_get_data()
{
	return &prx_433_buf;
}

volatile static char sendDoneFlag=1;
void usart_433_send_str(char* buffer)
{
	short length=strlen(buffer);
	while(0==sendDoneFlag);
	sendDoneFlag = 0;
	memcpy(tx_433_buf.data,buffer,length);
	DMA1_Stream3->M0AR = (uint32_t)tx_433_buf.data;
	DMA1_Stream3->NDTR = length;
	DMA_Cmd(DMA1_Stream3, ENABLE);
}

void usart_433_send(char *str)
{
	short len=strlen(str);
	int i=0;
	for(i=0;i<len;i++)
	{
		while((USART3->SR&0X40)==0);
		USART3->DR=str[i];
	}
}

/****************************** 串口接收完成中断********************************/
void USART3_IRQHandler(void)
{
	static OS_ERR err;
	OSIntEnter();    
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
	{
		USART_ClearITPendingBit(USART3, USART_IT_IDLE);//空闲
		USART_ReceiveData(USART3);                     //读清状态
		DMA_Cmd(DMA1_Stream1,DISABLE);
		DMA_ClearFlag(DMA1_Stream1,DMA_FLAG_TCIF1);    //这句有用吗？ 有！
		/*发送信号量给其他任务*/
		rx_433_buf.len=PACKET_433_MAX_LEN-DMA1_Stream1->NDTR;
		memcpy(prx_433_buf.data,rx_433_buf.data,PACKET_433_MAX_LEN);
		OSFlagPost(&IRQ_EVENTs,EVENTS_RF433_CMD,OS_OPT_POST_FLAG_SET,&err);	
		memset(&rx_433_buf,0,sizeof(rx_433_buf));
		DMA1_Stream1->M0AR = (uint32_t)rx_433_buf.data;
		DMA1_Stream1->NDTR=PACKET_433_MAX_LEN;
		DMA_Cmd(DMA1_Stream1,ENABLE);                  //打开接收
	}
	OSIntExit();    	//退出中断
}

/****************************串口5DMA发送完成中断************************************/
void DMA1_Stream3_IRQHandler(void)
{
	OSIntEnter();  
	if(DMA_GetITStatus(DMA1_Stream3,DMA_IT_TCIF3)==SET)
	{
		DMA_Cmd(DMA1_Stream3,DISABLE); 
		DMA_ClearITPendingBit(DMA1_Stream3,DMA_IT_TCIF3);
		sendDoneFlag = 1;
	}
	OSIntExit();
}
