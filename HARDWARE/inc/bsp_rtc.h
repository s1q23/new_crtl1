#ifndef _RTC_H__
#define _RTC_H__

#include "sys.h"
#include "stm32f4xx.h"                  // Device header
#include "com_libs.h"

typedef struct
{
  u8 year;
	u8 month;
	u8 day;
	u8 week;
	u8 hour;
	u8 mintue;
	u8 second;
}STR_SET_TIME;
	


extern void Config_RTC(void);
#endif
