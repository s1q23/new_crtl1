#include "task_rf433.h"
#include "bsp_usart_433.h"
#include "bsp_gpo.h"
#include "bsp_front_wheel.h"
#include "bsp_back_wheel.h"
//#include "task_pc_com.h"

//	ctl_cmd_button1 = ctl_cmd1 & 0x01;  //but1   ��ͣ��ť
//	ctl_cmd_button2 = ctl_cmd1 & 0x02;  //but2   ǰ��
//	ctl_cmd_button3 = ctl_cmd1 & 0x04;  //but3   ��˸��
//	ctl_cmd_button4 = ctl_cmd1 & 0x08;  //but4   ������
//	ctl_cmd_button5 = ctl_cmd1 & 0x10;  //but5   ���ȱ���
//	ctl_cmd_button6 = ctl_cmd1 & 0x20;  //but6   ���ȱ���
//	ctl_cmd_button7 = ctl_cmd2 &0x02;   //but7   Ѱ������
//	ctl_cmd_button8 = ctl_cmd1 & 0x80;  //but6   ���ȱ���
//	ctl_cmd_button9 = ctl_cmd2 & 0x01;  //but9   �����ؼ�

#define BIT_EMG_STOP           (0x01)
#define BIT_HEAD_LAMP          (0x02)
#define BIT_FLASH_LAMP         (0x04)
#define BIT_US_LAMP            (0x08)
#define BIT_BELL_ALARM         (0x10)
#define BIT_NAVI_1             (0x20)



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
		LED1=!LED1; 
		/*0.������*/
		p=Rf433_get_data();
		/*1.�������飬���ڲ��ù̶����������԰�����Ҳ��Ϊһ��������*/
		if(p->len!=RF433_CMD_LED) continue;
		/*2.��ͷ�Ͱ�β����*/
		if((p->data[0]==0xAA)&&(p->data[7] == 0xBB)&&(p->data[11]==0xCC))
		{
			/*����ɹ�*/
			memcpy(rf433_cmd.dat,&p->data[1],p->len);
			rf_433_action(&rf433_cmd);
		}
	}
}

static void rf_433_motor_action(rf433_cmd_t *cmd)
{
	s16 cmd_x=0, cmd_y=0;
	/*1.����ҡ�˵ķ�������*/
	cmd_x = (rf433_cmd.dat[1] << 8) + rf433_cmd.dat[2];
	cmd_y = (rf433_cmd.dat[3] << 8) + rf433_cmd.dat[4];
	if (cmd_x != 0 && cmd_x < SYS_MIN_FRONT_WHEEL_SPEED && cmd_x > -SYS_MIN_FRONT_WHEEL_SPEED) //���ٶ���Ϊ0
		cmd_x=0;  //��̨����λ
	if (cmd_y != 0 && cmd_y < SYS_MIN_BACK_WHEEL_SPEED && cmd_y > -SYS_MIN_BACK_WHEEL_SPEED) //���ٶ���Ϊ0
		cmd_y=0;  //��̨����λ
	
	if ((cmd_x == 0 && cmd_y == 0) ||(rf433_cmd.dat[5] == 0x01 && rf433_cmd.dat[6] == 0x00) )
	{ 
		/*ͣ��,�ر�ǰ�ֺͺ���*/
		front_wheel_stop();
		can_sent_speed(0);
	}
	else
	{
		if(cmd_x!=0)
		{
			if(cmd_x>0) front_wheel_turn(DIR_LEFT,300,0);
			if(cmd_x<0) front_wheel_turn(DIR_RIGHT,300,0);
		}
		if(cmd_y!=0)
		{
			can_sent_speed(cmd_y);
		}
	}
}

static void rf_433_button_action(rf433_cmd_t *cmd)
{
	u8 ctl_cmd1;
	u8 ctl_cmd2;
	static	u8 ctl_cmd_button1;
	static	u8 ctl_cmd_button2;
	static	u8 ctl_cmd_button3;
	static	u8 ctl_cmd_button4;
	static	u8 ctl_cmd_button5;
	static	u8 ctl_cmd_button6;
	static	u8 ctl_cmd_button7;
	static	u8 ctl_cmd_button8;
	static	u8 ctl_cmd_button9;
	
	static	u8 ctl_cmd_button1_last=0;
	static	u8 ctl_cmd_button2_last=0;
	static	u8 ctl_cmd_button3_last=0;
	static	u8 ctl_cmd_button4_last=0;
	static	u8 ctl_cmd_button5_last=0;
	static	u8 ctl_cmd_button6_last=0;
	static	u8 ctl_cmd_button7_last=0;
	static	u8 ctl_cmd_button8_last=0;
	static	u8 ctl_cmd_button9_last=0;
	
	ctl_cmd1 = cmd->dat[5];
	ctl_cmd2 = cmd->dat[6];
	ctl_cmd_button1 = ctl_cmd1 & 0x01;  //but1   ��ͣ��ť
	ctl_cmd_button2 = ctl_cmd1 & 0x02;  //but2   ǰ��
	ctl_cmd_button3 = ctl_cmd1 & 0x04;  //but3   ��˸��
	ctl_cmd_button4 = ctl_cmd1 & 0x08;  //but4   ������
	ctl_cmd_button5 = ctl_cmd1 & 0x10;  //but5   ���ȱ���
	ctl_cmd_button6 = ctl_cmd1 & 0x20;  //but6   ���ȱ���
	ctl_cmd_button7 = ctl_cmd2 &0x02;   //but7   Ѱ������
	ctl_cmd_button8 = ctl_cmd1 & 0x80;  //but6   ���ȱ���
	ctl_cmd_button9 = ctl_cmd2 & 0x01;  //but9   �����ؼ�
	if(ctl_cmd_button1!=ctl_cmd_button1_last) //  ��ͣ�� ״̬�ı�
	{
		if(ctl_cmd_button1 != 0)// ��ͣ
		{
			/*1.����ֹͣ*/
			can_sent_speed(0);
			/*2.ǰ��ֹͣ*/
			front_wheel_stop();
			/*3.״̬�޸�*/
			elecmbile_status.scram_rf433=1;//���� �˶����ƵĲ����޷�ִ��
		}
		else                    //ȡ����ͣ
		{
			elecmbile_status.scram_rf433=0;
		}
  }
	
	if(ctl_cmd_button2!=ctl_cmd_button2_last)// ǰ������
	{
		if(ctl_cmd_button2 != 0)               // ��ǰ��
		{
			Relay_frontlight(RELAY_ON,0);
		}
		else                                   // ��ǰ�� 
		{
			Relay_frontlight(RELAY_OFF,0);
		}	
  }
	
	if(ctl_cmd_button3!=ctl_cmd_button3_last)// ������
	{
		if(ctl_cmd_button3 != 0)               // ��������
		{		
			Relay_alarmlight(RELAY_ON,0);
		}
		else
		{
			Relay_alarmlight(RELAY_OFF,0);
		}
  }
	
	if(ctl_cmd_button4!=ctl_cmd_button4_last)// ������
	{
		if(ctl_cmd_button4 != 0)               //���ó�����
		{	
		}
		else                                   // ���ó�����
		{
		}
  }
	
	if(ctl_cmd_button5!=ctl_cmd_button5_last)// ��������
	{
		if(ctl_cmd_button5 != 0)               //��
		{
			Relay_highspeaker(RELAY_ON,0);
		}
		else                                   // �ر�
		{
			Relay_highspeaker(RELAY_OFF,0);
		}
  }
	if(ctl_cmd_button6!=ctl_cmd_button6_last) //  NAVI ����
	{
		if(ctl_cmd_button6 != 0) //
		{
		}
		else
		{
		}
  }	
	if(ctl_cmd_button7!=ctl_cmd_button7_last)//  NAVI ����
	{
		if(ctl_cmd_button7 != 0) //
		{
		}
		else
		{
		}
  }
	if(ctl_cmd_button8!=ctl_cmd_button8_last)
	{
		if(ctl_cmd_button8 != 0) //
		{
		}
		else
		{
		}
  }	
	if(ctl_cmd_button9!=ctl_cmd_button9_last)
	{
		if(ctl_cmd_button9 != 0) //
		{
		}
		else
		{
		}
  }		
	if(ctl_cmd2 == 0x08) //��ϼ�    ��������
	{
		
	}
	ctl_cmd_button1_last = ctl_cmd_button1;
	ctl_cmd_button2_last = ctl_cmd_button2;
	ctl_cmd_button3_last = ctl_cmd_button3;
	ctl_cmd_button4_last = ctl_cmd_button4;
	ctl_cmd_button5_last = ctl_cmd_button5;	
	ctl_cmd_button6_last = ctl_cmd_button6;	
	ctl_cmd_button7_last = ctl_cmd_button7;	
	ctl_cmd_button8_last = ctl_cmd_button8;	
	ctl_cmd_button9_last = ctl_cmd_button9;	
}

void rf_433_action(rf433_cmd_t *cmd)
{
	rf_433_motor_action(cmd);
	rf_433_button_action(cmd);
	memset(cmd->dat,0,RF433_CMD_LED);
}
