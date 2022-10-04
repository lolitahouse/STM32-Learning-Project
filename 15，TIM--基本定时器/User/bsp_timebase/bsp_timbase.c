#include "bsp_TiMbase.h" 


/* �ж����ȼ����� */
static void BASIC_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);								// �����ж���Ϊ0				
    NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQ ;					// �����ж���Դ		
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 		// ���������ȼ�Ϊ 0	  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;						// ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}



/* ��ʼ������ */
static void BASIC_TIM_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;		
		
    BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK, ENABLE);							// ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M			
    TIM_TimeBaseStructure.TIM_Period = BASIC_TIM_Period;				// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�	  
    TIM_TimeBaseStructure.TIM_Prescaler= BASIC_TIM_Prescaler;		// ʱ��Ԥ��Ƶ��Ϊ	  
    TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStructure);				// ��ʼ����ʱ��				
    TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);									// ����������жϱ�־λ	  		
    TIM_ITConfig(BASIC_TIM,TIM_IT_Update,ENABLE);								// �����������ж�				
    TIM_Cmd(BASIC_TIM, ENABLE);																	// ʹ�ܼ�����
}


void BASIC_TIM_Init(void)
{
	BASIC_TIM_NVIC_Config();
	BASIC_TIM_Mode_Config();
}


