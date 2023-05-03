#ifndef _ultrasonic_H
#define _ultrasonic_H
#include "sys.h"
#include "stm32f10x_type.h"
//-----------------------------ÒÆÖ²ÐÞ¸ÄÇø-----------------------------------

#define TRIG  PAout(0)
#define ECHO  PAin(1)


//---------------------------------------------------------------
void Ultran_Init(void);
u32 Ultra_Ranging(void);
u32 Ultrasonic(void);
#endif

