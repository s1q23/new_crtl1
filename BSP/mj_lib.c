#include "stm32f4xx.h"                  // Device header
#include "mj_lib.h"

/******************ϵͳ�õ���ȫ�ֱ�������****************/
OS_FLAG_GRP          IRQ_EVENTs;
mj_status elecmbile_status=
{
	.us_enable=1,     //������Ĭ�Ͽ���
};

volatile float dest_angel = 0.0f;
volatile unsigned int Idle_Cnt_Stop=IDLE_CNT_INIT_VAL;           // ��¼����ֹͣ��ʱ��
volatile unsigned int Idle_Cnt_Rf433_Cmd =IDLE_CNT_INIT_VAL;     // ��¼ң��������ֹͣʱ��
volatile unsigned int Idle_Cnt_Pc_Cmd =IDLE_CNT_INIT_VAL;        // ��¼PC����ֹͣʱ��
volatile unsigned int Idle_Cnt_CanRx_From_Drv=IDLE_CNT_INIT_VAL; // ��¼ǰ��������Can����ʱ��
volatile unsigned int Idle_Cnt_EmBrake =IDLE_CNT_INIT_VAL;       // ���ɲ����
volatile unsigned int Idle_Cnt_BrakeLight=IDLE_CNT_INIT_VAL;     // ɲ�����Զ��ر�
volatile unsigned int Idle_Cnt_Crash=IDLE_CNT_INIT_VAL;          // ��ײ����ʱ���Զ��������
volatile unsigned int Idle_Cnt_UsHead=IDLE_CNT_INIT_VAL;         // ǰ��������⵽�ϰ���󣬻��ɲ���ƣ�1.5S���Զ�Ϩ��
volatile unsigned int Idle_Cnt_UsTail=IDLE_CNT_INIT_VAL;         // ��������⵽�ϰ���
volatile unsigned int Idle_Cnt_US=IDLE_CNT_INIT_VAL;             // ��Ϊ������������ɲ���Ƶĳ�ʱ��������ʱ��ر�ɲ����
volatile unsigned int Idle_Cnt_BrkGls=IDLE_CNT_INIT_VAL;         // �������鴫�����������Զ�ֹͣ
volatile unsigned int Idle_Cnt_Shake=IDLE_CNT_INIT_VAL;          // �񶯴������������Զ�ֹͣ
volatile unsigned int Idle_Cnt_Smog=IDLE_CNT_INIT_VAL;           // ���������������Զ�ֹͣ
volatile unsigned int Idle_Cnt_ReInit=IDLE_CNT_INIT_VAL;         // �������³�ʼ����ʱ
volatile unsigned int Idle_Cnt_Water = IDLE_CNT_INIT_VAL;        // ˮλ������
/**********************�����λ����**********************/
void mj_reset()
{
	__set_FAULTMASK(1);      // �ر������ж�
	NVIC_SystemReset();      // ��λ
}

/*****************������תBCD��**********************/
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
//usart1 DMA�շ�   (DMA2-stream5-channel4,usart1-rx);(DMA2-stream7-channel4,usart1-tx);(PA9,usart1-tx);(PA10,usart1-rx)
//usart2 DMA�շ�   (DMA1-stream5-channel4,usart2-rx);(DMA1-stream6-channel4,usart2-tx);(PA2,usart2-tx);(PA3,usart2-rx)
//usart3 DMA�շ�   (DMA1-stream1-channel4,usart3-rx);(DMA1-stream3-channel4,usart3-tx);(PB10,usart3-tx);(PB11,usart3-rx)
//usart4 DMA�շ�   (DMA1-stream2-channel4,usart4-rx);(DMA1-stream4-channel4,usart4-tx);(PC10,usart4-tx);(PC11,usart4-rx)
//usart5 DMA�շ�   (DMA1-stream0-channel4,usart5-rx);(DMA1-stream7-channel4,usart5-tx);(PC12,usart5-tx);(PD2,usart5-rx)
//usart6 DMA�շ�   (DMA2-stream2-channel5,usart6-rx);(DMA2-stream6-channel5,usart6-tx);(PC6,usart6-tx);(PC7,usart6-rx)
