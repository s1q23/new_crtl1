#ifndef _TASK_PC_COM_H
#define _TASK_PC_COM_H

#include "includes.h"
#include "mj_lib.h"

/**********************PC和小车之间的通讯结构体*************************/
#define PC_CMD_DATA_LEN          256          //数据包最大长度

#define PC_CMD_HEAD              0xaa					// 包头标识

/*数据包状态回复*/
#define PC_STATUS_SUCCESS       0x00					// 数据包处理ok
#define PC_STATUS_ERROR         0x80					// 数据包处理错误
#define PC_STATUS_ERR_CMD       0x81					// 不支持此命令
#define PC_STATUS_ERR_BCC       0x82					// 数据域校验错误
#define PC_STATUS_ERR_OPT     	0x83					// 命令操作失败

/*数据的顺序：head,seq,cmd,state,len,data,bcc*/
typedef struct _pc_cmd_t 
{
	unsigned char  head;				                // 包头标识,固定值0xaa
	unsigned char  seq;					                // 包序列号
	unsigned char  cmd;					                // 命令码,包类型
	unsigned char	 state;		  	                // 包状态,用来回复上位机状态。			
	unsigned char  len;					                // 包数据域长度
	unsigned char  data[PC_CMD_DATA_LEN];	      // 包数据域
	unsigned char  bcc;                         // 校验位,add by wqj,2017-03-27
}pc_cmd_t;

typedef enum 
{
	PC_CMD_CODE_SYNC = 0, 								      // 心跳
	// RMS->PC                                  
	PC_CMD_CORD_SENT_RMS_STATE = 1,				      // 向上位机发送RMS状态
	PC_CMD_CORD_SENT_TALKBACK_REQ,				      // 向上位机发送语音对讲请求
	PC_CMD_CORD_SENT_ALAM_MSG,				          // 向上位机发送报警信息
	
	// PC->RMS
	PC_CMD_CORD_SET_RMS_NAVIGATION = 0x80,      //后台导航控制，add by gao
	PC_CMD_CORD_SET_RMS_CONTROL	= 0x81,				  // 上位机控制RMS , PC->RMS
	PC_CMD_CORD_SET_EMG_BRACK	,				          // RMS紧急制动, PC -> RMS
	PC_CMD_CORD_SET_F_WHEEL_TURN,				        // 上位机控制小车前轮转向
	PC_CMD_CORD_SET_B_WHEEL_SPEED,				      // 上位机控制小车后轮速度
	PC_CMD_CORD_SET_FRONT_LIGHT,				        // 上位机控制前大灯
	PC_CMD_CORD_SET_BACK_LIGHT,				          // 后大灯
	PC_CMD_CORD_SET_BRAKE_LIGHT,				        // 刹车灯
	PC_CMD_CORD_SET_TURN_LEFT_LIGHT,				    // 左转向
	PC_CMD_CORD_SET_TURN_RIGHT_LIGHT,				    // 右转向
	PC_CMD_CORD_SET_SOUND_LIGHT_ALARM=0x8A,	    // 声光报警        
	PC_CMD_CORD_SET_UPL_STATE=0x8B,						  // 设置超声波状态      
	PC_CMD_CORD_SET_TALKBACK_MODULE,				    // 语音对讲    
                                              
	PC_CMD_CORD_SET_SYS_MODE,				            // 0x8D设定当前模式
	PC_CMD_CORD_SET_YUNTAI,				              // 0x8E 云台
	PC_CMD_CORD_SET_DIRECTION_SPEED,				    // 0x8F	上位机控制小车转向与后轮速度
	PC_CMD_CORD_SET_HIGH_DECIBEL_SPEAKER,       
	PC_CMD_CORD_SET_BRIGHT_LIGHT ,              // 0X91 上位机控制强光灯和红外补光灯
	PC_CMD_CORD_SET_INFRARED_LIGHT,             // 0X92 上位机控制强光灯和红外补光灯
	PC_CMD_CORD_SET_SPEAKER_MUTE,               // 0X93 上位机控制高音广播静音
	PC_CMD_CORD_SET_NAVI_BRIGHT_LIGHT,          // 0X94 上位机控制导航强光灯

	PC_CMD_CODE_MAX_NO,

}pc_cmd_type;



typedef handler_result_t (*ipc_cmd_handler_event)(pc_cmd_t *f_ipc_control); 
typedef struct _can_cmd_handler_t {
	ipc_cmd_type_t cmd;
	ipc_cmd_handler_event   event;
}ipc_cmd_handler_t;

#define PC_CMD_INVALID(c)          ((c >= PC_CMD_CORD_SENT_RMS_STATE) && (c < PC_CMD_CODE_MAX_NO))
#define IR313_CMD_INVALID(c)       ((c >= 0x27) && (c < 0x30))  






// 语音对讲, RMS->pc,  0x02
typedef __packed  struct _talkback {
	u8 	robot_no:4;			                         // 机器人编号
	u8	track_no:4;			                         // 轨道编号
	u32 unixtime;			                           // uinix timer
	float latitude;			                         // 纬度, 北纬为正,南纬为负
	float longitude;		                         // 经度, 东经为正
	u8   retain;			                           // 预留
	u8	talkback_state;		                       // 对讲功能
}talkback_t, *ptalkback_t;

typedef __packed  struct _rms_ctl {
	u8 	robot_no:4;			                         // 机器人编号
	u8	track_no:4;			                         // 轨道编号
	u32 unixtime;			                           // uinix timer
	s8	angle;				                           // 前轮角度
	s16	speed;				                           // 后轮速度 L/s
                                               
	u8  high_dec_speaker:1;	                     // 高音喇叭 0:关闭，1：打开 , byte 最低位,bit0
	u8	light_alarm:1;		                       // 光报警	0:关闭，1：打开	, bit1
	u8	right_light:1;		                       // 右转向灯, byte的最低位	, bit2
	u8	left_light:1;		                         // 左转向灯
	u8	brake_light:1;		                       // 刹车灯
	u8	back_light:1;		                         // 后大灯
	u8	front_light:2;		                       // 前灯		0:关闭， 1:打开	, byte最高位,bit7
                                               
	u8  retain:5;			                           // 预留
	u8	yuntai_pos:1;		                         // 云台控制, 0:降到最低点, 1升到最高点
	u8	rms_mode:1;			                         // 模式, 0:自动模式，1:手动模式, bit6
	u8  talk_back:1;		                         // 语音对讲 0:关闭，1：打开 ,  byte最高位,bit7	

}rms_ctl_t, *prms_ctl_t;

// 机器人紧急制动控制 PC->RMS， 命令码0x82
typedef __packed struct _rms_emg_brake {
	u8	ctl_mod;			                           // 控制模式, 0x00 停止所有机器人,0x01启动机器人
	u8 	robot_no;			                           // 机器人编号
	u32 unixtime;			                           // uinix timer
}rms_emg_brake_t, *prms_emg_brake_t;

// 系统模式
typedef enum 
{
	AUTO_RUN,		                                // 自动循迹模式
	HUMAN_CTL,		                              // 后台控制模式(后台人工控制小车运动)
}sys_run_mode_t	;

void pc_cmd_send_char(unsigned char ch);
void pc_init(void);
unsigned char pc_cmd_handler(void);


// RMS->PC, 向后台请求开启语音对讲功能
void pc_sent_talkback_request(talkback_t tb);
void pc_sent_rms_state(rms_state_t rms_state);
void pc_sent_alarm_msg(alarm_t alarm, u8 *data);

//RMS->IR313, 向夜视仪发送数据
void ir313_sent_8_msg(u8 cmdtype);
void ir313_sent_9_msg(u8 cmdtype ,u8 cmddata);
void ir313_sent_10_msg(u8 cmdtype ,u8 cmddata1 ,u8 cmddata2);
void ir313_sent_11_msg(u8 cmdtype ,u8 cmddata1 ,u8 cmddata2 ,u8 cmddata3);

u8 ir313_cmd_decoder(void);
u8 fpl330_cmd_decoder(void);
u8 gps_cmd_decoder(void);
// RMS与ir313通信数据包结构体(接收目标距离和报警级别用)
typedef struct _ir313_cmd_t {
	unsigned char               head[2];				// 信息头
	unsigned char               cmd;					  // 命令字			
	unsigned char               len[2];					// 包数据域长度
	unsigned char               data[256];	    // 包数据域
	unsigned char               bcc;					  // 校验和
	unsigned char               tail[2];				// 结束标志
}ir313_cmd_t, *pir313_cmd_t;

// RMS与fpl330通信数据包结构体
typedef struct _fpl330_cmd_t {
	unsigned char               head;					  // 进程代码
	unsigned char								state;          // 状态位
	unsigned char               reserve;			  // 保留			
	unsigned char               fun;					  // 功能
	unsigned char               len[2];				  // 位计数
	unsigned char               crc1[2];			  // 校验和	
	unsigned char               data[2];		    // 包数据域
	unsigned char               crc2[2];			  // 校验和
}fpl330_cmd_t, *pfpl330_cmd_t;

// RMS与GPS	开发板通信数据包结构体(接收GPS的时间和经纬度等信息)
typedef struct _gps_cmd_t {
	unsigned char               head;					   // 包头标识	
	unsigned char               cmd;					   // 命令码,包类型	
	unsigned char               len;					   // 包数据域长度
	unsigned char               data[60];	       // 包数据域
	unsigned char         			crc[2];          //crc校验码 
}gps_cmd_t, *pgps_cmd_t;

// RMS与ir313通信数据包结构体（8字节长度时用）
typedef struct _ir313_cmd_t_8 {
	unsigned char               head[2];				 // 信息头
	unsigned char               cmd;					   // 命令字			
	unsigned char               len[2];					 // 包数据域长度
	unsigned char               bcc;					   // 校验和
	unsigned char               tail[2];				 // 结束标志
}ir313_cmd_t_8, *pir313_cmd_t_8;

// RMS与ir313通信数据包结构体（9字节长度时用）
typedef struct _ir313_cmd_t_9 {
	unsigned char               head[2];				 // 信息头
	unsigned char               cmd;					   // 命令字			
	unsigned char               len[2];					 // 包数据域长度
	unsigned char 							data;            
	unsigned char               bcc;					   // 校验和
	unsigned char               tail[2];				 // 结束标志
}ir313_cmd_t_9, *pir313_cmd_t_9;               
                                               
// RMS与ir313通信数据包结构体（10字节长度时用）
typedef struct _ir313_cmd_t_10 {               
	unsigned char               head[2];				 // 信息头
	unsigned char               cmd;					   // 命令字			
	unsigned char               len[2];					 // 包数据域长度
	unsigned char 							data[2];         
	unsigned char               bcc;					   // 校验和
	unsigned char               tail[2];				 // 结束标志
}ir313_cmd_t_10, *pir313_cmd_t_10;             
                                               
// RMS与ir313通信数据包结构体（10字节长度时用）
typedef struct _ir313_cmd_t_11 {               
	unsigned char               head[2];				 // 信息头
	unsigned char               cmd;					   // 命令字			
	unsigned char               len[2];					 // 包数据域长度
	unsigned char 							data[3];         
	unsigned char               bcc;					   // 校验和
	unsigned char               tail[2];				 // 结束标志
}ir313_cmd_t_11, *pir313_cmd_t_11;


/*pc com rx task 相关配置,处理从PC发下来的指令*/
#define PC_COMRx_STK_SIZE 		256

extern OS_TCB  PcComRxTaskTCB;                
extern CPU_STK PC_COMRx_TASK_STK[PC_COMRx_STK_SIZE];

void pc_comRx_task(void *p_arg);               

/*pc com tx task 相关配置，定时向PC发送全状态信息*/
#define PC_COMTx_STK_SIZE 		256

extern rms_state_t rms_state; 				         // 全状态

extern OS_TCB  PcComTxTaskTCB;                        
extern CPU_STK PC_COMTx_TASK_STK[PC_COMTx_STK_SIZE];

handler_result_t pc_cmd_handler_set_front_light(pc_cmd_t *f_ipc_control);// 工控机设置前灯
handler_result_t pc_cmd_handler_set_back_light(pc_cmd_t *f_ipc_control);// 工控机设置后灯
handler_result_t pc_cmd_handler_set_break_light(pc_cmd_t *f_ipc_control);// 工控机设置刹车灯
handler_result_t pc_cmd_handler_set_alarm_light(pc_cmd_t *f_ipc_control);// 工控机设置警灯
handler_result_t pc_cmd_handler_set_search_light(pc_cmd_t *f_ipc_control);// 工控机设置探照灯
handler_result_t pc_cmd_handler_set_highspeaker(pc_cmd_t *f_ipc_control);// 工控机设置高音喇叭
handler_result_t pc_cmd_handler_set_left_light(pc_cmd_t *f_ipc_control);// 工控机设置左转灯
handler_result_t pc_cmd_handler_set_right_light(pc_cmd_t *f_ipc_control);// 工控机设置右转灯
handler_result_t pc_cmd_handler_set_tracking(pc_cmd_t *f_ipc_control);// 工控机设置寻迹
handler_result_t pc_cmd_handler_set_front_speed(pc_cmd_t *f_ipc_control);// 工控机设置前轮速度
handler_result_t pc_cmd_handler_set_back_speed(pc_cmd_t *f_ipc_control);// 工控机设置后轮速度
handler_result_t pc_cmd_handler_set_front_angle(pc_cmd_t *f_ipc_control);// 工控机设置前轮角度
handler_result_t pc_cmd_handler_set_front_and_back_speed(pc_cmd_t *f_ipc_control);// 工控机设置前轮和后轮速度
handler_result_t pc_cmd_handler_set_frontangle_and_backspeed(pc_cmd_t *f_ipc_control);// 工控机设置前轮角度和后轮速度

handler_result_t pc_cmd_handler_get_front_angle(pc_cmd_t *f_ipc_control);//获取前轮角度
handler_result_t pc_cmd_handler_get_front_speed(pc_cmd_t *f_ipc_control);//获取前轮速度
handler_result_t pc_cmd_handler_get_back_speed(pc_cmd_t *f_ipc_control);//获取后轮速度
handler_result_t pc_cmd_handler_get_temperature(pc_cmd_t *f_ipc_control);//获取温度
handler_result_t pc_cmd_handler_get_humidity(pc_cmd_t *f_ipc_control);//获取湿度
handler_result_t pc_cmd_handler_get_fog(pc_cmd_t *f_ipc_control);//获取烟雾
handler_result_t pc_cmd_handler_get_co(pc_cmd_t *f_ipc_control);//获取一氧化碳
handler_result_t pc_cmd_handler_get_batc(pc_cmd_t *f_ipc_control);//获取电池电压
handler_result_t pc_cmd_handler_get_batv(pc_cmd_t *f_ipc_control);//获取电池电流
handler_result_t pc_cmd_handler_get_waterlevel(pc_cmd_t *f_ipc_control);//获取水位
handler_result_t pc_cmd_handler_get_shake(pc_cmd_t *f_ipc_control);//获取震动
handler_result_t pc_cmd_handler_get_glasscrash(pc_cmd_t *f_ipc_control);//获取玻璃破碎
handler_result_t pc_cmd_handler_get_mode(pc_cmd_t *f_ipc_control);//获取模式
handler_result_t pc_cmd_handler_get_ultrasonic(pc_cmd_t *f_ipc_control);//获取超声波


void pc_comRx_task(void *p_arg);
void pc_comTx_task(void *p_arg);

#endif
