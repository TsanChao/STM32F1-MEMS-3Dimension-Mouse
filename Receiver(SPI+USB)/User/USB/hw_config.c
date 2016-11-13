/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : hw_config.c
* Author             : MCD Application Team
* Version            : V1.0
* Date               : 10/08/2007
* Description        : Hardware Configuration & Setup
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "hw_config.h"
#include "usb_lib.h"
#include "stm32f10x_flash.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define JOY_UP       GPIO_Pin_0   /* PA.00 */
#define JOY_DOWN     GPIO_Pin_1   /* PA.01 */

#define JOY_LEFT     GPIO_Pin_1  /* PA.01 */
#define JOY_RIGHT    GPIO_Pin_0  /* PA.00 */

#define JOY_LEFT_BUTTON    GPIO_Pin_0  /* PC.13 */
#define JOY_RIGHT_BUTTON    GPIO_Pin_1  /* PC.12 */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ErrorStatus HSEStartUpStatus;

/* Extern variables ----------------------------------------------------------*/
extern __IO uint8_t PrevXferComplete;

extern u8 PreButton;      


extern u8 Mouse_Buffer[4];

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power.
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_System(void)
{  

//  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/   
//  /* RCC system reset(for debug purpose) */
//  RCC_DeInit();
//
//  /* Enable HSE */
//  RCC_HSEConfig(RCC_HSE_ON);
//
//  /* Wait till HSE is ready */
//  HSEStartUpStatus = RCC_WaitForHSEStartUp();
//
//  if(HSEStartUpStatus == SUCCESS)
//  {
//    /* Enable Prefetch Buffer */
//    //FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
//
//    /* Flash 2 wait state */
//    //FLASH_SetLatency(FLASH_Latency_2);
// 
//    /* HCLK = SYSCLK */
//    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
//  
//    /* PCLK2 = HCLK */
//    RCC_PCLK2Config(RCC_HCLK_Div1); 
//
//    /* PCLK1 = HCLK/2 */
//    RCC_PCLK1Config(RCC_HCLK_Div2);
//
//    /* ADCCLK = PCLK2/6 */
//    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
//
//    /* PLLCLK = 8MHz * 9 = 72 MHz */
//    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
//
//    /* Enable PLL */ 
//    RCC_PLLCmd(ENABLE);
//
//    /* Wait till PLL is ready */
//    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
//    {
//    }
//
//    /* Select PLL as system clock source */
//    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
//
//    /* Wait till PLL is used as system clock source */
//    while(RCC_GetSYSCLKSource() != 0x08)
//    {
//    }
//  }
  
  /* Enable GPIOD and GPIOE clock */

  GPIO_InitTypeDef GPIO_InitStructure;


  // RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE| RCC_APB2Periph_GPIOC, ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD,ENABLE);
  /* PD.02 used as USB pull-up */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                
  GPIO_Init(GPIOD, &GPIO_InitStructure);

//   /* Configure the JoyStick IOs */
//   /* Key up + Key down  */
//   GPIO_InitStructure.GPIO_Pin = JOY_UP | JOY_DOWN ;
//   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//   GPIO_Init(GPIOE, &GPIO_InitStructure);            
//   
//   /*GPIO_Init(GPIOD, &GPIO_InitStructure);*/
//   /* Key left + Key right */
//   GPIO_InitStructure.GPIO_Pin = JOY_LEFT | JOY_RIGHT;
//   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//   GPIO_Init(GPIOA, &GPIO_InitStructure);

//     /* Right_button + Left_button */
//   GPIO_InitStructure.GPIO_Pin = JOY_LEFT_BUTTON|JOY_RIGHT_BUTTON;
//   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//   GPIO_Init(GPIOC, &GPIO_InitStructure);
  
}

/*******************************************************************************
* Function Name  : Set_USBClock
* Description    : Configures USB Clock input (48MHz).
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Set_USBClock(void)
{
  /* Select USBCLK source */
  RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);

  /* Enable USB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}

/*******************************************************************************
* Function Name  : Enter_LowPowerMode.
* Description    : Power-off system clocks and power while entering suspend mode.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Enter_LowPowerMode(void)
{
}

/*******************************************************************************
* Function Name  : Leave_LowPowerMode.
* Description    : Restores system clocks and power while exiting suspend mode.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Leave_LowPowerMode(void)
{
}

/*******************************************************************************
* Function Name  : USB_Interrupts_Config.
* Description    : Configures the USB interrupts.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_Interrupts_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

#ifdef  VECT_TAB_RAM  
  /* Set the Vector Table base location at 0x20000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH */
  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif 

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : USB_Cable_Config.
* Description    : Software Connection/Disconnection of USB Cable.
* Input          : NewState: new state.
* Output         : None.
* Return         : None
*******************************************************************************/


void USB_Cable_Config (FunctionalState NewState)
{ 
  if (NewState != DISABLE)
  {
    
    GPIO_SetBits(GPIOD, GPIO_Pin_2);
  }
  else
  {
    GPIO_ResetBits(GPIOD, GPIO_Pin_2);
  }
}




/*******************************************************************************
* Function Name : JoyState.
* Description   : Decodes the Joystick direction.
* Input         : None.
* Output        : None.
* Return value  : The direction value.
*******************************************************************************/
u8 JoyState(void)
{   
  if (!GPIO_ReadInputDataBit(GPIOA, JOY_LEFT_BUTTON))
  {
    return LEFT_BUTTON;
  }
   if (!GPIO_ReadInputDataBit(GPIOA, JOY_RIGHT_BUTTON))
  {
    return RIGHT_BUTTON;
  }
  /* No key is pressed */
  else
  {
    return 0;
  }
}

/*******************************************************************************
* Function Name : Joystick_Send.
* Description   : prepares buffer to be sent containing Joystick event infos.
* Input         : Keys: keys received from terminal.
* Output        : None.
* Return value  : None.
*******************************************************************************/
void Joystick_Send(u8* recv)
{ 
 	u8 Mouse_Buffertmp[4]={0,0,0,0};
  Mouse_Buffer[0]=recv[2];       //recv数组为无线接收到的数据，以0x80 0x0a 开头 所以从recv[2] 鼠标按键状态
  Mouse_Buffer[1]=recv[3];       // 鼠标X位移
  Mouse_Buffer[2]=recv[4];       // 鼠标Y位移
  Mouse_Buffer[3]=recv[5];       // 滑轮滑动速度
   	
 	if( (PreButton != Mouse_Buffer[0] ) || Mouse_Buffer[1]!=0x00 || Mouse_Buffer[2]!=0x00 || Mouse_Buffer[3] != 0x00 )
	{
		 PrevXferComplete = 0;
		/*copy mouse position info in ENDP1 Tx Packet Memory Area*/
		UserToPMABufferCopy(Mouse_Buffer, GetEPTxAddr(ENDP1), 4);
		SetEPTxValid(ENDP1);
		
		if( PreButton != 0x00 && Mouse_Buffer[0] == 0x00 )
		{
			 while( PrevXferComplete != 1);
			 PrevXferComplete = 0;
			 UserToPMABufferCopy(Mouse_Buffertmp, GetEPTxAddr(ENDP1), 4);
     	 SetEPTxValid(ENDP1);
			 PreButton = 0x00;
		}
		else
		{
			PreButton = Mouse_Buffer[0];
		}
		
	}
  	

}


// u8 Joystick_data(u8* recv)
// {  
//   
//   //s8 BUTTON=0;
//   u8 flag=0;
//   Mouse_Buffer[0]=recv[2];
//   Mouse_Buffer[1]=recv[3];
//   Mouse_Buffer[2]=recv[4]; 
//   Mouse_Buffer[3]=recv[5];  
// //   if (JoyState()==LEFT_BUTTON)
// //   {
// //     BUTTON = BUTTON|0x01;
// // 	flag=1;
// //   }
// //    if(JoyState()==RIGHT_BUTTON)
// //   {
// //     BUTTON = BUTTON|0x02;
// // 	flag=1;
// //   }
// //   /* prepare buffer to send */
// //   //Mouse_Buffer[0] = BUTTON;

//   return flag;
//     
//   //Mouse_Buffer[1] = X;
//  // Mouse_Buffer[2] = Y; 
//  	
// }

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
