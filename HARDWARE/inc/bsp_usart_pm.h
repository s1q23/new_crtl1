#ifndef _BSP_USART_PM_H
#define _BSP_USART_PM_H

#include "sys.h"
#include "stm32f4xx.h"
#include "com_libs.h"
#include "mj_lib.h"

#define PACKET_PM_MAX_LEN          16

typedef struct _usart_pm_buffer
{
	unsigned char data[PACKET_PM_MAX_LEN];
	short len;
}usart_pm_buffer;

void usart_pm_init(uint32_t baud);
usart_pm_buffer * usart_pm_get_data(void);

#endif
