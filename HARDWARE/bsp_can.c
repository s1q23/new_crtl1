#include "bsp_can.h"
#include "bsp_gpo.h"
#include "bsp_back_wheel.h"

void Config_Can(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  CAN_InitTypeDef CAN_InitStructure;
  CAN_FilterInitTypeDef CAN_FilterInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  CAN_ITConfig(CAN1,CAN_IT_FMP0,DISABLE);//�ر�0�Ż������Ľ����ж�
  
  RCC_AHB1PeriphClockCmd( GPIO_MY_CAN1_CLOCK, ENABLE );
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_CAN1, ENABLE );
  
  //can�շ��ܽ�����
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
  //PCLK1��APB1�� = HCLK��AHB��/ 4 = 168/4=42MHZ
  //���������� = 42/(1+6+7)/6 = 500K
	//Can ����������Ǳ���� tBS1>=tBS2 tBS2>=1��CANʱ������  tBS2>=2tSJW
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
  CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_7tq;
  CAN_InitStructure.CAN_Prescaler = 6;
  
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;//��������ģʽ
  
  CAN_InitStructure.CAN_TXFP = ENABLE;  //�ڸ�ģʽ�£����͵����ȼ��ɷ���������������
  CAN_InitStructure.CAN_NART = ENABLE;  //�ڸ�ģʽ�£����Ͳ���ֻ��ִ��һ�Ρ�
  CAN_InitStructure.CAN_TTCM = DISABLE; //ʱ�䴥��ͨ��ģʽ,�����ڲ���(��������������)ʱ���
  CAN_InitStructure.CAN_RFLM = DISABLE; //FIFO��������,�����ˣ���ôFIFO������յ��ı��ľͱ��±��������ǡ������������յ��ı��Ĳ��ᱻ��������
  CAN_InitStructure.CAN_ABOM = ENABLE;  //���ABOMλΪ��1���� bxCAN��������״̬�󣬾��Զ������ָ����̡�
  CAN_InitStructure.CAN_AWUM = ENABLE;  //�Զ�����ģʽ,˯��ģʽͨ�����CAN���ģ���Ӳ���Զ����ѡ�
  CAN_Init( CAN1, &CAN_InitStructure );
  
  while(CAN_Init(CAN1, &CAN_InitStructure)!=CANINITOK); //�˴�Ϊ���ߵ��Զϵ㣬�����������ʼ���ɹ����������
   /**	*****������0-������0*********	**/
  CAN_FilterInitStructure.CAN_FilterNumber = 0;//0�Ź�����,0~13
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//������������0���Ƚ��ȳ�����������������������ÿ����������3������.
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;//CAN_FilterMode_IdMask;//����ģʽ�����μĴ���Ϊ1����Ӧλ����ƥ�䡣
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0;
  CAN_FilterInitStructure.CAN_FilterIdLow =  0;     //IDE = 1��չID��RTR=0����֡
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//ʹ�ܹ�����
  CAN_FilterInit( &CAN_FilterInitStructure );
	
  CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//ʹ��0�Ż������Ľ����ж�
  
  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;//can1��FIFO0�ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init( &NVIC_InitStructure );
	
  /*************	*****������1-������1****** *************/
//	CAN_FilterInitStructure.CAN_FilterNumber = 1;//1�Ź�����,0~13
//  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO1;//������������0���Ƚ��ȳ�����������������������ÿ����������3������.
//  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;//CAN_FilterMode_IdMask;//����ģʽ�����μĴ���Ϊ1����Ӧλ����ƥ�䡣
//  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
//  CAN_FilterInitStructure.CAN_FilterIdHigh = 0;
//  CAN_FilterInitStructure.CAN_FilterIdLow =  0;//IDE = 1��չID��RTR=0����֡
//	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0;
//  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0;
//  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//ʹ�ܹ�����
//  CAN_FilterInit( &CAN_FilterInitStructure );

//	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX1_IRQn;//can1��FIFO0�ж�
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init( &NVIC_InitStructure );

//	CAN_FilterInitStructure.CAN_FilterNumber = 2;//1�Ź�����,0~13
//  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO1;//������������0���Ƚ��ȳ�����������������������ÿ����������3������.
//  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;     //CAN_FilterMode_IdMask;//����ģʽ�����μĴ���Ϊ1����Ӧλ����ƥ�䡣
//  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
//  CAN_FilterInitStructure.CAN_FilterIdHigh = 0;
//  CAN_FilterInitStructure.CAN_FilterIdLow = (((uint16_t) 9) << 3)|(((uint16_t) 1) << 2)|(((uint16_t) 0) << 1)|(((uint16_t) 0) << 0);//IDE = 1��չID��RTR=0����֡
//	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0;
//  CAN_FilterInitStructure.CAN_FilterMaskIdLow = (((uint16_t) 1) << 3)|(((uint16_t) 1) << 2)|(((uint16_t) 0) << 1)|(((uint16_t) 0) << 0);
//  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;//ʹ�ܹ�����
//  CAN_FilterInit( &CAN_FilterInitStructure );
  
  CAN_ITConfig(CAN1,CAN_IT_FMP1,ENABLE);//ʹ��1�Ż������Ľ����ж�
}

bool mj_can_sent_to_NAVI()
{	
	CanTxMsg can_tx_msg;
	u8 TransmitMailbox ;
	u8 retry;

	can_tx_msg.StdId = 0;
	can_tx_msg.ExtId = 15;		 			// ʹ����չ֡��ʶ��
	can_tx_msg.IDE = CAN_ID_EXT;    // ��չ֡
	can_tx_msg.RTR = CAN_RTR_DATA;	// ����֡
	can_tx_msg.DLC = 7;		     		  // �������ݳ���
	can_tx_msg.Data[0] = 3;
	can_tx_msg.Data[1] = 0xAA;
	can_tx_msg.Data[2] = 0;
	can_tx_msg.Data[3] = motor_encoder.single_Encoder[0];
	can_tx_msg.Data[4] = motor_encoder.single_Encoder[1];
	can_tx_msg.Data[5] = motor_encoder.single_Encoder[2];
	can_tx_msg.Data[6] = motor_encoder.single_Encoder[3];
//	can_tx_msg.Data[7] = 0;
	for(retry=0;retry<3;retry++)
	{	
		TransmitMailbox = CAN_Transmit(CAN1,&can_tx_msg); 					// ͨ��CAN BUS ��������  
		if(TransmitMailbox!=CAN_TxStatus_NoMailBox) break;
		mj_delay_us(200);
	}
	if(TransmitMailbox!=CAN_TxStatus_NoMailBox)
		return true;
	return false;
}

static CanRxMsg f_can_rx_msg_fifo0;
CanRxMsg * can_get_msg_fifo0()
{
	return &f_can_rx_msg_fifo0;
}
void CAN1_RX0_IRQHandler(void)
{
	static OS_ERR err;
	OSIntEnter();
	/*1.�������*/
	memset(&f_can_rx_msg_fifo0, 0, sizeof(f_can_rx_msg_fifo0));
	/*2.��FIFO��ȡ����*/
  CAN_Receive( CAN1, CAN_FIFO0, &f_can_rx_msg_fifo0);
	/*3.�����ź���������*/
	OSFlagPost(&IRQ_EVENTs,EVENTS_CANRX_FIFO0,OS_OPT_POST_FLAG_SET,&err);	
	/*4.����жϱ�־λ*/
	CAN_ClearITPendingBit( CAN1, CAN_IT_FMP0 );//��������ж�	��Ϊһ����FIFO����1�����ģ�Ӳ���ͻ����FMP[1:0]λ��
	OSIntExit();
}

static CanRxMsg f_can_rx_msg_fifo1;
CanRxMsg * can_get_msg_fifo1()
{
	return &f_can_rx_msg_fifo1;
}
void CAN1_RX1_IRQHandler(void)//Ԥ��
{
	static OS_ERR err;
	OSIntEnter();
	memset(&f_can_rx_msg_fifo1, 0, sizeof(f_can_rx_msg_fifo1));
  CAN_Receive( CAN1, CAN_FIFO1, &f_can_rx_msg_fifo1);
	OSFlagPost(&IRQ_EVENTs,EVENTS_CANRX_FIFO0,OS_OPT_POST_FLAG_SET,&err);	
  CAN_ClearITPendingBit( CAN1, CAN_IT_FMP1 );//��������ж�	��Ϊһ����FIFO����1�����ģ�Ӳ���ͻ����FMP[1:0]λ��
	OSIntExit();
}
