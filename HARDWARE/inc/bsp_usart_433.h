#ifndef _BSP_USART_433_H_
#define _BSP_USART_433_H_

#include "sys.h"
#include "stm32f4xx.h"                  // Device header
#include "com_libs.h"
#include "mj_lib.h"

#define PACKET_433_MAX_LEN      128
#define RF433_CMD_LED           12
/*ͨ�ô��ڽ��սṹ�壬ûʲô��*/
typedef struct _usart_433_buffer
{
	unsigned char data[PACKET_433_MAX_LEN];
	short len;
}usart_433_buffer;

// cmd struct, ��433Mhz ����ͨ��
typedef struct _rf433_cmd_t 
{
	unsigned char   hdr;		  // ��ͷ��ʶ
	unsigned char   dat[12];	// ��������
}rf433_cmd_t;

void usart_433_init(uint32_t baud);
void usart_433_send(char* buffer);
void usart_433_send_str(char *str);
usart_433_buffer * Rf433_get_data(void);
#endif
