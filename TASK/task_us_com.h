#ifndef _TASK_US_COM_H_
#define _TASK_US_COM_H_
#include "includes.h"
#include "mj_lib.h"

typedef struct _pack_utlr
{
  uint8_t utlt_head_1;//  ?
  uint8_t utlt_head_2;//  ?
  uint8_t utlt_head_3;//  ?
  uint8_t utlt_DH;
  uint8_t utlt_CG;
  uint8_t utlt_BF;
  uint8_t utlt_AE;
  uint8_t utlt_end;
}pack_utlr;

#endif
