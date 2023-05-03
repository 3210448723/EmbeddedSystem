#include <stm32f10x_map.h>
#include <stm32f10x_nvic.h>
#include "delay.h"

#define HWREG(x)  (*((volatile unsigned long *)(x)))   /*  以字的方式访问内存   */
#define HWREGB(x)  (*((volatile unsigned long *)(x)))  /*  以字节方式访问内存   */
 
#define NVIC_APINT  0xE000ED0C               /*  应用中断和复位控制寄存器  */
#define PRI_15      0xE000ED23               /*  SysTick系统中断优先级地址 */
 
static u8  fac_us=0;//us延时倍乘数

static u16 delay_13us=0;//延时13us。这里用13us的77倍近似表达1ms的延时。 
						//因为本程序中的delay_ms函数使用是近似的，不需要精确，因此采用该方案

//初始化延迟函数
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void delay_init(u8 SYSCLK)
{
	SysTick->CTRL&=0xfffffffb;//bit2清空,选择外部时钟  HCLK/8
	fac_us=SYSCLK/8;		    
}	

//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//对72M条件下,nms<=1864 
void delay_ms(u16 nms)
{
	delay_13us = nms*77 ;
	while(delay_13us != 0) ;
		 		  	  
//	u32 temp;		   
//	SysTick->LOAD=(u32)nms*fac_ms;//时间加载(SysTick->LOAD为24bit)
//	SysTick->VAL =0x00;           //清空计数器
//	SysTick->CTRL=0x01 ;          //开始倒数  
//	do
//	{
//		temp=SysTick->CTRL;
//	}while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
//	SysTick->CTRL=0x00;       //关闭计数器
//	SysTick->VAL =0X00;       //清空计数器	  	    
}

//SysTick中断init
void SysTickIntInit()
{
	SysTick->LOAD=13*fac_us;    	//时间加载(SysTick->LOAD为24bit) //用于红外的中断13us
	SysTick->VAL =0x00;         	//清空计数器
	SysTick->CTRL |= (1<<1 | 1<<0) ;
	HWREG(NVIC_APINT) = (5 << 8) | (0x5FA << 16);    /*  2位组优先级，6位子优先级 */
	HWREGB(PRI_15) |= 0x16;//150;               	 /*  SysTick系统中断优先级为2 */	
}

//SysTick中断服务处理函数
void SysTick_Handler()
{
	extern u8 num ;
	//检测串口，若为1则翻转PC3，否则赋值0
	if(TX3 == 1)
	{
		IR_OUT = 0 ; 	
	}
	else if(TX3 == 0)
	{
		IR_OUT = ~IR_OUT ;
	}
	if(delay_13us != 0) 
		delay_13us -- ;
}
