//change text
#include "task_pc_com.h"
#include "mj_usart_pc.h"
#include "bsp_gpo.h"
#include "bsp_gpi.h"
#include "bsp_back_wheel.h"
#include "bsp_front_wheel.h"

#define SECOND_OF_DAY   86400

OS_TCB PcComRxTaskTCB;                           //������ƿ�
OS_TCB PcComTxTaskTCB;                           //������ƿ�
CPU_STK PC_COMRx_TASK_STK[PC_COMRx_STK_SIZE];    //�����ջ	
CPU_STK PC_COMTx_TASK_STK[PC_COMTx_STK_SIZE];    //�����ջ	
RTC_TimeTypeDef g_RTC_Time;
RTC_DateTypeDef g_RTC_Date;
const unsigned char DayOfMon[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
static unsigned int GetUnixTime(RTC_DateTypeDef *date,RTC_TimeTypeDef *time)
{
	unsigned short iYear, iMon, iDay, iHour, iMin, iSec;
  unsigned short i, Cyear=0;
  unsigned int CountDay=0;
  iYear = date->RTC_Year+2000; //��˵STM32������ʱ���Ǵ�2000�꿪ʼ�����
  iMon = date->RTC_Month;
  iDay = date->RTC_Date;
  iHour = time->RTC_Hours;
  iMin = time->RTC_Minutes;
  iSec = time->RTC_Seconds;
    
  for(i=1970; i<iYear; i++)
  {
    if(((i%4==0) && (i%100!=0)) || (i%400==0)) Cyear++;
  }
  CountDay = Cyear * 366 + (iYear-1970-Cyear) * 365;
  for(i=1; i<iMon; i++)
  {
    if((i==2) && (((iYear%4==0)&&(iYear%100!=0)) || (iYear%400==0)))
      CountDay += 29;
    else
      CountDay += DayOfMon[i-1];
  }
  CountDay += (iDay-1);
  
  CountDay = CountDay*SECOND_OF_DAY + (unsigned int)iHour*3600 + (unsigned int)iMin*60 + iSec;
  return CountDay;
}
pc_cmd_handler_t pc_cmd_handler_tbl[] = 
{
	/*С������PC*/
	{PC_CMD_CODE_SYNC             	, pc_cmd_handler_sync             	},      //ͬ����Ϣ
	{PC_CMD_CORD_SENT_RMS_STATE    	, pc_cmd_handler_sent_rms_state    	}, 	  	// �ϴ�ȫ״̬,(�յ���̨ȷ����Ϣ)
	{PC_CMD_CORD_SENT_TALKBACK_REQ  , pc_cmd_handler_sent_talkback_req  }, 		  // ���������Խ�(�յ���̨ȷ����Ϣ)
	/*PC ����С��*/
	{PC_CMD_CORD_SET_RMS_NAVIGATION	, pc_cmd_handler_set_navigation},	          // 0x80	��λ�������趨С��ת��λ������ٶ�
	{PC_CMD_CORD_SET_RMS_CONTROL	  , pc_cmd_handler_set_rms_control		},       	// 0x81��̨�ֶ�����С���˶�
	{PC_CMD_CORD_SET_EMG_BRACK		  , pc_cmd_handler_set_emergency_brake	},     	//0x82 RMS�����ƶ�, PC -> RMS
	{PC_CMD_CORD_SET_F_WHEEL_TURN		, pc_cmd_handler_set_f_wheel_turn	},	      // 0x83��λ������С��ǰ��ת��
	{PC_CMD_CORD_SET_B_WHEEL_SPEED	, pc_cmd_handler_set_b_wheel_speed	},  	// 0x84��λ������С�������ٶ�
	{PC_CMD_CORD_SET_FRONT_LIGHT		, pc_cmd_handler_set_front_light	},	      //0x85 ��λ������ǰ���
	{PC_CMD_CORD_SET_BACK_LIGHT			, pc_cmd_handler_set_back_light		},	      //0x86 ����
	{PC_CMD_CORD_SET_BRAKE_LIGHT		, pc_cmd_handler_set_brake_light	},	      //0x87 ɲ����
	{PC_CMD_CORD_SET_TURN_LEFT_LIGHT	, pc_cmd_handler_set_turn_left_light},	  //0x88 ��ת��
	{PC_CMD_CORD_SET_TURN_RIGHT_LIGHT	, pc_cmd_handler_set_turn_right_light},  	//0x89 ��ת��
	{PC_CMD_CORD_SET_SOUND_LIGHT_ALARM  , pc_cmd_handler_set_sound_light_alarm},//0x8A ���ⱨ��        
	{PC_CMD_CORD_SET_UPL_STATE          , pc_cmd_handler_set_upl_state},		    //0x8B ���ó�����״̬
	{PC_CMD_CORD_SET_TALKBACK_MODULE    , pc_cmd_handler_set_talkback_module},	//0x8c �����Խ�    
	{PC_CMD_CORD_SET_SYS_MODE			, pc_cmd_handler_set_sys_mode		},	          //0x8d �趨��ǰģʽ
	{PC_CMD_CORD_SET_YUNTAI				, pc_cmd_handler_set_yuntai			},	          //0x8e ��̨
	{PC_CMD_CORD_SET_DIRECTION_SPEED	, pc_cmd_handler_set_direction_speed},	  // 0x8F	��λ���趨С��ת��������ٶ�
	{PC_CMD_CORD_SET_HIGH_DECIBEL_SPEAKER	, pc_cmd_handler_set_high_decibel_speaker},	// 0x90	��λ�����Ƹ������ȿ���
	{PC_CMD_CORD_SET_BRIGHT_LIGHT	, pc_cmd_handler_set_bright_light},	           // 0x91	��λ������ǿ���
	{PC_CMD_CORD_SET_INFRARED_LIGHT	, pc_cmd_handler_set_infrared_light},	       //0x92	���ⲹ���
	{PC_CMD_CORD_SET_SPEAKER_MUTE	, pc_cmd_handler_set_speaker_mute},	           // 0X93 ��λ�����Ƹ����㲥����
	{PC_CMD_CORD_SET_NAVI_BRIGHT_LIGHT	, pc_cmd_handler_set_navi_bright_light}, // 0X94 ��λ�����Ƶ���̽�յ�

	{PC_CMD_CODE_MAX_NO           , NULL                            	}, 
};

static pc_cmd_t cmd_rms2pc;                      //������pc���͵�״̬��Ϣ       
rms_state_t rms_state=
{
	.track_no	= RMS_TRACK_NO,
	.robot_no	= RMS_ROBOT_NO
};

static void pc_cmd_respond(pc_cmd_t *in_cmd)
{
	int i=0;
	static char tx[PC_CMD_DATA_LEN];
	memset(tx,0,PC_CMD_DATA_LEN);
	/*1.�������ݵ�buffer�����������memcpy��д*/
	tx[0]=in_cmd->head;
	tx[1]=in_cmd->seq;
	tx[2]=in_cmd->cmd;
	tx[3]=in_cmd->state;
	tx[4]=in_cmd->len;
	for (i = 0; i < in_cmd->len; i ++) 
	{
		tx[5+i]=in_cmd->data[i];
	}
	tx[5+i]=in_cmd->bcc;
	usart_pc_send(tx,6+in_cmd->len);
	/*3.������ϣ��������*/
	memset(in_cmd->data,0,PC_CMD_DATA_LEN);
}

static bool pc_cmd_encode(rms_state_t *in_rms,pc_cmd_t *out_cmd)
{
	unsigned char bcc;int i=0;
	if(in_rms==NULL||out_cmd==NULL) return false;
	in_rms->angle=(unsigned char)((get_front_angel()-Min_Angel)/(Max_Angel*2)*255);
	RTC_GetDate(RTC_Format_BIN,&g_RTC_Date);
	RTC_GetTime(RTC_Format_BIN,&g_RTC_Time);
	in_rms->unixtime=GetUnixTime(&g_RTC_Date,&g_RTC_Time);
	out_cmd->head 	= PC_CMD_HEAD;
	out_cmd->seq++;
	out_cmd->cmd 	  = PC_CMD_CORD_SENT_RMS_STATE;
	out_cmd->state 	= 0;
	out_cmd->len		= sizeof(rms_state_t);
	memcpy(out_cmd->data, in_rms, sizeof(rms_state_t));
	if (out_cmd->len > 0) 
	{
		for (i=0;i<out_cmd->len;i++)
		{
			bcc^=out_cmd->data[i];
		}
	}
	out_cmd->bcc=bcc;
	return true;
}

/********************************************************************/
//���ڷ���ֵ�Ľ��ͣ�
//����0����ȷ
//����1�������ָ��
//����2����ͷ����
//����3��У�鲻��
/*********************************************************************/
static unsigned char pc_cmd_decode(unsigned char *in,pc_cmd_t *pack)
{
	int cnt=0;unsigned char bcc=0;
	/*0.�����Ϸ��Լ��*/
	if(in==NULL||pack==NULL) return 1;
	/*1.��ͷ���*/
	if(in[0]!=PC_CMD_HEAD) return 2;
	/*2.��������*/
	pack->head = in[0];
	pack->seq  = in[1];
	pack->cmd  = in[2];
	pack->state= in[3];
	pack->len  = in[4];
	memcpy(pack->data,in+5,pack->len);
	pack->bcc  = *(in+5+pack->len);
	for(cnt=0;cnt<pack->len;cnt++)
	{
		bcc^=(pack->data[cnt]);
	}
	/*3.У��bcc*/
	if(bcc==pack->bcc) return 0;
	else return 3;
}

void pc_cmd_process(pc_cmd_t *pack)
{
	u8 ret = 0;
	pc_cmd_handler_t *ptbl = &pc_cmd_handler_tbl[0];
	
	while (ptbl->event != NULL) 
	{	
		if(ptbl->cmd == pack->cmd) 
		{	
			ret = ptbl->event(pack);//����������޸�pack������
			break;
		}
		ptbl++;
	}
	if(ptbl->event==NULL) pack->state=PC_STATUS_ERR_CMD;
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
		/*ң�ص����ȼ��ȹ��ػ��ߣ�����ֱ�ӷ���*/
		if(elecmbile_status.controled_by_rf433==true) continue;
		Idle_Cnt_Pc_Cmd=0;
		p=PcCom_get_data();
		if(pc_cmd_decode(p->data,&cmd)==0)        //���յ�һ����ȷ�İ�
		{
			elecmbile_status.controled_by_pc=true;
			/*������Ӧ����*/
			pc_cmd_process(&cmd);
			/*�崦����*/
			memset(&cmd,0,sizeof(pc_cmd_t));
			memset(p->data,0,PACKET_PC_MAX_LEN);
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
		/*�������ݸ�PC��*/
		pc_cmd_respond(&cmd_rms2pc);
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_PERIODIC,&err);
	}
}

handler_result_t pc_cmd_handler_sync(pc_cmd_t *in_out_pcmd) 
{
	/*do nothing*/
	in_out_pcmd->len=0;
	return HANDLER_NOT_REPLY;
}            	     
handler_result_t pc_cmd_handler_sent_rms_state(pc_cmd_t *in_out_pcmd)
{
	in_out_pcmd->len=0;	
	return HANDLER_OK;
}
handler_result_t pc_cmd_handler_sent_talkback_req(pc_cmd_t *in_out_pcmd) 
{
	in_out_pcmd->len=0;
	return HANDLER_OK;
}
/*CMD==80,�����˵������ƣ����ýǶ�*/
handler_result_t pc_cmd_handler_set_navigation(pc_cmd_t *in_out_pcmd)
{
	short speed=(unsigned short)in_out_pcmd->data[6]<<8|in_out_pcmd->data[7];
	int8_t d_gear=in_out_pcmd->data[5];
	float d_angel=front_gear_to_angel(d_gear);
	float c_angel=get_front_angel();
	/*��������������ײ������*/
	if(elecmbile_status.us_enable==1&&elecmbile_status.us_head_status==1&&speed>0) 
		speed=0;
	if(elecmbile_status.us_enable==1&&elecmbile_status.us_tail_status==1&&speed<0)
		speed=0;
	if(elecmbile_status.crash_strip==1)
		speed=0;
	back_wheel_set_speed(speed);
	if(fabs(c_angel-d_angel)<Err_Angel) 
		goto ret_p;
	if(c_angel<d_angel)  //���ұ�������
		front_wheel_turn(DIR_LEFT,3500,d_angel);
	else
		front_wheel_turn(DIR_RIGHT,3500,d_angel);
ret_p:	
	in_out_pcmd->len=0;
	in_out_pcmd->state=PC_STATUS_SUCCESS;	
	return HANDLER_OK;
}

/*CMD==0x81,�����˿��ƣ�����ǰ�����ٶ�*/
handler_result_t pc_cmd_handler_set_rms_control(pc_cmd_t *pcmd)
{
	sensor_ctl_t  sensor_ctl;
	s8 f_angel=(s8)pcmd->data[5];//  ǰ���ٶ�
	s16 b_speed=(pcmd->data[7])<<8|(pcmd->data[6]);//�����ٶ�
	memcpy(&sensor_ctl, &pcmd->data[8], sizeof(sensor_ctl));
	if(f_angel>0)   //TODO:����ǽǶȵ��ٶȣ����滹Ҫ��
		front_wheel_turn(DIR_RIGHT,3500,Min_Angel);
	else if(f_angel<0)
	  front_wheel_turn(DIR_LEFT,3500,Max_Angel);
	else
	  front_wheel_stop();
	/*����ײ������ͳ���������*/
	if(elecmbile_status.us_enable==1&&elecmbile_status.us_head_status==1&&b_speed>0) 
		b_speed=0;
	if(elecmbile_status.us_enable==1&&elecmbile_status.us_tail_status==1&&b_speed<0)
		b_speed=0;
	if(elecmbile_status.crash_strip==1)
		b_speed=0;	
	
	back_wheel_set_speed(b_speed);
	Relay_frontlight(sensor_ctl.front_light,0);
	Relay_backlight(sensor_ctl.back_light,0);
  Relay_brakelight(sensor_ctl.brake_light,0);
	Relay_leftlight(sensor_ctl.left_light,0);
	Relay_rightlight(sensor_ctl.right_light,0);
	Relay_alarmlight(sensor_ctl.light_alarm,0);
  Relay_talkback_module(sensor_ctl.talk_back,0);
	//Relay_highspeaker();
	return HANDLER_OK;
} 		     
/*����ɲ��*/
handler_result_t pc_cmd_handler_set_emergency_brake(pc_cmd_t *pcmd) 
{
	/*���ú����ٶ�Ϊ0*/
	back_wheel_set_speed(0);
	/*ǰ��ֹͣ*/
	front_wheel_stop();
	return HANDLER_OK;
}
/*ת��ָ���ĽǶȣ�*/
handler_result_t pc_cmd_handler_set_f_wheel_turn(pc_cmd_t *pcmd)
{
	s8 f_angel=(s8)pcmd->data[5];
	if(f_angel>0)
		front_wheel_turn(DIR_RIGHT,3500,Min_Angel);
	else if(f_angel<0)
	  front_wheel_turn(DIR_LEFT,3500,Max_Angel);
	else
	  front_wheel_stop();
	return HANDLER_OK;
}
/*���������ٶ�*/
handler_result_t pc_cmd_handler_set_b_wheel_speed(pc_cmd_t *pcmd) 
{
	s16 speed=(pcmd->data[6])<<8|(pcmd->data[5]);
	/*����ײ������ͳ���������*/
	if(elecmbile_status.us_enable==1&&elecmbile_status.us_head_status==1&&speed>0) 
		speed=0;
	if(elecmbile_status.us_enable==1&&elecmbile_status.us_tail_status==1&&speed<0)
		speed=0;
	if(elecmbile_status.crash_strip==1)
		speed=0;	
	back_wheel_set_speed(speed);
	return HANDLER_OK;
}	  

handler_result_t pc_cmd_handler_set_front_light(pc_cmd_t *pcmd) 
{
	char f_lamp_status=pcmd->data[5];
	Relay_frontlight(f_lamp_status&0x03,0);
	return HANDLER_OK;
}	       
handler_result_t pc_cmd_handler_set_back_light(pc_cmd_t *pcmd)
{
	char b_lamp_status=pcmd->data[5];
	Relay_backlight(b_lamp_status&0x01,0);
	return HANDLER_OK;
} 		       
handler_result_t pc_cmd_handler_set_brake_light(pc_cmd_t *pcmd)
{
	char b_lamp_status=pcmd->data[5];
	Relay_brakelight(b_lamp_status,0);
	return HANDLER_OK;
} 	       
handler_result_t pc_cmd_handler_set_turn_left_light(pc_cmd_t *pcmd) //1��˸ 0�ر�
{
	char lamp_status=pcmd->data[5];
	Relay_leftlight(lamp_status,0);
	return HANDLER_OK;
}      
handler_result_t pc_cmd_handler_set_turn_right_light(pc_cmd_t *pcmd) //1��˸ 0�ر�
{
	char lamp_status=pcmd->data[5];
  Relay_rightlight(lamp_status,0);	
	return HANDLER_OK;
}    
handler_result_t pc_cmd_handler_set_sound_light_alarm(pc_cmd_t *pcmd) 
{
	char lamp_status=pcmd->data[5];
	Relay_alarmlight(lamp_status,0);	
	return HANDLER_OK;
}   

handler_result_t pc_cmd_handler_set_talkback_module(pc_cmd_t *pcmd) 
{
	char lamp_status=pcmd->data[5];
	Relay_talkback_module(lamp_status,0);
	return HANDLER_OK;
}
/*���÷�����ٶ�*/         	   
handler_result_t pc_cmd_handler_set_direction_speed(pc_cmd_t *pcmd)
{
	s8 f_angel=(s8)pcmd->data[5];
	s16 b_speed=(pcmd->data[7])<<8|(pcmd->data[6]);
	if(f_angel>0)
		front_wheel_turn(DIR_RIGHT,3500,Min_Angel);
	else if(f_angel<0)
	  front_wheel_turn(DIR_LEFT,3500,Max_Angel);
	else
		front_wheel_stop();
	/*����ײ������ͳ���������*/
	if(elecmbile_status.us_enable==1&&elecmbile_status.us_head_status==1&&b_speed>0) 
		b_speed=0;
	if(elecmbile_status.us_enable==1&&elecmbile_status.us_tail_status==1&&b_speed<0)
		b_speed=0;
	if(elecmbile_status.crash_strip==1)
		b_speed=0;	
	
	back_wheel_set_speed(b_speed);
	return HANDLER_OK;
}      
handler_result_t pc_cmd_handler_set_high_decibel_speaker(pc_cmd_t *pcmd) 
{
	char lamp_status=pcmd->data[5];
	Relay_highspeaker(lamp_status,0);
	return HANDLER_OK;
}
/*����ɲ���Ƶ�״̬*/
handler_result_t pc_cmd_handler_set_bright_light(pc_cmd_t *pcmd) 
{
	char lamp_status=pcmd->data[5];
	Relay_brightlight(lamp_status,0);
	return HANDLER_OK;
}
handler_result_t pc_cmd_handler_set_infrared_light(pc_cmd_t *pcmd)   
{
	char lamp_status=pcmd->data[5];
	Relay_infraredlight(lamp_status,0);
	return HANDLER_OK;
}    
handler_result_t pc_cmd_handler_set_speaker_mute(pc_cmd_t *pcmd)
{
	char lamp_status=pcmd->data[5];
	Relay_speakermute(lamp_status,0);
	return HANDLER_OK;
}         
handler_result_t pc_cmd_handler_set_navi_bright_light(pc_cmd_t *pcmd) 
{
	char lamp_status=pcmd->data[5];
	Relay_navibright(lamp_status,0);
	return HANDLER_OK;
}

handler_result_t pc_cmd_handler_set_yuntai(pc_cmd_t *pcmd) //��ʱû����̨
{
	/*TODO,����ط�Ӧ����485ָ��*/
	return HANDLER_OK;
} 
/*���ó�������״̬*/
handler_result_t pc_cmd_handler_set_upl_state(pc_cmd_t *pcmd)
{
	elecmbile_status.us_enable=elecmbile_status.us_enable;
	return HANDLER_OK;
}

handler_result_t pc_cmd_handler_set_sys_mode(pc_cmd_t *pcmd) 
{
	return HANDLER_OK;
}		
