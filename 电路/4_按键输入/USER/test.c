#include "sys.h"
#include "delay.h"	
#include "led.h" 
#include "key.h"	 	 
//��������ʵ��
int main(void)
{								  
	u8 t;	  
	Stm32_Clock_Init(9); //ϵͳʱ������
	delay_init(72);	     //��ʱ��ʼ�� 
	LED_Init();		  	 //��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
	while(1)
	{
		t=KEY_Scan();//�õ���ֵ
	   	if(t)
		{						   
			switch(t)
			{				 
				case 1:
					LED0=!LED0;
					break;
				case 2:
					LED3=!LED3;
					break;
				case 3:				
					LED7=!LED7;
					break;
			}
		}else delay_ms(10); 
	}	 
}

























