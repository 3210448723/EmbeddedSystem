#ifndef __LED_H
#define __LED_H
	 
#include "sys.h"

//LED驱动代码			 


//LED端口定义
#define LED_SEL PBout(3) //PB3

//位选
#define SEL0 PBout(0)
#define SEL1 PBout(1)
#define SEL2 PBout(2)

//段选
#define LED0 PEout(8)
#define LED1 PEout(9)
#define LED2 PEout(10)
#define LED3 PEout(11)
#define LED4 PEout(12)
#define LED5 PEout(13)
#define LED6 PEout(14)
#define LED7 PEout(15)



void LED_Init(void);//初始化		 				    
#endif
