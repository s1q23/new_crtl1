#include "stm32f4xx.h"                  // Device header
#include "mj_lib.h"

/******************系统用到的全局变量定义****************/
OS_FLAG_GRP          IRQ_EVENTs;
mj_status elecmbile_status;
float dest_angel = 0.0f;
unsigned short Idle_Cnt_Stop=0;
unsigned short Idle_Cnt_Rf433_Cmd =0;
unsigned short Idle_Cnt_Pc_Cmd =0;

/**********************软件复位函数**********************/
void mj_reset()
{
	__set_FAULTMASK(1);      // 关闭所有中端
	NVIC_SystemReset();      // 复位
}

/*****************格雷码转BCD码**********************/
unsigned int GraytoDecimal(unsigned int x)
{  
	int i;
	for(i=0;(1<<i)<sizeof(x)*8;i++) 
	{  
		x^=x>>(1<<i);
	}  
	return x; 
}
//usart1 DMA收发   (DMA2-stream5-channel4,usart1-rx);(DMA2-stream7-channel4,usart1-tx);(PA9,usart1-tx);(PA10,usart1-rx)
//usart2 DMA收发   (DMA1-stream5-channel4,usart2-rx);(DMA1-stream6-channel4,usart2-tx);(PA2,usart2-tx);(PA3,usart2-rx)
//usart3 DMA收发   (DMA1-stream1-channel4,usart3-rx);(DMA1-stream3-channel4,usart3-tx);(PB10,usart3-tx);(PB11,usart3-rx)
//usart4 DMA收发   (DMA1-stream2-channel4,usart4-rx);(DMA1-stream4-channel4,usart4-tx);(PC10,usart4-tx);(PC11,usart4-rx)
//usart5 DMA收发   (DMA1-stream0-channel4,usart5-rx);(DMA1-stream7-channel4,usart5-tx);(PC12,usart5-tx);(PD2,usart5-rx)
//usart6 DMA收发   (DMA2-stream2-channel5,usart6-rx);(DMA2-stream6-channel5,usart6-tx);(PC6,usart6-tx);(PC7,usart6-rx)
