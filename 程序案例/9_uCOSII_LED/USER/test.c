#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h" 
#include "key.h"	
#include "includes.h"	 
//Mini STM32��������չʵ�� 8
//ucos ʵ��
//����ԭ��@ALIENTEK
//����֧�֣�www.openedv.com

 
//���������ջ��С
#define LED_STK_SIZE     64
#define LED1_STK_SIZE    64
#define START_STK_SIZE   128

//�����������ȼ�
#define LED_TASK_Prio       9
#define LED1_TASK_Prio      5
#define START_TASK_Prio     10

//�����ջ
OS_STK  TASK_LED1_STK[LED_STK_SIZE];
OS_STK  TASK_LED_STK[LED_STK_SIZE];
OS_STK  TASK_START_STK[START_STK_SIZE];
//��������
void TaskStart(void *pdata);
void TaskLed(void *pdata);
void TaskLed1(void *pdata);

//ϵͳʱ�����ú���
void SysTick_Configuration(void);

	 
//Mini STM32������ucos����
//����ucos2.52�ں�.
//����ԭ��@ALIENTEK
//www.openedv.com
//2010.12.15			 
int main(void)
{			 	   
	Stm32_Clock_Init(9); //ϵͳʱ������
	delay_init(72);	     //��ʱ��ʼ�� 
	uart_init(72,9600);	 //���ڳ�ʼ��Ϊ9600
	LED_Init();		  	 //��ʼ����LED���ӵ�Ӳ���ӿ�  
	SysTick_Configuration();
	OSInit();
	OSTaskCreate( TaskStart,	//task pointer
					(void *)0,	//parameter
					(OS_STK *)&TASK_START_STK[START_STK_SIZE-1],	//task stack top pointer
					START_TASK_Prio );	//task priority
	
	OSStart();
	return 0;	   
}
//��ʼ����
void TaskStart(void * pdata)
{
	pdata = pdata; 
	OS_ENTER_CRITICAL();   
	OSTaskCreate(TaskLed, (void * )0, (OS_STK *)&TASK_LED_STK[LED_STK_SIZE-1], LED_TASK_Prio);
	OSTaskCreate(TaskLed1, (void * )0, (OS_STK *)&TASK_LED1_STK[LED1_STK_SIZE-1], LED1_TASK_Prio);
	OSTaskSuspend(START_TASK_Prio);	//suspend but not delete
	OS_EXIT_CRITICAL();
}
//����1
//����DS0������.
void TaskLed(void *pdata)
{
	while(1)
	{
		LED1=!LED1;
		OSTimeDlyHMSM(0,0,0,500);	
	}
}
//����2
//����DS1������.
void TaskLed1(void *pdata)
{
	while(1)
	{
		LED6=!LED6;
		OSTimeDlyHMSM(0,0,0,200);	
	}
}	  
//ϵͳʱ���жϷ�����
void SysTick_Handler(void)
{
	OS_ENTER_CRITICAL(); /* Tell uC/OS-II that we are starting an ISR  */
    OSIntNesting++;
    OS_EXIT_CRITICAL();

    OSTimeTick();        /* Call uC/OS-II's OSTimeTick()               */

    OSIntExit();         /* Tell uC/OS-II that we are leaving the ISR  */
}
//ϵͳʱ�����ã����1ms����һ���ж�
void SysTick_Configuration(void)
{
 	SysTick->CTRL&=~(1<<2);//SYSTICKʹ���ⲿʱ��Դ
	SysTick->CTRL|=1<<1;   //����SYSTICK�ж�
	SysTick->LOAD=9000;    //����1ms�ж�
	//bit2���,ѡ���ⲿʱ��  HCLK/8
	//MY_NVIC_Init(3,3,SystemHandler_SysTick,2);//��2��������ȼ� 
	SysTick->CTRL|=1<<0;   //����SYSTICK
}




















