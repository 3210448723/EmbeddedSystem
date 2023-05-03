#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
  

//�ⲿ�ж�2�������
void EXTI2_IRQHandler(void)
{
	extern u8 num ;
	delay_ms(30) ; //����
	if(key1 == 0)
	{
		//ͨ������3��������
		SendData_viaU3(num) ;			
	}
	EXTI->PR=1<<2;  //���LINE2�ϵ��жϱ�־λ
}



//�ⲿ�жϳ�ʼ������
//��ʼ��PA0,PA13,PA15Ϊ�ж�����.
void EXTIX_Init(void)
{
	RCC->APB2ENR|=1<<4;     	//ʹ��PORTCʱ��
	Ex_NVIC_Config(GPIO_C,2,FTIR); //�����ش���RTIR,�½��ش���FTIR
	MY_NVIC_Init(3,3,EXTI2_IRQChannel,2);    
}
