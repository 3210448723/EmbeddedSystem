#include "stm32f10x_map.h"
//STM32 GPIO TEST
//set GPIO Register
#define GPIOB_CRL ((unsigned long int *)0x40010C00)
#define GPIOB_ODR ((unsigned long int *)0x40010C0C)
#define GPIOE_CRH ((unsigned long int *)0x40011804)
#define GPIOE_ODR ((unsigned long int *)0x4001180C)

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
	RCC->APB2ENR|=1<<0;	//ʹ��AFIO
	RCC->APB2ENR|=1<<3;  //ʹ��PORTBʱ��
	RCC->APB2ENR|=1<<6;	//ʹ��PORTEʱ��


	AFIO->MAPR |= 0x02000000; //����PB.3ΪI/O�ڿ��ã��ҿ���SW����
	*GPIOB_CRL &= 0xFFFF0FFF;
	*GPIOB_CRL = *GPIOB_CRL | 0x00003000; //PB.3�������
	*GPIOB_ODR |= 0x00000008; //PB.3����ߵ�ƽ
	
	*GPIOE_CRH &=0X00000000;
	*GPIOE_CRH |= 0X33333333; //PE.8-15�������

	*GPIOE_ODR |= 0x0000FF00; //PE.8-15����ߵ�ƽ
	delay(2);
	*GPIOE_ODR &= 0xFFFF00FF; //PE.8-15����͵�ƽ
	delay(5);
	while(1)
	{
		*GPIOE_ODR = 0x00000F00; //LED1
		delay(2);
		*GPIOE_ODR = 0x0000F000; //LED2
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

