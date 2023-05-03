#ifndef __DELAY_H
#define __DELAY_H 			   
#include <stm32f10x_map.h>
#include <stm32f10x_nvic.h>
#include "sys.h"
 
#define TX3 	PCin(10)
#define IR_OUT 	PCout(3)

void delay_init(u8 SYSCLK);
void SysTickIntInit(void) ;
void delay_ms(u16 nms);

#endif





























