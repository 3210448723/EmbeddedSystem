#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//通用定时器 驱动代码			   
////////////////////////////////////////////////////////////////////////////////// 	  

//通过改变TIM3->CCR2的值来改变占空比，从而控制LED0的亮度
#define LED0_PWM_VAL TIM3->CCR2 

void Timerx_Init(u16 arr,u16 psc);
void PWM_Init(u16 arr,u16 psc);
#endif























