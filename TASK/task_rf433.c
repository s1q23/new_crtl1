#include "task_rf433.h"
#include "bsp_usart_433.h"
#include "bsp_gpo.h"
#include "bsp_front_wheel.h"
#include "bsp_back_wheel.h"
#include "task_timer.h"


OS_TCB Rf433TaskTCB;                         //������ƿ�
CPU_STK RF_433_TASK_STK[RF433_STK_SIZE];     //�����ջ	

rf433_cmd_t rf433_cmd;
void rf_433_action(rf433_cmd_t *cmd);

void rf_433_task(void *p_arg)
{
	OS_ERR err;
	usart_433_buffer *p=NULL;
	while(1)
	{
		OSFlagPend(&IRQ_EVENTs,EVENTS_RF433_CMD,0,OS_OPT_PEND_FLAG_SET_ALL|OS_OPT_PEND_FLAG_CONSUME,0,&err);
		Idle_Cnt_Rf433_Cmd=0;
		/*�������С����ң��������*/
		elecmbile_status.controled_by_rf433=true;
		/*1.��ȡ���ݰ�*/
		p=Rf433_get_data();
		/*2.��ͷ�Ͱ�β����*/
		if((p->data[0]==0xAA)&&(p->data[7] == 0xBB)&&(p->data[11]==0xCC))
		{
			memcpy(rf433_cmd.dat,p->data,RF433_CMD_LEN);
			rf_433_action(&rf433_cmd);
			memset(p,0,RF433_CMD_LEN);
		}
	}
}

static void rf_433_motor_action(rf433_cmd_t *cmd)
{
	s16 cmd_x=0, cmd_y=0;unsigned char b_dir=DIR_STOP;
	/*1.����ҡ�˵ķ�������*/
	cmd_x = (rf433_cmd.dat[1] << 8) + rf433_cmd.dat[2];
	cmd_y = (rf433_cmd.dat[3] << 8) + rf433_cmd.dat[4];
	if (cmd_x != 0 && cmd_x < SYS_MIN_FRONT_WHEEL_SPEED && cmd_x > -SYS_MIN_FRONT_WHEEL_SPEED) //���ٶ���Ϊ0
		cmd_x=0;  //��̨����λ
	if (cmd_y != 0 && cmd_y < SYS_MIN_BACK_WHEEL_SPEED && cmd_y > -SYS_MIN_BACK_WHEEL_SPEED) //���ٶ���Ϊ0
		cmd_y=0;  //��̨����λ
	/*�õ�������Ϣ*/
	if(elecmbile_status.scram_rf433==1||elecmbile_status.crash_strip==1) cmd_y=0;
	if(cmd_y>0) b_dir=DIR_FRONT;
	else if(cmd_y<0) b_dir=DIR_BACK;
	else b_dir=DIR_STOP;
	/*������״̬��ѯ,����Լ��������������ʱ���������ܶ���ǰ����ת��*/
	if(elecmbile_status.us_enable==1&&elecmbile_status.us_head_status==1&&b_dir==DIR_FRONT)
	{
		/*��Ϊ����������ʧ�ܣ���Ҫ��ɲ����,1.5S*/
		Idle_Cnt_US=0;
		Relay_brakelight(RELAY_ON,0);
		cmd_y=0;
	}
	if(elecmbile_status.us_enable==1&&elecmbile_status.us_tail_status==1&&b_dir==DIR_BACK)
	{
		/*��Ϊ����������ʧ�ܣ���Ҫ��ɲ����,1.5S*/
		Idle_Cnt_US=0;
		Relay_brakelight(RELAY_ON,0);
		cmd_y=0;
	}
	if((cmd_x==0 && cmd_y==0)||(rf433_cmd.dat[5]==0x01 && rf433_cmd.dat[6]==0x00))
	{ 
		/*ͣ��,�ر�ǰ�ֺͺ���*/
		front_wheel_stop();
		back_wheel_set_speed(0);
	}
	else
	{
		if(cmd_x>0) front_wheel_turn(DIR_RIGHT,3800,Max_Angel);
		else if(cmd_x<0) front_wheel_turn(DIR_LEFT,3800,Min_Angel);
		else front_wheel_stop();
		
		if(cmd_y!=0) back_wheel_set_speed(cmd_y);
		else back_wheel_set_speed(0);
	}
}

static void rf_433_button_action(rf433_cmd_t *cmd)
{
	u8 ctl_cmd1;
	u8 ctl_cmd2;
	static	u8 bt_EmgStop;
	static	u8 bt_FrtLight;
	static	u8 bt_AlarmLight;
	static	u8 bt_UltraSonic;
	static	u8 bt_HdBSpk;
	static	u8 bt_NAVI_1;
	static	u8 bt_NAVI_2;
	static	u8 bt8;
	static	u8 bt9;

	static	u8 bt_EmgStop_last=0xAA;
	static	u8 bt_FrtLight_last=0xAA;
	static	u8 bt_AlarmLight_last=0xAA;
	static	u8 bt_UltraSonic_last=0xAA;
	static	u8 bt_HdBSpk_last=0xAA;
	static	u8 bt_NAVI_1_last=0xAA;
	static	u8 bt_NAVI_2_last=0xAA;
	static	u8 bt8_last=0xAA;
	static	u8 bt9_last=0xAA;
	
	ctl_cmd1 = cmd->dat[5];
	ctl_cmd2 = cmd->dat[6];
	bt_EmgStop = ctl_cmd1 & 0x01;     //but1   ��ͣ��ť
	bt_FrtLight = ctl_cmd1 & 0x02;    //but2   ǰ��
	bt_AlarmLight = ctl_cmd1 & 0x04;  //but3   ��˸��
	bt_UltraSonic = ctl_cmd1 & 0x08;  //but4   ������
	bt_HdBSpk = ctl_cmd1 & 0x10;      //but5   ���ȱ���
	bt_NAVI_1 = ctl_cmd1 & 0x20;      //but6   ����
	bt_NAVI_2 = ctl_cmd2 & 0x02;      //but7   Ѱ������
	bt8 = ctl_cmd1 & 0x80;            //but8
	bt9 = ctl_cmd2 & 0x01;            //but9   �����ؼ�
	if(bt_EmgStop!=bt_EmgStop_last)   //  ��ͣ�� ״̬�ı�
	{
		if(bt_EmgStop != 0)// ��ͣ
		{
			/*1.����ֹͣ*/
			back_wheel_set_speed(0);
			/*2.ǰ��ֹͣ*/
			front_wheel_stop();
			/*3.״̬�޸�*/
			elecmbile_status.scram_rf433=true;//���� �˶����ƵĲ����޷�ִ�У����������������ң�����������ң������ʱ
		}
		else                    //ȡ����ͣ
		{
			elecmbile_status.scram_rf433=false;
		}
  }
	
	if(bt_FrtLight!=bt_FrtLight_last)  // ǰ������
	{
		if(bt_FrtLight != 0)                 // ��ǰ��
		{
			Relay_frontlight(RELAY_ON,0);
		}
		else                                     // ��ǰ�� 
		{
			Relay_frontlight(RELAY_OFF,0);
		}
  }
	
	if(bt_AlarmLight!=bt_AlarmLight_last)  // ������
	{
		if(bt_AlarmLight != 0)               // ��������
		{		
			Relay_alarmlight(RELAY_ON,0);
		}
		else
		{
			Relay_alarmlight(RELAY_OFF,0);
		}
  }
	
	if(bt_UltraSonic!=bt_UltraSonic_last)  // ������
	{
		if(bt_UltraSonic!= 0)               //���ó�����
		{	
			elecmbile_status.us_enable=0;
			rms_state.upl_state=0;
		}
		else                                   // ���ó�����
		{
			elecmbile_status.us_enable=1;
			rms_state.upl_state=1;
		}
  }
	
	if(bt_HdBSpk!=bt_HdBSpk_last)  // ��������
	{
		if(bt_HdBSpk != 0)               //��
		{
			Relay_highspeaker(RELAY_ON,0);
		}
		else                                   // �ر�
		{
			Relay_highspeaker(RELAY_OFF,0);
		}
  }
	if(bt_NAVI_1!=bt_NAVI_1_last)  //  NAVI ����
	{
		if(bt_NAVI_1 != 0) //
		{
		}
		else
		{
		}
  }	
	if(bt_NAVI_2!=bt_NAVI_2_last)  //  NAVI ����
	{
		if(bt_NAVI_2 != 0) //
		{
		}
		else
		{
		}
  }
	if(bt8!=bt8_last)
	{
		if(bt8!= 0) //
		{
		}
		else
		{
		}
  }	
	if(bt9!=bt9_last)
	{
		if(bt9!= 0) //
		{
		}
		else
		{
		}
  }		
	if(ctl_cmd2 == 0x08) //��ϼ�    ��������
	{
		
	}
	bt_EmgStop_last    = bt_EmgStop;
	bt_FrtLight_last   = bt_FrtLight;
	bt_AlarmLight_last = bt_AlarmLight;
	bt_UltraSonic_last = bt_UltraSonic;
	bt_HdBSpk_last     = bt_HdBSpk;	
	bt_NAVI_1_last     = bt_NAVI_1;	
	bt_NAVI_2_last     = bt_NAVI_2;	
	bt8_last           = bt8;	
	bt9_last           = bt9;	
}

void rf_433_action(rf433_cmd_t *cmd)
{
	rf_433_button_action(cmd);
	rf_433_motor_action(cmd);
	memset(cmd->dat,0,RF433_CMD_LEN);
}
