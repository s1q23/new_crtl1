#include "bsp_rtc.h"

STR_SET_TIME  SET_TIME={0x17,0x03,0x15,0x03,0x13,0x17,0x00};

void Config_RTC()
{
	RTC_InitTypeDef RTC_InitStructure;
	RTC_DateTypeDef RTC_DataStructure;
	RTC_TimeTypeDef RTC_TimeStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	PWR_BackupAccessCmd(ENABLE);/*ʹ�ܺ󱸼Ĵ�������*/

	if(RTC_ReadBackupRegister(RTC_BKP_DR0) != 0xA0A2)
	{
		RCC_LSEConfig(RCC_LSE_ON);/*ʹ��LSE����*/
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);/*�ȴ�����*/

		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);/*ѡ��RTCʱ��Դ*/
		RCC_RTCCLKCmd(ENABLE);/*ʹ��RTCʱ��*/
		RTC_WaitForSynchro();/*�ȴ�RTC APB�Ĵ���ͬ��*/
		/*����RTC���ݼĴ����ͷ�Ƶ��*/
		RTC_InitStructure.RTC_AsynchPrediv = 0x7f;
		RTC_InitStructure.RTC_SynchPrediv  = 0xff;
		RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;
		RTC_Init(&RTC_InitStructure);
		/*���������պ�����*/
		RTC_DataStructure.RTC_Year = SET_TIME.year;
		RTC_DataStructure.RTC_Month = SET_TIME.month;
		RTC_DataStructure.RTC_Date = SET_TIME.day;
		RTC_DataStructure.RTC_WeekDay = SET_TIME.week;
		RTC_SetDate(RTC_Format_BCD,&RTC_DataStructure);
		/*����ʱ���� ����ʾ��ʽ*/
		RTC_TimeStructure.RTC_H12 = RTC_H12_AM;
		RTC_TimeStructure.RTC_Hours = SET_TIME.hour;
		RTC_TimeStructure.RTC_Minutes = SET_TIME.mintue;
		RTC_TimeStructure.RTC_Seconds = SET_TIME.second;
		RTC_SetTime(RTC_Format_BCD,&RTC_TimeStructure);
		/*���ñ��ݼĴ�������ʾ�Ѿ����ù�RTC*/
		RTC_WriteBackupRegister(RTC_BKP_DR0,0xA0A2);
	}
}

//��������ʱ��(����������,24Сʱ��)
//week:���ڼ�(1~7) @ref  RTC_Alarm_Definitions
//hour,min,sec:Сʱ,����,����
void RTC_Set_AlarmA(u8 hour,u8 min,u8 sec)
{ 
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	RTC_AlarmTypeDef RTC_AlarmTypeInitStructure;
	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
	
	RTC_AlarmCmd(RTC_Alarm_A,DISABLE);//�ر�����A 
	
  RTC_TimeTypeInitStructure.RTC_Hours=hour;//Сʱ
	RTC_TimeTypeInitStructure.RTC_Minutes=min;//����
	RTC_TimeTypeInitStructure.RTC_Seconds=sec;//��
	RTC_TimeTypeInitStructure.RTC_H12=RTC_H12_AM;
  
	RTC_AlarmTypeInitStructure.RTC_AlarmDateWeekDay=1;//����
	RTC_AlarmTypeInitStructure.RTC_AlarmDateWeekDaySel=RTC_AlarmDateWeekDaySel_WeekDay;//��������
	RTC_AlarmTypeInitStructure.RTC_AlarmMask=RTC_AlarmMask_DateWeekDay;//�������ں�����
	RTC_AlarmTypeInitStructure.RTC_AlarmTime=RTC_TimeTypeInitStructure;
  RTC_SetAlarm(RTC_Format_BIN,RTC_Alarm_A,&RTC_AlarmTypeInitStructure);
 
	
	RTC_ClearITPendingBit(RTC_IT_ALRA);//���RTC����A�ı�־
	EXTI_ClearITPendingBit(EXTI_Line17);//���LINE17�ϵ��жϱ�־λ 
	
	RTC_ITConfig(RTC_IT_ALRA,ENABLE);//��������A�ж�
	RTC_AlarmCmd(RTC_Alarm_A,ENABLE);//��������A 
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;//LINE17
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش��� 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE17
  EXTI_Init(&EXTI_InitStructure);//����

	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//��ռ���ȼ�1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
}

//�����Ի��Ѷ�ʱ������  
/*wksel:  @ref RTC_Wakeup_Timer_Definitions
#define RTC_WakeUpClock_RTCCLK_Div16        ((uint32_t)0x00000000)
#define RTC_WakeUpClock_RTCCLK_Div8         ((uint32_t)0x00000001)
#define RTC_WakeUpClock_RTCCLK_Div4         ((uint32_t)0x00000002)
#define RTC_WakeUpClock_RTCCLK_Div2         ((uint32_t)0x00000003)
#define RTC_WakeUpClock_CK_SPRE_16bits      ((uint32_t)0x00000004)
#define RTC_WakeUpClock_CK_SPRE_17bits      ((uint32_t)0x00000006)
*/
//cnt:�Զ���װ��ֵ.����0,�����ж�.
void RTC_Set_WakeUp(u32 wksel,u16 cnt)
{ 
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	RTC_WakeUpCmd(DISABLE);//�ر�WAKE UP
	
	RTC_WakeUpClockConfig(wksel);//����ʱ��ѡ��
	
	RTC_SetWakeUpCounter(cnt);//����WAKE UP�Զ���װ�ؼĴ���
	
	
	RTC_ClearITPendingBit(RTC_IT_WUT); //���RTC WAKE UP�ı�־
  EXTI_ClearITPendingBit(EXTI_Line22);//���LINE22�ϵ��жϱ�־λ 
	 
	RTC_ITConfig(RTC_IT_WUT,ENABLE);//����WAKE UP ��ʱ���ж�
	RTC_WakeUpCmd( ENABLE);//����WAKE UP ��ʱ����
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line22;//LINE22
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش��� 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE22
  EXTI_Init(&EXTI_InitStructure);//����
 
 
	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
}

//RTC�����жϷ�����
void RTC_Alarm_IRQHandler(void)
{    
	if(RTC_GetFlagStatus(RTC_FLAG_ALRAF)==SET)//ALARM A�ж�?
	{
		RTC_ClearFlag(RTC_FLAG_ALRAF);//����жϱ�־
	}   
	EXTI_ClearITPendingBit(EXTI_Line17);	//����ж���17���жϱ�־ 											 
}
