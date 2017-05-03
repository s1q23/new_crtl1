#include "task_pm_com.h"
#include "bsp_usart_pm.h"

OS_TCB  PmComRxTaskTCB;                        
CPU_STK PM_COMRx_TASK_STK[PM_COMRx_STK_SIZE];

pm_pack PmPack;

/************************************************************
关于返回值的解释：
返回0：解码正确
返回1：传入空指针
返回2：包头错误
返回3：指令号错误
返回4：校验错误
返回5：包尾错误
*************************************************************/
uint8_t pm_pack_decode(const unsigned char *in,pm_pack *out)
{
	unsigned char acc=0;
	if(in==NULL||out==NULL) return 1;
	if(in[0]!=0xAA) return 2;
	if(in[1]!=0xC0) return 3;
	out->pm25L=in[2];
	acc+=out->pm25L;
	
	out->pm25H=in[3];
	acc+=out->pm25H;
	
	out->pm10L=in[4];
	acc+=out->pm10L;
	
	out->pm10H=in[5];
	acc+=out->pm10H;
	
	out->ID1=in[6];
	acc+=out->ID1;
	
	out->ID2=in[7];
	acc+=out->ID2;
	
	if(acc!=in[8])  return 4;
	if(in[9]!=0xAB) return 5;
	
	return 0;
}

void pm_comRx_task(void *p_arg)
{
	static OS_ERR err;
	uint8_t ret=0;
	static usart_pm_buffer *p=NULL;
	while(1)
	{
		OSFlagPend(&IRQ_EVENTs,EVENTS_PM_DATA,0,OS_OPT_PEND_FLAG_SET_ALL|OS_OPT_PEND_FLAG_CONSUME,0,&err);
		p=usart_pm_get_data();
		ret=pm_pack_decode(p->data,&PmPack);
		memset(p->data,0,PACKET_PM_MAX_LEN);
		if(ret!=0) continue;
		rms_state.pm25=((u16)PmPack.pm25H<<8)+PmPack.pm25L;
		rms_state.pm10=((u16)PmPack.pm10H<<8)+PmPack.pm10L;
	}
}
