#include <stm32f10x_map.h>
#include <stm32f10x_nvic.h> 
#include "led.h"

void LED_Init(void)
{
	RCC->APB2ENR|=1<<0;	//ʹ��AFIO
	RCC->APB2ENR|=1<<3;  //ʹ��PORTBʱ��
	RCC->APB2ENR|=1<<6;	//ʹ��PORTEʱ��	 
	   	 
	AFIO->MAPR |= 0x02000000; //����PB.3ΪI/O�ڿ��ã��ҿ���SW����
	GPIOB->CRL &= 0xFFFF0FFF;
	GPIOB->CRL |= 0x00003000; //PB.3�������
	GPIOB->ODR |= 0x00000008; //PB.3�����
												  
	GPIOE->CRH&=0X00000000;
	GPIOE->CRH|=0X33333333; //PE.8-15�������
	GPIOE->ODR|=0x0000FF00; //PE.8-15����� 
	
}
