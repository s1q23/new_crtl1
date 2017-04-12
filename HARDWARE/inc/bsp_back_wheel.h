#ifndef _BSP_BACK_WHEEL_H_
#define _BSP_BACK_WHEEL_H_
#include "mj_lib.h"
#include "bsp_can.h"

#define     BUKE      	     0            //布科驱动器
#define   SENCHUANG    		   1            //森创驱动器
#define   NANUODAKE    		   2            //纳诺达克驱动器
#define   SERVO_DRV    	  SENCHUANG       //根据车上使用的驱动器修改
#define   FRONT_MAX_SPEED   30000         //L/s

/************通用的设置小车速度接口***********/
bool can_sent_speed(s16 Speed);
bool can_sent_cmd(u8 cmd,u16 index,u8 value);

#endif
