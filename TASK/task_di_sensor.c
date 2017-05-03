#include "task_di_sensor.h"
#include "bsp_gpo.h"

OS_TCB  DITaskTCB;                
CPU_STK DI_TASK_STK[DI_STK_SIZE];

/*��ȡ����������Ϣ��״̬,20ms��ȡһ��*/
void task_di_sensor(void *p_arg)
{
	OS_ERR err;
	int BrknGls_Cnt=0,Shake_Cnt=0,Smog_Cnt=0,Water_Cnt=0;
	while(1)
	{
		/*�������鴫��������*/
		if(pBrokenGlassState==0)
		{
			BrknGls_Cnt++;
		}
		else
		{
			BrknGls_Cnt=0;
		}
		/*�񶯴���������*/
		if(pShakeState==0)
		{
			Shake_Cnt++;
		}
		else
		{
			Shake_Cnt=0;
			rms_state.vibration=0;
		}
		/*������������*/
		if(pSomkeSensor==0)
		{
			Smog_Cnt++;
		}
		else
		{
			Smog_Cnt=0;
			rms_state.smoke_state=0;
		}
		/*ˮλ�������ļ��*/
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
		/*��ͣ��ť����*/
		if(pEmgBtnState==0)                // �ɵ���ͣ��ť
		{
			if(elecmbile_status.emg_btn_state!=0)
			{
				Idle_Cnt_ReInit=0;
				elecmbile_status.emg_btn_state=0;
			}
		}
		else   // ���¼�ͣ��ť                    
		{
			elecmbile_status.emg_btn_state=1;
		}		
		/*�������鱨��*/		
		if(BrknGls_Cnt>FILTER_TIMES)
		{
			/*ȷ����Ҫ�����Ĵ��������¼�*/
			BrknGls_Cnt=-60;
			/*����*/
			Idle_Cnt_BrkGls=0;
			Relay_alarmlight(RELAY_ON,0);
		}
		/*�񶯱���*/
		if(Shake_Cnt>FILTER_TIMES)
		{
			Shake_Cnt=-60;
			/*����*/
			Idle_Cnt_Shake=0;
			Relay_highspeaker(RELAY_ON,0);
		}
		/*������*/
		if(Smog_Cnt>FILTER_TIMES)
		{
			Smog_Cnt=-60;
			/*����*/
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
		/*�����Դ��ѹ����*/
		if(SchmittTrigger(rms_state.battery_voltage,BATA_VOLTAGE_LOW,BATA_VOLTAGE_ERR)==1)
		{
			Set_PowerLED(PWRLED_GREEN);
		}
		if(SchmittTrigger(rms_state.battery_voltage,BATA_VOLTAGE_LOW,BATA_VOLTAGE_ERR)==-1)
		{
			Set_PowerLED(PWRLED_RED);
		}
		/*�������룬100ms��ȡһ��*/
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_PERIODIC,&err);
	}
}
