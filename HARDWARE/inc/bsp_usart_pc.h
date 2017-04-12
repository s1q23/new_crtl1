#ifndef _BSP_USART_PC_H_
#define _BSP_USART_PC_H_

#include "com_libs.h"     
#include "sys.h"

typedef handler_result_t (*ipc_cmd_handler_event)(ipc_control_t1 f_ipc_control); 

typedef struct _can_cmd_handler_t {
	ipc_cmd_type_t cmd;
	ipc_cmd_handler_event   event;
}ipc_cmd_handler_t;

void bsp_usart_pc_init(void);

#endif
