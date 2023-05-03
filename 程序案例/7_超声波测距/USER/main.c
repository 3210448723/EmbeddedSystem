#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "ultrasonic.h"


int main( void )
{
	u8 i;
	u32 d0 = 0;
	u32 d1 = 0;
	Stm32_Clock_Init(6); //ϵͳʱ������
	delay_init(72);	     //��ʱ��ʼ�� 
	uart_init(72,9600);	 //���ڳ�ʼ��Ϊ9600
	LED_Init();		  	 //��ʼ����LED���ӵ�Ӳ���ӿ�    
	LED_SEL = 0;
	Ultran_Init();
	while(1)
	{
		  d0 = Ultrasonic();
		  d1 = Ultra_Ranging();
		  printf("D0 = %ld \t D1 = %ld\n\r",d0,d1);
		for(i=0;i<100;i++){
			SetLed(7, d1%10);
	    delay_ms(2);
		  SetLed(6, d1/10%10);
	    delay_ms(2);
		  SetLed(5, d1/100%10);
			delay_ms(2);
			
			SetLed(3, d0%10);
	    delay_ms(2);
		  SetLed(2, d0/10%10);
	    delay_ms(2);
		  SetLed(1, d0/100%10);
			delay_ms(2);
		}
	    //delay_ms(2);
		  //delay_ms(2000);
			//LED0=!LED0;//��˸LED,��ʾϵͳ��������.
			//delay_ms(10);   
	}	
}
