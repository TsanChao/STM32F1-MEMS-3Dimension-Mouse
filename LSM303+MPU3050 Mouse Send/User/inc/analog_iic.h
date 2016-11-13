#ifndef ANALOG_IIC_H
#define ANALOG_IIC_H
#include "stm32f10x.h"

//************************************
/*模拟IIC端口输出输入定义*/
#define SCL_H         GPIOB->BSRR = GPIO_Pin_6
#define SCL_L         GPIOB->BRR  = GPIO_Pin_6 
   
#define SDA_H         GPIOB->BSRR = GPIO_Pin_7
#define SDA_L         GPIOB->BRR  = GPIO_Pin_7

#define SCL_Read      GPIOB->IDR  & GPIO_Pin_6
#define SDA_Read      GPIOB->IDR  & GPIO_Pin_7

/* 函数申明 -----------------------------------------------*/
// void RCC_Configuration(void);
// void GPIO_Configuration(void);
// void NVIC_Configuration(void);
// void USART1_Configuration(void);
//void WWDG_Configuration(void);
void I2C_GPIO_Config(void);
void I2C_delay(void);
void delay5ms(void);
u8 I2C_Start(void);
void I2C_Stop(void);
void I2C_Ack(void);
void I2C_NoAck(void);
u8 I2C_WaitAck(void);
void I2C_SendByte(u8 SendByte);
unsigned char I2C_RecvByte(void);
u8 I2C_WriteByte(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data);
unsigned char I2C_ReadByte(unsigned char SlaveAddress,unsigned char REG_Address);

#endif













