#include "task_pc_com.h"
#include "mj_usart_pc.h"
#include "bsp_gpo.h"
//#include "includes.h"
OS_TCB PcComRxTaskTCB;                           //任务控制块
OS_TCB PcComTxTaskTCB;                           //任务控制块
CPU_STK PC_COMRx_TASK_STK[PC_COMRx_STK_SIZE];    //任务堆栈	
CPU_STK PC_COMTx_TASK_STK[PC_COMTx_STK_SIZE];    //任务堆栈	


ipc_cmd_handler_t ipc_cmd_handler_table[] = {
	{IPC_CMD_SET_FRONT_LIGHT		, pc_cmd_handler_set_front_light	},		// 工控机设置前灯
	{IPC_CMD_SET_BACK_LIGHT			,	pc_cmd_handler_set_back_light},	// 后灯
	{IPC_CMD_SET_BREAK_LIGHT		,	pc_cmd_handler_set_break_light},	// 刹车灯
	{IPC_CMD_SET_SEARCH_LIGHT		,	pc_cmd_handler_set_search_light},// 探照灯
	{IPC_CMD_SET_ALARM_LIGHT		,	pc_cmd_handler_set_alarm_light	},// 警灯
	{IPC_CMD_SET_LEFT_LIGHT			,	pc_cmd_handler_set_left_light},// 左转灯
	{IPC_CMD_SET_RIGHT_LIGHT		,	pc_cmd_handler_set_right_light},	// 右转灯
	{IPC_CMD_SET_HIGHSPEAKER		,	pc_cmd_handler_set_highspeaker	},// 高音喇叭
	{IPC_CMD_SET_TRACKING				,	pc_cmd_handler_set_tracking},// 寻迹
	{IPC_CMD_SET_FRONT_SPEED		,	pc_cmd_handler_set_front_speed},// 前轮速度
	{IPC_CMD_SET_FRONT_ANGLE		,	pc_cmd_handler_set_front_angle},// 前轮角度
	{IPC_CMD_SET_BACK_SPEED			,	pc_cmd_handler_set_back_speed},// 后轮速度
	{IPC_CMD_SET_FRONT_AND_BACK_SPEED		,	pc_cmd_handler_set_front_and_back_speed},// 前轮和后轮速度
	{IPC_CMD_SET_FRONTANGLE_AND_BACKSPEED			,	pc_cmd_handler_set_frontangle_and_backspeed},// 前轮角度和后轮速度
	{IPC_CMD_GET_FRONT_ANGLE 	,		pc_cmd_handler_get_front_angle},		// 获取前轮角度
	{IPC_CMD_GET_FRONT_SPEED		,		pc_cmd_handler_get_front_speed},		// 获取前轮速度
	{IPC_CMD_GET_BACK_SPEED		,		pc_cmd_handler_get_back_speed},		// 获取后轮速度
	{IPC_CMD_GET_TEMPERATURE		,		pc_cmd_handler_get_temperature},		// 获取温度
	{IPC_CMD_GET_HUMIDITY		,			pc_cmd_handler_get_humidity},	// 获取湿度
	{IPC_CMD_GET_FOG 		,				pc_cmd_handler_get_fog},// 获取烟雾
	{IPC_CMD_GET_CO		,				pc_cmd_handler_get_co},// 获取一氧化碳
	{IPC_CMD_GET_BATV		,			pc_cmd_handler_get_batc},	// 获取电池电压
	{IPC_CMD_GET_BATC		,			pc_cmd_handler_get_batv},	// 获取电池电流
	{IPC_CMD_GET_WATERLEVEL		,	pc_cmd_handler_get_waterlevel},			// 获取水位
	{IPC_CMD_GET_SHAKE		,				pc_cmd_handler_get_shake},// 获取震动
	{IPC_CMD_GET_GLASSCRASH		,	pc_cmd_handler_get_glasscrash},			// 获取玻璃破碎
	{IPC_CMD_GET_MODE	,					pc_cmd_handler_get_mode},			//获取超声波信息
	{IPC_CMD_GET_ULTRASONIC		,	pc_cmd_handler_get_ultrasonic},			// 获取超声波信息
	
	{IPC_CMD_MAX_NO           	, NULL                     	}, 
};

typedef  handler_result_t (*pc_cmd_handler_event)(pc_cmd_t *pcmd);
typedef struct _pc_cmd_handler_t {
	pc_cmd_type              	cmd;
	pc_cmd_handler_event        event;
}pc_cmd_handler_t, *ppc_cmd_handler_t;

pc_cmd_handler_t pc_cmd_handler_tbl[] = 
{
//	{PC_CMD_CODE_SYNC             	, pc_cmd_handler_sync             	}, 
//	{PC_CMD_CORD_SENT_RMS_STATE    	, pc_cmd_handler_sent_rms_state    	}, 	  	// 上传全状态,(收到后台确认消息)
////	{PC_CMD_CORD_SENT_TALKBACK_REQ  , pc_cmd_handler_sent_talkback_req  }, 		// 请求语音对讲(收到后台确认消息)
//	
//	{PC_CMD_CORD_SET_RMS_NAVIGATION	, pc_cmd_handler_set_navigation},	// 0x80	上位机导航设定小车转向档位与后轮速度
//	{PC_CMD_CORD_SET_RMS_CONTROL	, pc_cmd_handler_set_rms_control		}, 	// 0x81后台手动控制小车运动
//	{PC_CMD_CORD_SET_EMG_BRACK		, pc_cmd_handler_set_emergency_brake	}, 	//0x82 RMS紧急制动, PC -> RMS
//	{PC_CMD_CORD_SET_F_WHEEL_TURN		, pc_cmd_handler_set_f_wheel_turn	},	// 0x83上位机控制小车前轮转向
//	{PC_CMD_CORD_SET_B_WHEEL_SPEED		, pc_cmd_handler_set_b_wheel_speed	},	// 0x84上位机控制小车后轮速度
//	{PC_CMD_CORD_SET_FRONT_LIGHT		, pc_cmd_handler_set_front_light	},	//0x85 上位机控制前大灯
//	{PC_CMD_CORD_SET_BACK_LIGHT			, pc_cmd_handler_set_back_light		},	//0x86 后大灯
//	{PC_CMD_CORD_SET_BRAKE_LIGHT		, pc_cmd_handler_set_brake_light	},	//0x87 刹车灯
//	{PC_CMD_CORD_SET_TURN_LEFT_LIGHT	, pc_cmd_handler_set_turn_left_light},	//0x88 左转向
//	{PC_CMD_CORD_SET_TURN_RIGHT_LIGHT	, pc_cmd_handler_set_turn_right_light},	//0x89 右转向
//	{PC_CMD_CORD_SET_SOUND_LIGHT_ALARM  , pc_cmd_handler_set_sound_light_alarm},//0x8A 声光报警        
//	{PC_CMD_CORD_SET_UPL_STATE          , pc_cmd_handler_set_upl_state},		//0x8B 设置超声波状态
//	{PC_CMD_CORD_SET_TALKBACK_MODULE    , pc_cmd_handler_set_talkback_module},	//0x8c 语音对讲    
//	{PC_CMD_CORD_SET_SYS_MODE			, pc_cmd_handler_set_sys_mode		},	//0x8d 设定当前模式
//	{PC_CMD_CORD_SET_YUNTAI				, pc_cmd_handler_set_yuntai			},	//0x8e 云台
//	{PC_CMD_CORD_SET_DIRECTION_SPEED	, pc_cmd_handler_set_direction_speed},	// 0x8F	上位机设定小车转向与后轮速度
//	{PC_CMD_CORD_SET_HIGH_DECIBEL_SPEAKER	, pc_cmd_handler_set_high_decibel_speaker},	// 0x90	上位机控制高音喇叭开关

//	{PC_CMD_CORD_SET_BRIGHT_LIGHT	, pc_cmd_handler_set_bright_light},	// 0x91	上位机控制强光灯
//	{PC_CMD_CORD_SET_INFRARED_LIGHT	, pc_cmd_handler_set_infrared_light},	//0x92	红外补光灯
//	{PC_CMD_CORD_SET_SPEAKER_MUTE	, pc_cmd_handler_set_speaker_mute},	// 0X93 上位机控制高音广播静音
//	{PC_CMD_CORD_SET_NAVI_BRIGHT_LIGHT	, pc_cmd_handler_set_navi_bright_light},	// 0X94 上位机控制导航探照灯

	{PC_CMD_CODE_MAX_NO           , NULL                            	}, 
};

static pc_cmd_t cmd_rms2pc;                      //车子向pc发送的状态信息       
rms_state_t rms_state; 				                   // 全状态

static void mj_USART_SendData(USART_TypeDef* USARTx, uint16_t Data)
{
	while((USARTx->SR&0X40)==0);
	USARTx->DR = (Data & (uint16_t)0x01FF);
}

static void pc_cmd_respond(pc_cmd_t *in_cmd)
{
	int i=0;
	/*1.发送数据*/
	mj_USART_SendData(UART5,in_cmd->head);
	mj_USART_SendData(UART5,in_cmd->seq);
	mj_USART_SendData(UART5,in_cmd->cmd);
	mj_USART_SendData(UART5,in_cmd->state);
	mj_USART_SendData(UART5,in_cmd->len);
	for (i = 0; i < in_cmd->len; i ++) 
	{
		mj_USART_SendData(UART5,in_cmd->data[i]);
	}
	mj_USART_SendData(UART5,in_cmd->bcc); 	
	/*2.发送完毕，清除数据*/
	memset(in_cmd->data,0,PC_CMD_DATA_LEN);
}

static bool pc_cmd_encode(rms_state_t *in_rms,pc_cmd_t *out_cmd)
{
	unsigned char bcc;int i=0;
	if(in_rms==NULL||out_cmd==NULL) return false;
	
//	memset(out_cmd->data, 0, PC_CMD_DATA_LEN);
	out_cmd->head 	= PC_CMD_HEAD;
	out_cmd->seq++;
	out_cmd->cmd 	  = PC_CMD_CORD_SENT_RMS_STATE;
	out_cmd->state 	= 0;
	out_cmd->len		= sizeof(rms_state);
	memcpy(out_cmd->data, in_rms, sizeof(rms_state));
	if (out_cmd->len > 0) 
	{
		for (i = 0; i < out_cmd->len; i ++) 
		{
			bcc ^= out_cmd->data[i];
		}
	}
	return true;
}

static bool pc_cmd_decode(unsigned char *in,pc_cmd_t *pack)
{
	int cnt=0;unsigned char bcc=0;
	/*0.参数合法性检查*/
	if(in==NULL||pack==NULL) return false;
	/*1.包头检测*/
	if(in[0]!=PC_CMD_HEAD) return false;
	/*2.解析数据*/
	pack->head = in[0];
	pack->seq  = in[1];
	pack->cmd  = in[2];
	pack->state= in[3];
	pack->len  = in[4];
	memcpy(pack->data,in+5,pack->len);
	pack->bcc  = *(in+5+pack->len);
//	bcc=pack->data[0];
	for(cnt=0;cnt<pack->len;cnt++)
	{
		bcc ^= (pack->data[cnt]);
	}
	/*3.校验bcc*/
	if(bcc==pack->bcc) return true;
	else return false;
}

void pc_cmd_process(pc_cmd_t *pack)
{
	u8 ret = 0;
	pc_cmd_handler_t *ptbl = &pc_cmd_handler_tbl[0];
	
	while (ptbl->event != NULL) 
	{			// 如果数据包数据处理函数池不为空
		if (ptbl->cmd == pack->cmd) 
		{	
			ret = ptbl->event(pack);//这个函数会修改pack的内容
			break;
		}
		ptbl++;
	}
	if(HANDLER_REPLY==ret)
	{
		pc_cmd_respond(pack);
	}
}

void pc_comRx_task(void *p_arg)
{
	OS_ERR err;
	static usart_pc_buffer *p=NULL;
	static pc_cmd_t cmd;
	while(1)
	{
		OSFlagPend(&IRQ_EVENTs,EVENTS_PC_CMD,0,OS_OPT_PEND_FLAG_SET_ALL|OS_OPT_PEND_FLAG_CONSUME,0,&err);
		LED2=!LED2;
		p=PcCom_get_data();
		if(pc_cmd_decode(p->data,&cmd))        //接收到一个正确的包
		{
			/*处理相应命令*/
			pc_cmd_process(&cmd);
			/*清处命令*/
			memset(&cmd,0,sizeof(pc_cmd_t));
		}
	}
}

void pc_comTx_task(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
		/*需要发送的内容，打包发送*/
		pc_cmd_encode(&rms_state,&cmd_rms2pc);
		pc_cmd_respond(&cmd_rms2pc);
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_PERIODIC,&err);
	}
}


handler_result_t pc_cmd_handler_set_front_light(pc_cmd_t *f_ipc_control)// 工控机设置前灯
{
	return HANDLER_NOT_REPLY;
}
handler_result_t pc_cmd_handler_set_back_light(pc_cmd_t *f_ipc_control)// 工控机设置后灯
{
	return HANDLER_NOT_REPLY;
}
handler_result_t pc_cmd_handler_set_break_light(pc_cmd_t *f_ipc_control)// 工控机设置刹车灯
{
	return HANDLER_NOT_REPLY;
}
handler_result_t pc_cmd_handler_set_alarm_light(pc_cmd_t *f_ipc_control)// 工控机设置警灯
{
	return HANDLER_NOT_REPLY;
}
handler_result_t pc_cmd_handler_set_search_light(pc_cmd_t *f_ipc_control)// 工控机设置探照灯
{
	return HANDLER_NOT_REPLY;
}
handler_result_t pc_cmd_handler_set_highspeaker(pc_cmd_t *f_ipc_control)// 工控机设置高音喇叭
{
	return HANDLER_NOT_REPLY;
}
handler_result_t pc_cmd_handler_set_left_light(pc_cmd_t *f_ipc_control)// 工控机设置左转灯
{
	return HANDLER_NOT_REPLY;
}
handler_result_t pc_cmd_handler_set_right_light(pc_cmd_t *f_ipc_control)// 工控机设置右转灯
{
	return HANDLER_NOT_REPLY;
}
handler_result_t pc_cmd_handler_set_tracking(pc_cmd_t *f_ipc_control)// 工控机设置寻迹
{
	return HANDLER_NOT_REPLY;
}
handler_result_t pc_cmd_handler_set_front_speed(pc_cmd_t *f_ipc_control)// 工控机设置前轮速度
{
	return HANDLER_NOT_REPLY;
}
handler_result_t pc_cmd_handler_set_back_speed(pc_cmd_t *f_ipc_control)// 工控机设置后轮速度
{
	return HANDLER_NOT_REPLY;
}
handler_result_t pc_cmd_handler_set_front_angle(pc_cmd_t *f_ipc_control)// 工控机设置前轮角度
{
	return HANDLER_NOT_REPLY;
}
handler_result_t pc_cmd_handler_set_front_and_back_speed(pc_cmd_t *f_ipc_control)// 工控机设置前轮和后轮速度
{
	return HANDLER_NOT_REPLY;
}
handler_result_t pc_cmd_handler_set_frontangle_and_backspeed(pc_cmd_t *f_ipc_control)// 工控机设置前轮角度和后轮速度
{
	return HANDLER_NOT_REPLY;
}
handler_result_t pc_cmd_handler_get_front_angle(pc_cmd_t *f_ipc_control)//获取前轮角度
{
	return HANDLER_OK;
}
handler_result_t pc_cmd_handler_get_front_speed(pc_cmd_t *f_ipc_control)//获取前轮速度
{
	return HANDLER_OK;
}
handler_result_t pc_cmd_handler_get_back_speed(pc_cmd_t *f_ipc_control)//获取后轮速度
{
	return HANDLER_OK;
}
handler_result_t pc_cmd_handler_get_temperature(pc_cmd_t *f_ipc_control)//获取温度
{
	return HANDLER_OK;
}
handler_result_t pc_cmd_handler_get_humidity(pc_cmd_t *f_ipc_control)//获取湿度
{
	return HANDLER_OK;
}
handler_result_t pc_cmd_handler_get_fog(pc_cmd_t *f_ipc_control)//获取烟雾
{
	return HANDLER_OK;
}
handler_result_t pc_cmd_handler_get_co(pc_cmd_t *f_ipc_control)//获取一氧化碳
{
	return HANDLER_OK;
}
handler_result_t pc_cmd_handler_get_batc(pc_cmd_t *f_ipc_control)//获取电池电压
{
	return HANDLER_OK;
}
handler_result_t pc_cmd_handler_get_batv(pc_cmd_t *f_ipc_control)//获取电池电流
{
	return HANDLER_OK;
}

handler_result_t pc_cmd_handler_get_waterlevel(pc_cmd_t *f_ipc_control)//获取水位
{
	return HANDLER_OK;
}
handler_result_t pc_cmd_handler_get_shake(pc_cmd_t *f_ipc_control)//获取震动
{
	return HANDLER_OK;
}
handler_result_t pc_cmd_handler_get_glasscrash(pc_cmd_t *f_ipc_control)//获取玻璃破碎
{
	return HANDLER_OK;
}
handler_result_t pc_cmd_handler_get_mode(pc_cmd_t *f_ipc_control)//获取模式       有问题左移几位
{
	return HANDLER_OK;
}
handler_result_t pc_cmd_handler_get_ultrasonic(pc_cmd_t *f_ipc_control)//获取超声波  其中的位置还没写好
{
	return HANDLER_OK;
}

