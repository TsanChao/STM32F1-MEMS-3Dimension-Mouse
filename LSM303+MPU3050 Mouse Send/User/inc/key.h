#ifndef _KEY_H
#define _KEY_H
/*************************************************************************************************/
#include "stm32f10x.h"

#define KEY1 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
#define KEY2 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)

#define KEY1_DOWN	0x01
#define KEY2_DOWN 0x02

void delay(int time);			//延时
void key_init(void);			//按键初始化
u8 key_scan(void);			//扫描按键
#endif
