#include "sys.h"
#include "delay.h"	
#include "led.h" 
#include "key.h"
#include "exti.h"	 	 
//Mini STM32�����巶������4
//�ⲿ�ж�ʵ��
//����ԭ��@ALIENTEK
//������̳:www.openedv.com	  

//ע��,�˴��뻹���޷�����SWD����!��Ϊʹ�����ж�,û������ͨ�ķ��������Ǽ�Ъ����SWD��! 
int main(void)
{			
 	Stm32_Clock_Init(9); //ϵͳʱ������
	delay_init(72);	     //��ʱ��ʼ��
	LED_Init();		  	 //��ʼ����LED���ӵ�Ӳ���ӿ�
	EXTIX_Init();		 //�ⲿ�жϳ�ʼ��
	while(1)
	{	    
		delay_ms(100);	  
	}	 
}

























