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
/*左转向灯闪烁300 ms 执行一次*/
void cb_TmrLeftLight(void *p_tmr,void *p_arg)
{
	LEFTLIGHT_TOGGLE;
}

/*右转向灯闪烁，300ms执行一次*/
void cb_TmrRightLight(void *p_tmr,void *p_arg)
{
	RIGHTLIGHT_TOGGLE;
}

/*前灯闪烁*/
void cb_TmrFrontLight(void *p_tmr,void *p_arg)
{
	HeadLight_TOGGLE;
}
/************************这个函数50ms执行一次********************/
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
	/*遥控器超时*/
	if(Idle_Cnt_Rf433_Cmd==RF433_TIMEOUT_TIME)// 10*50=500ms,遥控器超时
	{
		/*取消遥控器对小车的控制*/
		elecmbile_status.controled_by_rf433=false;
		/*解除遥控器对小车的锁定*/
		elecmbile_status.scram_rf433=false;
		/*此时如果车子在运动，肯定是遥控器的指令,让它停下来*/
		if(rms_state.speed!=0)
			back_wheel_set_speed(0);
	}
	/*工控机超时*/
	if(Idle_Cnt_Pc_Cmd==PC_TIMEOUT_TIME)   // 50*10=500ms,工控机超时
	{
		/*取消工控机对小车的控制*/
		elecmbile_status.controled_by_pc=false;
		/*工控机超时，并且遥控器不在控制车子，将后驱停下*/
		if((rms_state.speed!=0)&&(elecmbile_status.controled_by_rf433==false)) 
			back_wheel_set_speed(0);
	}
	/*满足条件后，自动回正，50ms执行一次*/
	if((Idle_Cnt_Pc_Cmd>=AUTO_MID_TIME)&&(Idle_Cnt_Rf433_Cmd>=AUTO_MID_TIME))
	{
		front_auto_mid();
	}
	/*急停按钮松掉后，重新初始化后驱*/
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
	/*停车后，关闭电磁刹*/
	if(Idle_Cnt_EmBrake==EM_BRAKE_ATUO_OFF)// 100*50=5S
	{
		/*关闭电磁刹*/
		em_brake_set(DISABLE);
		elecmbile_status.em_brake_status=0;//标记电磁刹状态
	}
	/*刹车灯自动关闭*/
	if(Idle_Cnt_BrakeLight==40) //50*40=2000ms,刹车灯自动关闭 
	{
		/*关闭刹车灯*/
		Relay_brakelight(RELAY_OFF,0);
	}
	/*解除防撞条锁定*/
	if(Idle_Cnt_Crash==60)      // 60*50=3000ms=3S,3秒后解除防撞条锁定
	{
		elecmbile_status.crash_strip=false;
		LED4=1;
	}
	if(Idle_Cnt_US==30)
	{
		Relay_brakelight(RELAY_OFF,0);
	}
	if(Idle_Cnt_BrkGls==100)     //玻璃破碎报警 5S
	{
		Relay_alarmlight(RELAY_OFF,0);
	}
	if(Idle_Cnt_Shake==100)      //振动传感器报警 5S
	{
		Relay_highspeaker(RELAY_OFF,0);
	}
	if(Idle_Cnt_Smog==100)       //烟雾传感器报警  5S
	{
		Relay_highspeaker(RELAY_OFF,0);
	}
	if(Idle_Cnt_Water==100)
	{
		Relay_alarmlight(RELAY_OFF,0);
	}
}

/*这个定时器监测前驱的角度信息，根据角度数据决定转向灯的闪烁*/
void cb_TmrAngelMonitor(void *p_tmr,void *p_arg)
{
	float current_angel=get_front_angel();
	if(current_angel<-Zero_Angel)      /*右转向闪烁*/
	{
		if(elecmbile_status.L_light_state!=RELAY_OFF)
			Relay_leftlight(RELAY_OFF,0);
		if(elecmbile_status.R_light_state!=RELAY_ON)
			Relay_rightlight(RELAY_ON,0);
	}
	else if(current_angel>Zero_Angel)  /*左转向闪烁*/
	{
		if(elecmbile_status.R_light_state!=RELAY_OFF)
			Relay_rightlight(RELAY_OFF,0);
		if(elecmbile_status.L_light_state!=RELAY_ON)
			Relay_leftlight(RELAY_ON,0);
	}
	else                               /*两个灯都停止*/
	{
		Relay_leftlight(RELAY_OFF,0);
		Relay_rightlight(RELAY_OFF,0);
	}
}
