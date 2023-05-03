#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h" 
#include "key.h"
#include "exti.h"
#include "wdg.h"
#include "timer.h"	 	 
//PWM��� ʵ��

int main(void)
{		
 	u16 led0pwmval=0;
	u8 dir=1;	
 	Stm32_Clock_Init(9); //ϵͳʱ������
	delay_init(72);	     //��ʱ��ʼ��
	uart_init(72,9600);  //���ڳ�ʼ�� 
	LED_Init();		  	 //��ʼ����LED���ӵ�Ӳ���ӿ�
	PWM_Init(900,0);	 //����Ƶ��PWMƵ��=72000/900=8Khz
   	while(1)
	{
 		delay_ms(10);	 
		if(dir)led0pwmval++;
		else led0pwmval--;

 		if(led0pwmval>300)dir=0;
		if(led0pwmval==0)dir=1;	
												 
		LED0_PWM_VAL=led0pwmval;	   
	}	 
}

























