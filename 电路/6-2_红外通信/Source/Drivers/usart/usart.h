#ifndef __USART_H
#define __USART_H
#include <stm32f10x_map.h>
#include <stm32f10x_nvic.h>
#include "stdio.h"	 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
////////////////////////////////////////////////////////////////////////////////// 	  
 
	  	
extern u8 USART1_RX_BUF[64];     //���ջ���,���63���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u8 USART1_RX_STA;         //����״̬���	

extern u8 USART2_RX_BUF[64];     //���ջ���,���63���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u8 USART2_RX_STA;         //����״̬���	

//����봮���жϽ��գ��벻Ҫע�����º궨��
#define EN_USART1_RX //ʹ�ܴ���1����
#define EN_USART2_RX //ʹ�ܴ���2����
#define EN_USART3_RX //ʹ�ܴ���3����
void uart1_init(u32 pclk2,u32 bound);
void uart2_init(u32 pclk1,u32 bound);
void uart3_init(u32 pclk1,u32 bound);
void SendData_viaU3(u8 value);

#endif	   
















