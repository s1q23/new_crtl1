#include "task_adc_calc.h"
#include "bsp_adc.h"
#include "bsp_gpi.h"
#include "bsp_front_wheel.h"

OS_TCB AdcCalcTaskTCB;                          //任务控制块
CPU_STK ADC_CALC_TASK_STK[ADC_CALC_STK_SIZE];   //任务堆栈	

void adc_calc_task(void *p_arg)
{
	static OS_ERR err;
	while(1)
	{
		OSFlagPend(&IRQ_EVENTs,EVENTS_DMA_CVTOK,0,OS_OPT_PEND_FLAG_SET_ALL|OS_OPT_PEND_FLAG_CONSUME,0,&err);
		DMA2_Stream0_IRQHandler_ADC1();
		/*暂时不对其处理，仅仅上传AD转换后的数据*/
		rms_state.co=adc_value.CO;       
		rms_state.envir_hum=adc_value.HUMI;
		rms_state.envir_temp=adc_value.TEMP;
//		rms_state.battery_voltage=adc_value.BAT_V;            //电池电压目前是从后驱驱动器读到，不走ADC，所以注释掉
		rms_state.angle=front_angel_to_gear(get_front_angel()); //目前角度信息是从这个接口拿到的
	}
}
