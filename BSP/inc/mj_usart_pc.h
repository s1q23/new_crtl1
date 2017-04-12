#ifndef _MJ_USART_PC_H_
#define _MJ_USART_PC_H_

#include "sys.h"
#include "stm32f4xx.h"                  // Device header
#include "com_libs.h"
#include "mj_lib.h"

#define PACKET_PC_MAX_LEN      128

typedef struct _usart_pc_buffer
{
	unsigned char data[PACKET_PC_MAX_LEN];
	short len;
}usart_pc_buffer;

void usart_pc_init(uint32_t baud);
void usart_pc_send(char* buffer);
usart_pc_buffer *PcCom_get_data(void);

#endif
