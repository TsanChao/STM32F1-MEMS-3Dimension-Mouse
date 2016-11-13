/**************************************************************************
*------------------------STM32接收无线数据--------------------------------*
*-------------------------------------------------------------------------*/
 #include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "usart.h"
#include "24l01.h"
#include <stdio.h> 

#include "usb_lib.h"
#include "hw_config.h"
#include "led.h"
//#include "sys.h"

//JTAG模式设置定义
#define JTAG_SWD_DISABLE   0X02
#define SWD_ENABLE         0X01
#define JTAG_SWD_ENABLE    0X00	


int data_ready=0;							//接收到数据的标志位
u8 recv[TX_PLOAD_WIDTH];		//接收到的数据数组

//在头文件中定义的用来存放要发送的数据（拷贝到这个数组中再发送，防止出现溢出现象）
u8 TX_ADDRESS[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//本地地址
u8 RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//接收地址
/**************************************************************************
*------------------------STM32接收无线数据--------------------------------*
*-------------------------------------------------------------------------*/


__IO uint8_t PrevXferComplete = 1;	                     //是否发送的标志

u8 PreButton;                                              //记录前一时刻鼠标的按键状态

u8 Mouse_Buffer[4]={0,0,0,0};

void Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}


void JTAG_Set(u8 mode)
{
	u32 temp;
	temp=mode;
	temp<<=25;//如果mode=SWD_ENABLE=0x01,0x01<<25位
	RCC->APB2ENR|=1<<0;     //开启辅助时钟	AFIO   
	AFIO->MAPR&=0XF8FFFFFF; //清除MAPR的[26:24]
	AFIO->MAPR|=temp;       //设置jtag模式
} 

int main()
{
//	int i;
	JTAG_Set(SWD_ENABLE);//SWD_ENABLE
	
	//LED_Init();       //LED端口初始化
	//LED1_0;	           //PA2            (LED3--PA3 是24101的CE )	
  Set_System();     //USB上拉端口初始化设置   
	
  USB_Interrupts_Config();  //USB中断使能    
  Set_USBClock();     //USB时钟设置
  USB_Init();         //设备相关初始化  
	
	usart_init();//串口1初始化
		//printf("\n\r串口成功打开\n\r");
	NRF24L01_Init();//NRF24L01初始化
	//while(NRF24L01_Check())			//NRF24L01的检测函数，用作测试。用的时候在24l01.h和24l01.c中把注释取消即可
		//	printf("未检测到24L01");
	RX_Mode();//配置成接收模式

	while(1)
	{	
	
	if( data_ready && PrevXferComplete )
	 {			
			data_ready = 0;		
			Joystick_Send(recv);		 
	 }	 
   }	 
}



/*************鼠标画正方形代码******************************/
//   u8 X=0x05;
//   u8 Y=0x00;
//   u8 count=0;
//   LED_Init();

//   Set_System(); 
//      
//   USB_Interrupts_Config();
//     
//   Set_USBClock();
//   
//   USB_Init();
//   LED0_0;

//   while (1)
//   {	
//     if ((Joystick_data(X,Y))&&PrevXferComplete)
//     {
// 	  LED0_1;
// 	  //Joystick_data();	  
// 	  Joystick_Send(Mouse_Buffer);	  	  
// 	  LED0_0;
// 	  LED1_0;
// 	  LED2_0;
// 	if(count<=50)
// 	{
// 	   X=0x05;
// 	   Y=0x00;
// 	}
// 	if(count >50&& count<= 100)
// 	{	
// 	   X=0x00;
// 	   Y=0x05;		
// 	}
// 	if(count >100&& count<=150)
// 	{
// 	   X=0xfb;
// 	   Y=0x00;

// 	}
// 	if(count >150&& count<=200)
// 	{
// 	   X=0x00;
// 	   Y=0xfb;	   
// 	}
// 	if(count> 200)
// 	{
// 	  count=0;
// 	}
// 	count++; 
//     }		
//   }	
/*****************************************************/



// int main()
// {
// 	int i=0;
// 	JTAG_Set(SWD_ENABLE);//SWD_ENABLE
// 	usart_init();//串口1初始化
// 		//printf("\n\r串口成功打开\n\r");
// 	NRF24L01_Init();//NRF24L01初始化
// //  	while(NRF24L01_Check())			//NRF24L01的检测函数，用作测试。用的时候在24l01.h和24l01.c中把注释取消即可
// //  			printf("未检测到24L01");
// 	RX_Mode();
// 	while(1)
// 	{
// 	 	if(data_ready==1)
// 		{
// 			data_ready = 0;
// 			for(i=0;i<6;i++)
// 				printf("%c",recv[i]);
// 		}
// 	}
// }





