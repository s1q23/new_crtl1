#include "task_can_com.h"
#include "bsp_can.h"
#include "bsp_gpo.h"
#include "bsp_back_wheel.h"



OS_TCB  CanRx0TaskTCB;                            //任务控制块
OS_TCB  CanTxTaskTCB;                            //任务控制块
CPU_STK CANRx0_TASK_STK[CANRx_STK_SIZE];         //任务堆栈	
CPU_STK CANTx_TASK_STK[CANRx_STK_SIZE];         //任务堆栈	

int can_cnt=0;

MotorDrvEncoder motor_encoder;

void task_canRx_FIFO0(void *p_arg)
{
	OS_ERR err;
	unsigned short Case_Index=0;
	CanRxMsg *pMsg=NULL;
	while(1)
	{
		OSFlagPend(&IRQ_EVENTs,EVENTS_CANRX_FIFO0,0,MJ_FLAG_OPT,0,&err);
		pMsg=can_get_msg_fifo0();
#if (SERVO_DRV == BUKE)
	;
#elif (SERVO_DRV == SENCHUANG)
	if( can_rx_msg.StdId == 0x700) 
	{
		Case_Index = (((u16)can_rx_msg.Data[2]) << 8)+ can_rx_msg.Data[3];
		Idle_Cnt_CanRx_From_Drv=0;
		switch( Case_Index )
		{
			case 0x0000: //速度
			{
				rms_state.speed=(s16)((((u16)can_rx_msg.Data[4] )<<8)+(u16)can_rx_msg.Data[5]);
				break;
			}
	  	case 0x0002: //电流
			{
				s32 bata_Current =(s32)((((u32)can_rx_msg.Data[4])<<8)+(u32)can_rx_msg.Data[5]);
	      rms_state.battery_current=((float) bata_Current)*15.6*0.001; //
				break;
			}
			case 0x0080: //电压
			{
				u32 bata_Voltage =(u32) (((( u32 )can_rx_msg.Data[4] ) << 8 ) +  ( u32 )can_rx_msg.Data[5] );
			  rms_state.battery_voltage=(u8)(bata_Voltage/10);
				break;
			}
			case 0x001D://编码器计数
			{
				motor_encoder.single_Encoder[0] = can_rx_msg.Data[7];//高位在前
				motor_encoder.single_Encoder[1] = can_rx_msg.Data[6];
				motor_encoder.single_Encoder[2] = can_rx_msg.Data[5];
				motor_encoder.single_Encoder[3] = can_rx_msg.Data[4];
			}
			case 0x0036: //故障信息
			{
				u16 err_inform=(((u16)can_rx_msg.Data[4]) << 8)+ can_rx_msg.Data[5]; 
				break;
			}
			default: break;
		}	
	}
#elif (SERVO_DRV == NANUODAKE)	
	if(pMsg->StdId == 0x581)  //低位在前 高位在后
	{
		Case_Index = (((u16)pMsg->Data[2])<<8)+pMsg->Data[1];
		Idle_Cnt_CanRx_From_Drv=0;
		switch( Case_Index )
		{
			case 0x6042:  //读速度
			{
				s16 motor_SPEED = (s16)((pMsg->Data[5] <<8)+pMsg->Data[4]);
				rms_state.speed =motor_SPEED;
				break;
			}
			case 0x2039:  //电流
			{
				int bata_Current = (((u32)pMsg->Data[7])<<24)+(((u32)pMsg->Data[6])<<16)+(((u32)pMsg->Data[5])<<8)+pMsg->Data[4];
				rms_state.battery_current=(s8)(bata_Current/1000);
				break;
			}
			case 0x3010:  //电压  
			{
				int bata_Voltage = (((u32)pMsg->Data[7])<<24)+(((u32)pMsg->Data[6])<<16)+(((u32)pMsg->Data[5])<<8)+pMsg->Data[4];
				rms_state.battery_voltage=(u8)(bata_Voltage/1000); 
				break;
			}
			case 0x6064:  //编码器计数  发送怎么做
			{
				can_cnt--;
				motor_encoder.single_Encoder[0] = pMsg->Data[4];
				motor_encoder.single_Encoder[1] = pMsg->Data[5];
				motor_encoder.single_Encoder[2] = pMsg->Data[6];
				motor_encoder.single_Encoder[3] = pMsg->Data[7];
				break;
			}
		default:
			break;
		}
	}
#endif
	}
}

void task_canTx(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_PERIODIC,&err);
		/*****************************查询电流*****************************/
#if (SERVO_DRV == BUKE)	//步科
		;
#elif (SERVO_DRV == SENCHUANG)//森创
		syntron_can_sent_cmd(0x0A,0x0002,0x00);
#elif (SERVO_DRV == NANUODAKE)//纳诺达克
		nanuo_can_sent(0x40,0x2039,2,0x00); //电流
#endif
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_PERIODIC,&err);
		
		/*****************************查询电压******************************/
#if (SERVO_DRV == BUKE)	      //步科
		;
#elif (SERVO_DRV == SENCHUANG)//森创
		syntron_can_sent_cmd(0x0A,0x0080,0x00);
#elif (SERVO_DRV == NANUODAKE)//纳诺达克
		nanuo_can_sent(0x40,0x3010,2,0x00); //电压
#endif		
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_PERIODIC,&err);
		
		/*****************************读码盘*****************************/
#if (SERVO_DRV == BUKE)	      //步科
		;
#elif (SERVO_DRV == SENCHUANG)//森创
		syntron_can_sent_cmd(0x0D,0x001D,0x00);
#elif (SERVO_DRV == NANUODAKE)//纳诺达克
		nanuo_can_sent(0x40,0x6064,0,0x00); //编码器值
#endif		
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_PERIODIC,&err);
		
		/*****************************读速度*****************************/
#if (SERVO_DRV == BUKE)	//步科
		;
#elif (SERVO_DRV == SENCHUANG)//森创
		syntron_can_sent_cmd(0x0A,0x0000,0x00);
#elif (SERVO_DRV == NANUODAKE)          //纳诺达克
		nanuo_can_sent(0x40,0x6042,0,0x00); //电机速度
#endif		
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_PERIODIC,&err);
		
		/*****************************预留操作*****************************/
#if (SERVO_DRV == BUKE)	       //步科
		;
#elif (SERVO_DRV == SENCHUANG) //森创
		syntron_can_sent_cmd(0x0A,0x0036,0x00) ;   //查故障
#elif (SERVO_DRV == NANUODAKE) //纳诺达克
		;
#endif
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_PERIODIC,&err);
		#ifdef SEND_TO_NAVI
		motor_encoder.sum_Encoder=(-motor_encoder.sum_Encoder)*2;
		mj_can_sent_to_NAVI();//发送数据到导航版
		#endif
		can_cnt++;
	}
}
