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

/***************start task 相关配置*****************/
#define START_STK_SIZE 		512                 //任务堆栈大小	
OS_TCB StartTaskTCB;                          //任务控制块
CPU_STK START_TASK_STK[START_STK_SIZE];       //任务堆栈	
void start_task(void *p_arg);                 //任务函数

OS_TMR tmr_light;

//主函数
int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	delay_init(168);       //时钟初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断分组配置
	
	usart_pc_init(38400);
	usart_433_init(9600);

	LED_Init();                 //LED初始化	
	FSMC_SRAM_Init();         	//初始化SRAM
	my_mem_init(SRAMIN);        //初始化内部RAM
	Config_Tim3();
	OSInit(&err);		            //初始化UCOSIII
	OS_CRITICAL_ENTER();	      //进入临界区			 
	//创建开始任务
	OSTaskCreate(&StartTaskTCB,"start task",start_task,0,START_TASK_PRIO,
                START_TASK_STK,START_STK_SIZE/10,START_STK_SIZE,0,0,0,OS_OPT_DEFAULT,&err);
	OS_CRITICAL_EXIT();	//退出临界区	 
	OSStart(&err);      //开启UCOSIII
}

void light_blink(void *p_tmr,void *p_arg)
{
	static int cnt=0;
	cnt++;
	LED4=!LED4;
}


//开始任务函数，在这里创建其他任务，然后删除自己
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	
	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		  //如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN     //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif	
	OS_CRITICAL_ENTER();	            //进入临界区
	OSFlagCreate(&IRQ_EVENTs,"IRQ_EVENTs",0,&err);
	//创建主任务
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
	OS_CRITICAL_EXIT();	               //退出临界区
	OSTaskDel((OS_TCB*)0,&err);	       //删除start_task任务自身
}
