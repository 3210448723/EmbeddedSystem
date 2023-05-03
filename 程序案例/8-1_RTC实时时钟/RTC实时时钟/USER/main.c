#include "sys.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "rtc.h"
#include "usart.h"

const u8 *COMPILED_DATE=__DATE__;//获得编译日期
const u8 *COMPILED_TIME=__TIME__;//获得编译时间
const u8* Week[7]={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};

int main(void)
{
	u8 key = 0;
	Stm32_Clock_Init( 6 ); 
	delay_init( 72 );
	LED_Init();
	LED_SEL = 0;
	KeyInit();
	RtcInit();
		
	
	while(1)
	{
			key = KeyScan();
			SetLed(0,timer.hour/10);
			delay_ms(1);
			SetLed(1,timer.hour%10);
			delay_ms(1);
			SetLed(2,10);
			delay_ms(1);
			SetLed(3,timer.minute/10);
			delay_ms(1);
			SetLed(4,timer.minute%10);
			delay_ms(1);
			SetLed(5,10);
			delay_ms(1);
			SetLed(6,timer.sec/10);
			delay_ms(1);
			SetLed(7,timer.sec%10);
			delay_ms(1);
			if( key==1 )
			{
				while( key==1||key==0 )
				{
					SetLed(0,timer.year/1000);
					delay_ms(1);
					SetLed(1,timer.year%1000/100);
					delay_ms(1);
					SetLed(2,timer.year%100/10);
					delay_ms(1);
					SetLed(3,timer.year%10);
					delay_ms(1);
					SetLed(4,timer.month/10);
					delay_ms(1);
					SetLed(5,timer.month%10);
					delay_ms(1);
					SetLed(6,timer.date/10);
					delay_ms(1);
					SetLed(7,timer.date%10);
					delay_ms(1);
					key = KeyScan();
				}
			}
	}
}


