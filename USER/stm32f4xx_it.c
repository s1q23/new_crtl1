#include "stm32f4xx_it.h"
#include "bsp_gpo.h" 

static void mj_delay(unsigned int us)
{
	while(us--);
}

void NMI_Handler(void)
{
}

/*硬件错误中断发生时，会进入这个函数，LED4闪烁，此时程序已经崩溃*/
void HardFault_Handler(void)
{
  while (1)
  {
//		LED4=!LED4;
		mj_delay(0xFFFFFF);
  }
}

void MemManage_Handler(void)
{
  while (1)
  {
//		LED4=!LED4;
		mj_delay(0xFFFFFF);		
  }
}

void BusFault_Handler(void)
{
  while (1)
  {
//		LED4=!LED4;
		mj_delay(0xFFFFFF);
  }
}

void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
//		LED4=!LED4;
		mj_delay(0xFFFFFF);		
  }
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

