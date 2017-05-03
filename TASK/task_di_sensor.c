#include "task_di_sensor.h"
#include "bsp_gpo.h"

OS_TCB  DITaskTCB;                
CPU_STK DI_TASK_STK[DI_STK_SIZE];

/*读取数字输入信息的状态,20ms读取一次*/
void task_di_sensor(void *p_arg)
{
	OS_ERR err;
	int BrknGls_Cnt=0,Shake_Cnt=0,Smog_Cnt=0,Water_Cnt=0;
	while(1)
	{
		/*玻璃破碎传感器输入*/
		if(pBrokenGlassState==0)
		{
			BrknGls_Cnt++;
		}
		else
		{
			BrknGls_Cnt=0;
		}
		/*振动传感器输入*/
		if(pShakeState==0)
		{
			Shake_Cnt++;
		}
		else
		{
			Shake_Cnt=0;
			rms_state.vibration=0;
		}
		/*烟雾传感器输入*/
		if(pSomkeSensor==0)
		{
			Smog_Cnt++;
		}
		else
		{
			Smog_Cnt=0;
			rms_state.smoke_state=0;
		}
		/*水位传感器的检测*/
		if(pWaterState==0)
		{
			Water_Cnt++;
		}
		else
		{
			Water_Cnt=0;
			rms_state.water_state=0;
			Idle_Cnt_Water=0;   
		}
		/*急停按钮输入*/
		if(pEmgBtnState==0)                // 松掉急停按钮
		{
			if(elecmbile_status.emg_btn_state!=0)
			{
				Idle_Cnt_ReInit=0;
				elecmbile_status.emg_btn_state=0;
			}
		}
		else   // 拍下急停按钮                    
		{
			elecmbile_status.emg_btn_state=1;
		}		
		/*玻璃破碎报警*/		
		if(BrknGls_Cnt>FILTER_TIMES)
		{
			/*确保不要连续的触发报警事件*/
			BrknGls_Cnt=-60;
			/*报警*/
			Idle_Cnt_BrkGls=0;
			Relay_alarmlight(RELAY_ON,0);
		}
		/*振动报警*/
		if(Shake_Cnt>FILTER_TIMES)
		{
			Shake_Cnt=-60;
			/*报警*/
			Idle_Cnt_Shake=0;
			Relay_highspeaker(RELAY_ON,0);
		}
		/*烟雾报警*/
		if(Smog_Cnt>FILTER_TIMES)
		{
			Smog_Cnt=-60;
			/*报警*/
			rms_state.smoke_state=1;
			Idle_Cnt_Smog=0;       //
			Relay_highspeaker(RELAY_ON,0);
		}
		if(Water_Cnt>FILTER_TIMES)
		{
			Water_Cnt=-60;
			rms_state.water_state=1;
			Relay_alarmlight(RELAY_OFF,0);
		}
		/*处理电源电压问题*/
		if(SchmittTrigger(rms_state.battery_voltage,BATA_VOLTAGE_LOW,BATA_VOLTAGE_ERR)==1)
		{
			Set_PowerLED(PWRLED_GREEN);
		}
		if(SchmittTrigger(rms_state.battery_voltage,BATA_VOLTAGE_LOW,BATA_VOLTAGE_ERR)==-1)
		{
			Set_PowerLED(PWRLED_RED);
		}
		/*数组输入，100ms读取一次*/
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_PERIODIC,&err);
	}
}
