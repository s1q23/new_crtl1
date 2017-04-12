#include "bsp_can.h"

void Config_Can(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  CAN_InitTypeDef CAN_InitStructure;
  CAN_FilterInitTypeDef CAN_FilterInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  CAN_ITConfig(CAN1,CAN_IT_FMP0,DISABLE);//关闭0号缓冲区的接收中断
  
  RCC_AHB1PeriphClockCmd( GPIO_MY_CAN1_CLOCK, ENABLE );
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_CAN1, ENABLE );
  
  //can收发管脚配置
	GPIO_InitStructure.GPIO_Pin = PIN_MY_CAN1_RX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MY_CAN1_GPIO,&GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = PIN_MY_CAN1_TX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MY_CAN1_GPIO,&GPIO_InitStructure);
  
  GPIO_PinAFConfig( MY_CAN1_GPIO, PINSOURCE_MY_CAN1_RX, GPIO_AF_CAN1 );
  GPIO_PinAFConfig( MY_CAN1_GPIO, PINSOURCE_MY_CAN1_TX, GPIO_AF_CAN1 );
  
  CAN_DeInit( CAN1 );
  CAN_StructInit(&CAN_InitStructure);
  //PCLK1（APB1） = HCLK（AHB）/ 4 = 168/4=42MHZ
  //波特率配置 = 42/(1+6+7)/6 = 500K
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
  CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_7tq;
  CAN_InitStructure.CAN_Prescaler = 6;
  
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;//正常工作模式
  
  CAN_InitStructure.CAN_TXFP = ENABLE;  //在该模式下，发送的优先级由发送请求次序决定。
  CAN_InitStructure.CAN_NART = ENABLE;  //在该模式下，发送操作只会执行一次。
  CAN_InitStructure.CAN_TTCM = DISABLE; //时间触发通信模式,被用于产生(发送与接收邮箱的)时间戳
  CAN_InitStructure.CAN_RFLM = DISABLE; //FIFO锁定功能,禁用了，那么FIFO中最后收到的报文就被新报文所覆盖。这样，最新收到的报文不会被丢弃掉。
  CAN_InitStructure.CAN_ABOM = ENABLE;  //如果ABOM位为’1’， bxCAN进入离线状态后，就自动开启恢复过程。
  CAN_InitStructure.CAN_AWUM = ENABLE;  //自动唤醒模式,睡眠模式通过检测CAN报文，由硬件自动唤醒。
  CAN_Init( CAN1, &CAN_InitStructure );
  
  while(CAN_Init(CAN1, &CAN_InitStructure)!=CANINITOK); //此处为在线调试断点，程序卡在这里，初始化成功则继续往下
   /**	*****缓冲区0-过滤器0*********	**/
  CAN_FilterInitStructure.CAN_FilterNumber = 0;//0号过滤器,0~13
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//过滤器关联到0号先进先出缓冲区，共两个缓冲区，每个缓冲区有3个邮箱.
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;//CAN_FilterMode_IdMask;//屏蔽模式，屏蔽寄存器为1的相应位必须匹配。
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0;
  CAN_FilterInitStructure.CAN_FilterIdLow =  0;     //IDE = 1扩展ID；RTR=0数据帧
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0028;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0004;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//使能过滤器
  CAN_FilterInit( &CAN_FilterInitStructure );
	
  CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//使能0号缓冲区的接收中断
  
  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;//can1的FIFO0中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init( &NVIC_InitStructure );
	
  /*************	*****缓冲区1-过滤器1****** *************/
	CAN_FilterInitStructure.CAN_FilterNumber = 1;//1号过滤器,0~13
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO1;//过滤器关联到0号先进先出缓冲区，共两个缓冲区，每个缓冲区有3个邮箱.
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;//CAN_FilterMode_IdMask;//屏蔽模式，屏蔽寄存器为1的相应位必须匹配。
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0;
  CAN_FilterInitStructure.CAN_FilterIdLow =  0;//IDE = 1扩展ID；RTR=0数据帧
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//使能过滤器
  CAN_FilterInit( &CAN_FilterInitStructure );

	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX1_IRQn;//can1的FIFO0中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init( &NVIC_InitStructure );

//	CAN_FilterInitStructure.CAN_FilterNumber = 2;//1号过滤器,0~13
//  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO1;//过滤器关联到0号先进先出缓冲区，共两个缓冲区，每个缓冲区有3个邮箱.
//  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;     //CAN_FilterMode_IdMask;//屏蔽模式，屏蔽寄存器为1的相应位必须匹配。
//  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
//  CAN_FilterInitStructure.CAN_FilterIdHigh = 0;
//  CAN_FilterInitStructure.CAN_FilterIdLow = (((uint16_t) 9) << 3)|(((uint16_t) 1) << 2)|(((uint16_t) 0) << 1)|(((uint16_t) 0) << 0);//IDE = 1扩展ID；RTR=0数据帧
//	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0;
//  CAN_FilterInitStructure.CAN_FilterMaskIdLow = (((uint16_t) 1) << 3)|(((uint16_t) 1) << 2)|(((uint16_t) 0) << 1)|(((uint16_t) 0) << 0);
//  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//使能过滤器
//  CAN_FilterInit( &CAN_FilterInitStructure );
  
  CAN_ITConfig(CAN1,CAN_IT_FMP1,ENABLE);//使能1号缓冲区的接收中断
}

static CanRxMsg f_can_rx_msg_fifo0;
CanRxMsg * can_get_msg_fifo0()
{
	return &f_can_rx_msg_fifo0;
}
void CAN1_RX0_IRQHandler(void)
{
	OS_ERR err;
	/*1.清除数组*/
	memset(&f_can_rx_msg_fifo0, 0, sizeof(f_can_rx_msg_fifo0));
	/*2.从FIFO读取数据*/
  CAN_Receive( CAN1, CAN_FIFO0, &f_can_rx_msg_fifo0);
	/*3.发送信号量给任务*/
	OSFlagPost(&IRQ_EVENTs,EVENTS_CANRX_FIFO0,OS_OPT_POST_FLAG_SET,&err);	
	/*4.清除中断标志位*/
	CAN_ClearITPendingBit( CAN1, CAN_IT_FMP0 );//清除挂起中断	因为一旦往FIFO存入1个报文，硬件就会更新FMP[1:0]位，
}

static CanRxMsg f_can_rx_msg_fifo1;
CanRxMsg * can_get_msg_fifo1()
{
	return &f_can_rx_msg_fifo1;
}
void CAN1_RX1_IRQHandler(void)//预留
{
	OS_ERR err;
	memset(&f_can_rx_msg_fifo1, 0, sizeof(f_can_rx_msg_fifo1));
  CAN_Receive( CAN1, CAN_FIFO1, &f_can_rx_msg_fifo1);
	OSFlagPost(&IRQ_EVENTs,EVENTS_CANRX_FIFO0,OS_OPT_POST_FLAG_SET,&err);	
  CAN_ClearITPendingBit( CAN1, CAN_IT_FMP1 );//清除挂起中断	因为一旦往FIFO存入1个报文，硬件就会更新FMP[1:0]位，
}
