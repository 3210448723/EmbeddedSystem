/****************普通按键以及导航按键有关实现函数********************
* 作者：宁晓兰
******************************************************************/

#include "key.h"

/****************普通按键初始化函数********************
* PC0:key3	PC1:key2	PC2:key1	
******************************************************/
void KeyInit()
{
	//**硬件初始化
	RCC->APB2ENR|=1<<4; //使能PORTC时钟

	GPIOC->CRL &= 0xFFFFF000;
	GPIOC->CRL |= 0x00000888; //PC.0-2上拉输入
	GPIOC->ODR |= 0x00000007; //PC.0-2输出高

	//**变量初始化
// 	keyCountTotal = keyCountMax; //设置按键检测次数
// 	keyCount3 = 0x80+keyCountMax*2/3; 
// 	keyCount2 = 0x80+keyCountMax*2/3; 
// 	keyCount1 = 0x80+keyCountMax*2/3; 
// 	keyC3 = 1; //key3当前状态为 没有按
// 	keyP3 = 1; //key3前一个状态为 没有按
// 	keyC2 = 1; //key2当前状态为 没有按
// 	keyP2 = 1; //key2前一个状态为 没有按
// 	keyC1 = 1; //key1当前状态为 没有按
// 	keyP1 = 1; //key1前一个状态为 没有按
}

/****************普通按键处理函数********************
*返回按键值
* 0，没有任何按键按下
* 1，KEY1按下
* 2，KEY2按下
* 3，KEY3按下 
* 注意此函数有响应优先级,KEY1>KEY2>KEY3!!
******************************************************/
// u8 KeyScan(void)
// {	 
// 	if( key3==0 )
// 		keyCount3--;
// 	if( key2==0 )
// 		keyCount2--;
// 	if( key1!=1 )
// 		keyCount1--;

// 	keyCountTotal--;

// 	if( !keyCountTotal )
// 	{
// 		//**key3动作
// 		if( keyCount3<0x80 )
// 		{
// 			keyC3 = 0;
// 			if( keyP3==1 ) //下降沿动作
// 			{
// 				keyP3=0;
// 				return 3; 
// 			}
// 		}
// 		else 
// 		{
// 			keyC3 = 1;
// 			if( keyP3==0 ) //上升沿
// 				keyP3=1;
// 		}
// 		
// 		//**key2动作
// 		if( keyCount2<0x80 )
// 		{
// 			keyC2 = 0;
// 			if( keyP2==1 ) //下降沿
// 			{
// 				keyP2 = 0;
// 				return 2;
// 			}
// 		}
// 		else
// 		{
// 			keyC2 = 1;
// 			if( keyP2==0 ) //上升沿
// 				keyP2=1;
// 		}

// 		//**key1动作
// 		if( keyCount1<0x80 )
// 		{
// 			keyC1 = 0;
// 			if( keyP1==1 ) //key1下降沿
// 			{
// 				keyP1 = 0;
// 				return 1;
// 			}
// 		}
// 		else
// 		{
// 			keyC1 = 1;
// 			if( keyP1==0 ) //key1上升沿
// 				keyP1 = 1;
// 		}

// 		keyCountTotal = keyCountMax;
// 		keyCount3 = 0x80+keyCountMax*2/3;
// 		keyCount2 = 0x80+keyCountMax*2/3;
// 		keyCount1 = 0x80+keyCountMax*2/3;
// 	}
// 	return 0;
// }

/****************普通按键处理函数********************
*返回按键值
* 0，没有任何按键按下
* 1，KEY1按下
* 2，KEY2按下
* 3，KEY3按下 
* 注意此函数有响应优先级,KEY1>KEY2>KEY3!!
******************************************************/
u8 KeyScan(void)
{	 
	static u8 key_up=1;//按键按松开标志	
	
	if( key_up&&(key1==0||key2==0||key3==1))
	{
		delay_ms(10);//去抖动 
		key_up = 0;
		if( key1==0 )
		{
			return 1;
		}
		else if( key2==0 )
		{
			return 2;
		}
		else if( key3==1 )
		{
			return 3;
		}
	}
	else if( key1==1&&key2==1&&key3==0 )
		key_up=1; 	    
	return 0;// 无按键按下
}

/****************导航按键初始化函数********************
* 初始化导航按键
* PC0
* 采用ADC1通道10
******************************************************/
void KeyNavInit(void)
{
	//先初始化IO口
 	RCC->APB2ENR|=1<<4;    //使能PORTC口时钟 
	GPIOC->CRL &= 0xFFFFFFF0;//PC0 模拟输入 7:上拉输入
	//通道10/11设置			 
	RCC->APB2ENR|=1<<9;    //ADC1时钟使能	  
	RCC->APB2RSTR|=1<<9;   //ADC1复位
	RCC->APB2RSTR&=~(1<<9);//复位结束	    
	RCC->CFGR&=~(3<<14);   //分频因子清零	
	//SYSCLK/DIV2=12M ADC时钟设置为12M,ADC最大时钟不能超过14M!
	//否则将导致ADC准确度下降! 
	RCC->CFGR|=2<<14;      	 

	ADC1->CR1&=0XF0FFFF;   //工作模式清零
	ADC1->CR1|=0<<16;      //独立工作模式  
	ADC1->CR1&=~(1<<8);    //非扫描模式	  
	ADC1->CR2&=~(1<<1);    //单次转换模式
	ADC1->CR2&=~(7<<17);	   
	ADC1->CR2|=7<<17;	   //软件控制转换  
	ADC1->CR2|=1<<20;      //使用用外部触发(SWSTART)!!!	必须使用一个事件来触发
	ADC1->CR2&=~(1<<11);   //右对齐	 
	ADC1->SQR1&=~(0XF<<20);
	ADC1->SQR1&=0<<20;     //1个转换在规则序列中 也就是只转换规则序列1 			   
	//设置通道0~3的采样时间
	ADC1->SMPR1&=0XFFFFFFF0;//通道10的采样时间清空	  
	ADC1->SMPR1|=7<<0;      //通道10  239.5周期,提高采样时间可以提高精确度	 

	ADC1->CR2|=1<<0;	    //开启AD转换器	 
	ADC1->CR2|=1<<3;        //使能复位校准  
	while( ADC1->CR2&1<<3 );  //等待校准结束 			 
    //该位由软件设置并由硬件清除。在校准寄存器被初始化后该位将被清除。 		 
	ADC1->CR2|=1<<2;        //开启AD校准	   
	while( ADC1->CR2&1<<2 );  //等待校准结束
	//该位由软件设置以开始校准，并在校准结束时由硬件清除  
	
}

/****************************************************
* 获取ADC的值
* ch:通道值
* 返回值:ADC的值
******************************************************/
u16 GetAdc(u8 ch)
{
	//设置转换序列
	ADC1->SQR3 &= 0xffffffe0; //规则序列1 通道ch
	ADC1->SQR3 |= ch;
	ADC1->CR2 |= 1<<22; //启动规则转换通道
	while( !(ADC1->SR & 1<<1) )
		; //等待转换结束
	return ADC1->DR; //返回ADC值
}

/****************************************************
* 唯一区分导航方向
* ch:ADC的值
* 返回值:唯一区分导航方向的数值
* 共7个方向
* 无按键时:7	key3:0	上:5	右:1	下:2	左:4	里:5
******************************************************/
u8 ScanNavKey(u16 ch) //扫描并判断导航方向
{
	u16 adcValue;
	adcValue = ch;
	//获取唯一可区分导航方向的三位二进制数9：取11 10 9位区分导航按键
	adcValue = adcValue>>9; 
	adcValue &= 7<<0;
	return adcValue;
}
	




