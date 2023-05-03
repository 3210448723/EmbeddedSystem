#include "rtc.h"
#include "delay.h"

tm timer; //ʵʱʱ�ӽṹ��

/****************************************************
* ʵʱʱ������
* ��ʼ��RTCʱ��,ͬʱ���ʱ���Ƿ�������
* BKP->DR1���ڱ����Ƿ��һ�����õ�����
* ����0:����
* ����:�������
******************************************************/
u8 RtcInit(void)
{
	//����Ƿ��ǵ�һ������ʱ��
	u8 temp = 0;
	if( BKP->DR1!=0x5050 )
	{
		RCC->APB1ENR |= 1<<28; //ʹ�ܵ�Դʱ��
		RCC->APB1ENR |= 1<<27; //ʹ�ܱ���ʱ��
		PWR->CR |= 1<<8; //ȡ������ȥд����
		RCC->BDCR |= 1<<16; //����������λ
		RCC->BDCR &= ~(1<<16); //����������λ����
		RCC->BDCR |= 1<<0; //�����ⲿ��������
		while( ( !(RCC->BDCR&0x02) ) && (temp<250) ) //�ȴ��ⲿʱ�Ӿ���
		{
			temp++;
			delay_ms(10);
		}
		if( temp>=250 )
			return 1; //��ʼ��ʱ��ʧ�ܣ�����������
		RCC->BDCR |= 1<<8; //LSI��ΪRTCʱ��
		RCC->BDCR |= 1<<15; //RTCʱ��ʹ��
		while( !(RTC->CRL & (1<<5)) )
			; //�ȴ�RTC�Ĵ����������
		while( !(RTC->CRL&(1<<3)) )
			; //�ȴ�RTC�Ĵ���ͬ��
		RTC->CRH |= 0x01; //�������ж�
		while( !(RTC->CRL&(1<<5)) )
			; //�ȴ�RTC�Ĵ����������
		RTC->CRL |= 1<<4; //��������
		RTC->PRLH = 0x0000;
		RTC->PRLL = 0x7fff; //32767 ʱ����������(�д��۲�,���Ƿ�������?)����ֵ��32767
		AutoTimeSet(); 
		//RtcSet(2014,12,2,10,0,55); //����ʱ��
		RTC->CRL &= ~(1<<4); //���ø���
		while( !(RTC->CRL & (1<<5)) )
			; //�ȴ�RTC�Ĵ����������
		BKP->DR1 = 0x5050;
		//BKP_Write(1,0X5050);;//�ڼĴ���1����Ѿ������� 
		//printf("FIRST TIME\n");
	}
	else //ϵͳ������ʱ
	{
		while( !(RTC->CRL & (1<<3)) )
			; //�ȴ�RTC�Ĵ���ͬ��
		RTC->CRH |= 0x01; //�������ж�
		while( !(RTC->CRL&(1<<5)) )
			; //�ȴ�RTC�Ĵ����������
	}
	MY_NVIC_Init(0, 0, RTC_IRQChannel, 2); //RTC,G2,P2,S2.���ȼ����  
	RtcGet(); //����ʱ��
	return 0; //�ɹ�
}

/****************************************************
* RTC�жϷ�����		
* RTCʱ���ж�
* ÿ�봥��һ��
******************************************************/
void RTC_IRQHandler(void)
{
	if( RTC->CRL & 0x0001 ) //�����ж�
	{
		RtcGet(); //����ʱ��
	}
	if( RTC->CRL & 0x0002 ) //�����ж�
	{
		RTC->CRL &= ~(0x0002); //�������ж�
		//���Ӵ���  doing
	}
	RTC->CRL &= 0x0ffa; //�������������жϱ�־
	while( !(RTC->CRL&(1<<5)) )
			; //�ȴ�RTC�Ĵ����������
}
	
/****************************************************
* �ж��Ƿ������꺯��
* �·�   1  2  3  4  5  6  7  8  9  10 11 12
* ����   31 29 31 30 31 30 31 31 30 31 30 31
* ������ 31 28 31 30 31 30 31 31 30 31 30 31
* ����:���
* ���:������ǲ�������.1,��.0,����
******************************************************/
u8 IsLeapYear(u16 y)
{
	if( y%4==0 ) //�����ܱ�4����
	{
		if( y%100==0 )
		{
			if( y%400==0 )
				return 1; //�����00��β�����ܱ�400����
			else
				return 0;
		}
		return 1;
	}
	else
		return 0;
}

/****************************************************
* ����ʱ��
* �������ʱ��ת��Ϊ����
* ��1970��1��1��Ϊ��׼
* 1970~2099��Ϊ�Ϸ����
* ����ֵ:0,�ɹ�;����:�������.
* �·����ݱ�
******************************************************/
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //���������ݱ�	  
//ƽ����·����ڱ�
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};
u8 RtcSet(u16 sy, u8 sm, u8 sd, u8 hour, u8 min, u8 sec)
{
	u16 t;
	u32 seccount = 0;
	if( sy<1970 || sy>2099 )
		return 1;
	for( t=1970; t<sy; t++ ) //��������ݵ��������
	{
		if( IsLeapYear(t) )
			seccount += 31622400; //�����������
		else //ƽ���������
			seccount += 31536000; 
	}
	sm -= 1;
	for( t=0; t<sm; t++ ) //��ǰ���·ݵ����������
	{
		seccount += (u32)mon_table[t] * 86400; //�·����������
		if( IsLeapYear(sy)&&t==1 )
			seccount += 86400; //����2�·�����һ�������
	}
	seccount += (u32)(sd - 1)*86400; //��ǰ�����ڵ����������
	seccount += (u32)hour*3600; //Сʱ������
	seccount += (u32)min*60;
	seccount += sec; //�������Ӽ���ȥ
	
	//����ʱ��
	RCC->APB1ENR |= 1<<28; //ʹ�ܵ�Դʱ��
	RCC->APB1ENR |= 1<<27; //ʹ�ܱ���ʱ��
	PWR->CR |= 1<<8; //ȡ��������д����
	//������������
	RTC->CRL |= 1<<4; //��������
	RTC->CNTL = seccount&0xffff;
	RTC->CNTH = seccount>>16;
	RTC->CRL &= ~(1<<4); //���ø���
	while( !(RTC->CRL&(1<<5)) )
		; //�ȴ�RTC�Ĵ����������
	return 0;
}

/****************************************************
* �õ���ǰ��ʱ��
* ����ֵ:0,�ɹ�;����:�������.
******************************************************/
u8 RtcGet(void)
{
	static u16 daycnt = 0;
	u32 timecount = 0;
	u32 temp = 0;
	u16 temp1 = 0;
	timecount = RTC->CNTH; //�õ��������е�ֵ����������
	timecount <<= 16;
	timecount += RTC->CNTL;
	
	temp = timecount/86400; //�õ�����
	if( daycnt!=temp ) //������һ��
	{
		daycnt = temp;
		temp1 = 1970; //��1970�꿪ʼ
		while( temp>=365 )
		{
			if( IsLeapYear(temp1) ) //������
			{
				if( temp>=366 )
					temp-=366; //�����������
				else
					break;
			}
			else
				temp-=365; //ƽ��
			temp1++;
		}
		timer.year = temp1; //�õ����
		temp1 = 0;
		while( temp>=28 ) //������һ����
		{
			if( IsLeapYear(timer.year) && temp1==1 ) //����������·�
			{
				if( temp>29 )
					temp-=29; //�����������
				else
					break;
			}
			else
			{
				if( temp>=mon_table[temp1] )
					temp-=mon_table[temp1]; //ƽ��
				else 
					break;
			}
			temp1++;
		}
		timer.month = temp1+1; //�õ��·�
		timer.date = temp+1; //�õ�����
	}
	temp = timecount%86400; //�õ�������
	timer.hour = temp/3600; //Сʱ
	timer.minute = (temp%3600)/60; //����
	timer.sec = (temp%3600)%60; //����
	timer.week = RtcGetWeek( timer.year, timer.month, timer.date); //��ȡ����
	return 0;
}

/****************************************************
* ������������ڼ�
* ��������:���빫�����ڵõ�����(ֻ����1901-2099��)
* ������������������� 
* ����ֵ�����ں�
******************************************************/
u8 RtcGetWeek(u16 y, u8 m, u8 day)
{
	u16 temp;
	u8 yearH, yearL;
	
	yearH = y/100;
	yearL = y%100;
	//���Ϊ21���ͣ��������100
	if( yearH>19 )
		yearL += 100;
	//����������ֻ��1900��֮���
	temp = yearL + yearL/4;
	temp = temp%7;
	temp = temp+day+table_week[m-1];
	if( yearL%4==0 && m<3 )
		temp--;
	return temp%7;
}

/****************************************************
* �Ƚ������ַ���ָ�����ȵ������Ƿ����
* ����:s1,s2Ҫ�Ƚϵ������ַ���;len,�Ƚϳ���
* ����ֵ:1,���;0,�����
******************************************************/
u8 str_cmpx(u8 *s1, u8 *s2, u8 len)
{
	u8 i;
	for( i=0; i<len; i++ )
		if( (*s1++) != *s2++ )
			return 0;
	return 1;
}

extern const u8 *COMPILED_DATE; //��ñ�������
extern const u8 *COMPILED_TIME; //��ñ���ʱ��

const u8 MonthTab[12][3] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"}; 
/****************************************************
* �Զ�����ʱ��Ϊ������ʱ��
******************************************************/
void AutoTimeSet(void)
{
	u8 temp[3];
	u8 i;
	u8 mon, date;
	u16 year;
	u8 sec, min, hour;
	for( i=0; i<3; i++ )
		temp[i] = COMPILED_DATE[i]; 
	for( i=0; i<12; i++ )
		if( str_cmpx( (u8*)MonthTab[i], temp, 3 ) )
			break;
	mon = i + 1; //�õ��·�
	if( COMPILED_DATE[4]==' ' )
		date = COMPILED_DATE[5] - '0';
	else
		date = 10*(COMPILED_DATE[4]-'0') + COMPILED_DATE[5]-'0';
	year = 1000*(COMPILED_DATE[7]-'0')+100*(COMPILED_DATE[8]-'0')+10*(COMPILED_DATE[9]-'0')+COMPILED_DATE[10]-'0';
	hour = 10*(COMPILED_TIME[0]-'0')+COMPILED_TIME[1]-'0';
	min = 10*(COMPILED_TIME[3]-'0')+COMPILED_TIME[4]-'0';
	sec = 10*(COMPILED_TIME[6]-'0')+COMPILED_TIME[7]-'0';
	RtcSet(year, mon, date, hour, min, sec);
}


