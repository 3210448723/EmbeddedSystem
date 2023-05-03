#include "sys.h"
#include "usart.h"

extern u8 num;
//////////////////////////////////////////////////////////////////////////////////	 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART2->SR&0X40)==0) ;	//����USART2      
	USART2->DR = (u8) ch;
	return ch;
}
#endif 


//end
//////////////////////////////////////////////////////////////////



//��ʼ��IO ����3
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������
void uart3_init(u32 pclk1,u32 bound)  		//IR����
{
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk1*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 

   	RCC->APB2ENR|=1<<4;   //ʹ��PORTC��ʱ��    	//RCC->APB2ENR APB2����ʱ��ʹ�ܼĴ���
	RCC->APB1ENR|=1<<18 ; //ʹ�ܴ���3ʱ�� 

	GPIOC->CRH&=0XFFFF00FF;
	GPIOC->CRH|=0X00008B00; //IO״̬����
	GPIOC->CRL&=0XFFFF0FFF;
	GPIOC->CRL|=0X00003000;	//PC3 : IR���Ͷ˿�

	RCC->APB2ENR|=1<<0 ;	//AFIO�˿�ʱ��ʹ��
	AFIO->MAPR|=1<<4 ;
	AFIO->MAPR&=~(1<<5) ;	//UART3_REMAP����Ϊ01.

	RCC->APB1RSTR|=1<<18 ;	//��λ����3
	RCC->APB1RSTR&=~(1<<18);//ֹͣ��λ
	//����������
 	USART3->BRR=mantissa; //����������	 
	USART3->CR1|=0X200C;  //1λֹͣ,��У��λ

#ifdef EN_USART3_RX		  //���ʹ���˽���
	//ʹ�ܽ����ж�
	USART3->CR1|=1<<8;    //PE�ж�ʹ��
	USART3->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(2,2,USART3_IRQChannel,2);//��2,�������ж����ȼ����ڴ���3�������ȼ� 
#endif
}

void USART3_IRQHandler(void)
{
	if(USART3->SR&(1<<5))//���յ�����
	{
		num = USART3->DR; 		//��USART_DR�Ĵ���������ʱ��USART3->SR&(1<<5)��λ����	 									     
	}
	//num = 0x0e;				//���Ͳ�������жϴ������
}

//ͨ������3��������
void SendData_viaU3(u8 value)
{
	USART3->DR = value ;	 	
	while((USART3->SR&0X40)==0) ;
//	{
//		num = 0x0e;
//	}//�ȴ����ͽ��� 
}
