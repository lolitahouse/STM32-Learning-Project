
#include "bsp_AdvanceTim.h" 

/* �ж����ȼ����� */
static void ADVANCE_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
   
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		 					// �����ж���Ϊ0		
    NVIC_InitStructure.NVIC_IRQChannel = ADVANCE_TIM_IRQ ;			// �����ж���Դ		
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 	// ���������ȼ�Ϊ 0	  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;					// ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


static void ADVANCE_TIM_Mode_Config(void)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;		
		
    ADVANCE_TIM_APBxClock_FUN(ADVANCE_TIM_CLK, ENABLE);					// ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M		
    TIM_TimeBaseStructure.TIM_Period=ADVANCE_TIM_Period;				// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�	  
    TIM_TimeBaseStructure.TIM_Prescaler= ADVANCE_TIM_Prescaler;	// ʱ��Ԥ��Ƶ��		
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;				// ʱ�ӷ�Ƶ���� ��û�õ����ù�		
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	// ����������ģʽ������Ϊ���ϼ���				
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;							// �ظ���������ֵ��û�õ����ù�	  
    TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);			// ��ʼ����ʱ��	
	
    TIM_ClearFlag(ADVANCE_TIM, TIM_FLAG_Update);								// ����������жϱ�־λ	  		
    TIM_ITConfig(ADVANCE_TIM,TIM_IT_Update,ENABLE);							// �����������ж�				
		TIM_Cmd(ADVANCE_TIM, ENABLE);																// ʹ�ܼ�����
}


void ADVANCE_TIM_Init(void)
{
	ADVANCE_TIM_NVIC_Config();
	ADVANCE_TIM_Mode_Config();		
}



