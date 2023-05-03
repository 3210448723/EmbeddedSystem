#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"


int main( void )
{
	/*u8 i = 1;
	Stm32_Clock_Init( 6 );  //9倍频
	delay_init( 72 ); //12M外部晶振
	uart_init( 72, 9600 );
	while( 1 )
	{
		//printf( "i = %d\r\n", i );
		i++;
		delay_ms( 500 );
	}
	*/
	u8 t;
	u8 len;	
	u16 times=0;  
	Stm32_Clock_Init(6); //系统时钟设置
	delay_init(72);	     //延时初始化 
	uart_init(72,9600);	 //串口初始化为9600
	LED_Init();		  	 //初始化与LED连接的硬件接口    
	while(1)
	{
		if(USART_RX_STA&0x80)
		{					   
			len=USART_RX_STA&0x3f;//得到此次接收到的数据长度
			printf("\n Your MSG: \n");
			for(t=0;t<len;t++)
			{
				USART1->DR=USART_RX_BUF[t];
				while((USART1->SR&0X40)==0);//等待发送结束
			}
			printf("\n\n");//插入换行
			USART_RX_STA=0;
		}else
		{
			times++;
			if(times%5000==0)
			{
				printf("\nSTM32A Usart\n");
			}
			if(times%200==0) printf("Please Input end with return\n");  
			if(times%30==0) LED0=!LED0;//闪烁LED,提示系统正在运行.
			delay_ms(10);   
		}
	}	
}
