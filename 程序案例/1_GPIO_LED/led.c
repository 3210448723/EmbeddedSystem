#include <stm32f10x_map.h>
#include <stm32f10x_nvic.h> 
#include "led.h"

void LED_Init(void)
{
	RCC->APB2ENR|=1<<0;	//使能AFIO
	RCC->APB2ENR|=1<<3;  //使能PORTB时钟
	RCC->APB2ENR|=1<<6;	//使能PORTE时钟	 
	   	 
	AFIO->MAPR |= 0x02000000; //设置PB.3为I/O口可用，且可以SW仿真
	GPIOB->CRL &= 0xFFFF0FFF;
	GPIOB->CRL |= 0x00003000; //PB.3推挽输出
	GPIOB->ODR |= 0x00000008; //PB.3输出高
												  
	GPIOE->CRH&=0X00000000;
	GPIOE->CRH|=0X33333333; //PE.8-15推挽输出
	GPIOE->ODR|=0x0000FF00; //PE.8-15输出高 
	
}
