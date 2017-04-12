#ifndef _MJ_LIB_H_
#define _MJ_LIB_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "sys.h"
#include "stm32f4xx.h"                           // Device header
#include "com_libs.h"
#include "includes.h"

/***************************ͨ�õĺ궨��***************************/
#define FOREVER                      1           // ûʲô��
#define RELAY_OFF                    0
#define RELAY_ON                     1
#define RELAY_FLASH                  2
#define Max_Angel                    25          // ǰ���Ƕȵ����ֵ
#define Min_Angel                   -25          // ǰ���Ƕȵ���Сֵ
#define Mid_Angel			               0           // �м�ֵ	
#define Err_Angel                    2           // �Ƕ�����2�����ڣ���Ϊ�ǶԵ�

#define SYS_MIN_FRONT_WHEEL_SPEED    200         // ң�����ҵ�600 3000
#define SYS_MIN_BACK_WHEEL_SPEED     200         // ��С�ٶ����ٴ���100  
#define SYS_MAX_FRONT_WHEEL_SPEED    10000   	   // ��������ٶ�
#define SYS_MAX_BACK_WHEEL_SPEED     2000  	     // ��������ٶ�          ң��ǰ��ĵֵ�300  �ߵ�600              

#define MJ_FLAG_OPT              (OS_OPT_PEND_FLAG_SET_ALL|OS_OPT_PEND_FLAG_CONSUME) 
/*************************�¼���־λ����***************************/
#define EVENTS_CRASH_STRIP       0x00000001      // ��ײ���쳣�¼�
#define EVENTS_RF433_CMD         0x00000002      // �յ�ң��������
#define EVENTS_PC_CMD            0x00000004      // �յ�PC����
#define EVENTS_US_HEAD_DATA      0x00000008      // ǰ��������������
#define EVENTS_US_TAIL_DATA      0x00000010      // ��������������
#define EVENTS_CANRX_FIFO0       0x00000020      // can fifo0�յ�����
#define EVENTS_CANRX_FIFO1       0x00000040      // can fifo1�յ�����
#define EVENTS_DMA_CVTOK         0x00000080      // dma adc ת�����

/**************************�������ȼ�����**************************/
//���ȼ�0���жϷ������������� OS_IntQTask()
//���ȼ�1��ʱ�ӽ������� OS_TickTask()
//���ȼ�OS_CFG_PRIO_MAX-3����ʱ���� OS_TmrTask()
//���ȼ�OS_CFG_PRIO_MAX-2��ͳ������ OS_StatTask()
//���ȼ�OS_CFG_PRIO_MAX-1���������� OS_IdleTask()

#define USER_TASK_PRIO_MIN           2
#define START_TASK_PRIO		           2           //���������������ȼ�
#define CRASH_STRIP_TASK_PRIO        3           //��ײ���������ȼ�
#define PC_COMRx_TASK_PRIO	         5           //PC�·������������ȼ�
#define RF433_COMRx_TASK_PRIO	       4           //�������ȼ�
#define CANRX_FIFO0_TASK_PRIO        6           //CAN FIFO0�յ�����
#define CANRX_FIFO1_TASK_PRIO        7           //CAN FIFO1�յ�����
#define PC_COMTx_TASK_PRIO	         14          //PC�·������������ȼ�

#define USER_TASK_PRIO_MAX           (OS_CFG_PRIO_MAX-4)


/******************************����궨��**************************/
#define RESOLUTION_512	           0.703125      //�������ķֱ��� ��
#define TIMEOUT_RF433                40
#define TIMEOUT_PC                   40
#define TIMEOUT_IDLE                 40
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
	u8 	robot_no:4;			                         // �����˱��
	u8	track_no:4;			                         // ������
	u32 unixtime;			                           // uinix timer
 	u8 latitude[4];			                         // γ��, ��γΪ��,��γΪ��
 	u8 longitude[4];		                         // ����, ����Ϊ��	
	u8	retain;				                           // Ԥ��
	u16 pm25;				                             // pm2.5
	u16 pm10;				                             // pm10
	u16 co;					                             // co
                                               
	u8 	rms_mode:1;			                         // ģʽ, 0:�Զ�ģʽ��1:�ֶ�ģʽ
	u8  yuntai_pos:2;		                         // ��̨λ��, 0:��͵㣬 1: ��ߵ� 2: �м�λ��	
	u8	charge_pos:1;		                         // ��紫���� 0:δ����������, 1:����������
	u8  vibration:1;		                         // 0:����, 1:����
	u8	talkback:1;			                         // �����Խ���״̬ 0: �ر�, 1:���� 	bit5
	u8	smoke_state:1;		                       // 0: ������, 1:������				bit6
	u8	walter_state:1;		                       // ˮλ��0����ˮ��1:��ˮ			bit7
		
	u8 	battery_capacity;	                       // ������� AH
	u8 	battery_voltage;	                       // ��ص�ѹ V
	u8 	battery_current;	                       // ��طŵ���� A
	u8 	battery_temp;		                         // ����¶� 
	u16 envir_temp;			                         // �����¶�,
	u16 envir_hum;			                         // ����ʪ��,
	u8 	angle;				                           // ǰ�ֽǶ�, 0-255,
	s16	speed;				                           // �����ٶ�, L/s, ÿ����
	 
//	u8  high_dec_speaker:1;	                   // �������� 0:�رգ�1���� , byte ���λ,bit0    �Լ�����ƹ�
	u8  upl_state:1;		                         // ������״̬
	u8	light_alarm:1;		                       // �ⱨ��	0:�رգ�1����	, bit1
	u8	right_light:1;		                       // ��ת���, byte�����λ	, bit2
	u8	left_light:1;		                         // ��ת���
	u8	brake_light:1;		                       // ɲ����
	u8	back_light:1;		                         // ����
	u8	front_light:2;		                       // ǰ��		0:�رգ� 1:��	, 2:��˸, byte���λ,bit7
	                                             
	u16 track_sensor_quality;	                   // ѭ���������ź�����(��5�����ź�������õ��Ǹ�)
	
	u16 ir313_distance;			                     //IR313����ҹ����Ŀ�����˾�����Ϣ
	u8  ir313_alarm_level;	                     //IR313����ҹ���Ǳ��������ʶ��Ϣ
	gpsINFO gpsinfo;
}rms_state_t, *prms_state_t;	

typedef struct _mj_status
{
	unsigned char scram_rf433:1;                 // ң�������¼�ͣ
	unsigned char crash_strip:1;                 // ����ײ��������ͣ
	unsigned can_control_by_pc:1;                // ���PC�ܷ����С��
	unsigned can_control_by_rf433:1;             // ���ң�����ܷ����С��  
}mj_status;

/**********************һЩȫ�ֱ���������*************************/
extern OS_FLAG_GRP IRQ_EVENTs;
extern float dest_angel;
extern unsigned short Idle_Cnt_Stop;
extern unsigned short Idle_Cnt_Rf433_Cmd ;
extern unsigned short Idle_Cnt_Pc_Cmd ;
extern rms_state_t rms_state;
extern mj_status elecmbile_status;
void mj_reset(void);
unsigned int GraytoDecimal(unsigned int Gray);
#endif
