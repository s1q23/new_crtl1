#include "task_timer.h"
#include "bsp_gpo.h"
#include "bsp_front_wheel.h"
#include "bsp_back_wheel.h"
#include "bsp_gpi.h"

OS_TMR TmrLeftLight;
OS_TMR TmrRightLight;
OS_TMR TmrFrontLight;
OS_TMR TmrTimeOutCheck;
OS_TMR TmrAngelMonitor;
/*��ת�����˸300 ms ִ��һ��*/
void cb_TmrLeftLight(void *p_tmr,void *p_arg)
{
	LEFTLIGHT_TOGGLE;
}

/*��ת�����˸��300msִ��һ��*/
void cb_TmrRightLight(void *p_tmr,void *p_arg)
{
	RIGHTLIGHT_TOGGLE;
}

/*ǰ����˸*/
void cb_TmrFrontLight(void *p_tmr,void *p_arg)
{
	HeadLight_TOGGLE;
}
/************************�������50msִ��һ��********************/
void cb_TmrTimeOutCheck(void *p_tmr,void *p_arg)
{
	Idle_Cnt_Stop++;
	Idle_Cnt_Rf433_Cmd++;
	Idle_Cnt_Pc_Cmd++;
	Idle_Cnt_CanRx_From_Drv++;
	Idle_Cnt_EmBrake++;
	Idle_Cnt_BrakeLight++;
	Idle_Cnt_Crash++;
	Idle_Cnt_US++;
	Idle_Cnt_UsHead++;
	Idle_Cnt_UsTail++;
	Idle_Cnt_BrkGls++;
	Idle_Cnt_Shake++;
	Idle_Cnt_Smog++;
	Idle_Cnt_ReInit++;
	Idle_Cnt_Water++;
	/*ң������ʱ*/
	if(Idle_Cnt_Rf433_Cmd==RF433_TIMEOUT_TIME)// 10*50=500ms,ң������ʱ
	{
		/*ȡ��ң������С���Ŀ���*/
		elecmbile_status.controled_by_rf433=false;
		/*���ң������С��������*/
		elecmbile_status.scram_rf433=false;
		/*��ʱ����������˶����϶���ң������ָ��,����ͣ����*/
		if(rms_state.speed!=0)
			back_wheel_set_speed(0);
	}
	/*���ػ���ʱ*/
	if(Idle_Cnt_Pc_Cmd==PC_TIMEOUT_TIME)   // 50*10=500ms,���ػ���ʱ
	{
		/*ȡ�����ػ���С���Ŀ���*/
		elecmbile_status.controled_by_pc=false;
		/*���ػ���ʱ������ң�������ڿ��Ƴ��ӣ�������ͣ��*/
		if((rms_state.speed!=0)&&(elecmbile_status.controled_by_rf433==false)) 
			back_wheel_set_speed(0);
	}
	/*�����������Զ�������50msִ��һ��*/
	if((Idle_Cnt_Pc_Cmd>=AUTO_MID_TIME)&&(Idle_Cnt_Rf433_Cmd>=AUTO_MID_TIME))
	{
		front_auto_mid();
	}
	/*��ͣ��ť�ɵ������³�ʼ������*/
	if(Idle_Cnt_ReInit==200)
	{
		nanuo_can_sent(0x2B, 0x6060, 0x00, 0x0002);
	}
	else if(Idle_Cnt_ReInit==201)
	{
		nanuo_can_sent(0x2B, 0x6040, 0x00, 0x0006);
	}
	else if(Idle_Cnt_ReInit==202)
	{
		nanuo_can_sent(0x2B, 0x6040, 0x00, 0x0007);
	}
	else if(Idle_Cnt_ReInit==203)
	{
		nanuo_can_sent(0x2B, 0x6040, 0x00, 0x000F);
	}
	/*ͣ���󣬹رյ��ɲ*/
	if(Idle_Cnt_EmBrake==EM_BRAKE_ATUO_OFF)// 100*50=5S
	{
		/*�رյ��ɲ*/
		em_brake_set(DISABLE);
		elecmbile_status.em_brake_status=0;//��ǵ��ɲ״̬
	}
	/*ɲ�����Զ��ر�*/
	if(Idle_Cnt_BrakeLight==40) //50*40=2000ms,ɲ�����Զ��ر� 
	{
		/*�ر�ɲ����*/
		Relay_brakelight(RELAY_OFF,0);
	}
	/*�����ײ������*/
	if(Idle_Cnt_Crash==60)      // 60*50=3000ms=3S,3�������ײ������
	{
		elecmbile_status.crash_strip=false;
		LED4=1;
	}
	if(Idle_Cnt_US==30)
	{
		Relay_brakelight(RELAY_OFF,0);
	}
	if(Idle_Cnt_BrkGls==100)     //�������鱨�� 5S
	{
		Relay_alarmlight(RELAY_OFF,0);
	}
	if(Idle_Cnt_Shake==100)      //�񶯴��������� 5S
	{
		Relay_highspeaker(RELAY_OFF,0);
	}
	if(Idle_Cnt_Smog==100)       //������������  5S
	{
		Relay_highspeaker(RELAY_OFF,0);
	}
	if(Idle_Cnt_Water==100)
	{
		Relay_alarmlight(RELAY_OFF,0);
	}
}

/*�����ʱ�����ǰ���ĽǶ���Ϣ�����ݽǶ����ݾ���ת��Ƶ���˸*/
void cb_TmrAngelMonitor(void *p_tmr,void *p_arg)
{
	float current_angel=get_front_angel();
	if(current_angel<-Zero_Angel)      /*��ת����˸*/
	{
		if(elecmbile_status.L_light_state!=RELAY_OFF)
			Relay_leftlight(RELAY_OFF,0);
		if(elecmbile_status.R_light_state!=RELAY_ON)
			Relay_rightlight(RELAY_ON,0);
	}
	else if(current_angel>Zero_Angel)  /*��ת����˸*/
	{
		if(elecmbile_status.R_light_state!=RELAY_OFF)
			Relay_rightlight(RELAY_OFF,0);
		if(elecmbile_status.L_light_state!=RELAY_ON)
			Relay_leftlight(RELAY_ON,0);
	}
	else                               /*�����ƶ�ֹͣ*/
	{
		Relay_leftlight(RELAY_OFF,0);
		Relay_rightlight(RELAY_OFF,0);
	}
}
