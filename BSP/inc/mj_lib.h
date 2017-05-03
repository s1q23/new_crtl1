#ifndef _MJ_LIB_H_
#define _MJ_LIB_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "sys.h"
#include "stm32f4xx.h"                           // Device header
#include "com_libs.h"
#include "includes.h"
#include "mj_config.h"
/***************************ͨ�õĺ궨��***************************/
#define FOREVER                      1           // ûʲô��
#define RELAY_OFF                    0
#define RELAY_ON                     1
#define RELAY_FLASH                  2

#define PWRLED_BLACK                 0
#define PWRLED_GREEN                 1
#define PWRLED_RED                   2
#define PWRLED_YELLOW                3

#define Max_Angel                    20.0f       // ǰ���Ƕȵ����ֵ
#define Min_Angel                   -20.0f       // ǰ���Ƕȵ���Сֵ
#define Mid_Angel			               0.0f        // �м�ֵ	
#define Err_Angel                    1.0f        // �Ƕ�����1�����ڣ���Ϊ�ǶԵ�
#define Zero_Angel                   1.5f        // �ڡ�1.5�㷶Χ�ڣ�����Ϊ��0
#define Angel_Gears                  16          // �Ƕȷֵ���Ŀ Ĭ��16��

#define BATA_VOLTAGE_LOW             39.0f
#define BATA_VOLTAGE_ERR             1.0f

#define SYS_MIN_FRONT_WHEEL_SPEED    200         // ң�����ҵ�600 3000
#define SYS_MIN_BACK_WHEEL_SPEED     200         // ��С�ٶ����ٴ���100  
#define SYS_MAX_FRONT_WHEEL_SPEED    10000   	   // ��������ٶ�
#define SYS_MAX_BACK_WHEEL_SPEED     2000  	     // ��������ٶ�          ң��ǰ��ĵֵ�300  �ߵ�600              
#define IDLE_CNT_INIT_VAL            1000

#define RMS_TRACK_NO		             1		// ������
#define	RMS_ROBOT_NO		             1		// �����˱��

#define MJ_FLAG_OPT              (OS_OPT_PEND_FLAG_SET_ALL|OS_OPT_PEND_FLAG_CONSUME) 
/*************************�¼���־λ����***************************/
#define EVENTS_CRASH_STRIP       0x00000001      // ��ײ���쳣�¼�
#define EVENTS_RF433_CMD         0x00000002      // �յ�ң��������
#define EVENTS_PC_CMD            0x00000004      // �յ�PC����
#define EVENTS_US_HEAD_DATA      0x00000008      // ǰ��������������
#define EVENTS_US_TAIL_DATA      0x00000010      // ��������������
#define EVENTS_CANRX_FIFO0       0x00000020      // can fifo0�յ�����
#define EVENTS_CANRX_FIFO1       0x00000040      // can fifo1�յ�����
#define EVENTS_PM_DATA           0x00000080      // pm 2.5��������������
#define EVENTS_DMA_CVTOK         0x00000100      // dma adc ת�����

/**************************�������ȼ�����**************************/
//���ȼ�0���жϷ������������� OS_IntQTask()
//���ȼ�1��ʱ�ӽ������� OS_TickTask()
//���ȼ�2����ʱ������
//���ȼ�OS_CFG_PRIO_MAX-3����ʱ���� OS_TmrTask()
//���ȼ�OS_CFG_PRIO_MAX-2��ͳ������ OS_StatTask()
//���ȼ�OS_CFG_PRIO_MAX-1���������� OS_IdleTask()
#define USER_TASK_PRIO_MIN           3
#define START_TASK_PRIO		           3           //���������������ȼ�
#define CRASH_STRIP_TASK_PRIO        4           //��ײ���������ȼ�
#define PC_COMRx_TASK_PRIO	         7           //PC�·������������ȼ�
#define RF433_COMRx_TASK_PRIO	       6           //�������ȼ�
#define CANRX_FIFO0_TASK_PRIO        5           //CAN FIFO0�յ�����
#define CANTX_TASK_PRIO              8           //CAN ��������
#define PC_COMTx_TASK_PRIO	         9           //PC�·������������ȼ�
#define US_COMRx_HEAD_TASK_PRIO     10           //ǰ���������ڽ���
#define US_COMRx_TAIL_TASK_PRIO     11           //���������ڽ���
#define PM_COMRx_TASK_PRIO          12           //Pm2.5�������������ݴ�������
#define DI_SENSOR_TASK_PRIO         13           //���������봫�����ļ������
#define ADC_CALC_TASK_PRIO	        24           //��ADC���������˲����ȼ��ŵ����


#define USER_TASK_PRIO_MAX           (OS_CFG_PRIO_MAX-4)




#define MJ_Bit_0                 ((uint8_t)0x0001)   /* Bit 0 selected */
#define MJ_Bit_1                 ((uint8_t)0x0002)   /* Bit 1 selected */
#define MJ_Bit_2                 ((uint8_t)0x0004)   /* Bit 2 selected */
#define MJ_Bit_3                 ((uint8_t)0x0008)   /* Bit 3 selected */
#define MJ_Bit_4                 ((uint8_t)0x0010)   /* Bit 4 selected */
#define MJ_Bit_5                 ((uint8_t)0x0020)   /* Bit 5 selected */
#define MJ_Bit_6                 ((uint8_t)0x0040)   /* Bit 6 selected */
#define MJ_Bit_7                 ((uint8_t)0x0080)   /* Bit 7 selected */
#define MJ_Bit_8                 ((uint16_t)0x0100)  /* Bit 8 selected */
#define MJ_Bit_9                 ((uint16_t)0x0200)  /* Bit 9 selected */
#define MJ_Bit_10                ((uint16_t)0x0400)  /* Bit 10 selected */
#define MJ_Bit_11                ((uint16_t)0x0800)  /* Bit 11 selected */
#define MJ_Bit_12                ((uint16_t)0x1000)  /* Bit 12 selected */
#define MJ_Bit_13                ((uint16_t)0x2000)  /* Bit 13 selected */
#define MJ_Bit_14                ((uint16_t)0x4000)  /* Bit 14 selected */
#define MJ_Bit_15                ((uint16_t)0x8000)  /* Bit 15 selected */

/******************************����궨��**************************/
#define RESOLUTION_512	           0.703125      //�������ķֱ��ʣ�����
#define TIMEOUT_RF433                40
#define TIMEOUT_PC                   40
#define TIMEOUT_IDLE                 40
#define TIMEOUT_NEVER             1000u          //��Զ���ᳬʱ     

#define DIR_LEFT                   1             // ����ķ��� 
#define DIR_RIGHT                  0             // ���ҵķ���
#define DIR_STOP                   2             // ֹͣ״̬
#define DIR_FRONT                  3             // ��ǰ
#define DIR_BACK                   4             // ����

/*********************���ܻ��õ����������Ͷ���**********************/
typedef enum mj_bool{
	false=0,
	true =1
}bool;

// ��������
typedef enum {
	ALARM_UPL	 = 1,				                       // ����������, 		[̽ͷ���,̽�����]
	ALARM_BAT_VOL	,				                       // ��ص�ѹ����		[��ѹֵ(��λV)]
	ALARM_EMG_STOP	,			                       // ��ͣ����(����)����[0-�ޱ�����1-�б���]
	ALARM_WALTER	,				                       // ˮ������			[0-�ޱ�����1-�б���]
	ALARM_VIBRATION	,				                     // �񶯱���			[0-�ޱ�����1-�б���]
	ALARM_F_DRIVER	,				                     // ǰ�����������ϱ���[0-�ޱ�����1-�б���]
	ALARM_B_DRIVER	,				                     // �������������ϱ���[0-�ޱ�����1-�б���]
                                               
	ALARM_NULL = 0xe0,			                     // �ޱ���
}alarm_t;

typedef __packed struct _nmeaTIME
{
    int     year;                              /**< Years since 1900 */
    int     mon;                               /**< Months since January - [0,11] */
    int     day;                               /**< Day of the month - [1,31] */
    int     hour;                              /**< Hours since midnight - [0,23] */
    int     min;                               /**< Minutes after the hour - [0,59] */
    int     sec;                               /**< Seconds after the minute - [0,59] */
    int     hsec;                              /**< Hundredth part of second - [0,99] */
} nmeaTIME;

typedef __packed struct _gpsINFO
{
	nmeaTIME utc;                                /**< UTC of position */
	double  PDOP;                                /**< Position Dilution Of Precision */
	double  lat;                                 /**< Latitude in NDEG - +/-[degree][min].[sec/60] */
  double  lon;                                 /**< Longitude in NDEG - +/-[degree][min].[sec/60] */
 	double  direction;                           /**< Track angle in degrees True */
} gpsINFO;
// RMS ״̬ RMS->pc, ������0x01, data feild 35bytes
typedef __packed  struct _rms_state{
	volatile u8 	robot_no:4;			                         // �����˱��
	volatile u8	track_no:4;			                           // ������
	volatile u32 unixtime;			                           // unix timer
 	volatile u8 latitude[4];			                         // γ��, ��γΪ��,��γΪ��
 	volatile u8 longitude[4];		                           // ����, ����Ϊ��	
	volatile u8	retain;				                             // Ԥ��
	volatile u16 pm25;				                             // pm2.5
	volatile u16 pm10;				                             // pm10
	volatile u16 co;					                             // co
                                               
	volatile u8 	rms_mode:1;			                         // ģʽ, 0:�Զ�ģʽ��1:�ֶ�ģʽ
	volatile u8  yuntai_pos:2;		                         // ��̨λ��, 0:��͵㣬 1: ��ߵ� 2: �м�λ��	
	volatile u8	charge_pos:1;		                           // ��紫���� 0:δ����������, 1:����������
	volatile u8  vibration:1;		                           // 0:����, 1:����
	volatile u8	talkback:1;			                           // �����Խ���״̬ 0: �ر�, 1:���� 	bit5
	volatile u8	smoke_state:1;		                         // 0: ������, 1:������				bit6
	volatile u8	water_state:1;		                         // ˮλ��0����ˮ��1:��ˮ			bit7
		
	volatile u8 	battery_capacity;	                       // ������� AH
	volatile u8 	battery_voltage;	                       // ��ص�ѹ V
	volatile u8 	battery_current;	                       // ��طŵ���� A
	volatile u8 	battery_temp;		                         // ����¶� 
	volatile u16  envir_temp;			                         // �����¶�,
	volatile u16  envir_hum;			                         // ����ʪ��,
	volatile u8 	angle;				                           // ǰ�ֽǶ�, 0-255,
	volatile s16	speed;				                           // �����ٶ�, L/s, ÿ����
	 
//	volatile u8  high_dec_speaker:1;	                   // �������� 0:�رգ�1���� , byte ���λ,bit0    �Լ�����ƹ�
	volatile u8  upl_state:1;		                           // ������״̬
	volatile u8	light_alarm:1;		                         // �ⱨ��	0:�رգ�1����	, bit1
	volatile u8	right_light:1;		                         // ��ת���, byte�����λ	, bit2
	volatile u8	left_light:1;		                           // ��ת���
	volatile u8	brake_light:1;		                         // ɲ����
	volatile u8	back_light:1;		                           // ����
	volatile u8	front_light:2;		                         // ǰ��		0:�رգ� 1:��	, 2:��˸, byte���λ,bit7
	                                             
	volatile u16 track_sensor_quality;	                   // ѭ���������ź�����(��5�����ź�������õ��Ǹ�)
	
	volatile u16 ir313_distance;			                     //IR313����ҹ����Ŀ�����˾�����Ϣ
	volatile u8  ir313_alarm_level;	                       //IR313����ҹ���Ǳ��������ʶ��Ϣ
	volatile gpsINFO gpsinfo;
}rms_state_t, *prms_state_t;                   

typedef struct _mj_status
{
	volatile unsigned char scram_rf433:1;                 // ң�������¼�ͣ
	volatile unsigned char crash_strip:1;                 // ����ײ��������ͣ
	volatile unsigned char controled_by_pc:1;             // ���PC�ܷ����С��
	volatile unsigned char controled_by_rf433:1;          // ���ң�����ܷ����С��  
	volatile unsigned char em_brake_status:1;             // ��ǵ�ǰ���ɲ��״̬
	volatile unsigned char us_enable:1;                   // ������ʹ�ܱ��λ
	volatile unsigned char us_head_status:1;              // ���ǰ������״̬
	volatile unsigned char us_tail_status:1;              // ��Ǻ�����״̬
	volatile unsigned char brakelight_status:1;           // ���ɲ����״̬   
	volatile unsigned char F_light_state:2;               // ���ǰ�Ƶ�״̬
	volatile unsigned char L_light_state:2;               // ���תת�Ƶ�״̬
	volatile unsigned char R_light_state:2;               // �����ת�Ƶ�״̬
	volatile unsigned char RTOS_Running:1;                // ��ǲ���ϵͳ��û�п�ʼ����
	volatile unsigned char emg_btn_state:1;               // ��Ǽ�ͣ������״̬
}mj_status;

/**********************һЩȫ�ֱ���������*************************/
extern OS_FLAG_GRP IRQ_EVENTs;
extern volatile float dest_angel;
/*���µĳ�ʱ������Էŵ�һ��ȫ�ֵĽṹ������*/
extern volatile unsigned int Idle_Cnt_Stop;
extern volatile unsigned int Idle_Cnt_Rf433_Cmd;
extern volatile unsigned int Idle_Cnt_Pc_Cmd ;
extern volatile unsigned int Idle_Cnt_CanRx_From_Drv;
extern volatile unsigned int Idle_Cnt_EmBrake;
extern volatile unsigned int Idle_Cnt_BrakeLight;
extern volatile unsigned int Idle_Cnt_Crash;
extern volatile unsigned int Idle_Cnt_UsHead; 
extern volatile unsigned int Idle_Cnt_UsTail;
extern volatile unsigned int Idle_Cnt_US;
extern volatile unsigned int Idle_Cnt_BrkGls; 
extern volatile unsigned int Idle_Cnt_Shake;
extern volatile unsigned int Idle_Cnt_Smog;
extern volatile unsigned int Idle_Cnt_ReInit;
extern volatile unsigned int Idle_Cnt_Water;
extern rms_state_t rms_state;
extern mj_status elecmbile_status;
void mj_reset(void);
unsigned int GraytoDecimal(unsigned int Gray);
int SchmittTrigger(float in,const float V_TH,const float E_RR);
#endif
