#include "delay.h"
#include "bsp_gpo.h"
#include "bsp_adc.h"
#include "bsp_can.h"
#include "bsp_rtc.h"
#include "malloc.h"
#include "bsp_gpi.h"
#include "includes.h"
#include "mj_usart_pc.h"
#include "bsp_usart_433.h"
#include "bsp_usart_pm.h"
#include "bsp_front_wheel.h"
#include "bsp_back_wheel.h"
#include "bsp_usart_us.h"
#include "task_rf433.h"
#include "task_pc_com.h"
#include "task_us_com.h"
#include "task_can_com.h"
#include "task_timer.h"
#include "task_adc_calc.h"
#include "task_di_sensor.h"
#include "task_pm_com.h"

/***************start task 相关配置*****************/
#define START_STK_SIZE 		256                 //任务堆栈大小	
OS_TCB StartTaskTCB;                          //任务控制块
CPU_STK START_TASK_STK[START_STK_SIZE];       //任务堆栈	
void start_task(void *p_arg);                 //任务函数

//主函数
int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	delay_init(168);                  //时钟初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断分组配置
	mj_delay_ms(100);
	LED_Init();                        //LED初始化，方便调试使用
	Config_Can();                      //Can通讯配置，MCU通过CAN和后驱伺服驱动器通讯
	servo_driver_init(0);              //后驱初始化，给后驱发送配置命令			
	Config_RTC();                      //RTC 配置，时间需要手动设置
	usart_pc_init(38400);              //与工控机通讯的串口配置
	usart_433_init(9600);              //与遥控器通讯的串口配置
	usart_us_head_init(9600);          //与超声波模块通讯的串口配置
	usart_pm_init(9600);               //pm传感器串口配置
	Config_GPIO_Control_Relay();       //继电器的配置，控制灯，喇叭等外设
	Config_GPIO_Input();
	Config_GPIO_EXTI_BUMPER();         //防撞条初始化
	my_mem_init(SRAMIN);               //初始化内部RAM
	Config_Control_Front_Wheel();      //前驱初始化
	Config_Tim3();                     //TIM3定时器中断，进行前驱角度监测防止前驱打死   
	Config_GPIO_Encoder_TRD_MA();      //前驱角度传感器配置
	Config_ADC1_DMA();                 //ADC采样配置，开启ADC1和DMA
	LED1=0;LED2=0;LED3=0;LED4=0;
	mj_delay_ms(100);
	LED1=1;LED2=1;LED3=1;LED4=1;
	OSInit(&err);		                   //初始化UCOSIII
	OS_CRITICAL_ENTER();	             //进入临界区			 
	//创建开始任务
	OSTaskCreate(&StartTaskTCB,"start task",start_task,0,START_TASK_PRIO,
                START_TASK_STK,START_STK_SIZE/10,START_STK_SIZE,0,0,0,OS_OPT_DEFAULT,&err);
	OS_CRITICAL_EXIT();	               //退出临界区	 
	OSStart(&err);                     //开启UCOSIII
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
	/*创建事件标志组，中断和任务的同步使用事件标志组*/
	OSFlagCreate(&IRQ_EVENTs,"IRQ_EVENTs",0,&err);
	/*遥控器的命令处理任务*/
	OSTaskCreate(&Rf433TaskTCB,"Rf433TaskTCB",rf_433_task,0,RF433_COMRx_TASK_PRIO,
								RF_433_TASK_STK,RF433_STK_SIZE/10,RF433_STK_SIZE,0,0,0,OS_OPT_DEFAULT,&err);
	/*工控机的命令处理任务*/					
	OSTaskCreate(&PcComRxTaskTCB,"PcComRxTaskTCB",pc_comRx_task,0,PC_COMRx_TASK_PRIO,
								PC_COMRx_TASK_STK,PC_COMRx_STK_SIZE/10,PC_COMRx_STK_SIZE,0,0,0,OS_OPT_DEFAULT,&err);	
	/*给工控机上报全状态信息的任务*/							
	OSTaskCreate(&PcComTxTaskTCB,"PcComTxTaskTCB",pc_comTx_task,0,PC_COMTx_TASK_PRIO,
								PC_COMTx_TASK_STK,PC_COMTx_STK_SIZE/10,PC_COMTx_STK_SIZE,0,0,0,OS_OPT_DEFAULT,&err);		
	/*Can数据接收任务，主要接收从驱动器得到的相关信息*/
	OSTaskCreate(&CanRx0TaskTCB,"CanRx0TaskTCB",task_canRx_FIFO0,0,CANRX_FIFO0_TASK_PRIO,
								CANRx0_TASK_STK,CANRx_STK_SIZE/10,CANRx_STK_SIZE,0,0,0,OS_OPT_DEFAULT,&err);		
	/*Can定时发送数据，定时查询驱动器的数据*/
	OSTaskCreate(&CanTxTaskTCB,"CanTxTaskTCB",task_canTx,0,CANTX_TASK_PRIO,
								CANTx_TASK_STK,CANRx_STK_SIZE/10,CANRx_STK_SIZE,0,0,0,OS_OPT_DEFAULT,&err);
	/*超声波串口接收任务，前超声波数据处理*/
	OSTaskCreate(&UsHeadTaskTCB,"UsHeadTaskTCB",us_Head_task,0,US_COMRx_HEAD_TASK_PRIO,
								US_HEAD_TASK_STK,US_COMRx_STK_SIZE/10,US_COMRx_STK_SIZE,0,0,0,OS_OPT_DEFAULT,&err);		
	/*超声波串口接收任务，后超声波数据处理*/
	OSTaskCreate(&UsTailTaskTCB,"UsTailTaskTCB",us_Tail_task,0,US_COMRx_HEAD_TASK_PRIO,
								US_TAIL_TASK_STK,US_COMRx_STK_SIZE/10,US_COMRx_STK_SIZE,0,0,0,OS_OPT_DEFAULT,&err);		
	/*Pm传感器数据处理任务*/
	OSTaskCreate(&PmComRxTaskTCB,"PmComRxTaskTCB",pm_comRx_task,0,PM_COMRx_TASK_PRIO,
								PM_COMRx_TASK_STK,PM_COMRx_STK_SIZE/10,PM_COMRx_STK_SIZE,0,0,0,OS_OPT_DEFAULT,&err);		
	/*数字量输入的传感器检测任务*/
	OSTaskCreate(&DITaskTCB,"DITaskTCB",task_di_sensor,0,DI_SENSOR_TASK_PRIO,
								DI_TASK_STK,DI_STK_SIZE/10,DI_STK_SIZE,0,0,0,OS_OPT_DEFAULT,&err);		
	/*Adc 采样数据的计算任务，排序，滤波等的处理*/
	OSTaskCreate(&AdcCalcTaskTCB,"AdcCalcTaskTCB",adc_calc_task,0,ADC_CALC_TASK_PRIO,
								ADC_CALC_TASK_STK,ADC_CALC_STK_SIZE/10,ADC_CALC_STK_SIZE,0,0,0,OS_OPT_DEFAULT,&err);		

	/*下面三个函数是状态等闪烁的控制，转向灯400ms执行一次，闪烁的周期800ms*/						
	OSTmrCreate(&TmrLeftLight,"TmrLeftLight",0,50,OS_OPT_TMR_PERIODIC,cb_TmrLeftLight,0,&err);
	OSTmrCreate(&TmrRightLight,"TmrRightLight",0,50,OS_OPT_TMR_PERIODIC,cb_TmrRightLight,0,&err);
	OSTmrCreate(&TmrFrontLight,"TmrFrontLight",0,50,OS_OPT_TMR_PERIODIC,cb_TmrFrontLight,0,&err);
	/*这个定时器记录各种控制的超时时间，50ms执行一次回调函数*/
	OSTmrCreate(&TmrTimeOutCheck,"TmrTimeOutCheck",0,5,OS_OPT_TMR_PERIODIC,cb_TmrTimeOutCheck,0,&err);
	/*根据前驱角度，让转向灯闪烁定时器*/
	OSTmrCreate(&TmrAngelMonitor,"TmrAngelMonitor",0,5,OS_OPT_TMR_PERIODIC,cb_TmrAngelMonitor,0,&err);

	OSTmrStart(&TmrTimeOutCheck,&err);  //启动超时检测任务
	OSTmrStart(&TmrAngelMonitor,&err);  //启动角度监测任务，准确的说是转向灯监测任务
	
	OS_CRITICAL_EXIT();	                //退出临界区
	elecmbile_status.RTOS_Running = 1;  //操作系统准备好,开启多任务运行
	OSTaskDel((OS_TCB*)0,&err);	        //删除start_task任务自身
}
