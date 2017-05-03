#ifndef _RTC_H__
#define _RTC_H__

#include "sys.h"
#include "stm32f4xx.h"                  // Device header
#include "com_libs.h"

#define RTC_CHECK_VALUE       0xA0A2    // ºó±³¼Ä´æÆ÷µÄÖµ

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
	

void Config_RTC(void);

#endif
