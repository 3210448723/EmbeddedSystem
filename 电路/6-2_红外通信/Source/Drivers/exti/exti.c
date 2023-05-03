#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
  

//外部中断2服务程序
void EXTI2_IRQHandler(void)
{
	extern u8 num ;
	delay_ms(30) ; //消抖
	if(key1 == 0)
	{
		//通过串口3发送数据
		SendData_viaU3(num) ;			
	}
	EXTI->PR=1<<2;  //清除LINE2上的中断标志位
}



//外部中断初始化程序
//初始化PA0,PA13,PA15为中断输入.
void EXTIX_Init(void)
{
	RCC->APB2ENR|=1<<4;     	//使能PORTC时钟
	Ex_NVIC_Config(GPIO_C,2,FTIR); //上升沿触发RTIR,下降沿触发FTIR
	MY_NVIC_Init(3,3,EXTI2_IRQChannel,2);    
}
