#ifndef _MJ_LIB_H_
#define _MJ_LIB_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "sys.h"
#include "stm32f4xx.h"                           // Device header
#include "com_libs.h"
#include "includes.h"

/***************************通用的宏定义***************************/
#define FOREVER                      1           // 没什么用
#define RELAY_OFF                    0
#define RELAY_ON                     1
#define RELAY_FLASH                  2
#define Max_Angel                    25          // 前驱角度的最大值
#define Min_Angel                   -25          // 前驱角度的最小值
#define Mid_Angel			               0           // 中间值	
#define Err_Angel                    2           // 角度误差，在2度以内，认为是对的

#define SYS_MIN_FRONT_WHEEL_SPEED    200         // 遥感左右的600 3000
#define SYS_MIN_BACK_WHEEL_SPEED     200         // 最小速度至少大于100  
#define SYS_MAX_FRONT_WHEEL_SPEED    10000   	   // 后轮最高速度
#define SYS_MAX_BACK_WHEEL_SPEED     2000  	     // 后轮最高速度          遥感前后的抵挡300  高档600              

#define MJ_FLAG_OPT              (OS_OPT_PEND_FLAG_SET_ALL|OS_OPT_PEND_FLAG_CONSUME) 
/*************************事件标志位定义***************************/
#define EVENTS_CRASH_STRIP       0x00000001      // 防撞条异常事件
#define EVENTS_RF433_CMD         0x00000002      // 收到遥控器命令
#define EVENTS_PC_CMD            0x00000004      // 收到PC命令
#define EVENTS_US_HEAD_DATA      0x00000008      // 前超声波发来数据
#define EVENTS_US_TAIL_DATA      0x00000010      // 后超声波发来数据
#define EVENTS_CANRX_FIFO0       0x00000020      // can fifo0收到数据
#define EVENTS_CANRX_FIFO1       0x00000040      // can fifo1收到数据
#define EVENTS_DMA_CVTOK         0x00000080      // dma adc 转换完成

/**************************任务优先级定义**************************/
//优先级0：中断服务服务管理任务 OS_IntQTask()
//优先级1：时钟节拍任务 OS_TickTask()
//优先级OS_CFG_PRIO_MAX-3：定时任务 OS_TmrTask()
//优先级OS_CFG_PRIO_MAX-2：统计任务 OS_StatTask()
//优先级OS_CFG_PRIO_MAX-1：空闲任务 OS_IdleTask()

#define USER_TASK_PRIO_MIN           2
#define START_TASK_PRIO		           2           //启动任务任务优先级
#define CRASH_STRIP_TASK_PRIO        3           //防撞条任务优先级
#define PC_COMRx_TASK_PRIO	         5           //PC下发命令任务优先级
#define RF433_COMRx_TASK_PRIO	       4           //任务优先级
#define CANRX_FIFO0_TASK_PRIO        6           //CAN FIFO0收到数据
#define CANRX_FIFO1_TASK_PRIO        7           //CAN FIFO1收到数据
#define PC_COMTx_TASK_PRIO	         14          //PC下发命令任务优先级

#define USER_TASK_PRIO_MAX           (OS_CFG_PRIO_MAX-4)


/******************************杂项宏定义**************************/
#define RESOLUTION_512	           0.703125      //编码器的分辨率 度
#define TIMEOUT_RF433                40
#define TIMEOUT_PC                   40
#define TIMEOUT_IDLE                 40
/*********************可能会用到的数据类型定义**********************/
typedef enum mj_bool{
	false=0,
	true =1
}bool;

// 报警类型
typedef enum {
	ALARM_UPL	 = 1,				                       // 超声波报警, 		[探头编号,探测距离]
	ALARM_BAT_VOL	,				                       // 电池电压过低		[电压值(单位V)]
	ALARM_EMG_STOP	,			                       // 急停开关(按下)报警[0-无报警，1-有报警]
	ALARM_WALTER	,				                       // 水浸报警			[0-无报警，1-有报警]
	ALARM_VIBRATION	,				                     // 振动报警			[0-无报警，1-有报警]
	ALARM_F_DRIVER	,				                     // 前轮驱动器故障报警[0-无报警，1-有报警]
	ALARM_B_DRIVER	,				                     // 后驱驱动器故障报警[0-无报警，1-有报警]
                                               
	ALARM_NULL = 0xe0,			                     // 无报警
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
// RMS 状态 RMS->pc, 命令码0x01, data feild 35bytes
typedef __packed  struct _rms_state{
	u8 	robot_no:4;			                         // 机器人编号
	u8	track_no:4;			                         // 轨道编号
	u32 unixtime;			                           // uinix timer
 	u8 latitude[4];			                         // 纬度, 北纬为正,南纬为负
 	u8 longitude[4];		                         // 经度, 东经为正	
	u8	retain;				                           // 预留
	u16 pm25;				                             // pm2.5
	u16 pm10;				                             // pm10
	u16 co;					                             // co
                                               
	u8 	rms_mode:1;			                         // 模式, 0:自动模式，1:手动模式
	u8  yuntai_pos:2;		                         // 云台位置, 0:最低点， 1: 最高点 2: 中间位置	
	u8	charge_pos:1;		                         // 充电传感器 0:未进入充电区域, 1:进入充电区域
	u8  vibration:1;		                         // 0:无振动, 1:有振动
	u8	talkback:1;			                         // 语音对讲的状态 0: 关闭, 1:开启 	bit5
	u8	smoke_state:1;		                       // 0: 无烟雾, 1:有烟雾				bit6
	u8	walter_state:1;		                       // 水位，0：无水，1:有水			bit7
		
	u8 	battery_capacity;	                       // 电池容量 AH
	u8 	battery_voltage;	                       // 电池电压 V
	u8 	battery_current;	                       // 电池放电电流 A
	u8 	battery_temp;		                         // 电池温度 
	u16 envir_temp;			                         // 环境温度,
	u16 envir_hum;			                         // 环境湿度,
	u8 	angle;				                           // 前轮角度, 0-255,
	s16	speed;				                           // 后轮速度, L/s, 每秒线
	 
//	u8  high_dec_speaker:1;	                   // 高音喇叭 0:关闭，1：打开 , byte 最低位,bit0    以及红外灯光
	u8  upl_state:1;		                         // 超声波状态
	u8	light_alarm:1;		                       // 光报警	0:关闭，1：打开	, bit1
	u8	right_light:1;		                       // 右转向灯, byte的最低位	, bit2
	u8	left_light:1;		                         // 左转向灯
	u8	brake_light:1;		                       // 刹车灯
	u8	back_light:1;		                         // 后大灯
	u8	front_light:2;		                       // 前灯		0:关闭， 1:打开	, 2:闪烁, byte最高位,bit7
	                                             
	u16 track_sensor_quality;	                   // 循迹传感器信号质量(是5个中信号质量最好的那个)
	
	u16 ir313_distance;			                     //IR313车载夜视仪目标行人距离信息
	u8  ir313_alarm_level;	                     //IR313车载夜视仪报警级别标识信息
	gpsINFO gpsinfo;
}rms_state_t, *prms_state_t;	

typedef struct _mj_status
{
	unsigned char scram_rf433:1;                 // 遥控器按下急停
	unsigned char crash_strip:1;                 // 被防撞条触发急停
	unsigned can_control_by_pc:1;                // 标记PC能否控制小车
	unsigned can_control_by_rf433:1;             // 标记遥控器能否控制小车  
}mj_status;

/**********************一些全局变量的声明*************************/
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
