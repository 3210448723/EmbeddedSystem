#include "led.h"
#include "stm32f10x_map.h"
//STM32 GPIO TEST
//set GPIO Register
#define GPIOB_CRL ((unsigned long int *)0x40010C00)
#define GPIOB_ODR ((unsigned long int *)0x40010C0C)
#define GPIOE_CRH ((unsigned long int *)0x40011804)
#define GPIOE_ODR ((unsigned long int *)0x4001180C)

//u32 *PE08 = (u32 *)(0x42000000 +(0x4001180D-0x40000000)*32 + 0*4);
u32 *PEO8 = (u32 *)(0x42000000 +(0x4001180C-0x40000000)*32 + 8*4);

int delay(int Time)
{
	unsigned short t,i,j;
	for(t=0;t<Time;t++)
		for(i=0;i<1000;i++)
			for(j=0;j<1000;j++)
				;
	return 0;
}


int  main(void)
{
	//LED_Init();
	
	RCC->APB2ENR|=1<<0;	//使能AFIO
	RCC->APB2ENR|=1<<3;  //使能PORTB时钟
	RCC->APB2ENR|=1<<6;	//使能PORTE时钟


	AFIO->MAPR |= 0x02000000; //设置PB.3为I/O口可用，且可以SW仿真
	*GPIOB_CRL &= 0xFFFF0FFF;
	*GPIOB_CRL = *GPIOB_CRL | 0x00003000; //PB.3推挽输出
	*GPIOB_ODR |= 0x00000008; //PB.3输出高
	
	*GPIOE_CRH &=0X00000000;
	*GPIOE_CRH |= 0X33333333; //PE.8-15推挽输出
	*GPIOE_ODR |= 0x0000FF00; //PE.8-15输出高
	delay(5);
	while(1)
	{
		*GPIOE_ODR = 0x00000100; //LED1
		delay(2);
		*PEO8 = 0;
		delay(2);
		*PEO8 = 1;
		delay(2);
		*GPIOE_ODR = 0x00000200; //LED2
		delay(2);
		*GPIOE_ODR = 0x00000400; //LED3
		delay(2);
		*GPIOE_ODR = 0x00000800; //LED4
		delay(2);
		*GPIOE_ODR = 0x00001000; //LED5
		delay(2);
		*GPIOE_ODR = 0x00002000; //LED6
		delay(2);
		*GPIOE_ODR = 0x00004000; //LED7
		delay(2);
		*GPIOE_ODR = 0x00008000; //LED8
		delay(2);
	}
	
	return 0;
}

