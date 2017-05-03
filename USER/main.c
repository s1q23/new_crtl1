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

/***************start task �������*****************/
#define START_STK_SIZE 		256                 //�����ջ��С	
OS_TCB StartTaskTCB;                          //������ƿ�
CPU_STK START_TASK_STK[START_STK_SIZE];       //�����ջ	
void start_task(void *p_arg);                 //������

//������
int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	delay_init(168);                  //ʱ�ӳ�ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�жϷ�������
	mj_delay_ms(100);
	LED_Init();                        //LED��ʼ�����������ʹ��
	Config_Can();                      //CanͨѶ���ã�MCUͨ��CAN�ͺ����ŷ�������ͨѶ
	servo_driver_init(0);              //������ʼ����������������������			
	Config_RTC();                      //RTC ���ã�ʱ����Ҫ�ֶ�����
	usart_pc_init(38400);              //�빤�ػ�ͨѶ�Ĵ�������
	usart_433_init(9600);              //��ң����ͨѶ�Ĵ�������
	usart_us_head_init(9600);          //�볬����ģ��ͨѶ�Ĵ�������
	usart_pm_init(9600);               //pm��������������
	Config_GPIO_Control_Relay();       //�̵��������ã����Ƶƣ����ȵ�����
	Config_GPIO_Input();
	Config_GPIO_EXTI_BUMPER();         //��ײ����ʼ��
	my_mem_init(SRAMIN);               //��ʼ���ڲ�RAM
	Config_Control_Front_Wheel();      //ǰ����ʼ��
	Config_Tim3();                     //TIM3��ʱ���жϣ�����ǰ���Ƕȼ���ֹǰ������   
	Config_GPIO_Encoder_TRD_MA();      //ǰ���Ƕȴ���������
	Config_ADC1_DMA();                 //ADC�������ã�����ADC1��DMA
	LED1=0;LED2=0;LED3=0;LED4=0;
	mj_delay_ms(100);
	LED1=1;LED2=1;LED3=1;LED4=1;
	OSInit(&err);		                   //��ʼ��UCOSIII
	OS_CRITICAL_ENTER();	             //�����ٽ���			 
	//������ʼ����
	OSTaskCreate(&StartTaskTCB,"start task",start_task,0,START_TASK_PRIO,
                START_TASK_STK,START_STK_SIZE/10,START_STK_SIZE,0,0,0,OS_OPT_DEFAULT,&err);
	OS_CRITICAL_EXIT();	               //�˳��ٽ���	 
	OSStart(&err);                     //����UCOSIII
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
	/*�����¼���־�飬�жϺ������ͬ��ʹ���¼���־��*/
	OSFlagCreate(&IRQ_EVENTs,"IRQ_EVENTs",0,&err);
	/*ң���������������*/
	OSTaskCreate(&Rf433TaskTCB,"Rf433TaskTCB",rf_433_task,0,RF433_COMRx_TASK_PRIO,
								RF_433_TASK_STK,RF433_STK_SIZE/10,RF433_STK_SIZE,0,0,0,OS_OPT_DEFAULT,&err);
	/*���ػ������������*/					
	OSTaskCreate(&PcComRxTaskTCB,"PcComRxTaskTCB",pc_comRx_task,0,PC_COMRx_TASK_PRIO,
								PC_COMRx_TASK_STK,PC_COMRx_STK_SIZE/10,PC_COMRx_STK_SIZE,0,0,0,OS_OPT_DEFAULT,&err);	
	/*�����ػ��ϱ�ȫ״̬��Ϣ������*/							
	OSTaskCreate(&PcComTxTaskTCB,"PcComTxTaskTCB",pc_comTx_task,0,PC_COMTx_TASK_PRIO,
								PC_COMTx_TASK_STK,PC_COMTx_STK_SIZE/10,PC_COMTx_STK_SIZE,0,0,0,OS_OPT_DEFAULT,&err);		
	/*Can���ݽ���������Ҫ���մ��������õ��������Ϣ*/
	OSTaskCreate(&CanRx0TaskTCB,"CanRx0TaskTCB",task_canRx_FIFO0,0,CANRX_FIFO0_TASK_PRIO,
								CANRx0_TASK_STK,CANRx_STK_SIZE/10,CANRx_STK_SIZE,0,0,0,OS_OPT_DEFAULT,&err);		
	/*Can��ʱ�������ݣ���ʱ��ѯ������������*/
	OSTaskCreate(&CanTxTaskTCB,"CanTxTaskTCB",task_canTx,0,CANTX_TASK_PRIO,
								CANTx_TASK_STK,CANRx_STK_SIZE/10,CANRx_STK_SIZE,0,0,0,OS_OPT_DEFAULT,&err);
	/*���������ڽ�������ǰ���������ݴ���*/
	OSTaskCreate(&UsHeadTaskTCB,"UsHeadTaskTCB",us_Head_task,0,US_COMRx_HEAD_TASK_PRIO,
								US_HEAD_TASK_STK,US_COMRx_STK_SIZE/10,US_COMRx_STK_SIZE,0,0,0,OS_OPT_DEFAULT,&err);		
	/*���������ڽ������񣬺��������ݴ���*/
	OSTaskCreate(&UsTailTaskTCB,"UsTailTaskTCB",us_Tail_task,0,US_COMRx_HEAD_TASK_PRIO,
								US_TAIL_TASK_STK,US_COMRx_STK_SIZE/10,US_COMRx_STK_SIZE,0,0,0,OS_OPT_DEFAULT,&err);		
	/*Pm���������ݴ�������*/
	OSTaskCreate(&PmComRxTaskTCB,"PmComRxTaskTCB",pm_comRx_task,0,PM_COMRx_TASK_PRIO,
								PM_COMRx_TASK_STK,PM_COMRx_STK_SIZE/10,PM_COMRx_STK_SIZE,0,0,0,OS_OPT_DEFAULT,&err);		
	/*����������Ĵ������������*/
	OSTaskCreate(&DITaskTCB,"DITaskTCB",task_di_sensor,0,DI_SENSOR_TASK_PRIO,
								DI_TASK_STK,DI_STK_SIZE/10,DI_STK_SIZE,0,0,0,OS_OPT_DEFAULT,&err);		
	/*Adc �������ݵļ������������˲��ȵĴ���*/
	OSTaskCreate(&AdcCalcTaskTCB,"AdcCalcTaskTCB",adc_calc_task,0,ADC_CALC_TASK_PRIO,
								ADC_CALC_TASK_STK,ADC_CALC_STK_SIZE/10,ADC_CALC_STK_SIZE,0,0,0,OS_OPT_DEFAULT,&err);		

	/*��������������״̬����˸�Ŀ��ƣ�ת���400msִ��һ�Σ���˸������800ms*/						
	OSTmrCreate(&TmrLeftLight,"TmrLeftLight",0,50,OS_OPT_TMR_PERIODIC,cb_TmrLeftLight,0,&err);
	OSTmrCreate(&TmrRightLight,"TmrRightLight",0,50,OS_OPT_TMR_PERIODIC,cb_TmrRightLight,0,&err);
	OSTmrCreate(&TmrFrontLight,"TmrFrontLight",0,50,OS_OPT_TMR_PERIODIC,cb_TmrFrontLight,0,&err);
	/*�����ʱ����¼���ֿ��Ƶĳ�ʱʱ�䣬50msִ��һ�λص�����*/
	OSTmrCreate(&TmrTimeOutCheck,"TmrTimeOutCheck",0,5,OS_OPT_TMR_PERIODIC,cb_TmrTimeOutCheck,0,&err);
	/*����ǰ���Ƕȣ���ת�����˸��ʱ��*/
	OSTmrCreate(&TmrAngelMonitor,"TmrAngelMonitor",0,5,OS_OPT_TMR_PERIODIC,cb_TmrAngelMonitor,0,&err);

	OSTmrStart(&TmrTimeOutCheck,&err);  //������ʱ�������
	OSTmrStart(&TmrAngelMonitor,&err);  //�����Ƕȼ������׼ȷ��˵��ת��Ƽ������
	
	OS_CRITICAL_EXIT();	                //�˳��ٽ���
	elecmbile_status.RTOS_Running = 1;  //����ϵͳ׼����,��������������
	OSTaskDel((OS_TCB*)0,&err);	        //ɾ��start_task��������
}
