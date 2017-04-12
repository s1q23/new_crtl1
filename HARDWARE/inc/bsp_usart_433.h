#ifndef _BSP_USART_433_H_
#define _BSP_USART_433_H_

#include "sys.h"
#include "stm32f4xx.h"                  // Device header
#include "com_libs.h"
#include "mj_lib.h"

#define PACKET_433_MAX_LEN      128
#define RF433_CMD_LED           12
/*通用串口接收结构体，没什么用*/
typedef struct _usart_433_buffer
{
	unsigned char data[PACKET_433_MAX_LEN];
	short len;
}usart_433_buffer;

// cmd struct, 与433Mhz 无线通信
typedef struct _rf433_cmd_t 
{
	unsigned char   hdr;		  // 包头标识
	unsigned char   dat[12];	// 包数据域
}rf433_cmd_t;

void usart_433_init(uint32_t baud);
void usart_433_send(char* buffer);
void usart_433_send_str(char *str);
usart_433_buffer * Rf433_get_data(void);
#endif
