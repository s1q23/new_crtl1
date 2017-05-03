#include "stm32f4xx.h"                  // Device header
#include "mj_lib.h"

/******************系统用到的全局变量定义****************/
OS_FLAG_GRP          IRQ_EVENTs;
mj_status elecmbile_status=
{
	.us_enable=1,     //超声波默认开启
};

volatile float dest_angel = 0.0f;
volatile unsigned int Idle_Cnt_Stop=IDLE_CNT_INIT_VAL;           // 记录车子停止的时间
volatile unsigned int Idle_Cnt_Rf433_Cmd =IDLE_CNT_INIT_VAL;     // 记录遥控器命令停止时间
volatile unsigned int Idle_Cnt_Pc_Cmd =IDLE_CNT_INIT_VAL;        // 记录PC命令停止时间
volatile unsigned int Idle_Cnt_CanRx_From_Drv=IDLE_CNT_INIT_VAL; // 记录前驱发来的Can数据时间
volatile unsigned int Idle_Cnt_EmBrake =IDLE_CNT_INIT_VAL;       // 电磁刹计数
volatile unsigned int Idle_Cnt_BrakeLight=IDLE_CNT_INIT_VAL;     // 刹车灯自动关闭
volatile unsigned int Idle_Cnt_Crash=IDLE_CNT_INIT_VAL;          // 防撞条超时后，自动解除锁定
volatile unsigned int Idle_Cnt_UsHead=IDLE_CNT_INIT_VAL;         // 前超声波检测到障碍物后，会打开刹车灯，1.5S后，自动熄灭
volatile unsigned int Idle_Cnt_UsTail=IDLE_CNT_INIT_VAL;         // 后超声波检测到障碍物
volatile unsigned int Idle_Cnt_US=IDLE_CNT_INIT_VAL;             // 因为超声波而点亮刹车灯的超时计数，超时后关闭刹车灯
volatile unsigned int Idle_Cnt_BrkGls=IDLE_CNT_INIT_VAL;         // 玻璃破碎传感器报警后，自动停止
volatile unsigned int Idle_Cnt_Shake=IDLE_CNT_INIT_VAL;          // 振动传感器报警后，自动停止
volatile unsigned int Idle_Cnt_Smog=IDLE_CNT_INIT_VAL;           // 烟雾传感器报警后，自动停止
volatile unsigned int Idle_Cnt_ReInit=IDLE_CNT_INIT_VAL;         // 后驱重新初始化计时
volatile unsigned int Idle_Cnt_Water = IDLE_CNT_INIT_VAL;        // 水位传感器
/**********************软件复位函数**********************/
void mj_reset()
{
	__set_FAULTMASK(1);      // 关闭所有中端
	NVIC_SystemReset();      // 复位
}

/*****************格雷码转BCD码**********************/
unsigned int GraytoDecimal(unsigned int x)
{  
	int i;
	for(i=0;(1<<i)<sizeof(x)*8;i++) 
	{  
		x^=x>>(1<<i);
	}  
	return x; 
}

int SchmittTrigger(float in,const float V_TH,const float E_RR)
{
    int ret=0;
    static float last=0;
    if(in>last)
    {
        if(in>V_TH+E_RR) ret=1;
        else ret=0;
    }
    else
    {
        if(in<V_TH-E_RR)
            ret=-1;
        else ret=0;
    }
		if(in>V_TH+E_RR) ret=1;
		if(in<V_TH-E_RR) ret=-1;
    last=in;
    return ret;
}
//usart1 DMA收发   (DMA2-stream5-channel4,usart1-rx);(DMA2-stream7-channel4,usart1-tx);(PA9,usart1-tx);(PA10,usart1-rx)
//usart2 DMA收发   (DMA1-stream5-channel4,usart2-rx);(DMA1-stream6-channel4,usart2-tx);(PA2,usart2-tx);(PA3,usart2-rx)
//usart3 DMA收发   (DMA1-stream1-channel4,usart3-rx);(DMA1-stream3-channel4,usart3-tx);(PB10,usart3-tx);(PB11,usart3-rx)
//usart4 DMA收发   (DMA1-stream2-channel4,usart4-rx);(DMA1-stream4-channel4,usart4-tx);(PC10,usart4-tx);(PC11,usart4-rx)
//usart5 DMA收发   (DMA1-stream0-channel4,usart5-rx);(DMA1-stream7-channel4,usart5-tx);(PC12,usart5-tx);(PD2,usart5-rx)
//usart6 DMA收发   (DMA2-stream2-channel5,usart6-rx);(DMA2-stream6-channel5,usart6-tx);(PC6,usart6-tx);(PC7,usart6-rx)
