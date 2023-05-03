#include <stm32f10x_map.h>
#include <stm32f10x_nvic.h>
#include "delay.h"

#define HWREG(x)  (*((volatile unsigned long *)(x)))   /*  ���ֵķ�ʽ�����ڴ�   */
#define HWREGB(x)  (*((volatile unsigned long *)(x)))  /*  ���ֽڷ�ʽ�����ڴ�   */
 
#define NVIC_APINT  0xE000ED0C               /*  Ӧ���жϺ͸�λ���ƼĴ���  */
#define PRI_15      0xE000ED23               /*  SysTickϵͳ�ж����ȼ���ַ */
 
static u8  fac_us=0;//us��ʱ������

static u16 delay_13us=0;//��ʱ13us��������13us��77�����Ʊ��1ms����ʱ�� 
						//��Ϊ�������е�delay_ms����ʹ���ǽ��Ƶģ�����Ҫ��ȷ����˲��ø÷���

//��ʼ���ӳٺ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��
void delay_init(u8 SYSCLK)
{
	SysTick->CTRL&=0xfffffffb;//bit2���,ѡ���ⲿʱ��  HCLK/8
	fac_us=SYSCLK/8;		    
}	

//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��72M������,nms<=1864 
void delay_ms(u16 nms)
{
	delay_13us = nms*77 ;
	while(delay_13us != 0) ;
		 		  	  
//	u32 temp;		   
//	SysTick->LOAD=(u32)nms*fac_ms;//ʱ�����(SysTick->LOADΪ24bit)
//	SysTick->VAL =0x00;           //��ռ�����
//	SysTick->CTRL=0x01 ;          //��ʼ����  
//	do
//	{
//		temp=SysTick->CTRL;
//	}while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
//	SysTick->CTRL=0x00;       //�رռ�����
//	SysTick->VAL =0X00;       //��ռ�����	  	    
}

//SysTick�ж�init
void SysTickIntInit()
{
	SysTick->LOAD=13*fac_us;    	//ʱ�����(SysTick->LOADΪ24bit) //���ں�����ж�13us
	SysTick->VAL =0x00;         	//��ռ�����
	SysTick->CTRL |= (1<<1 | 1<<0) ;
	HWREG(NVIC_APINT) = (5 << 8) | (0x5FA << 16);    /*  2λ�����ȼ���6λ�����ȼ� */
	HWREGB(PRI_15) |= 0x16;//150;               	 /*  SysTickϵͳ�ж����ȼ�Ϊ2 */	
}

//SysTick�жϷ�������
void SysTick_Handler()
{
	extern u8 num ;
	//��⴮�ڣ���Ϊ1��תPC3������ֵ0
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
