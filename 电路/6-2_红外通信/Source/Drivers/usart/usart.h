#ifndef __USART_H
#define __USART_H
#include <stm32f10x_map.h>
#include <stm32f10x_nvic.h>
#include "stdio.h"	 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
////////////////////////////////////////////////////////////////////////////////// 	  
 
	  	
extern u8 USART1_RX_BUF[64];     //接收缓冲,最大63个字节.末字节为换行符 
extern u8 USART1_RX_STA;         //接收状态标记	

extern u8 USART2_RX_BUF[64];     //接收缓冲,最大63个字节.末字节为换行符 
extern u8 USART2_RX_STA;         //接收状态标记	

//如果想串口中断接收，请不要注释以下宏定义
#define EN_USART1_RX //使能串口1接收
#define EN_USART2_RX //使能串口2接收
#define EN_USART3_RX //使能串口3接收
void uart1_init(u32 pclk2,u32 bound);
void uart2_init(u32 pclk1,u32 bound);
void uart3_init(u32 pclk1,u32 bound);
void SendData_viaU3(u8 value);

#endif	   
















