
#include "bsp_AdvanceTim.h" 


static void ADVANCE_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
	RCC_APB2PeriphClockCmd(ADVANCE_TIM_CH1_GPIO_CLK, ENABLE);				// ����Ƚ�ͨ�� GPIO ��ʼ��
  GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM_CH1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(ADVANCE_TIM_CH1_PORT, &GPIO_InitStructure);
  
	RCC_APB2PeriphClockCmd(ADVANCE_TIM_CH1N_GPIO_CLK, ENABLE);			// ����Ƚ�ͨ������ͨ�� GPIO ��ʼ��
  GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM_CH1N_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(ADVANCE_TIM_CH1N_PORT, &GPIO_InitStructure);
  
	RCC_APB2PeriphClockCmd(ADVANCE_TIM_BKIN_GPIO_CLK, ENABLE);			// ����Ƚ�ͨ��ɲ��ͨ�� GPIO ��ʼ��
  GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM_BKIN_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(ADVANCE_TIM_BKIN_PORT, &GPIO_InitStructure);
	
	GPIO_ResetBits(ADVANCE_TIM_BKIN_PORT,ADVANCE_TIM_BKIN_PIN);			// BKIN����Ĭ��������͵�ƽ
}


/* ----------------   PWM�ź� ���ں�ռ�ձȵļ���--------------- */
// ARR ���Զ���װ�ؼĴ�����ֵ
// CLK_cnt����������ʱ�ӣ����� Fck_int / (psc+1) = 72M/(psc+1)
// PWM �źŵ����� T = (ARR+1) * (1/CLK_cnt) = (ARR+1)*(PSC+1) / 72M
// ռ�ձ�P=CCR/(ARR+1)
static void ADVANCE_TIM_Mode_Config(void)
{ 
	ADVANCE_TIM_APBxClock_FUN(ADVANCE_TIM_CLK,ENABLE);						// ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
/*--------------------ʱ���ṹ���ʼ��-------------------------*/
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	
	TIM_TimeBaseStructure.TIM_Period = ADVANCE_TIM_PERIOD;					// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�	
	TIM_TimeBaseStructure.TIM_Prescaler = ADVANCE_TIM_PSC;					// ����CNT��������ʱ�� = Fck_int/(psc+1)	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;					// ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;			// ����������ģʽ������Ϊ���ϼ���	
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;								// �ظ���������ֵ��û�õ����ù�	
	TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);					// ��ʼ����ʱ��
	/*--------------------����ȽϽṹ���ʼ��-------------------*/		
	TIM_OCInitTypeDef  TIM_OCInitStructure;	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;								// ����ΪPWMģʽ1	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;		// ���ʹ��	
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; // �������ʹ��	
	TIM_OCInitStructure.TIM_Pulse = ADVANCE_TIM_PULSE;							// ����ռ�ձȴ�С	
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;				// ���ͨ����ƽ��������	
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;			// �������ͨ����ƽ��������	
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;			// ���ͨ�����е�ƽ��������	
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;	// �������ͨ�����е�ƽ��������
	TIM_OC1Init(ADVANCE_TIM, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(ADVANCE_TIM, TIM_OCPreload_Enable);
	/*-------------------ɲ���������ṹ���ʼ��-------------------*/	
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;											// �й�ɲ���������ṹ��ĳ�Ա����ɲο�BDTR�Ĵ���������
  TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
  TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
  TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;
  TIM_BDTRInitStructure.TIM_DeadTime = 11;												// ����Ƚ��ź�����ʱ�����ã��˴�����Ϊ152ns
  TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;	
  TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;// ��BKIN���ż�⵽�ߵ�ƽ��ʱ������Ƚ��źű���ֹ���ͺ�����ɲ��һ��
  TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
  TIM_BDTRConfig(ADVANCE_TIM, &TIM_BDTRInitStructure);	
	
	TIM_Cmd(ADVANCE_TIM, ENABLE);																		// ʹ�ܼ�����	
	TIM_CtrlPWMOutputs(ADVANCE_TIM, ENABLE);												// �����ʹ�ܣ���ʹ�õ���ͨ�ö�ʱ��ʱ����䲻��Ҫ
}


void ADVANCE_TIM_Init(void)
{
	ADVANCE_TIM_GPIO_Config();
	ADVANCE_TIM_Mode_Config();		
}



