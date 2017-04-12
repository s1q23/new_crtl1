#include "task_us_com.h"

/********************************************************
 *关于返回值的解释：
 *返回 0：正确
 *返回 1：传入NULL指针
**********************************************************/
int us_pack_decode(const char *in_str,pack_utlr *out_pack)
{
	/*1.参数合法性检查*/
	if(in_str==NULL||out_pack==NULL) return 1;
	/*2.包头的检验*/
	// TODO
	return 0;
}
