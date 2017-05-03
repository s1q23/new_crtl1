#include "bsp_front_wheel.h"
#include "bsp_gpi.h"
#include "bsp_gpo.h"

static unsigned int front_pls_cnt=0;

#define xARRAY_NUM 1601
static const uint16_t SpeedUP[]=
{
//  5543,5516,5489,5463,5436,5409,5383,5357,5330,5304,5279,5253,5227,5202,
//  5176,5151,5126,5101,5076,5051,5027,5002,4978,4953,4929,4905,4881,4857,
//  4834,4810,4787,4763,4740,4717,4694,4671,4649,4626,4603,4581,4559,4537,
//  4514,4492,4471,4449,4427,4406,4384,4363,4342,4321,4300,4279,4258,4237,
//  4217,4196,4176,4155,4135,4115,4095,4075,4055,4036,4016,3997,3977,3958,
//  3939,3920,3901,3882,3863,3844,3825,3807,3788,3770,3752,3734,3715,3697,
//  3680,3662,3644,3626,3609,3591,3574,3557,3539,3522,3505,3488,3471,3455,
  3438,3421,3405,3388,3372,3356,3339,3323,3307,3291,3275,3260,3244,3228,
  3213,3197,3182,3166,3151,3136,3121,3106,3091,3076,3061,3046,3031,3017,
  3002,2988,2973,2959,2945,2931,2917,2903,2889,2875,2861,2847,2833,2820,
  2806,2793,2779,2766,2753,2740,2726,2713,2700,2687,2674,2662,2649,2636,
  2624,2611,2598,2586,2574,2561,2549,2537,2525,2513,2501,2489,2477,2465,
  2453,2441,2430,2418,2406,2395,2384,2372,2361,2350,2338,2327,2316,2305,
  2294,2283,2272,2261,2251,2240,2229,2219,2208,2198,2187,2177,2166,2156,
  2146,2136,2125,2115,2105,2095,2085,2075,2066,2056,2046,2036,2027,2017,
  2008,1998,1989,1979,1970,1960,1951,1942,1933,1924,1915,1906,1897,1888,
  1879,1870,1861,1852,1843,1835,1826,1818,1809,1800,1792,1784,1775,1767,
  1758,1750,1742,1734,1726,1718,1709,1701,1693,1685,1678,1670,1662,1654,
  1646,1639,1631,1623,1616,1608,1601,1593,1586,1578,1571,1564,1556,1549,
  1542,1535,1527,1520,1513,1506,1499,1492,1485,1478,1471,1465,1458,1451,
  1444,1438,1431,1424,1418,1411,1405,1398,1392,1385,1379,1372,1366,1360,
  1353,1347,1341,1335,1329,1322,1316,1310,1304,1298,1292,1286,1280,1275,
  1269,1263,1257,1251,1246,1240,1234,1228,1223,1217,1212,1206,1201,1195,
  1190,1184,1179,1173,1168,1163,1157,1152,1147,1142,1136,1131,1126,1121,
  1116,1111,1106,1101,1096,1091,1086,1081,1076,1071,1066,1062,1057,1052,
  1047,1042,1038,1033,1028,1024,1019,1015,1010,1006,1001,997,992,988,
  983,979,974,970,966,961,957,953,948,944,940,936,932,928,
  923,919,915,911,907,903,899,895,891,887,883,879,875,872,
  868,864,860,856,852,849,845,841,838,834,830,827,823,819,
  816,812,809,805,802,798,795,791,788,784,781,777,774,771,
  767,764,761,757,754,751,748,744,741,738,735,732,728,725,
  722,719,716,713,710,707,704,701,698,695,692,689,686,683,
  680,677,674,671,669,666,663,660,657,654,652,649,646,643,
  641,638,635,633,630,627,625,622,620,617,614,612,609,607,
  604,602,599,597,594,592,589,587,584,582,579,577,575,572,
  570,568,565,563,561,558,556,554,552,549,547,545,543,540,
  538,536,534,532,529,527,525,523,521,519,517,515,513,511,
  508,506,504,502,500,498,496,494,492,490,488,487,485,483,
  481,479,477,475,473,471,470,468,466,464,462,460,459,457,
  455,453,451,450,448,446,444,443,441,439,438,436,434,433,
  431,429,428,426,424,423,421,420,418,416,415,413,412,410,
  408,407,405,404,402,401,399,398,396,395,393,392,390,389,
  388,386,385,383,382,380,379,378,376,375,373,372,371,369,
  368,367,365,364,363,361,360,359,358,356,355,354,352,351,
  350,349,347,346,345,344,342,341,340,339,338,336,335,334,
  333,332,331,329,328,327,326,325,324,323,322,320,319,318,
  317,316,315,314,313,312,311,310,309,308,306,305,304,303,
  302,301,300,299,298,297,296,295,294,293,292,291,291,290,
  289,288,287,286,285,284,283,282,281,280,279,278,278,277,
  276,275,274,273,272,271,271,270,269,268,267,266,265,265,
  264,263,262,261,261,260,259,258,257,257,256,255,254,253,
  253,252,251,250,250,249,248,247,247,246,245,244,244,243,
  242,242,241,240,239,239,238,237,237,236,235,235,234,233,
  233,232,231,231,230,229,229,228,227,227,226,225,225,224,
  224,223,222,222,221,221,220,219,219,218,218,217,216,216,
  215,215,214,213,213,212,212,211,211,210,209,209,208,208,
  207,207,206,206,205,205,204,204,203,203,202,202,201,200,
  200,199,199,198,198,197,197,197,196,196,195,195,194,194,
  193,193,192,192,191,191,190,190,189,189,189,188,188,187,
  187,186,186,185,185,185,184,184,183,183,183,182,182,181,
  181,180,180,180,179,179,178,178,178,177,177,176,176,176,
  175,175,175,174,174,173,173,173,172,172,172,171,171,171,
  170,170,169,169,169,168,168,168,167,167,167,166,166,166,
  165,165,165,164,164,164,163,163,163,162,162,162,161,161,
  161,161,160,160,160,159,159,159,158,158,158,158,157,157,
  157,156,156,156,156,155,155,155,154,154,154,154,153,153,
  153,153,152,152,152,151,151,151,151,150,150,150,150,149,
  149,149,149,148,148,148,148,147,147,147,147,146,146,146,
  146,146,145,145,145,145,144,144,144,144,143,143,143,143,
  143,142,142,142,142,142,141,141,141,141,140,140,140,140,
  140,139,139,139,139,139,138,138,138,138,138,137,137,137,
  137,137,137,136,136,136,136,136,135,135,135,135,135,135,
  134,134,134,134,134,133,133,133,133,133,133,132,132,132,
  132,132,132,131,131,131,131,131,131,130,130,130,130,130,
  130,130,129,129,129,129,129,129,129,128,128,128,128,128,
  128,127,127,127,127,127,127,127,127,126,126,126,126,126,
  126,126,125,125,125,125,125,125,125,125,124,124,124,124,
  124,124,124,124,123,123,123,123,123,123,123,123,122,122,
  122,122,122,122,122,122,122,121,121,121,121,121,121,121,
  121,121,120,120,120,120,120,120,120,120,120,119,119,119,
  119,119,119,119,119,119,119,118,118,118,118,118,118,118,
  118,118,118,118,117,117,117,117,117,117,117,117,117,117,
  117,116,116,116,116,116,116,116,116,116,116,116,116,115,
  115,115,115,115,115,115,115,115,115,115,115,114,114,114,
  114,114,114,114,114,114,114,114,114,114,114,113,113,113,
  113,113,113,113,113,113,113,113,113,113,113,112,112,112,
  112,112,112,112,112,112,112,112,112,112,112,112,111,111,
  111,111,111,111,111,111,111,111,111,111,111,111,111,111,
  111,110,110,110,110,110,110,110,110,110,110,110,110,110,
  110,110,110,110,110,109,109,109,109,109,109,109,109,109,
  109,109,109,109,109,109,109,109,109,109,109,108,108,108,
  108,108,108,108,108,108,108,108,108,108,108,108,108,108,
  108,108,108,108,108,107,107,107,107,107,107,107,107,107,
  107,107,107,107,107,107,107,107,107,107,107,107,107,107,
  107,107,106,106,106,106,106,106,106,106,106,106,106,106,
  106,106,106,106,106,106,106,106,106,106,106,106,106,106,
  106,106,106,105,105,105,105,105,105,105,105,105,105,105,
  105,105,105,105,105,105,105,105,105,105,105,105,105,105,
  105,105,105,105,105,105,105,105,104,104,104,104,104,104,
  104,104,104,104,104,104,104,104,104,104,104,104,104,104,
  104,104,104,104,104,104,104,104,104,104,104,104,104,104,
  104,104,104,104,104,104,103,103,103,103,103,103,103,103,
  103,103,103,103,103,103,103,103,103,103,103,103,103,103,
  103,103,103,103,103,103,103,103,103,103,103,103,103,103,
  103,103,103,103,103,103,103,103,103,103,103,103,103,103,
  103,102,102,102,102,102,102,102,102,102,102,102,102,102,
  102,102,102,102,102,102,102,102,102,102,102,102,102,102,
  102,102,102,102,102,102,102,102,102,102,102,102,102,102,
  102,102,102,102,102,102,102,102,102,102,102,102,102,102,
  102,102,102,102,102,102,102,102,102,102,102,102,101,101,
  101,101,101,101,101,101,101,101,101,101,101,101,101,101,
  101,101,101,101,101,101,101,101,101,101,101,101,101,101,
  101,101,101,101,101,101,101,101,101,101,101,101,101,101,
  101,101,101,101,101,101,101,101,101,101,101,101,101,101,
  101,101,101,101,101,
};

const int ARRAY_NUM = sizeof(SpeedUP)/sizeof(SpeedUP[0]);

static const unsigned short MIN_s_unARR_X=101;//
static const unsigned short MIN_s_unARR_Y=101;//

volatile char front_dir=DIR_STOP;
/*配置步进电机的使能、方向、脉冲引脚*/
void Config_Control_Front_Wheel()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;	
	TIM_OCInitTypeDef TIM_OCInitStructure; 
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM9,ENABLE );
	//步进电机的 使能 引脚配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = PIN_FRONT_EN;
	GPIO_Init( GPIO_FRONT_EN, &GPIO_InitStructure );
	FRONT_EN_OFF();
	//步进电机的 方向引脚配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//开漏+上拉 低功耗
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = PIN_FRONT_DIR;
	GPIO_Init(GPIO_FRONT_DIR, &GPIO_InitStructure);
	//步进电机的 pwm引脚配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//开漏+上拉 低功耗 不影响pwm输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//不影响pwm输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Pin = PIN_FRONT_PWM;
	GPIO_Init( GPIO_FRONT_PWM, &GPIO_InitStructure );
	GPIO_PinAFConfig( GPIO_FRONT_PWM, PINSOURCE_FRONT_PWM, GPIO_AF_FRONT_PWM );

	/******************PWM 输出频率  1KHz******************/
	TIM_DeInit( TIMx_FRONT_PWM );
	TIM_TimeBaseInitStructure.TIM_Prescaler = 167;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;   //重复次数，指重复溢出多少次才进中断
	TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;       //自动重装载0-65536，同函数  void TIM_SetAutoreload(TIM_TypeDef* TIMx, uint32_t Autoreload)
	TIM_TimeBaseInit( TIMx_FRONT_PWM, &TIM_TimeBaseInitStructure );

	//TIM Output Compare Init
	TIM_OCStructInit( &TIM_OCInitStructure );
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;   //TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 500;                           //比较值0-65536 同 函数void TIM_SetCompare2(TIM_TypeDef* TIMx, uint32_t Compare2)
	TIM_OC2Init( TIMx_FRONT_PWM, &TIM_OCInitStructure );
	TIM_OC2PreloadConfig( TIMx_FRONT_PWM, TIM_OCPreload_Enable );  //使能预装载寄存器
	TIM_ARRPreloadConfig( TIMx_FRONT_PWM, ENABLE );                //使能自动重装载功能
	TIM_Cmd( TIMx_FRONT_PWM, ENABLE );                             //使能TIM9

  NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  TIM_ITConfig(TIMx_FRONT_PWM, TIM_IT_CC2, DISABLE);	
}

/************基本定时器Tim3配置，定时处理角度传感器信息***********/
//定时器的优先级比任务高，在紧急情况下使用
//当转向达到最大值时，车子一定要停下来
/*****************************************************************/
void Config_Tim3()
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3,ENABLE );
	
	TIM_TimeBaseInitStructure.TIM_Prescaler = 839;    //84MHz---->100KHz
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInitStructure.TIM_Period = 4999;     //   50ms 一次中断
	TIM_TimeBaseInit( TIM3, &TIM_TimeBaseInitStructure );
		
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;//定时器3中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init( &NVIC_InitStructure );
	
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	TIM_ARRPreloadConfig(TIM3, DISABLE);    //关闭自动重装载功能
	TIM_Cmd(TIM3, DISABLE);                  //使能TIM3
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);       //使能定时器更新终端
}

/*
	档位划分的思路：
1.总工17个档位，从-8到+8
2.角度范围从Min_Angel到Max_Angel
*/

/*前驱的角度到档位的转换*/
char front_angel_to_gear(float in_angel)
{
	uint8_t ret=0;
	in_angel+=(Max_Angel)/Angel_Gears;
	if(in_angel<Min_Angel) ret=0;
	else if(in_angel>Max_Angel) ret=Angel_Gears;
	else
	{
			ret=(in_angel-Min_Angel)/(2*Max_Angel)*Angel_Gears;
	}
	return ret-(Angel_Gears>>1);
}

float front_gear_to_angel(int8_t in_gear)
{
	if(in_gear>=(int8_t)(Angel_Gears>>1)) return Max_Angel;
	if(in_gear<=-(int8_t)(Angel_Gears>>1)) return Min_Angel;
	return ((float)in_gear+(Angel_Gears>>1))/Angel_Gears*2*Max_Angel+Min_Angel;
}
/*****************************************************************/
//dir:方向想信号
//speed:前驱运行速度，范围0~3999
//angel:运行到指定角度，当到达这个角度后，前驱自动停止转向
/******************************************************************/
void xfront_wheel_turn(unsigned char dir,unsigned short speed,float angel)
{
	/*0.合法性检查：如果车子已经在最左边，还要求继续往左，直接返回；
		如果车子已经在最右，还要求继续往右，直接返回*/
	float current_angel=get_front_angel();
	if((current_angel>Max_Angel)&&(dir ==DIR_LEFT))  return;
	if((current_angel<Min_Angel)&&(dir ==DIR_RIGHT)) return;
	speed=4000-speed;
	/*1.设置PWM 频率,ARR越大，频率越低*/
	TIM9->ARR  = speed;
	/*2.设置占空比,默认50%*/
	TIM9->CCR2 = (speed>>1);
	/*3.设置运动方向*/
	if(dir==DIR_LEFT) 
		FRONT_DIR_LEFT();
	else if(dir==DIR_RIGHT)
		FRONT_DIR_RIGHT();
	/*4.打开驱动器的使能*/
	 FRONT_EN_ON();
	/*5.打开脉冲*/
	TIM9->CCMR1 |=0x6000;
	dest_angel = angel;
	/*6.开启定时器3，做角度限位处理*/
	if(DIR_STOP!=front_dir) TIM3->CNT=0;
	TIM_Cmd(TIM3, ENABLE); 
	/*7.修改状态参数*/
	front_dir=dir;
}

void front_wheel_turn(unsigned char dir,unsigned short speed,float angel)
{
	/*0.合法性检查：如果车子已经在最左边，还要求继续往左，直接返回；
		如果车子已经在最右，还要求继续往右，直接返回*/
	float current_angel=get_front_angel();
	if((current_angel>Max_Angel)&&(dir ==DIR_LEFT))  return;
	if((current_angel<Min_Angel)&&(dir ==DIR_RIGHT)) return;
//	speed=4000-speed;
//	/*1.设置PWM 频率,ARR越大，频率越低*/
//	TIM9->ARR  = speed;
//	/*2.设置占空比,默认50%*/
//	TIM9->CCR2 = (speed>>1);
	/*3.设置运动方向*/
	if(dir==DIR_LEFT) 
		FRONT_DIR_LEFT();
	else if(dir==DIR_RIGHT)
		FRONT_DIR_RIGHT();
	/*4.打开驱动器的使能*/
	 FRONT_EN_ON();
	/*5.打开脉冲*/
	TIM9->CCMR1 |=0x6000;
	TIM9->DIER |= TIM_IT_CC2;
	dest_angel = angel;
	/*6.开启定时器3，做角度限位处理*/
	if(DIR_STOP!=front_dir) TIM3->CNT=0;
	TIM_Cmd(TIM3, ENABLE); 
	/*7.修改状态参数*/
	front_dir=dir;
}

/**************************前驱停止运行**********************/
void front_wheel_stop()
{
	/*0.关闭脉冲*/
	TIM9->CCMR1 &=0x8FFF;
	TIM9->DIER &= (uint16_t)~TIM_IT_CC2;
	/*1.关闭使能*/
	FRONT_EN_OFF();
	/*2.关闭定时器3*/
	TIM_Cmd(TIM3, DISABLE); 
	/*3.修改状态参数*/
	front_dir=DIR_STOP;
	front_pls_cnt=0;
}

void front_auto_mid()
{
	float angel=get_front_angel();
	front_pls_cnt=100;
	if(fabs(angel-Mid_Angel)<Err_Angel) return;
	if(angel>0) 
		front_wheel_turn(DIR_RIGHT,3500,Mid_Angel);
	else 
		front_wheel_turn(DIR_LEFT,3500,Mid_Angel);
}

/*************************************************************
这个任务主要监测前驱角度的变化，防止前驱打到死
**************************************************************/
void TIM3_IRQHandler()
{
	static float current_angel=0;
	OSIntEnter();
	if( TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		current_angel = get_front_angel();
		if(fabs(current_angel-dest_angel)<Err_Angel)
			front_wheel_stop();
		if((current_angel>Max_Angel)&&(front_dir==DIR_LEFT))
			front_wheel_stop();
		if((current_angel<Min_Angel)&&(front_dir==DIR_RIGHT))
			front_wheel_stop();
		TIM3->SR = ~TIM_FLAG_Update;          //清中断
	}
	OSIntExit();
}

/*电机加速启动控制*/
void TIM1_BRK_TIM9_IRQHandler()
{
	OSIntEnter();
	if(TIM_GetITStatus(TIM9, TIM_IT_CC2) != RESET)
	{
		if(front_pls_cnt<ARRAY_NUM)
		{
			TIM9->ARR=SpeedUP[front_pls_cnt];
			TIM9->CCR2 = (SpeedUP[front_pls_cnt]>>1);
		}
		front_pls_cnt+=2;
		TIM_ClearITPendingBit(TIM9, TIM_IT_CC2);
	}
	OSIntExit();
}
