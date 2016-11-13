#include <stm32f10x.h>	   
#include "led.h"
//Mini STM32开发板
//LED驱动代码			 


// V1.0
//初始化PA8和PD2为输出口.并使能这两个口的时钟		    
	 
//LED IO初始化
void LED_Init(void)
{
// //   RCC->APB2ENR|=1<<3;    //使能PORTB时钟
// //   GPIOB->CRL&=0XFFFFF0FF;
// // 	GPIOB->CRL|=0X00000300;//PB.2推挽输出
// // 	GPIOB->ODR|=1<<2;      //PB.2输出

	RCC->APB2ENR|=1<<2;    //使能PORTA时钟
  GPIOA->CRL&=0XFFFF0FFF;
	GPIOA->CRL|=0X00003000;//PA.2,3推挽输出
	GPIOA->ODR|=10<<2;      //PA.2,3输出
	
}






