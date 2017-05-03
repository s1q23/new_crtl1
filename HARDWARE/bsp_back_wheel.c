#include "bsp_back_wheel.h"
#include "bsp_gpi.h"
#include "bsp_gpo.h"
#include "task_pc_com.h"

volatile char back_dir=DIR_STOP;

void mj_delay_us(unsigned int us)
{
	us=us*50;
	while(us--);
}
void mj_delay_ms(unsigned int ms)
{
	ms*=1000*50;
	while(ms--);
}
bool buke_can_sent_cmd(u8 cmd,u16 index,u8 vlaue)
{
	return true;
}
bool syntron_can_sent_cmd(u8 cmd,u16 index,u8 vlaue)
{
	CanTxMsg can_tx_msg;
	u8 TransmitMailbox ;
	u8 retry=0;
	can_tx_msg.StdId = 0x0101;    
	can_tx_msg.ExtId = 0;		 			// 使用扩展帧标识符                                       
	can_tx_msg.IDE = CAN_ID_STD;          		// 数据帧                                                 
	can_tx_msg.RTR = CAN_RTR_DATA;	     		// 数据帧		                                       
	can_tx_msg.DLC = 8;		     		// 发送数据长度                                                    
	can_tx_msg.Data[0] = 0x00;
	can_tx_msg.Data[1] = cmd;
	can_tx_msg.Data[2] = ((u16)index)>>8; //
	can_tx_msg.Data[3] = index&0xff;
	can_tx_msg.Data[4] = 0x00 ;
	can_tx_msg.Data[5] = vlaue ;//高位在前
	can_tx_msg.Data[6] = 0x00;
	can_tx_msg.Data[7] = 0x00;
	for(retry=0;retry<3;retry++)
	{
		TransmitMailbox = CAN_Transmit(CAN1,&can_tx_msg);
		if(TransmitMailbox!=CAN_TxStatus_NoMailBox)  break;
		mj_delay_us(200);
	}
	if(TransmitMailbox!=CAN_TxStatus_NoMailBox)		
		return true;
	else		         
		return false;
}
bool nanuo_can_sent(u8 Command, u16 Index, u8 Subindex, s16 Speed)
{	
	CanTxMsg can_tx_msg;
	u8 TransmitMailbox ;
	u8 retry;

	can_tx_msg.StdId = 0x601;
	can_tx_msg.ExtId = 0;		 			// 使用扩展帧标识符                                       
	can_tx_msg.IDE = CAN_ID_STD;          		// 数据帧                                                 
	can_tx_msg.RTR = CAN_RTR_DATA;	     		// 数据帧		                                       
	can_tx_msg.DLC = 8;		     		// 发送数据长度                                                    
	can_tx_msg.Data[0] = Command;
	can_tx_msg.Data[1] = (u8)( Index & 0xFF );
	can_tx_msg.Data[2] = (u8)( ( Index >> 8 ) & 0xFF );
	can_tx_msg.Data[3] = Subindex;
	can_tx_msg.Data[4] = ( u8 )( Speed & 0xff ) ;
	can_tx_msg.Data[5] = ( u8 )( ( Speed >> 8 ) & 0xff ) ;
	can_tx_msg.Data[6] = 0;
	can_tx_msg.Data[7] = 0;
	for(retry=0;retry<3;retry++)
	{	
		TransmitMailbox = CAN_Transmit(CAN1,&can_tx_msg); 					// 通过CAN BUS 发送数据  
		if(TransmitMailbox!=CAN_TxStatus_NoMailBox) break;
		mj_delay_us(200);
	}
	if(TransmitMailbox!=CAN_TxStatus_NoMailBox)
		return true;
	return false;
}

#if (SERVO_DRV == BUKE)
static bool buke_can_set_speed(s16 Speed)
{
	return true;
}
#elif (SERVO_DRV == SENCHUANG)
static bool syntron_can_set_speed(s16 Speed)
{
	CanTxMsg can_tx_msg;
	u8 TransmitMailbox ;
	u8 retry=0;

	can_tx_msg.StdId = 0x0101;    
	can_tx_msg.ExtId = 0;		 			            // 使用扩展帧标识符                                       
	can_tx_msg.IDE = CAN_ID_STD;          		// 数据帧                                                 
	can_tx_msg.RTR = CAN_RTR_DATA;	     		  // 数据帧		                                       
	can_tx_msg.DLC = 8;		     		            // 发送数据长度                                                    
	can_tx_msg.Data[0] = 0x00;
	can_tx_msg.Data[1] = 0x28;
	can_tx_msg.Data[2] = 0x00;
	can_tx_msg.Data[3] = 0x00;
	can_tx_msg.Data[4] = ( u8 )( ( Speed >> 8 ) & 0xff ) ;//高位在前
	can_tx_msg.Data[5] = ( u8 )( Speed & 0xff ) ;
	can_tx_msg.Data[6] = 0x00;
	can_tx_msg.Data[7] = 0x00;
	for(retry=0;retry<3;retry++)
	{	
		TransmitMailbox = CAN_Transmit(CAN1,&can_tx_msg);
		if(CAN_TxStatus_NoMailBox!=TransmitMailbox) break;
		mj_delay_ms(1);
	}

	/*申请到邮箱，认为肯定会发送成功*/
	if (TransmitMailbox!= CAN_TxStatus_NoMailBox)		
		return true;
	else          		
		return false;
}
#elif (SERVO_DRV == NANUODAKE)
static bool nanuodake_can_set_speed(s16 Speed)
{
	CanTxMsg can_tx_msg;
	u8 TransmitMailbox ;
	u8 retry=0;

	can_tx_msg.StdId = 0x0601;    
	can_tx_msg.ExtId = 0;		 			            // 使用扩展帧标识符                                       
	can_tx_msg.IDE = CAN_ID_STD;          		// 数据帧                                                 
	can_tx_msg.RTR = CAN_RTR_DATA;	     		  // 数据帧		                                       
	can_tx_msg.DLC = 8;		     		            // 发送数据长度                                                    
	can_tx_msg.Data[0] = 0x2B;//0x23;
	can_tx_msg.Data[1] = 0x42;//0x00;
	can_tx_msg.Data[2] = 0x60;//0x20;
	can_tx_msg.Data[3] = 0x00;//0x01;
	can_tx_msg.Data[4] = ( u8 )( Speed & 0xff ) ;
	can_tx_msg.Data[5] = ( u8 )( ( Speed >> 8 ) & 0xff ) ;
	can_tx_msg.Data[6] = 0x00;
	can_tx_msg.Data[7] = 0x00;
	for(retry=0;retry<3;retry++)
	{	
		TransmitMailbox = CAN_Transmit(CAN1,&can_tx_msg);
		if(CAN_TxStatus_NoMailBox!=TransmitMailbox) break;
		mj_delay_ms(1);
	}
	/*申请到邮箱，认为肯定会发送成功*/
	if (TransmitMailbox!= CAN_TxStatus_NoMailBox)		
		return true;
	else          		
		return false;	
}
#endif
/*设置速度接口*/
bool back_wheel_set_speed(s16 Speed)
{
	/*1.参数检查*/
	if(abs(Speed)> FRONT_MAX_SPEED) return false;
	/*2.修改方向信息*/
	if(Speed>0)
	{	
		Idle_Cnt_EmBrake=100;//写100就永远不会再回到0，除非等到变量溢出，大概需要6年
		Idle_Cnt_BrakeLight=100;
		back_dir=DIR_FRONT;
		Relay_brakelight(RELAY_OFF,0);
	}
	else if(Speed<0) 
	{
		Idle_Cnt_EmBrake=100;     // 永远不会关闭电磁刹
		Idle_Cnt_BrakeLight=100;  // 永远不会关闭电磁刹
		back_dir=DIR_BACK;
		Relay_brakelight(RELAY_OFF,0);
	}
	else //速度为0，停止后驱，5s后，电磁刹关闭
	{
		/*这个值在cb_TmrTimeOutCheck()函数中被修改*/
		Idle_Cnt_BrakeLight=0;
		Idle_Cnt_EmBrake=0;//写0，5s过后会关闭电磁刹
		/*开刹车灯*/
		if(back_dir!=DIR_STOP)
			Relay_brakelight(RELAY_ON,0);
		back_dir=DIR_STOP;
	}
	/*3.打开电磁刹*/
	if(elecmbile_status.em_brake_status==0&&Speed!=0)
	{
		em_brake_set(ENABLE);
		elecmbile_status.em_brake_status=1;
	}
	/*4.发送速度给后驱伺服驱动器*/
#if (SERVO_DRV == BUKE)
	buke_can_set_speed(Speed);
#elif (SERVO_DRV == SENCHUANG)
	syntron_can_set_speed(Speed);
#elif (SERVO_DRV == NANUODAKE)	
	nanuodake_can_set_speed(Speed);
#endif
	/*5.修改全状态速度信息*/
	rms_state.speed=Speed;
	return true;
}


/*电磁刹的控制命令*/
#if (SERVO_DRV == BUKE)
bool static buke_em_brake_set(int on_off)
{
	return false;
}
#elif (SERVO_DRV == SENCHUANG)
bool static syntron_em_brake_set(int on_off)
{
	if(on_off)
		return can_sent_cmd(0x1E,0x0010,0x01);
	else
		return can_sent_cmd(0x1E,0x0010,0x00);
}
#elif (SERVO_DRV == NANUODAKE)
bool static nanuodake_em_brake_set(int on_off)
{
	if(on_off)
		return nanuo_can_sent(0x2B,0x6040,0x00,0x0F);
	else
		return nanuo_can_sent(0x2B,0x6040,0x00,0x86);;
}
#endif
bool em_brake_set(int on_off)
{
#if (SERVO_DRV == BUKE)
	return buke_em_brake_set(on_off);
#elif (SERVO_DRV == SENCHUANG)
	return syntron_em_brake_set(on_off);
#elif (SERVO_DRV == NANUODAKE)	
	return nanuodake_em_brake_set(on_off);
#endif	
}

/*后驱初始化配置*/
#if (SERVO_DRV == BUKE)
static bool buke_driver_init(u8 mode)
{
	return false;
}
#elif (SERVO_DRV == SENCHUANG)
static bool syntron_driver_init(u8 mode)
{
	can_sent_cmd(0x1E,0x0010,0x01);      //打开电磁刹
	back_wheel_set_speed(0);             //发送速度	
	return true;
}
#elif (SERVO_DRV == NANUODAKE)	
static bool nanuodake_driver_init(u8 mode)
{
	if(mode==0)
	{
		mj_delay_ms(1000);
		LED1=0;
		mj_delay_ms(1000);
		LED2=0;
		mj_delay_ms(1000);
		LED3=0;
		mj_delay_ms(1000);
		LED4=0;
		mj_delay_ms(1000);
		LED4=1;
		mj_delay_ms(1000);
		LED3=1;
		mj_delay_ms(1000);
		LED2=1;
		mj_delay_ms(1000);
		LED1=1;
		mj_delay_ms(1000);
	}
	nanuo_can_sent(0x2B, 0x6060, 0x00, 0x0002); //使能电机7
	mj_delay_ms(1);
	nanuo_can_sent(0x2B, 0x6040, 0x00, 0x0006); //使能电机6
	mj_delay_ms(1);
	nanuo_can_sent(0x2B, 0x6040, 0x00, 0x0007); //使能电机6
	mj_delay_ms(1);
	nanuo_can_sent(0x2B, 0x6040, 0x00, 0x000F); //使能电机6
	mj_delay_ms(1);	
	return true;
}
#endif
void servo_driver_init(u8 mode)
{
#if(SERVO_DRV == NANUODAKE)
	nanuodake_driver_init(mode);
#elif (SERVO_DRV==SENCHUANG)
	syntron_driver_init(mode);
#elif (SERVO_DRV==BUKE)
	buke_driver_init(mode);
#endif
}
