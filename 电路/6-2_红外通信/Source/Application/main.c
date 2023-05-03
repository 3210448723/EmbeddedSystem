/******************************红外通信测试************************
* 功能：红外通信，使用按键控制数码管值加减和发送
* 描述：数码管显示数值，按键控制数值的加减和发送。发送使用红外。
		KEY3控制值加1，KEY2控制值减1，KEY3和KEY2是按键扫描实现；
		KEY1控制数据发送，是通过外部中断实现。
* 作者：周佳
******************************************************************/

#include "sys.h"
#include "led.h"
#include "delay.h"
#include "key.h"
#include "exti.h"
#include "usart.h"

u8 num ;
/***************************主函数*****************************/
int main()
{
	u8 i = 0 ;
	num = 0 ;
	Stm32_Clock_Init( 6 ); 
	delay_init( 72 );
	LED_Init();
	LED_SEL = 0; 		//0选择数码管，1选择二极管
	SysTickIntInit() ;	//定时器中断初始化
	KeyInit() ;			//按键初始化
	EXTIX_Init() ;		//外部中断初始化
	uart3_init(36,1200) ;//串口3初始化 //使用红外发送数据的时候波特率不能太高

	while(1)
	{
		i =   KeyScan() ;
		SetLed(7,num) ;

		if(i == 2)	//与 if KeyScan() == 2 的区别。
		{
			num-- ;
		}
		if(i == 3)
		{
			num++ ;
		}
	}	
}

