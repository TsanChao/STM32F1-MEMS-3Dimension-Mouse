#include "timer.h"


void TIM2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;


	NVIC_InitTypeDef NVIC_InitStructure;	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//设置优先级分组：先占优先级2位,从优先级2位
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	/* ---------------------------------------------------------------
	TIM2 
	--------------------------------------------------------------- */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_DeInit(TIM2);
	//TIM_InternalClockConfig(TIM2);

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 500 - 1; 
	TIM_TimeBaseStructure.TIM_Prescaler = 4800 - 1;  
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	TIM_ARRPreloadConfig(TIM2, DISABLE);//禁止ARR预装载缓冲器
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
 		
	/* TIM IT enable */
	TIM_ITConfig(  //清除溢出中断标志
		TIM2, //TIM2
		TIM_IT_Update,  //TIM 中断源
		ENABLE  //使能
		);
	
	/* TIM3 enable counter */
	TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设
}

