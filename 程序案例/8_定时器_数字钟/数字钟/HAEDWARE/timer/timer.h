/****************定时器********************
* 
*******************************************/

#ifndef _TIMER_H
#define _TIMER_H

#include "sys.h"
#include "delay.h"
#include "led.h"

void TimerxInit(u16 arr, u16 psc); //定时器初始化
void DisplayDigitalClock(void); //显示数字时钟


#endif



