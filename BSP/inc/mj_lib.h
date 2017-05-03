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
/***************************通用的宏定义***************************/
#define FOREVER                      1           // 没什么用
#define RELAY_OFF                    0
#define RELAY_ON                     1
#define RELAY_FLASH                  2

#define PWRLED_BLACK                 0
#define PWRLED_GREEN                 1
#define PWRLED_RED                   2
#define PWRLED_YELLOW                3

#define Max_Angel                    20.0f       // 前驱角度的最大值
#define Min_Angel                   -20.0f       // 前驱角度的最小值
#define Mid_Angel			               0.0f        // 中间值	
#define Err_Angel                    1.0f        // 角度误差，在1度以内，认为是对的
#define Zero_Angel                   1.5f        // 在±1.5°范围内，都认为是0
#define Angel_Gears                  16          // 角度分档数目 默认16当

#define BATA_VOLTAGE_LOW             39.0f
#define BATA_VOLTAGE_ERR             1.0f

#define SYS_MIN_FRONT_WHEEL_SPEED    200         // 遥感左右的600 3000
#define SYS_MIN_BACK_WHEEL_SPEED     200         // 最小速度至少大于100  
#define SYS_MAX_FRONT_WHEEL_SPEED    10000   	   // 后轮最高速度
#define SYS_MAX_BACK_WHEEL_SPEED     2000  	     // 后轮最高速度          遥感前后的抵挡300  高档600              
#define IDLE_CNT_INIT_VAL            1000

#define RMS_TRACK_NO		             1		// 轨道编号
#define	RMS_ROBOT_NO		             1		// 机器人编号

#define MJ_FLAG_OPT              (OS_OPT_PEND_FLAG_SET_ALL|OS_OPT_PEND_FLAG_CONSUME) 
/*************************事件标志位定义***************************/
#define EVENTS_CRASH_STRIP       0x00000001      // 防撞条异常事件
#define EVENTS_RF433_CMD         0x00000002      // 收到遥控器命令
#define EVENTS_PC_CMD            0x00000004      // 收到PC命令
#define EVENTS_US_HEAD_DATA      0x00000008      // 前超声波发来数据
#define EVENTS_US_TAIL_DATA      0x00000010      // 后超声波发来数据
#define EVENTS_CANRX_FIFO0       0x00000020      // can fifo0收到数据
#define EVENTS_CANRX_FIFO1       0x00000040      // can fifo1收到数据
#define EVENTS_PM_DATA           0x00000080      // pm 2.5传感器发来数据
#define EVENTS_DMA_CVTOK         0x00000100      // dma adc 转换完成

/**************************任务优先级定义**************************/
//优先级0：中断服务服务管理任务 OS_IntQTask()
//优先级1：时钟节拍任务 OS_TickTask()
//优先级2：定时器任务
//优先级OS_CFG_PRIO_MAX-3：定时任务 OS_TmrTask()
//优先级OS_CFG_PRIO_MAX-2：统计任务 OS_StatTask()
//优先级OS_CFG_PRIO_MAX-1：空闲任务 OS_IdleTask()
#define USER_TASK_PRIO_MIN           3
#define START_TASK_PRIO		           3           //启动任务任务优先级
#define CRASH_STRIP_TASK_PRIO        4           //防撞条任务优先级
#define PC_COMRx_TASK_PRIO	         7           //PC下发命令任务优先级
#define RF433_COMRx_TASK_PRIO	       6           //任务优先级
#define CANRX_FIFO0_TASK_PRIO        5           //CAN FIFO0收到数据
#define CANTX_TASK_PRIO              8           //CAN 发送数据
#define PC_COMTx_TASK_PRIO	         9           //PC下发命令任务优先级
#define US_COMRx_HEAD_TASK_PRIO     10           //前超声波串口接收
#define US_COMRx_TAIL_TASK_PRIO     11           //后超声波串口接收
#define PM_COMRx_TASK_PRIO          12           //Pm2.5传感器串口数据处理任务
#define DI_SENSOR_TASK_PRIO         13           //数字量输入传感器的监测任务
#define ADC_CALC_TASK_PRIO	        24           //对ADC采样数据滤波优先级放到最低


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

/******************************杂项宏定义**************************/
#define RESOLUTION_512	           0.703125      //编码器的分辨率，精度
#define TIMEOUT_RF433                40
#define TIMEOUT_PC                   40
#define TIMEOUT_IDLE                 40
#define TIMEOUT_NEVER             1000u          //永远不会超时     

#define DIR_LEFT                   1             // 往左的方向 
#define DIR_RIGHT                  0             // 往右的方向
#define DIR_STOP                   2             // 停止状态
#define DIR_FRONT                  3             // 往前
#define DIR_BACK                   4             // 后退

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
	volatile u8 	robot_no:4;			                         // 机器人编号
	volatile u8	track_no:4;			                           // 轨道编号
	volatile u32 unixtime;			                           // unix timer
 	volatile u8 latitude[4];			                         // 纬度, 北纬为正,南纬为负
 	volatile u8 longitude[4];		                           // 经度, 东经为正	
	volatile u8	retain;				                             // 预留
	volatile u16 pm25;				                             // pm2.5
	volatile u16 pm10;				                             // pm10
	volatile u16 co;					                             // co
                                               
	volatile u8 	rms_mode:1;			                         // 模式, 0:自动模式，1:手动模式
	volatile u8  yuntai_pos:2;		                         // 云台位置, 0:最低点， 1: 最高点 2: 中间位置	
	volatile u8	charge_pos:1;		                           // 充电传感器 0:未进入充电区域, 1:进入充电区域
	volatile u8  vibration:1;		                           // 0:无振动, 1:有振动
	volatile u8	talkback:1;			                           // 语音对讲的状态 0: 关闭, 1:开启 	bit5
	volatile u8	smoke_state:1;		                         // 0: 无烟雾, 1:有烟雾				bit6
	volatile u8	water_state:1;		                         // 水位，0：无水，1:有水			bit7
		
	volatile u8 	battery_capacity;	                       // 电池容量 AH
	volatile u8 	battery_voltage;	                       // 电池电压 V
	volatile u8 	battery_current;	                       // 电池放电电流 A
	volatile u8 	battery_temp;		                         // 电池温度 
	volatile u16  envir_temp;			                         // 环境温度,
	volatile u16  envir_hum;			                         // 环境湿度,
	volatile u8 	angle;				                           // 前轮角度, 0-255,
	volatile s16	speed;				                           // 后轮速度, L/s, 每秒线
	 
//	volatile u8  high_dec_speaker:1;	                   // 高音喇叭 0:关闭，1：打开 , byte 最低位,bit0    以及红外灯光
	volatile u8  upl_state:1;		                           // 超声波状态
	volatile u8	light_alarm:1;		                         // 光报警	0:关闭，1：打开	, bit1
	volatile u8	right_light:1;		                         // 右转向灯, byte的最低位	, bit2
	volatile u8	left_light:1;		                           // 左转向灯
	volatile u8	brake_light:1;		                         // 刹车灯
	volatile u8	back_light:1;		                           // 后大灯
	volatile u8	front_light:2;		                         // 前灯		0:关闭， 1:打开	, 2:闪烁, byte最高位,bit7
	                                             
	volatile u16 track_sensor_quality;	                   // 循迹传感器信号质量(是5个中信号质量最好的那个)
	
	volatile u16 ir313_distance;			                     //IR313车载夜视仪目标行人距离信息
	volatile u8  ir313_alarm_level;	                       //IR313车载夜视仪报警级别标识信息
	volatile gpsINFO gpsinfo;
}rms_state_t, *prms_state_t;                   

typedef struct _mj_status
{
	volatile unsigned char scram_rf433:1;                 // 遥控器按下急停
	volatile unsigned char crash_strip:1;                 // 被防撞条触发急停
	volatile unsigned char controled_by_pc:1;             // 标记PC能否控制小车
	volatile unsigned char controled_by_rf433:1;          // 标记遥控器能否控制小车  
	volatile unsigned char em_brake_status:1;             // 标记当前电磁刹的状态
	volatile unsigned char us_enable:1;                   // 超声波使能标记位
	volatile unsigned char us_head_status:1;              // 标记前超声波状态
	volatile unsigned char us_tail_status:1;              // 标记后超声波状态
	volatile unsigned char brakelight_status:1;           // 标记刹车灯状态   
	volatile unsigned char F_light_state:2;               // 标记前灯的状态
	volatile unsigned char L_light_state:2;               // 标记转转灯的状态
	volatile unsigned char R_light_state:2;               // 标记右转灯的状态
	volatile unsigned char RTOS_Running:1;                // 标记操作系统有没有开始运行
	volatile unsigned char emg_btn_state:1;               // 标记急停按键的状态
}mj_status;

/**********************一些全局变量的声明*************************/
extern OS_FLAG_GRP IRQ_EVENTs;
extern volatile float dest_angel;
/*以下的超时问题可以放到一个全局的结构体里面*/
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
