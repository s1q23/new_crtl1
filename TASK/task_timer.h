#ifndef _TASK_TIMER_COM_H
#define _TASK_TIMER_COM_H

#include "includes.h"
#include "mj_lib.h"

extern OS_TMR TmrLeftLight;
extern OS_TMR TmrRightLight;

void cb_TmrLeftLight(void *p_tmr,void *p_arg);
void cb_TmrRightLight(void *p_tmr,void *p_arg);

#endif
