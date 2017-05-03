#include "task_us_com.h"
#include "bsp_usart_us.h"
#include "bsp_gpo.h"
#include "bsp_gpi.h"
#include "bsp_back_wheel.h"
#include "bsp_front_wheel.h"

OS_TCB  UsHeadTaskTCB;                
CPU_STK US_HEAD_TASK_STK[US_COMRx_STK_SIZE];
OS_TCB  UsTailTaskTCB;                
CPU_STK US_TAIL_TASK_STK[US_COMRx_STK_SIZE];

/********************************************************
 *关于返回值的解释：
 *返回 0：正确
 *返回 1：传入NULL指针
 *返回 2：第一个包头错误
 *返回 3：第二个包头错误
 *返回 4：方向信息非法
 *返回 5：数据包校验错误
**********************************************************/
static pack_utlr head_utlr;
static pack_utlr tail_utlr;

unsigned char us_pack_decode(const unsigned char *in_str,pack_utlr *out_pack)
{
	unsigned char bcc=0;
	/*1.参数合法性检查*/
	if(in_str==NULL||out_pack==NULL) return 1;
	/*2.包头的检验*/
	if(in_str[0]!=0xAA) return 2;
	if(in_str[1]!=0x55) return 3;
	if((in_str[2]!=0x5A)&&(in_str[2]!=0xA5)) return 4;
	out_pack->dir=in_str[2];
	bcc+=in_str[2];
	if(out_pack->dir==0x5A)   //前面的超声波
	{
		out_pack->dist_A=in_str[3];
		bcc+=out_pack->dist_A;
		out_pack->dist_B=in_str[4];
		bcc+=out_pack->dist_B;
		out_pack->dist_C=in_str[5];
		bcc+=out_pack->dist_C;
		out_pack->dist_D=in_str[6];
		bcc+=out_pack->dist_D;
	}
	else                      //后面的超声波
	{
		out_pack->dist_E=in_str[3];
		bcc+=out_pack->dist_E;
		out_pack->dist_F=in_str[4];
		bcc+=out_pack->dist_F;
		out_pack->dist_G=in_str[5];
		bcc+=out_pack->dist_G;
		out_pack->dist_H=in_str[6];
		bcc+=out_pack->dist_H;
	}	
	if((bcc&0xFF)!=in_str[7])	return 5;
	return 0;
}
void us_Head_task(void *p_arg)
{
	static OS_ERR err;
	unsigned short decode_ret=0;
	unsigned int usH_cnt=0;
	static usart_us_buffer *p=NULL;
	while(1)
	{
		OSFlagPend(&IRQ_EVENTs,EVENTS_US_HEAD_DATA,0,OS_OPT_PEND_FLAG_SET_ALL|OS_OPT_PEND_FLAG_CONSUME,0,&err);	
		p=us_head_get_data();
		decode_ret=us_pack_decode(p->data,&head_utlr);
		if(decode_ret!=0) continue;
		if(head_utlr.dist_A<US_LIMIT_DISTENCE_CM||head_utlr.dist_B<US_LIMIT_DISTENCE_CM||head_utlr.dist_C<US_LIMIT_DISTENCE_CM||head_utlr.dist_D<US_LIMIT_DISTENCE_CM)
		{
			usH_cnt++;
		}
		else
		{
			elecmbile_status.us_head_status=0;
			usH_cnt=0;
		}
		if(usH_cnt>=3)
		{
			elecmbile_status.us_head_status=1;
			LED1=!LED1;
		}
		if(elecmbile_status.us_enable==0) continue;
		else if(elecmbile_status.us_head_status&&back_dir==DIR_FRONT)
		{
			back_wheel_set_speed(0);
		}
	}
}

void us_Tail_task(void *p_arg)
{
	static OS_ERR err;
	unsigned char decode_ret=0;
	unsigned int usT_cnt=0;
	static usart_us_buffer *p=NULL;
	while(1)
	{
		OSFlagPend(&IRQ_EVENTs,EVENTS_US_TAIL_DATA,0,OS_OPT_PEND_FLAG_SET_ALL|OS_OPT_PEND_FLAG_CONSUME,0,&err);	
		p=us_tail_get_data();
		decode_ret=us_pack_decode(p->data,&tail_utlr);
		if(decode_ret!=0) continue;
		if(tail_utlr.dist_E<US_LIMIT_DISTENCE_CM||tail_utlr.dist_F<US_LIMIT_DISTENCE_CM||tail_utlr.dist_G<US_LIMIT_DISTENCE_CM||tail_utlr.dist_H<US_LIMIT_DISTENCE_CM)
		{
			usT_cnt++;
		}
		else
		{
			usT_cnt=0;
			elecmbile_status.us_tail_status=0;
		}
		if(usT_cnt>=3) 
		{
			elecmbile_status.us_tail_status=1;
			LED2=!LED2;
		}
		if(elecmbile_status.us_enable==0) continue;
		else if(elecmbile_status.us_tail_status&&back_dir==DIR_BACK)
		{
			back_wheel_set_speed(0);
		}
	}
}
