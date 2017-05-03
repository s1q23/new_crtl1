#ifndef _BSP_BACK_WHEEL_H_
#define _BSP_BACK_WHEEL_H_
#include "mj_lib.h"
#include "bsp_can.h"

#define     BUKE      	     0            //布科驱动器
#define   SENCHUANG    		   1            //森创驱动器
#define   NANUODAKE    		   2            //纳诺达克驱动器
#define   SERVO_DRV    	  NANUODAKE       //根据车上使用的驱动器修改
#define   FRONT_MAX_SPEED   30000         //L/s

typedef union
{
 volatile int             sum_Encoder;
 volatile unsigned char   single_Encoder[4];
}MotorDrvEncoder;

extern MotorDrvEncoder motor_encoder;
extern volatile char back_dir;

/************通用的设置小车速度接口***********/
bool back_wheel_set_speed(s16 Speed);
void servo_driver_init(u8 mode);//0  init  1  init_again
bool em_brake_set(int on_off);
void mj_delay_ms(unsigned int ms);
void mj_delay_us(unsigned int us);
bool buke_can_sent_cmd(u8 cmd,u16 index,u8 vlaue);
bool nanuo_can_sent(u8 Command, u16 Index, u8 Subindex, s16 Speed);
bool syntron_can_sent_cmd(u8 cmd,u16 index,u8 vlaue);

#endif
