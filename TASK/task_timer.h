#ifndef _TASK_TIMER_COM_H
#define _TASK_TIMER_COM_H

#include "includes.h"
#include "mj_lib.h"

extern OS_TMR TmrLeftLight;
extern OS_TMR TmrRightLight;
extern OS_TMR TmrFrontLight;
extern OS_TMR TmrTimeOutCheck;
extern OS_TMR TmrAngelMonitor;

void cb_TmrLeftLight(void *p_tmr,void *p_arg);
void cb_TmrRightLight(void *p_tmr,void *p_arg);
void cb_TmrFrontLight(void *p_tmr,void *p_arg);
void cb_TmrCheckAngel(void *p_tmr,void *p_arg);
void cb_TmrTimeOutCheck(void *p_tmr,void *p_arg);
void cb_TmrAngelMonitor(void *p_tmr,void *p_arg);

#endif
