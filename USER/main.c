#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "bsp_gpo.h"
#include "bsp_sram.h"
#include "malloc.h"
#include "bsp_gpi.h"
#include "includes.h"
#include "mj_usart_pc.h" 
#include "bsp_usart_433.h"
#include "bsp_front_wheel.h"
#include "bsp_usart_us.h"
#include "task_rf433.h"
#include "task_pc_com.h"
#include "task_can_com.h"

/***************start task �������*****************/
#define START_STK_SIZE 		512                 //�����ջ��С	
OS_TCB StartTaskTCB;                          //������ƿ�
CPU_STK START_TASK_STK[START_STK_SIZE];       //�����ջ	
void start_task(void *p_arg);                 //������

OS_TMR tmr_light;

//������
int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	delay_init(168);       //ʱ�ӳ�ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�жϷ�������
	
	usart_pc_init(38400);
	usart_433_init(9600);

	LED_Init();                 //LED��ʼ��	
	FSMC_SRAM_Init();         	//��ʼ��SRAM
	my_mem_init(SRAMIN);        //��ʼ���ڲ�RAM
	Config_Tim3();
	OSInit(&err);		            //��ʼ��UCOSIII
	OS_CRITICAL_ENTER();	      //�����ٽ���			 
	//������ʼ����
	OSTaskCreate(&StartTaskTCB,"start task",start_task,0,START_TASK_PRIO,
                START_TASK_STK,START_STK_SIZE/10,START_STK_SIZE,0,0,0,OS_OPT_DEFAULT,&err);
	OS_CRITICAL_EXIT();	//�˳��ٽ���	 
	OSStart(&err);      //����UCOSIII
}

void light_blink(void *p_tmr,void *p_arg)
{
	static int cnt=0;
	cnt++;
	LED4=!LED4;
}


//��ʼ�������������ﴴ����������Ȼ��ɾ���Լ�
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	
	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//ͳ������                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		  //���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN     //��ʹ��ʱ��Ƭ��ת��ʱ��
	 //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif	
	OS_CRITICAL_ENTER();	            //�����ٽ���
	OSFlagCreate(&IRQ_EVENTs,"IRQ_EVENTs",0,&err);
	//����������
	OSTaskCreate(&Rf433TaskTCB,"Rf433TaskTCB",rf_433_task,0,RF433_COMRx_TASK_PRIO,
								RF_433_TASK_STK,RF433_STK_SIZE/10,RF433_STK_SIZE,0,0,0,OS_OPT_DEFAULT,&err);
								
	OSTaskCreate(&PcComRxTaskTCB,"PcComRxTaskTCB",pc_comRx_task,0,PC_COMRx_TASK_PRIO,
								PC_COMRx_TASK_STK,PC_COMRx_STK_SIZE/10,PC_COMRx_STK_SIZE,0,0,0,OS_OPT_DEFAULT,&err);	
								
	OSTaskCreate(&PcComTxTaskTCB,"PcComTxTaskTCB",pc_comTx_task,0,PC_COMTx_TASK_PRIO,
								PC_COMTx_TASK_STK,PC_COMTx_STK_SIZE/10,PC_COMTx_STK_SIZE,0,0,0,OS_OPT_DEFAULT,&err);		
	OSTaskCreate(&CanRx0TaskTCB,"CanRx0TaskTCB",task_canRx_FIFO0,0,CANRX_FIFO0_TASK_PRIO,
								CANRx0_TASK_STK,CANRx_STK_SIZE/10,CANRx_STK_SIZE,0,0,0,OS_OPT_DEFAULT,&err);		
	OSTaskCreate(&CanRx1TaskTCB,"CanRx1TaskTCB",task_canRx_FIFO1,0,CANRX_FIFO1_TASK_PRIO,
								CANRx1_TASK_STK,CANRx_STK_SIZE/10,CANRx_STK_SIZE,0,0,0,OS_OPT_DEFAULT,&err);		
							
	OSTmrCreate(&tmr_light,"tmr_light",0,50,OS_OPT_TMR_PERIODIC,light_blink,0,&err);
	OSTmrStart(&tmr_light,&err);
	OS_CRITICAL_EXIT();	               //�˳��ٽ���
	OSTaskDel((OS_TCB*)0,&err);	       //ɾ��start_task��������
}
