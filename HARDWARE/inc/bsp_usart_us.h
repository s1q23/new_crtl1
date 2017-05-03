#ifndef _BSP_USART_US_H_
#define _BSP_USART_US_H_
#include "sys.h"
#include "stm32f4xx.h"                  // Device header
#include "com_libs.h"
#include "mj_lib.h"

#define PACKET_US_MAX_LEN          8

#define US_LIMIT_DISTENCE_CM      100  //unit:cm

typedef struct _usart_us_buffer
{
	unsigned char data[PACKET_US_MAX_LEN];
	short len;
}usart_us_buffer;

void usart_us_head_init(uint32_t baud);
void usart_us_end_init(uint32_t baud);

void usart_us_head_send(char* buffer);
void usart_us_tail_send(char* buffer);
usart_us_buffer * us_head_get_data(void);
usart_us_buffer * us_tail_get_data(void);
#endif
