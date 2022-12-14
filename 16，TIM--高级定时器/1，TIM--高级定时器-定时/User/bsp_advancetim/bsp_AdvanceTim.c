
#include "bsp_AdvanceTim.h" 

/* 中断优先级配置 */
static void ADVANCE_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
   
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		 					// 设置中断组为0		
    NVIC_InitStructure.NVIC_IRQChannel = ADVANCE_TIM_IRQ ;			// 设置中断来源		
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 	// 设置主优先级为 0	  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;					// 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


static void ADVANCE_TIM_Mode_Config(void)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;		
		
    ADVANCE_TIM_APBxClock_FUN(ADVANCE_TIM_CLK, ENABLE);					// 开启定时器时钟,即内部时钟CK_INT=72M		
    TIM_TimeBaseStructure.TIM_Period=ADVANCE_TIM_Period;				// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断	  
    TIM_TimeBaseStructure.TIM_Prescaler= ADVANCE_TIM_Prescaler;	// 时钟预分频数		
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;				// 时钟分频因子 ，没用到不用管		
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	// 计数器计数模式，设置为向上计数				
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;							// 重复计数器的值，没用到不用管	  
    TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);			// 初始化定时器	
	
    TIM_ClearFlag(ADVANCE_TIM, TIM_FLAG_Update);								// 清除计数器中断标志位	  		
    TIM_ITConfig(ADVANCE_TIM,TIM_IT_Update,ENABLE);							// 开启计数器中断				
		TIM_Cmd(ADVANCE_TIM, ENABLE);																// 使能计数器
}


void ADVANCE_TIM_Init(void)
{
	ADVANCE_TIM_NVIC_Config();
	ADVANCE_TIM_Mode_Config();		
}



