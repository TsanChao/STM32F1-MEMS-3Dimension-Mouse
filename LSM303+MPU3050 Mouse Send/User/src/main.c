/***************************INCLUDES**********************************/
#include "stm32f10x.h"
#include "analog_iic.h"
#include "delay.h"
#include "key.h"
#include "usart.h"
#include "timer.h"
#include "24l01.h"
#include "gyroscope.h"
#include "accelerometer.h"
#include "magnetometer.h"
#include "calibration.h"
#include "AHRS_states.h"
/****************************GLOBAL DECLARE****************************/
u32 data_ready=1;
float interval_us=33333.333;
u8 TX_ADDRESS[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//本地地址
u8 RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//接收地址

//JTAG模式设置定义
#define JTAG_SWD_DISABLE   0X02
#define SWD_ENABLE         0X01
#define JTAG_SWD_ENABLE    0X00	

void JTAG_Set(u8 mode)
{
	u32 temp;
	temp=mode;
	temp<<=25;//如果mode=SWD_ENABLE=0x01,0x01<<25位
	RCC->APB2ENR|=1<<0;     //开启辅助时钟	AFIO   
	AFIO->MAPR&=0XF8FFFFFF; //清除MAPR的[26:24]
	AFIO->MAPR|=temp;       //设置jtag模式
} 
/**********************************************************************
*函数名称：main
*函数功能：主函数
*入口参数：无
*出口参数：无
**********************************************************************/
int main(void)
{
	JTAG_Set(SWD_ENABLE);//SWD_ENABLE
	delay_init();
	I2C_GPIO_Config();
	usart_init();
	NRF24L01_Init();
	Gyro_Init();
	Acc_Init();
	Mag_Init();
	Mag_Zero_Test();
	Sensor_Calibration();
	TX_Mode();
	TIM2_Configuration();
	while(1)
	{
	}
}
