#include "task_rf433.h"
#include "bsp_usart_433.h"
#include "bsp_gpo.h"
#include "bsp_front_wheel.h"
#include "bsp_back_wheel.h"
//#include "task_pc_com.h"

//	ctl_cmd_button1 = ctl_cmd1 & 0x01;  //but1   急停按钮
//	ctl_cmd_button2 = ctl_cmd1 & 0x02;  //but2   前灯
//	ctl_cmd_button3 = ctl_cmd1 & 0x04;  //but3   闪烁灯
//	ctl_cmd_button4 = ctl_cmd1 & 0x08;  //but4   超声波
//	ctl_cmd_button5 = ctl_cmd1 & 0x10;  //but5   喇叭报警
//	ctl_cmd_button6 = ctl_cmd1 & 0x20;  //but6   喇叭报警
//	ctl_cmd_button7 = ctl_cmd2 &0x02;   //but7   寻迹功能
//	ctl_cmd_button8 = ctl_cmd1 & 0x80;  //but6   喇叭报警
//	ctl_cmd_button9 = ctl_cmd2 & 0x01;  //but9   导航回家

#define BIT_EMG_STOP           (0x01)
#define BIT_HEAD_LAMP          (0x02)
#define BIT_FLASH_LAMP         (0x04)
#define BIT_US_LAMP            (0x08)
#define BIT_BELL_ALARM         (0x10)
#define BIT_NAVI_1             (0x20)



OS_TCB Rf433TaskTCB;                         //任务控制块
CPU_STK RF_433_TASK_STK[RF433_STK_SIZE];     //任务堆栈	

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
		/*0.拿数据*/
		p=Rf433_get_data();
		/*1.包长检验，由于采用固定包长，所以包长度也作为一个检验项*/
		if(p->len!=RF433_CMD_LED) continue;
		/*2.包头和包尾检验*/
		if((p->data[0]==0xAA)&&(p->data[7] == 0xBB)&&(p->data[11]==0xCC))
		{
			/*解码成功*/
			memcpy(rf433_cmd.dat,&p->data[1],p->len);
			rf_433_action(&rf433_cmd);
		}
	}
}

static void rf_433_motor_action(rf433_cmd_t *cmd)
{
	s16 cmd_x=0, cmd_y=0;
	/*1.解析摇杆的方向数据*/
	cmd_x = (rf433_cmd.dat[1] << 8) + rf433_cmd.dat[2];
	cmd_y = (rf433_cmd.dat[3] << 8) + rf433_cmd.dat[4];
	if (cmd_x != 0 && cmd_x < SYS_MIN_FRONT_WHEEL_SPEED && cmd_x > -SYS_MIN_FRONT_WHEEL_SPEED) //低速度设为0
		cmd_x=0;  //后台不限位
	if (cmd_y != 0 && cmd_y < SYS_MIN_BACK_WHEEL_SPEED && cmd_y > -SYS_MIN_BACK_WHEEL_SPEED) //低速度设为0
		cmd_y=0;  //后台不限位
	
	if ((cmd_x == 0 && cmd_y == 0) ||(rf433_cmd.dat[5] == 0x01 && rf433_cmd.dat[6] == 0x00) )
	{ 
		/*停车,关闭前轮和后轮*/
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
	ctl_cmd_button1 = ctl_cmd1 & 0x01;  //but1   急停按钮
	ctl_cmd_button2 = ctl_cmd1 & 0x02;  //but2   前灯
	ctl_cmd_button3 = ctl_cmd1 & 0x04;  //but3   闪烁灯
	ctl_cmd_button4 = ctl_cmd1 & 0x08;  //but4   超声波
	ctl_cmd_button5 = ctl_cmd1 & 0x10;  //but5   喇叭报警
	ctl_cmd_button6 = ctl_cmd1 & 0x20;  //but6   喇叭报警
	ctl_cmd_button7 = ctl_cmd2 &0x02;   //but7   寻迹功能
	ctl_cmd_button8 = ctl_cmd1 & 0x80;  //but6   喇叭报警
	ctl_cmd_button9 = ctl_cmd2 & 0x01;  //but9   导航回家
	if(ctl_cmd_button1!=ctl_cmd_button1_last) //  急停键 状态改变
	{
		if(ctl_cmd_button1 != 0)// 急停
		{
			/*1.后驱停止*/
			can_sent_speed(0);
			/*2.前驱停止*/
			front_wheel_stop();
			/*3.状态修改*/
			elecmbile_status.scram_rf433=1;//锁定 运动控制的操作无法执行
		}
		else                    //取消急停
		{
			elecmbile_status.scram_rf433=0;
		}
  }
	
	if(ctl_cmd_button2!=ctl_cmd_button2_last)// 前灯命令
	{
		if(ctl_cmd_button2 != 0)               // 开前灯
		{
			Relay_frontlight(RELAY_ON,0);
		}
		else                                   // 关前灯 
		{
			Relay_frontlight(RELAY_OFF,0);
		}	
  }
	
	if(ctl_cmd_button3!=ctl_cmd_button3_last)// 报警灯
	{
		if(ctl_cmd_button3 != 0)               // 开报警灯
		{		
			Relay_alarmlight(RELAY_ON,0);
		}
		else
		{
			Relay_alarmlight(RELAY_OFF,0);
		}
  }
	
	if(ctl_cmd_button4!=ctl_cmd_button4_last)// 超声波
	{
		if(ctl_cmd_button4 != 0)               //禁用超声波
		{	
		}
		else                                   // 启用超声波
		{
		}
  }
	
	if(ctl_cmd_button5!=ctl_cmd_button5_last)// 高音喇叭
	{
		if(ctl_cmd_button5 != 0)               //打开
		{
			Relay_highspeaker(RELAY_ON,0);
		}
		else                                   // 关闭
		{
			Relay_highspeaker(RELAY_OFF,0);
		}
  }
	if(ctl_cmd_button6!=ctl_cmd_button6_last) //  NAVI 导航
	{
		if(ctl_cmd_button6 != 0) //
		{
		}
		else
		{
		}
  }	
	if(ctl_cmd_button7!=ctl_cmd_button7_last)//  NAVI 导航
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
	if(ctl_cmd2 == 0x08) //组合键    返航呼叫
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
