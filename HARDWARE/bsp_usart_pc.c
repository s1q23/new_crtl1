#include "bsp_usart_pc.h"
#include "com_libs.h"
#include "string.h"

//usart5 DMA收发   (DMA1-stream0-channel4,usart5-rx);(DMA1-stream7-channel4,usart5-tx);(PC12,usart5-tx);(PD2,usart5-rx)

volatile uint8_t usart_IPC_receive_buff[SIZE_OF_DATA_IPC_R] = {0};
volatile uint8_t usart_IPC_transmit_buff[SIZE_OF_DATA_IPC_T+6] = {0};

static s_uart_config uart_IPC = {3,UART5,115200,GPIOC,GPIOD,GPIO_Pin_12,GPIO_Pin_2,GPIO_PinSource12,GPIO_PinSource2\
																	,GPIO_AF_UART5,RCC_APB1Periph_UART5,RCC_AHB1Periph_GPIOC,RCC_AHB1Periph_GPIOD\
																	,RCC_AHB1Periph_DMA1,DMA1_Stream7,DMA1_Stream0,DMA_Channel_4,DMA_Channel_4,SIZE_OF_DATA_IPC_T,SIZE_OF_DATA_IPC_R\
																	,(uint32_t)&usart_IPC_transmit_buff[0],(uint32_t)&usart_IPC_receive_buff[0]};

void bsp_usart_pc_init()
{
	Config_UART(uart_IPC);
}

//一次IPC_DMA传输,全状态上报的时间为1.7~2ms
void IPC_Uart_DMA_Tx_OneTime(u8 f_data_size)
{ 
	int f_i;
	usart_IPC_transmit_buff[0] = 0xAD;
	usart_IPC_transmit_buff[1] = 20;
	usart_IPC_transmit_buff[2] = 0;
	usart_IPC_transmit_buff[3] = 0;
	usart_IPC_transmit_buff[22] = 0xED;
	usart_IPC_transmit_buff[23] = 0;
	memcpy((u8 *)&usart_IPC_transmit_buff[4],(const robot_info_t *)&robot_info,SIZE_OF_DATA_IPC_T);
	
	for(f_i = 0;f_i < 23;f_i++)
		usart_IPC_transmit_buff[23] += usart_IPC_transmit_buff[f_i];
	
	DMA_SetCurrDataCounter( uart_IPC.dma_stream_utx, f_data_size );//要在启动之前进行设置，设置之后，dma传输容量为f_data_size
	
	DMA_Cmd(uart_IPC.dma_stream_utx,ENABLE);
	//等待发送完成？
	while( DMA_GetFlagStatus(uart_IPC.dma_stream_utx, DMA_FLAG_TCIF7) == RESET );
	
	DMA_ClearFlag(uart_IPC.dma_stream_utx, DMA_FLAG_TCIF7);//需要手动清除
	
	DMA_Cmd(uart_IPC.dma_stream_utx,DISABLE );	
}	

void IPC_Uart_DMA_Tx_data(u8 f_data_size,u8 *f_pdata)
{ 
	int f_i;
	usart_IPC_transmit_buff[0] = 0xAD;
	usart_IPC_transmit_buff[1] = f_data_size;
	usart_IPC_transmit_buff[f_data_size+2] = 0xED;
	usart_IPC_transmit_buff[f_data_size+3] = 0;
	memcpy((u8 *)&usart_IPC_transmit_buff[2],(const u8 *)f_pdata,f_data_size);
	
	for(f_i = 0;f_i < f_data_size+3;f_i++)
		usart_IPC_transmit_buff[f_data_size+3] += usart_IPC_transmit_buff[f_i];
	
	DMA_SetCurrDataCounter( uart_IPC.dma_stream_utx, f_data_size+4 );//要在启动之前进行设置，设置之后，dma传输容量为f_data_size+4
	
	DMA_Cmd(uart_IPC.dma_stream_utx,ENABLE);

	while( DMA_GetFlagStatus(uart_IPC.dma_stream_utx, DMA_FLAG_TCIF7) == RESET );
	
	DMA_ClearFlag(uart_IPC.dma_stream_utx, DMA_FLAG_TCIF7);//需要手动清除
	
	DMA_Cmd(uart_IPC.dma_stream_utx,DISABLE );	
}	
