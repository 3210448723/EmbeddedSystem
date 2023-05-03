/******************************8位数码管动态扫描************************
* 8位数码管动态扫描
* 每位数码管显示的数字从0加到9
***********************************************************************/

#include "sys.h"
#include "delay.h"
#include "led.h"

 
#define uchar unsigned char

uchar show_w1,show_w2,show_w3,show_w4,show_w5,show_w6,show_w7,show_w8,flag,count;

/***************************主函数*****************************/
int main()
{
	Stm32_Clock_Init( 6 ); 
	delay_init( 72 );
	LED_Init();
	LED_SEL = 0;
	show_w1=0;
	show_w2=1;
	show_w3=2;
	show_w4=3;
	show_w5=4;
	show_w6=5;
	show_w7=6;
	show_w8=7;
	while(1)
	{
		SetLed(0, show_w1%10);
		delay_ms(100);
		SetLed(1, show_w2%10);
		delay_ms(100);
		SetLed(2, show_w3%10);
		delay_ms(100);
		SetLed(3, show_w4%10);
		delay_ms(100);
		SetLed(4, show_w5%10);
		delay_ms(100);
		SetLed(5, show_w6%10);
		delay_ms(100);
		SetLed(6, show_w7%10);
		delay_ms(100);
		SetLed(7, show_w8%10);
		delay_ms(100);

		show_w1++;
		show_w2++;
		show_w3++;
		show_w4++;
		show_w5++;
		show_w6++;
		show_w7++;
		show_w8++;
	}
}
