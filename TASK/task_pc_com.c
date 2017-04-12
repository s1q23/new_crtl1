#include "task_pc_com.h"
#include "mj_usart_pc.h"
#include "bsp_gpo.h"
//#include "includes.h"
OS_TCB PcComRxTaskTCB;                           //������ƿ�
OS_TCB PcComTxTaskTCB;                           //������ƿ�
CPU_STK PC_COMRx_TASK_STK[PC_COMRx_STK_SIZE];    //�����ջ	
CPU_STK PC_COMTx_TASK_STK[PC_COMTx_STK_SIZE];    //�����ջ	


ipc_cmd_handler_t ipc_cmd_handler_table[] = {
	{IPC_CMD_SET_FRONT_LIGHT		, pc_cmd_handler_set_front_light	},		// ���ػ�����ǰ��
	{IPC_CMD_SET_BACK_LIGHT			,	pc_cmd_handler_set_back_light},	// ���
	{IPC_CMD_SET_BREAK_LIGHT		,	pc_cmd_handler_set_break_light},	// ɲ����
	{IPC_CMD_SET_SEARCH_LIGHT		,	pc_cmd_handler_set_search_light},// ̽�յ�
	{IPC_CMD_SET_ALARM_LIGHT		,	pc_cmd_handler_set_alarm_light	},// ����
	{IPC_CMD_SET_LEFT_LIGHT			,	pc_cmd_handler_set_left_light},// ��ת��
	{IPC_CMD_SET_RIGHT_LIGHT		,	pc_cmd_handler_set_right_light},	// ��ת��
	{IPC_CMD_SET_HIGHSPEAKER		,	pc_cmd_handler_set_highspeaker	},// ��������
	{IPC_CMD_SET_TRACKING				,	pc_cmd_handler_set_tracking},// Ѱ��
	{IPC_CMD_SET_FRONT_SPEED		,	pc_cmd_handler_set_front_speed},// ǰ���ٶ�
	{IPC_CMD_SET_FRONT_ANGLE		,	pc_cmd_handler_set_front_angle},// ǰ�ֽǶ�
	{IPC_CMD_SET_BACK_SPEED			,	pc_cmd_handler_set_back_speed},// �����ٶ�
	{IPC_CMD_SET_FRONT_AND_BACK_SPEED		,	pc_cmd_handler_set_front_and_back_speed},// ǰ�ֺͺ����ٶ�
	{IPC_CMD_SET_FRONTANGLE_AND_BACKSPEED			,	pc_cmd_handler_set_frontangle_and_backspeed},// ǰ�ֽǶȺͺ����ٶ�
	{IPC_CMD_GET_FRONT_ANGLE 	,		pc_cmd_handler_get_front_angle},		// ��ȡǰ�ֽǶ�
	{IPC_CMD_GET_FRONT_SPEED		,		pc_cmd_handler_get_front_speed},		// ��ȡǰ���ٶ�
	{IPC_CMD_GET_BACK_SPEED		,		pc_cmd_handler_get_back_speed},		// ��ȡ�����ٶ�
	{IPC_CMD_GET_TEMPERATURE		,		pc_cmd_handler_get_temperature},		// ��ȡ�¶�
	{IPC_CMD_GET_HUMIDITY		,			pc_cmd_handler_get_humidity},	// ��ȡʪ��
	{IPC_CMD_GET_FOG 		,				pc_cmd_handler_get_fog},// ��ȡ����
	{IPC_CMD_GET_CO		,				pc_cmd_handler_get_co},// ��ȡһ����̼
	{IPC_CMD_GET_BATV		,			pc_cmd_handler_get_batc},	// ��ȡ��ص�ѹ
	{IPC_CMD_GET_BATC		,			pc_cmd_handler_get_batv},	// ��ȡ��ص���
	{IPC_CMD_GET_WATERLEVEL		,	pc_cmd_handler_get_waterlevel},			// ��ȡˮλ
	{IPC_CMD_GET_SHAKE		,				pc_cmd_handler_get_shake},// ��ȡ��
	{IPC_CMD_GET_GLASSCRASH		,	pc_cmd_handler_get_glasscrash},			// ��ȡ��������
	{IPC_CMD_GET_MODE	,					pc_cmd_handler_get_mode},			//��ȡ��������Ϣ
	{IPC_CMD_GET_ULTRASONIC		,	pc_cmd_handler_get_ultrasonic},			// ��ȡ��������Ϣ
	
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
//	{PC_CMD_CORD_SENT_RMS_STATE    	, pc_cmd_handler_sent_rms_state    	}, 	  	// �ϴ�ȫ״̬,(�յ���̨ȷ����Ϣ)
////	{PC_CMD_CORD_SENT_TALKBACK_REQ  , pc_cmd_handler_sent_talkback_req  }, 		// ���������Խ�(�յ���̨ȷ����Ϣ)
//	
//	{PC_CMD_CORD_SET_RMS_NAVIGATION	, pc_cmd_handler_set_navigation},	// 0x80	��λ�������趨С��ת��λ������ٶ�
//	{PC_CMD_CORD_SET_RMS_CONTROL	, pc_cmd_handler_set_rms_control		}, 	// 0x81��̨�ֶ�����С���˶�
//	{PC_CMD_CORD_SET_EMG_BRACK		, pc_cmd_handler_set_emergency_brake	}, 	//0x82 RMS�����ƶ�, PC -> RMS
//	{PC_CMD_CORD_SET_F_WHEEL_TURN		, pc_cmd_handler_set_f_wheel_turn	},	// 0x83��λ������С��ǰ��ת��
//	{PC_CMD_CORD_SET_B_WHEEL_SPEED		, pc_cmd_handler_set_b_wheel_speed	},	// 0x84��λ������С�������ٶ�
//	{PC_CMD_CORD_SET_FRONT_LIGHT		, pc_cmd_handler_set_front_light	},	//0x85 ��λ������ǰ���
//	{PC_CMD_CORD_SET_BACK_LIGHT			, pc_cmd_handler_set_back_light		},	//0x86 ����
//	{PC_CMD_CORD_SET_BRAKE_LIGHT		, pc_cmd_handler_set_brake_light	},	//0x87 ɲ����
//	{PC_CMD_CORD_SET_TURN_LEFT_LIGHT	, pc_cmd_handler_set_turn_left_light},	//0x88 ��ת��
//	{PC_CMD_CORD_SET_TURN_RIGHT_LIGHT	, pc_cmd_handler_set_turn_right_light},	//0x89 ��ת��
//	{PC_CMD_CORD_SET_SOUND_LIGHT_ALARM  , pc_cmd_handler_set_sound_light_alarm},//0x8A ���ⱨ��        
//	{PC_CMD_CORD_SET_UPL_STATE          , pc_cmd_handler_set_upl_state},		//0x8B ���ó�����״̬
//	{PC_CMD_CORD_SET_TALKBACK_MODULE    , pc_cmd_handler_set_talkback_module},	//0x8c �����Խ�    
//	{PC_CMD_CORD_SET_SYS_MODE			, pc_cmd_handler_set_sys_mode		},	//0x8d �趨��ǰģʽ
//	{PC_CMD_CORD_SET_YUNTAI				, pc_cmd_handler_set_yuntai			},	//0x8e ��̨
//	{PC_CMD_CORD_SET_DIRECTION_SPEED	, pc_cmd_handler_set_direction_speed},	// 0x8F	��λ���趨С��ת��������ٶ�
//	{PC_CMD_CORD_SET_HIGH_DECIBEL_SPEAKER	, pc_cmd_handler_set_high_decibel_speaker},	// 0x90	��λ�����Ƹ������ȿ���

//	{PC_CMD_CORD_SET_BRIGHT_LIGHT	, pc_cmd_handler_set_bright_light},	// 0x91	��λ������ǿ���
//	{PC_CMD_CORD_SET_INFRARED_LIGHT	, pc_cmd_handler_set_infrared_light},	//0x92	���ⲹ���
//	{PC_CMD_CORD_SET_SPEAKER_MUTE	, pc_cmd_handler_set_speaker_mute},	// 0X93 ��λ�����Ƹ����㲥����
//	{PC_CMD_CORD_SET_NAVI_BRIGHT_LIGHT	, pc_cmd_handler_set_navi_bright_light},	// 0X94 ��λ�����Ƶ���̽�յ�

	{PC_CMD_CODE_MAX_NO           , NULL                            	}, 
};

static pc_cmd_t cmd_rms2pc;                      //������pc���͵�״̬��Ϣ       
rms_state_t rms_state; 				                   // ȫ״̬

static void mj_USART_SendData(USART_TypeDef* USARTx, uint16_t Data)
{
	while((USARTx->SR&0X40)==0);
	USARTx->DR = (Data & (uint16_t)0x01FF);
}

static void pc_cmd_respond(pc_cmd_t *in_cmd)
{
	int i=0;
	/*1.��������*/
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
	/*2.������ϣ��������*/
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
	/*0.�����Ϸ��Լ��*/
	if(in==NULL||pack==NULL) return false;
	/*1.��ͷ���*/
	if(in[0]!=PC_CMD_HEAD) return false;
	/*2.��������*/
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
	/*3.У��bcc*/
	if(bcc==pack->bcc) return true;
	else return false;
}

void pc_cmd_process(pc_cmd_t *pack)
{
	u8 ret = 0;
	pc_cmd_handler_t *ptbl = &pc_cmd_handler_tbl[0];
	
	while (ptbl->event != NULL) 
	{			// ������ݰ����ݴ������ز�Ϊ��
		if (ptbl->cmd == pack->cmd) 
		{	
			ret = ptbl->event(pack);//����������޸�pack������
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
		if(pc_cmd_decode(p->data,&cmd))        //���յ�һ����ȷ�İ�
		{
			/*������Ӧ����*/
			pc_cmd_process(&cmd);
			/*�崦����*/
			memset(&cmd,0,sizeof(pc_cmd_t));
		}
	}
}

void pc_comTx_task(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
		/*��Ҫ���͵����ݣ��������*/
		pc_cmd_encode(&rms_state,&cmd_rms2pc);
		pc_cmd_respond(&cmd_rms2pc);
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_PERIODIC,&err);
	}
}


handler_result_t pc_cmd_handler_set_front_light(pc_cmd_t *f_ipc_control)// ���ػ�����ǰ��
{
	return HANDLER_NOT_REPLY;
}
handler_result_t pc_cmd_handler_set_back_light(pc_cmd_t *f_ipc_control)// ���ػ����ú��
{
	return HANDLER_NOT_REPLY;
}
handler_result_t pc_cmd_handler_set_break_light(pc_cmd_t *f_ipc_control)// ���ػ�����ɲ����
{
	return HANDLER_NOT_REPLY;
}
handler_result_t pc_cmd_handler_set_alarm_light(pc_cmd_t *f_ipc_control)// ���ػ����þ���
{
	return HANDLER_NOT_REPLY;
}
handler_result_t pc_cmd_handler_set_search_light(pc_cmd_t *f_ipc_control)// ���ػ�����̽�յ�
{
	return HANDLER_NOT_REPLY;
}
handler_result_t pc_cmd_handler_set_highspeaker(pc_cmd_t *f_ipc_control)// ���ػ����ø�������
{
	return HANDLER_NOT_REPLY;
}
handler_result_t pc_cmd_handler_set_left_light(pc_cmd_t *f_ipc_control)// ���ػ�������ת��
{
	return HANDLER_NOT_REPLY;
}
handler_result_t pc_cmd_handler_set_right_light(pc_cmd_t *f_ipc_control)// ���ػ�������ת��
{
	return HANDLER_NOT_REPLY;
}
handler_result_t pc_cmd_handler_set_tracking(pc_cmd_t *f_ipc_control)// ���ػ�����Ѱ��
{
	return HANDLER_NOT_REPLY;
}
handler_result_t pc_cmd_handler_set_front_speed(pc_cmd_t *f_ipc_control)// ���ػ�����ǰ���ٶ�
{
	return HANDLER_NOT_REPLY;
}
handler_result_t pc_cmd_handler_set_back_speed(pc_cmd_t *f_ipc_control)// ���ػ����ú����ٶ�
{
	return HANDLER_NOT_REPLY;
}
handler_result_t pc_cmd_handler_set_front_angle(pc_cmd_t *f_ipc_control)// ���ػ�����ǰ�ֽǶ�
{
	return HANDLER_NOT_REPLY;
}
handler_result_t pc_cmd_handler_set_front_and_back_speed(pc_cmd_t *f_ipc_control)// ���ػ�����ǰ�ֺͺ����ٶ�
{
	return HANDLER_NOT_REPLY;
}
handler_result_t pc_cmd_handler_set_frontangle_and_backspeed(pc_cmd_t *f_ipc_control)// ���ػ�����ǰ�ֽǶȺͺ����ٶ�
{
	return HANDLER_NOT_REPLY;
}
handler_result_t pc_cmd_handler_get_front_angle(pc_cmd_t *f_ipc_control)//��ȡǰ�ֽǶ�
{
	return HANDLER_OK;
}
handler_result_t pc_cmd_handler_get_front_speed(pc_cmd_t *f_ipc_control)//��ȡǰ���ٶ�
{
	return HANDLER_OK;
}
handler_result_t pc_cmd_handler_get_back_speed(pc_cmd_t *f_ipc_control)//��ȡ�����ٶ�
{
	return HANDLER_OK;
}
handler_result_t pc_cmd_handler_get_temperature(pc_cmd_t *f_ipc_control)//��ȡ�¶�
{
	return HANDLER_OK;
}
handler_result_t pc_cmd_handler_get_humidity(pc_cmd_t *f_ipc_control)//��ȡʪ��
{
	return HANDLER_OK;
}
handler_result_t pc_cmd_handler_get_fog(pc_cmd_t *f_ipc_control)//��ȡ����
{
	return HANDLER_OK;
}
handler_result_t pc_cmd_handler_get_co(pc_cmd_t *f_ipc_control)//��ȡһ����̼
{
	return HANDLER_OK;
}
handler_result_t pc_cmd_handler_get_batc(pc_cmd_t *f_ipc_control)//��ȡ��ص�ѹ
{
	return HANDLER_OK;
}
handler_result_t pc_cmd_handler_get_batv(pc_cmd_t *f_ipc_control)//��ȡ��ص���
{
	return HANDLER_OK;
}

handler_result_t pc_cmd_handler_get_waterlevel(pc_cmd_t *f_ipc_control)//��ȡˮλ
{
	return HANDLER_OK;
}
handler_result_t pc_cmd_handler_get_shake(pc_cmd_t *f_ipc_control)//��ȡ��
{
	return HANDLER_OK;
}
handler_result_t pc_cmd_handler_get_glasscrash(pc_cmd_t *f_ipc_control)//��ȡ��������
{
	return HANDLER_OK;
}
handler_result_t pc_cmd_handler_get_mode(pc_cmd_t *f_ipc_control)//��ȡģʽ       ���������Ƽ�λ
{
	return HANDLER_OK;
}
handler_result_t pc_cmd_handler_get_ultrasonic(pc_cmd_t *f_ipc_control)//��ȡ������  ���е�λ�û�ûд��
{
	return HANDLER_OK;
}

