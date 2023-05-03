#include "sys.h"
#include "usart.h"

extern u8 num;
//////////////////////////////////////////////////////////////////////////////////	 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART2->SR&0X40)==0) ;	//对于USART2      
	USART2->DR = (u8) ch;
	return ch;
}
#endif 


//end
//////////////////////////////////////////////////////////////////



//初始化IO 串口3
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率
void uart3_init(u32 pclk1,u32 bound)  		//IR红外
{
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk1*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 

   	RCC->APB2ENR|=1<<4;   //使能PORTC口时钟    	//RCC->APB2ENR APB2外设时钟使能寄存器
	RCC->APB1ENR|=1<<18 ; //使能串口3时钟 

	GPIOC->CRH&=0XFFFF00FF;
	GPIOC->CRH|=0X00008B00; //IO状态设置
	GPIOC->CRL&=0XFFFF0FFF;
	GPIOC->CRL|=0X00003000;	//PC3 : IR发送端口

	RCC->APB2ENR|=1<<0 ;	//AFIO端口时钟使能
	AFIO->MAPR|=1<<4 ;
	AFIO->MAPR&=~(1<<5) ;	//UART3_REMAP重置为01.

	RCC->APB1RSTR|=1<<18 ;	//复位串口3
	RCC->APB1RSTR&=~(1<<18);//停止复位
	//波特率设置
 	USART3->BRR=mantissa; //波特率设置	 
	USART3->CR1|=0X200C;  //1位停止,无校验位

#ifdef EN_USART3_RX		  //如果使能了接收
	//使能接收中断
	USART3->CR1|=1<<8;    //PE中断使能
	USART3->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(2,2,USART3_IRQChannel,2);//组2,按键的中断优先级低于串口3发送优先级 
#endif
}

void USART3_IRQHandler(void)
{
	if(USART3->SR&(1<<5))//接收到数据
	{
		num = USART3->DR; 		//对USART_DR寄存器读操作时将USART3->SR&(1<<5)该位清零	 									     
	}
	//num = 0x0e;				//发送不会进入中断处理程序
}

//通过串口3发送数据
void SendData_viaU3(u8 value)
{
	USART3->DR = value ;	 	
	while((USART3->SR&0X40)==0) ;
//	{
//		num = 0x0e;
//	}//等待发送结束 
}
