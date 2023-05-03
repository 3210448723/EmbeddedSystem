/******************************流水灯************************
* 流水灯
* 现象：二极管从左至右依次全部点亮
*************************************************************/

#include "sys.h"
#include "delay.h"
#include "led.h"

u8 light;

int main( void )
{
	Stm32_Clock_Init( 6 );  //6倍频
	delay_init( 72 ); //12M外部晶振
	LED_Init();
	GPIOE->ODR &= ~(0xff<<8);
	LED_SEL = 1; //选择二极管	
	light = 0x01;
	while( 1 )
	{
// 		LED0 = (light&0x01)?1:0;
// 		LED1 = light&0x02?1:0;
// 		LED2 = light&0x04?1:0;
// 		LED3 = light&0x08?1:0;
// 		LED4 = light&0x10?1:0;
// 		LED5 = light&0x20?1:0;
// 		LED6 = light&0x40?1:0;
// 		LED7 = light&0x80?1:0;
		GPIOE->ODR |= (light<<8);
		delay_ms( 300 ); 
		light = light<<1;
		
		if( light==0x00 )
		{
			GPIOE->ODR &= ~(0xff<<8);
			delay_ms( 300 ); 
			light = 0x01;
		}					
	}
}


