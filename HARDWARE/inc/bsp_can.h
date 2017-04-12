#ifndef _BSP_CAN_H_
#define _BSP_CAN_H_

#include "includes.h"
#include "mj_lib.h"

#define MY_CAN1_GPIO                  GPIOA
#define GPIO_MY_CAN1_CLOCK						RCC_AHB1Periph_GPIOA
#define PIN_MY_CAN1_RX                GPIO_Pin_11
#define PIN_MY_CAN1_TX                GPIO_Pin_12
#define PINSOURCE_MY_CAN1_RX          GPIO_PinSource11
#define PINSOURCE_MY_CAN1_TX          GPIO_PinSource12

#define KEYA_STDID            0x601
#define KEYA_R_BAT                0     //系统电压
#define KEYA_R_CSPEED             1     //编码器反馈的速度
#define KEYA_R_HSPEED             2     //霍尔反馈的速度
#define KEYA_R_CCOUNT             3     //编码器计数相对值
#define KEYA_R_HCOUNT             4     //霍尔计数相对值
#define KEYA_S_STOPON             5     //紧急停机
#define KEYA_S_STOPOFF            6     //紧急停机释放
#define KEYA_S_SPEED              7     //设置速度




void Config_Can(void);
CanRxMsg * can_get_msg_fifo0(void);
CanRxMsg * can_get_msg_fifo1(void);

#endif
