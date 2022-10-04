
#include "bsp_AdvanceTim.h" 


 /**
  * @brief  �߼����ƶ�ʱ�� TIMx,x[1,8]�ж����ȼ�����
  * @param  ��
  * @retval ��
  */
static void ADVANCE_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);							// �����ж���Ϊ0		
    NVIC_InitStructure.NVIC_IRQChannel = ADVANCE_TIM_IRQ; 			// �����ж���Դ		
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 	// ������ռ���ȼ�	  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;					// ���������ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**
  * @brief  �߼���ʱ��PWM�����õ���GPIO��ʼ��
  * @param  ��
  * @retval ��
  */
static void ADVANCE_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(ADVANCE_TIM_CH1_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM_CH1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_Init(ADVANCE_TIM_CH1_PORT, &GPIO_InitStructure);	
}


/* ----------------   PWM�ź� ���ں�ռ�ձȵļ���--------------- */
// ARR ���Զ���װ�ؼĴ�����ֵ
// CLK_cnt����������ʱ�ӣ����� Fck_int / (psc+1) = 72M/(psc+1)
// PWM �źŵ����� T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// ռ�ձ�P=CCR/(ARR+1)

/**
  * @brief  �߼���ʱ��PWM�����ʼ�����õ���GPIO��ʼ��
  * @param  ��
  * @retval ��
  */
static void ADVANCE_TIM_Mode_Config(void)
{ 
	ADVANCE_TIM_APBxClock_FUN(ADVANCE_TIM_CLK,ENABLE);				// ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
/*--------------------ʱ���ṹ���ʼ��-------------------------*/
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	
	TIM_TimeBaseStructure.TIM_Period = ADVANCE_TIM_PERIOD;			// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�	
	TIM_TimeBaseStructure.TIM_Prescaler = ADVANCE_TIM_PSC;			// ����CNT��������ʱ�� = Fck_int/(psc+1)	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;			// ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	// ����������ģʽ������Ϊ���ϼ���		
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;						// �ظ���������ֵ��û�õ����ù�	
	TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);			// ��ʼ����ʱ��
	/*--------------------���벶��ṹ���ʼ��-------------------*/	  	
	TIM_ICInitTypeDef  TIM_ICInitStructure;									// ʹ��PWM����ģʽʱ����Ҫռ����������Ĵ�����һ�������ڣ�����һ����ռ�ձ�
	// ����ͨ��IC1����	
  TIM_ICInitStructure.TIM_Channel = ADVANCE_TIM_IC1PWM_CHANNEL;		// ѡ�񲶻�ͨ��	
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;			// ���ò���ı���	
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;	// ���ò���ͨ�����ź��������ĸ�����ͨ������ֱ���ͷ�ֱ������	
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;						// 1��Ƶ���������źŵ�ÿ����Ч���ض�����	
  TIM_ICInitStructure.TIM_ICFilter = 0x0;													// ���˲�	
  TIM_PWMIConfig(ADVANCE_TIM, &TIM_ICInitStructure);							// ��ʼ��PWM����ģʽ
	
	// ��������PWM����ģʽʱ,ֻ��Ҫ���ô����źŵ���һ·���ɣ����ڲ������ڣ�
	// ����һ·�����ڲ���ռ�ձȣ�����Ӳ���Դ����ã�����Ҫ������

	// ����ͨ��IC2����	
//	TIM_ICInitStructure.TIM_Channel = ADVANCE_TIM_IC1PWM_CHANNEL;
//  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
//  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI;
//  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
//  TIM_ICInitStructure.TIM_ICFilter = 0x0;
//  TIM_PWMIConfig(ADVANCE_TIM, &TIM_ICInitStructure);	
	
  TIM_SelectInputTrigger(ADVANCE_TIM, TIM_TS_TI1FP1);							// ѡ�����벶��Ĵ����ź�

	// ѡ���ģʽ: ��λģʽ
	// PWM����ģʽʱ,��ģʽ���빤���ڸ�λģʽ��������ʼʱ,������CNT�ᱻ��λ
  TIM_SelectSlaveMode(ADVANCE_TIM, TIM_SlaveMode_Reset);
  TIM_SelectMasterSlaveMode(ADVANCE_TIM,TIM_MasterSlaveMode_Enable); 
	
  TIM_ITConfig(ADVANCE_TIM, TIM_IT_CC1, ENABLE);									// ʹ�ܲ����ж�,����ж���Ե���������ͨ�������������Ǹ���	
	TIM_ClearITPendingBit(ADVANCE_TIM, TIM_IT_CC1);									// ����жϱ�־λ		 
  TIM_Cmd(ADVANCE_TIM, ENABLE);																		// ʹ�ܸ߼����ƶ�ʱ������������ʼ����
}
/**
  * @brief  �߼���ʱ��PWM�����ʼ�����õ���GPIO��ʼ��
  * @param  ��
  * @retval ��
  */
void ADVANCE_TIM_Init(void)
{
	ADVANCE_TIM_GPIO_Config();
	ADVANCE_TIM_NVIC_Config();
	ADVANCE_TIM_Mode_Config();		
}




