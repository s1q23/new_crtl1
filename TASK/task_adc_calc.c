#include "task_adc_calc.h"
#include "bsp_adc.h"
#include "bsp_gpi.h"
#include "bsp_front_wheel.h"

OS_TCB AdcCalcTaskTCB;                          //������ƿ�
CPU_STK ADC_CALC_TASK_STK[ADC_CALC_STK_SIZE];   //�����ջ	

void adc_calc_task(void *p_arg)
{
	static OS_ERR err;
	while(1)
	{
		OSFlagPend(&IRQ_EVENTs,EVENTS_DMA_CVTOK,0,OS_OPT_PEND_FLAG_SET_ALL|OS_OPT_PEND_FLAG_CONSUME,0,&err);
		DMA2_Stream0_IRQHandler_ADC1();
		/*��ʱ�����䴦�������ϴ�ADת���������*/
		rms_state.co=adc_value.CO;       
		rms_state.envir_hum=adc_value.HUMI;
		rms_state.envir_temp=adc_value.TEMP;
//		rms_state.battery_voltage=adc_value.BAT_V;            //��ص�ѹĿǰ�ǴӺ�������������������ADC������ע�͵�
		rms_state.angle=front_angel_to_gear(get_front_angel()); //Ŀǰ�Ƕ���Ϣ�Ǵ�����ӿ��õ���
	}
}
