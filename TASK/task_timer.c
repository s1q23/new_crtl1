#include "task_timer.h"
#include "bsp_gpo.h"
#include "bsp_front_wheel.h"
#include "bsp_gpi.h"


OS_TMR TmrLeftLight;
OS_TMR TmrRightLight;

/*��ת�����˸*/
void cb_TmrLeftLight(void *p_tmr,void *p_arg)
{
	LEFTLIGHT_TOGGLE;
}

/*��ת�����˸*/
void cb_TmrRightLight(void *p_tmr,void *p_arg)
{
	RIGHTLIGHT_TOGGLE;
}

/*�Զ������������е�ָ��λ�ÿ���ʹ�����cb*/
void cb_TmrCheckAngel(void *p_tmr,void *p_arg)
{
	/*����50msִ��һ��*/
	float current_angel = get_front_angel();
	if(fabs(current_angel-dest_angel)<Err_Angel)
	{
		front_wheel_stop();
	}
}

void cb_TmrTick()
{
	Idle_Cnt_Stop++;
	Idle_Cnt_Rf433_Cmd++;
	Idle_Cnt_Pc_Cmd++;
	/*ֹͣ����ʱ��ﵽNms���Զ�����*/
	if(Idle_Cnt_Stop==40)
		front_auto_mid();
}
