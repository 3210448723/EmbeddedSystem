/******************************8位数码管动态扫描************************
* 8位数码管动态扫描
* 每位数码管显示的数字从0加到9
***********************************************************************/

#include "sys.h"
#include "delay.h"
#include "led.h"

 
#define uchar unsigned char

uchar flag,count;
const uchar unshow=200;
int total_time=0;
uchar seg[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
uchar id[] = {2,0,0,5,0,1,0,2,1,5,unshow,unshow,unshow,unshow,unshow,unshow,unshow,unshow};
uchar show[8]={0},i=0,j=0;

/***************************主函数*****************************/
int main()
{
	//uchar i;
	Stm32_Clock_Init( 6 ); 
	delay_init( 72 );
	LED_Init();
	LED_SEL = 0;
	/*
	show_w1=0;
	show_w2=1;
	show_w3=2;
	show_w4=3;
	show_w5=4;
	show_w6=5;
	show_w7=6;
	show_w8=7;
	*/
	j=0,i=0;
	while(1)
	{
		if(total_time%1000==0){
			if(j==11){
				j=0;
			}
			for(i=0;i<8;i++){
				show[i]=id[i+j];
			}
			j++;
		}
		for(i=0;i<8;i++){
			if(show[i]==unshow){
				SEL0 = i%2;
				SEL1 = i/2%2;
				SEL2 = i/4;
				LedValue(0x00);
			}
			else SetLed(i, show[i]);
			delay_ms(1);
			total_time++;
		}
	}
}
