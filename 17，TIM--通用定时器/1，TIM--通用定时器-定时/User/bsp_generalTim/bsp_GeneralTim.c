
#include "bsp_GeneralTim.h" 

/* �ж����ȼ����� */
static void GENERAL_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;     
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);						// �����ж���Ϊ0		
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM_IRQ ;		// �����ж���Դ		
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	// ���������ȼ�Ϊ 0  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;				// ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


static void GENERAL_TIM_Mode_Config(void)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;				
    GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK, ENABLE);						// ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
		
    TIM_TimeBaseStructure.TIM_Period=GENERAL_TIM_Period;					// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�	  
    TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM_Prescaler;		// ʱ��Ԥ��Ƶ��		
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;					// ʱ�ӷ�Ƶ���� ��û�õ����ù�		
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 		// ����������ģʽ������Ϊ���ϼ���			
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;								// �ظ���������ֵ��û�õ����ù�	 
    TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure); 				// ��ʼ����ʱ��	
		
    TIM_ClearFlag(GENERAL_TIM, TIM_FLAG_Update);									// ����������жϱ�־λ	  		
    TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,ENABLE);								// �����������ж�				
    TIM_Cmd(GENERAL_TIM, ENABLE);																	// ʹ�ܼ�����
}

void GENERAL_TIM_Init(void)
{
	GENERAL_TIM_NVIC_Config();
	GENERAL_TIM_Mode_Config();		
}

/*********************************************END OF FILE**********************/
