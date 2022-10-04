
#include "bsp_GeneralTim.h" 


static void GENERAL_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // ����Ƚ�ͨ��1 GPIO ��ʼ��
	RCC_APB2PeriphClockCmd(GENERAL_TIM_CH1_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GENERAL_TIM_CH1_PORT, &GPIO_InitStructure);
	
	// ����Ƚ�ͨ��2 GPIO ��ʼ��
	RCC_APB2PeriphClockCmd(GENERAL_TIM_CH2_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH2_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GENERAL_TIM_CH2_PORT, &GPIO_InitStructure);
	
	// ����Ƚ�ͨ��3 GPIO ��ʼ��
	RCC_APB2PeriphClockCmd(GENERAL_TIM_CH3_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH3_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GENERAL_TIM_CH3_PORT, &GPIO_InitStructure);
	
	// ����Ƚ�ͨ��4 GPIO ��ʼ��
	RCC_APB2PeriphClockCmd(GENERAL_TIM_CH4_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH4_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GENERAL_TIM_CH4_PORT, &GPIO_InitStructure);
}



/* ----------------   PWM�ź� ���ں�ռ�ձȵļ���--------------- */
// ARR ���Զ���װ�ؼĴ�����ֵ
// CLK_cnt����������ʱ�ӣ����� Fck_int / (psc+1) = 72M/(psc+1)
// PWM �źŵ����� T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// ռ�ձ�P=CCR/(ARR+1)

static void GENERAL_TIM_Mode_Config(void)
{ 
	GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK,ENABLE);// ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
/*--------------------ʱ���ṹ���ʼ��-------------------------*/	
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	
	TIM_TimeBaseStructure.TIM_Period=GENERAL_TIM_Period;				// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�	
	TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM_Prescaler;	// ����CNT��������ʱ�� = Fck_int/(psc+1)	
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;				// ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�	
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		// ����������ģʽ������Ϊ���ϼ���	
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;							// �ظ���������ֵ��û�õ����ù�	
	TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);			// ��ʼ����ʱ��
	/*--------------------����ȽϽṹ���ʼ��-------------------*/		
	uint16_t CCR1_Val = 5;																			// ռ�ձ�����
	uint16_t CCR2_Val = 4;
	uint16_t CCR3_Val = 3;
	uint16_t CCR4_Val = 2;
	
	TIM_OCInitTypeDef  TIM_OCInitStructure;	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;								// ����ΪPWMģʽ1	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;		// ���ʹ��		
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;				// ���ͨ����ƽ��������
	
	// ����Ƚ�ͨ�� 1
	TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
	TIM_OC1Init(GENERAL_TIM, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);
	
	// ����Ƚ�ͨ�� 2
	TIM_OCInitStructure.TIM_Pulse = CCR2_Val;
	TIM_OC2Init(GENERAL_TIM, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);
	
	// ����Ƚ�ͨ�� 3
	TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
	TIM_OC3Init(GENERAL_TIM, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);
	
	// ����Ƚ�ͨ�� 4
	TIM_OCInitStructure.TIM_Pulse = CCR4_Val;
	TIM_OC4Init(GENERAL_TIM, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);	
	
	TIM_Cmd(GENERAL_TIM, ENABLE);																		// ʹ�ܼ�����
}

void GENERAL_TIM_Init(void)
{
	GENERAL_TIM_GPIO_Config();
	GENERAL_TIM_Mode_Config();		
}

/*********************************************END OF FILE**********************/
