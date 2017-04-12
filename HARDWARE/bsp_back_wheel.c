#include "bsp_back_wheel.h"
#include "bsp_gpi.h"
#include "bsp_gpo.h"
#include "task_pc_com.h"

static void mj_delay_us(unsigned int us)
{
	us=us*168;
	while(us--);
}

static bool buke_can_set_speed(s16 Speed)
{
	return true;
}

static bool syntron_can_set_speed(s16 Speed)
{
	CanTxMsg can_tx_msg;
	u8 TransmitMailbox ;
	u8 i=0;

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
										                              
	TransmitMailbox = CAN_Transmit(CAN1,&can_tx_msg); 					// 通过CAN BUS 发送数据  
	while((CAN_TransmitStatus(CAN1,TransmitMailbox) != CANTXOK) && (i < 0xFF))	
	{
		mj_delay_us(1);
		i++;
	}
	if (i != 0xFF)		return true;
	else          		return false;
}

static bool nanuodake_can_set_speed(s16 Speed)
{
	return true;
}
/*设置速度接口*/
bool can_sent_speed(s16 Speed)
{
	/*0.参数检查*/
	if(Speed==rms_state.speed) return true;
	if(abs(Speed)> FRONT_MAX_SPEED) return false;
	/*1.检查是否需要发使能指令*/
	if(rms_state.speed==0)
	{
	
	}
#if (SERVO_DRV == BUKE)
	buke_can_set_speed(Speed);
#elif (SERVO_DRV == SENCHUANG)
	syntron_can_set_speed(Speed);
#elif (SERVO_DRV == NANUODAKE)	
	nanuodake_can_set_speed(Speed);
#endif
	rms_state.speed=Speed;
	return true;
}

static bool buke_can_sent_cmd(u8 cmd,u16 index,u8 vlaue)
{
	return true;
}

static bool syntron_can_sent_cmd(u8 cmd,u16 index,u8 vlaue)
{
	CanTxMsg can_tx_msg;
	u8 TransmitMailbox ;
	u8 i=0;
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
										                              
	TransmitMailbox = CAN_Transmit(CAN1,&can_tx_msg); 					// 通过CAN BUS 发送数据  
	while((CAN_TransmitStatus(CAN1,TransmitMailbox) != CANTXOK) && (i < 0xFF))	{
		mj_delay_us(1);
		i++;
	}
	
	if(i!=0xFF)		return true;
	else		          return false;
}

static bool nanuodake_can_sent_cmd(u8 cmd,u16 index,u8 vlaue)
{
	return true;
}

/*设置参数接口*/
bool can_sent_cmd(u8 cmd,u16 index,u8 value)
{
	/*0.参数检查*/
#if (SERVO_DRV == BUKE)
	return buke_can_sent_cmd(cmd,index,value);
#elif (SERVO_DRV == SENCHUANG)
	return syntron_can_sent_cmd(cmd,index,value);
#elif (SERVO_DRV == NANUODAKE)	
	return nanuodake_can_sent_cmd(cmd,index,value);
#endif
}
