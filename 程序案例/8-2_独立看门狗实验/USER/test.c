#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h" 
#include "key.h"
#include "exti.h"
#include "wdg.h"	 	 
//�������Ź�ʵ��
int main(void)
{			
 	Stm32_Clock_Init(9); //ϵͳʱ������
	delay_init(72);	     //��ʱ��ʼ��
	uart_init(72,9600);  //���ڳ�ʼ�� 
	LED_Init();		  	 //��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();          //������ʼ��	 
	delay_ms(300);   	 //���˿��õ���
	IWDG_Init(4,625);    //���Ƶ��Ϊ64,����ֵΪ625,���ʱ��Ϊ1s	   
	LED0=1;				 //����LED0
	LED3=1;
	while(1)
	{
		if(KEY_Scan()==1)IWDG_Feed();//���K1����,��ι��
		delay_ms(10);
	};	 
}

























