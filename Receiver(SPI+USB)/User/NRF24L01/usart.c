#include "usart.h"
//串口1初始化			    

//V1.3
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.

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
	USART1->DR = (u8) ch;      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	return ch;
}
#endif 
//end
//////////////////////////////////////////////////////////////////										 
//初始化串口1
void usart_init(void)
{
	//GPIOA的配置结构体/*神舟Ⅰ号的PA9对应USART1_TX*/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//USART的配置结构体
	USART_InitTypeDef USART_InitStructure;
	/* USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//使能PORTA口和USART1口时钟
				      /*
								//使能PORTA口时钟和USART1口时钟，这两种表达式一样的
								RCC->APB2ENR |=RCC_APB2Periph_GPIOA;
								RCC->APB2ENR |=RCC_APB2Periph_USART1;
								//这个表达与上边是等价的
							*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
	
	/* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//对应神舟一号的PA9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Configure USART Rx as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//对应神舟一号的PA10
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	

	USART_Init(USART1,&USART_InitStructure);
	//启动USART1
	USART_Cmd(USART1,ENABLE);
}

