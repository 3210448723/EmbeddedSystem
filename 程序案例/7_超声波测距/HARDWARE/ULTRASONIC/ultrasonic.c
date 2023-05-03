#include "ultrasonic.h"
#include"delay.h"

/********************************************************************************************************************************************
@f_name: void Ultran_Init(void)
@brief:	 超声波硬件模块初始化
@param:	 None
@return: None
*********************************************************************************************************************************************/
void Ultran_Init(void)
{
	RCC->APB2ENR|=1<<2;     //使能PORTA时钟
	//发送电平引脚 PA0 设置成 推挽输出
	//返回电平引脚 PA1 设置成 浮空输入
	GPIOA->CRL&=0XFFFFFF00;
	GPIOA->CRL|=0X00000083; 		
}
/********************************************************************************************************************************************
@f_name: void Ranging(void)
@brief:	 超声波测距，均值5次，实际 测量过程发现测量静态的比较稳定，动态的不稳定，3米范围内比较稳定
@param:	 float *p:测距缓存变量
@return: None
*********************************************************************************************************************************************/
u32 Ultra_Ranging(void)
{
	//u8 i;
	u32 j=0;
	u32 Ultr_Temp;	
	//for(i=0;i<5;i++)
	{
		TRIG=1;
		delay_ms(10);
		TRIG=0;
		while(!ECHO);
		while(ECHO)
		{
			delay_us(10);
			j++;
		}
		Ultr_Temp=j*340/200;//每次延时10us  //把us转成s *10 /2 /1000  = /200
		//delay_ms(60);//防止发射信号对回响信号的影响
	}
	//Ultr_Temp = Ultr_Temp/10; //   /2?
	
	return Ultr_Temp;
}

u32 Ultrasonic(void)
{
    unsigned int  E;
    u32 distance=0;
    TRIG=1;//给超声波一个信号
   delay_ms(11);;
   TRIG=0;
   while(ECHO==0);//等待低电
   while(ECHO==1)
     ++distance;
   delay_ms(6);
   E=distance/24;
   return E;
}
