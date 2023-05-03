/****************定时器实现********************
* 
* 作者：宁晓兰
***************************************************/

#include "timer.h"

//数字钟的时，分、秒
u8 hour = 0, minute = 0, second = 0;

/****************普通按键初始化函数********************
* 通用定时器中断初始化
* 这里时钟选择为APB1的2倍，而APB1为36M
* arr：自动重装值。
* psc：时钟预分频数
* 这里使用的是定时器3!
******************************************************/
void TimerxInit(u16 arr, u16 psc)
{
	RCC->APB1ENR |= 1<<1; //TIM3时钟使能
	TIM3->ARR = arr; //设定计数器自动重装值，10为1ms
	TIM3->PSC = psc; //预分频器7200，得到10KHZ的计数时钟
	
	TIM3->DIER |= 1<<0; //允许更新中断
	TIM3->CR1 |= 0x01; //使能定时器3
	
	MY_NVIC_Init(1, 3, TIM3_IRQChannel, 2); //抢占1，子优先级3，组2
}

/****************定时器3的中断函数********************
* 定时器3的中断函数
* 每次中断，second加一
******************************************************/

void TIM3_IRQHandler( void )
{
	if( TIM3->SR & 0x0001) //溢出中断
	{
		second++;
		if( second>59 )
		{
			second = 0;
			minute++;
			if( minute>59 )
			{
				minute = 0;
				hour++;
				if( hour>23 )
					hour = 0;
			}
		}
	}
	TIM3->SR &= ~(1<<0); //清除中断标志位
}

/*****************************************************
* 数字钟显示函数
******************************************************/
void DisplayDigitalClock(void)
{
	SetLed(0, hour/10);
	delay_ms(1);
	SetLed(1, hour%10);
	delay_ms(1);
	SetLed(2, 10);
	delay_ms(1);
	SetLed(3, minute/10);
	delay_ms(1);
	SetLed(4, minute%10);
	delay_ms(1);
	SetLed(5, 10);
	delay_ms(1);
	SetLed(6, second/10);
	delay_ms(1);
	SetLed(7, second%10);
	delay_ms(1);
}


