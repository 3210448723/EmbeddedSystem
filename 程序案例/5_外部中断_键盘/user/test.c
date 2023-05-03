#include "sys.h"
#include "delay.h"	
#include "led.h" 
#include "key.h"
#include "exti.h"	 	 
//Mini STM32开发板范例代码4
//外部中断实验
//正点原子@ALIENTEK
//技术论坛:www.openedv.com	  

//注意,此代码还是无法进行SWD仿真!因为使用了中断,没法用普通的方法来考虑间歇复用SWD口! 
int main(void)
{			
 	Stm32_Clock_Init(9); //系统时钟设置
	delay_init(72);	     //延时初始化
	LED_Init();		  	 //初始化与LED连接的硬件接口
	EXTIX_Init();		 //外部中断初始化
	while(1)
	{	    
		delay_ms(100);	  
	}	 
}

























