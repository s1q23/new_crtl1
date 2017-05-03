#ifndef _COM_LIBS_H_
#define _COM_LIBS_H_

#include "stm32f4xx.h" 

#define UART_DEBUG USART6

#define SIZE_OF_IPC_CMD	               10
#define SIZE_OF_433_CMD                12
#define SIZE_OF_DATA_ULTRX             24
#define SIZE_OF_DATA_433               24
#define SIZE_OF_DATA_485_T             24
#define SIZE_OF_DATA_485_R             24
#define SIZE_OF_DATA_IPC_T             18
#define SIZE_OF_DATA_IPC_R             100


#define IPC_HEAD                       0xAD
#define IPC_END                        0xED

#define IPC_CMD_HEAD                   IPC_HEAD
#define IPC_CMD_END 									 IPC_END
#define IPC_CMD_MODE 									 0x55
#define SIZE_OF_433_CONTROL						 5
#define RS433_CMD_MARK1 							 0xAA
#define RS433_CMD_MARK2 							 0xBB
#define RS433_CMD_MARK3 							 0xDD
#define RS433_CMD_MARK4 							 0xCC

typedef struct
{
	uint8_t mark1;
	uint8_t front_speed_h;
	uint8_t front_speed_l;
	uint8_t back_speed_h;
	uint8_t back_speed_l;
	uint8_t key1 : 1;
	uint8_t key2 : 1;
	uint8_t key3 : 1;
	uint8_t key4 : 1;
	uint8_t key5 : 1;
	uint8_t key6 : 1;
	uint8_t key7 : 1;
	uint8_t key8 : 1;
	uint8_t ignore;
	uint8_t mark2;
	uint8_t mark3;
	uint8_t send;
	uint8_t psc;
	uint8_t mark4;
}rs433_cmd_t;

typedef struct{
	uint8_t front_speed_h;
	uint8_t front_speed_l;
	uint8_t back_speed_h;
	uint8_t back_speed_l;
	uint8_t key1 : 1;
	uint8_t key2 : 1;
	uint8_t key3 : 1;
	uint8_t key4 : 1;
	uint8_t key5 : 1;
	uint8_t key6 : 1;
	uint8_t key7 : 1;
	uint8_t key8 : 1;
}rs433_control_t;


typedef struct{
	uint8_t head;
	uint8_t mode;
	uint8_t cmd;
	uint8_t parameter1;
	uint8_t parameter2;
	uint8_t parameter3;
	uint8_t parameter4;	
	uint8_t end;
	uint16_t crc;
}ipc_cmd_t;

typedef struct{
	uint8_t head;
	uint8_t len;
	uint8_t mode;
	uint8_t cmd;
	uint8_t *parameters;
	uint8_t end;
	uint8_t crcsum8;
}ipc_cmd_t1;

//小端模式:数据的低位保存在内存的低地址中，而数据的高位保存在内存的高地址中。
typedef struct{
	uint8_t cmd;
	union
	{
		short parameter[2];      //[0]=0x5678,[1]=0x1234
		int parameter12;         //0x12345678
	}parameter;
}ipc_control_t;

typedef struct{
	uint16_t cmd;
	uint8_t lenghtParameters;
	uint8_t *parameters;
}ipc_control_t1;
typedef enum 
{
	HANDLER_OK = 0,					    // 处理ok
	HANDLER_ERROR,				  // 出错
	HANDLER_REPLY,				  // 需要回复
	HANDLER_NOT_REPLY,			// 不需要回复
}handler_result_t;

typedef enum 
{
	IPC_CMD_SET_LEFT_LIGHT	= 0x0100	,	 			 // 左转灯
	IPC_CMD_SET_RIGHT_LIGHT		,				         // 右转灯
	IPC_CMD_SET_FRONT_LIGHT			,				       // 前灯
	IPC_CMD_SET_BACK_LIGHT		,				         // 后灯
	IPC_CMD_SET_BREAK_LIGHT		,				         // 刹车灯
	IPC_CMD_SET_SEARCH_LIGHT	,				         // 探照灯
	IPC_CMD_SET_ULTRASONIC	,								   // 开关超声波
	IPC_CMD_SET_ELECTROMAGNETICBREAKE	,		     // 电磁刹
	IPC_CMD_SET_ALARM_LIGHT		,								 // 警灯
	IPC_CMD_SET_HIGHSPEAKER		,						  	 // 高音喇叭
	IPC_CMD_SET_SOUNDHEAD	,										 // 拾音器
	IPC_CMD_SET_MEGAPHONE	,									   // 扬声器
	
	IPC_CMD_SET_TRACKING		= 0x010C	,				 // 寻迹
	IPC_CMD_SET_FRONT_ANGLE 	= 0x0200	,			 // 前轮角度
	IPC_CMD_SET_FRONT_SPEED		,				         // 前轮速度
	IPC_CMD_SET_BACK_SPEED		,				         // 后轮速度
	IPC_CMD_SET_FRONT_AND_BACK_SPEED		,			 // 前轮和后轮速度
	IPC_CMD_SET_FRONTANGLE_AND_BACKSPEED	,		 // 前轮角度和后轮速度
	
	IPC_CMD_GET_FRONT_ANGLE 	= 0x0300	,		   // 获取前轮角度
	IPC_CMD_GET_FRONT_SPEED		,								 // 获取前轮速度
	IPC_CMD_GET_BACK_SPEED		,								 // 获取后轮速度
	IPC_CMD_GET_TEMPERATURE		,			 					 // 获取温度
	IPC_CMD_GET_HUMIDITY		,				           // 获取湿度
	IPC_CMD_GET_FOG 		,				               // 获取烟雾
	IPC_CMD_GET_CO		,				                 // 获取一氧化碳
	IPC_CMD_GET_BATV		,			                 // 获取电池电压
	IPC_CMD_GET_BATC		,				               // 获取电池电流
	IPC_CMD_GET_WATERLEVEL		,				         // 获取水位
	IPC_CMD_GET_SHAKE		,				               // 获取震动
	IPC_CMD_GET_GLASSCRASH		,				         // 获取玻璃破碎
	IPC_CMD_GET_MODE	,								         // 获取超声波信息
	IPC_CMD_GET_ULTRASONIC		,				         // 获取超声波信息
	
	
	IPC_CMD_MAX_NO 	,	                         //最后一个，作为结束标志 			
}ipc_cmd_type_t;


/* 
灯光类状态: 
power light : 0 = green,1 = red;
front light : 0 = close,1 = open;
back light : 0 = close,1 = open;
left light : 0 = close,1 = open,2 = flash;
right light : 0 = close,1 = open,2 = flash;
break light : 0 = close,1 = open;
alarm light : 0 = close,1 = open;
search light : 0 = close,1 = open;
cradle strong light : 0 = close,1 = open;
cradle fill-in light : 0 = close,1 = open;
obligate light :预留，4位
其他状态：
electromagnetic break(em_break) : 0 = no break, 1 = break;
shake : 0 = no, 1 = yes;
glass crash : 0 = no, 1 = yes;
high speaker: 0 = close,1 = open;
megaphone : 0 = close,1 = open;
sound head : 0 = close,1 = open;
bumper strip : 0 = no,1 = yes;
scram button : 0 = no,1 = yes;
scram_433 :  0 = no,1 = yes;
keya shutdown : 0 = no,1 = yes;
water level : 0 = no,1 = yes;
co :0 = no,1 = yes;
fog : 0 = no,1 = yes;
ultr_c_o: 0 = close,1 = open;
tarcking_c_o: 0 = close,1 = open;
obligate_other : 预留，2位
具体数值：
f_speed,b_speed,f_angle,
 
*/
typedef struct{

	u8 left_light : 2;
	u8 right_light : 2;
	u8 front_light : 1;
	u8 back_light : 1;
	u8 break_light : 1;
	u8 search_light : 1;
	
	u8 ultr_c_o : 1;
	u8 em_break : 1;	
	u8 alarm_light : 1;
	u8 high_speaker : 1;
	u8 sound_head : 1;
	u8 megaphone : 1;
	u8 tarcking_c_o : 1;
	u8 fog : 1;
	
	u8 co : 1;                 //低位在前
	u8 water_level : 1;
	u8 shake : 1;
	u8 glass_crash : 1;
	u8 mode_null : 1;
	u8 mode_tracking : 1;	
	u8 mode_ipc : 1;
	u8 mode_rs433 : 1;

	
	u8 ult_state;//超声波（bit0,1,2=前左中右，bit4,5,6=后左中右）

	u8 f_angle_h;
	u8 f_angle_l;//上传编码器的值，上位机自己做换算-360~360
	u8 f_speed_h;
	u8 f_speed_l;
	u8 b_speed_h;
	u8 b_speed_l;
	
	u8 temperature_h;
	u8 temperature_l;//温度实际值*100上传，-128 ~ 127
	u8 humidity;//湿度上传百分比0~100
	
	u8 bat_v_h;
	u8 bat_v_l;//电压实际值*100上传，-128 ~ 127
	u8 bat_c_h;
	u8 bat_c_l;//电流实际值*100上传，-128 ~ 127
	
	//u8 obigate;
	u8 power_light : 1;
	u8 bumper_strip : 1;
	u8 obigate : 6;
}robot_info_t;

typedef struct{
	s16 f_angle;
	s16 f_speed;
	s16 b_speed;
	s16 temperature;
	s16 bat_v;
	s16 bat_c;
}set_atomic;
typedef struct{
	u8 head;
	u8 em_break;
	u8 left_light;
	u8 right_light;
	u8 front_light;
	u8 back_light;
	u8 break_light;
	u8 alarm_light;
	u8 search_light;
	u8 power_light;
	u8 high_speaker;
	u8 back_speed;
	u8 front_speed;
	u8 ultr_c_o;//close or open ultr
	u8 tracking_c_o;
	u8 specified_angle;
	u8 end;
}priority_t;


extern volatile robot_info_t robot_info;

#endif
