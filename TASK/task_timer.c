#include "task_timer.h"
#include "bsp_gpo.h"
#include "bsp_front_wheel.h"
#include "bsp_gpi.h"


OS_TMR TmrLeftLight;
OS_TMR TmrRightLight;

/*左转向灯闪烁*/
void cb_TmrLeftLight(void *p_tmr,void *p_arg)
{
	LEFTLIGHT_TOGGLE;
}

/*又转向灯闪烁*/
void cb_TmrRightLight(void *p_tmr,void *p_arg)
{
	RIGHTLIGHT_TOGGLE;
}

/*自动回正或者运行到指定位置可以使用这个cb*/
void cb_TmrCheckAngel(void *p_tmr,void *p_arg)
{
	/*正常50ms执行一次*/
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
	/*停止空闲时间达到Nms，自动回正*/
	if(Idle_Cnt_Stop==40)
		front_auto_mid();
}
