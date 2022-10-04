
#include "bsp_GeneralTim.h" 

// ��ʱ�����벶���û��Զ�������ṹ�嶨��
TIM_ICUserValueTypeDef TIM_ICUserValueStructure = {0,0,0,0};

/* �ж����ȼ����� */
static void GENERAL_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;     
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);							// �����ж���Ϊ0		
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM_IRQ ;			// �����ж���Դ		
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 	// ���������ȼ�Ϊ 0	  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;					// ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void GENERAL_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // ���벶��ͨ�� GPIO ��ʼ��
	RCC_APB2PeriphClockCmd(GENERAL_TIM_CH1_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GENERAL_TIM_CH1_PORT, &GPIO_InitStructure);	
}


/* ----------------   PWM�ź� ���ں�ռ�ձȵļ���--------------- */
// ARR ���Զ���װ�ؼĴ�����ֵ
// CLK_cnt����������ʱ�ӣ����� Fck_int / (psc+1) = 72M/(psc+1)
// PWM �źŵ����� T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// ռ�ձ�P=CCR/(ARR+1)

static void GENERAL_TIM_Mode_Config(void)
{ 
	GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK,ENABLE);				// ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
/*--------------------ʱ���ṹ���ʼ��-------------------------*/	
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	
	TIM_TimeBaseStructure.TIM_Period=GENERAL_TIM_PERIOD;			// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�	
	TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM_PSC;			// ����CNT��������ʱ�� = Fck_int/(psc+1)	
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;			// ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�	
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;	// ����������ģʽ������Ϊ���ϼ���		
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;						// �ظ���������ֵ��û�õ����ù�	
	TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);		// ��ʼ����ʱ��
	/*--------------------���벶��ṹ���ʼ��-------------------*/	
	TIM_ICInitTypeDef TIM_ICInitStructure;	
	TIM_ICInitStructure.TIM_Channel = GENERAL_TIM_CHANNEL_x;						// �������벶���ͨ������Ҫ���ݾ����GPIO������	
	TIM_ICInitStructure.TIM_ICPolarity = GENERAL_TIM_STRAT_ICPolarity;	// ���벶���źŵļ�������	
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;			// ����ͨ���Ͳ���ͨ����ӳ���ϵ����ֱ���ͷ�ֱ������	
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;								// �������Ҫ��������źŵķ�Ƶϵ��	
	TIM_ICInitStructure.TIM_ICFilter = 0;																// �������Ҫ��������źŵ��˲�ϵ��	
	TIM_ICInit(GENERAL_TIM, &TIM_ICInitStructure);											// ��ʱ�����벶���ʼ��	
	
  TIM_ClearFlag(GENERAL_TIM, TIM_FLAG_Update|GENERAL_TIM_IT_CCx);							// ������ºͲ����жϱ�־λ   
	TIM_ITConfig (GENERAL_TIM, TIM_IT_Update | GENERAL_TIM_IT_CCx, ENABLE ); 		// �������ºͲ����ж�		
	TIM_Cmd(GENERAL_TIM, ENABLE);																								// ʹ�ܼ�����
}

void GENERAL_TIM_Init(void)
{
	GENERAL_TIM_GPIO_Config();
	GENERAL_TIM_NVIC_Config();
	GENERAL_TIM_Mode_Config();		
}

/*********************************************END OF FILE**********************/
