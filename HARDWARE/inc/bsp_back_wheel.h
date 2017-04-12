#ifndef _BSP_BACK_WHEEL_H_
#define _BSP_BACK_WHEEL_H_
#include "mj_lib.h"
#include "bsp_can.h"

#define     BUKE      	     0            //����������
#define   SENCHUANG    		   1            //ɭ��������
#define   NANUODAKE    		   2            //��ŵ���������
#define   SERVO_DRV    	  SENCHUANG       //���ݳ���ʹ�õ��������޸�
#define   FRONT_MAX_SPEED   30000         //L/s

/************ͨ�õ�����С���ٶȽӿ�***********/
bool can_sent_speed(s16 Speed);
bool can_sent_cmd(u8 cmd,u16 index,u8 value);

#endif
