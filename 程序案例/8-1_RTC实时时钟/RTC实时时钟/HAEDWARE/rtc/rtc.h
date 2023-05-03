#ifndef _RTC_H
#define _RTC_H

#include "sys.h"

//ʱ��ṹ��
typedef struct
{
	u8 hour;
	u8 minute;
	u8 sec;
	//����
	u16 year;
	u8 month;
	u8 date;
	u8 week;
}tm;

extern tm timer;
extern u8 const mon_table[12]; //�·��������ݱ�
void DispTimer(u8 x, u8 y, u8 size); //��ָ��λ�ÿ�ʼ��ʾʱ��
void DispWeek(u8 x, u8 y, u8 size, u8 lang); //��ָ��λ����ʾ����
u8 RtcInit(void); //��ʼ��RTC���ɹ�����0�� ʧ�ܷ���1
u8 IsLeapYear(u16 y); //ƽ�꣬�����ж�
u8 RtcGet(void); //����ʱ��
u8 RtcGetWeek(u16 y, u8 m, u8 day);
u8 RtcSet(u16 sy, u8 sm, u8 sd, u8 hour, u8 min, u8 sec); //����ʱ��
void AutoTimeSet(void); //����ʱ��Ϊ����ʱ��



#endif



