#include "bsp_adc.h"

uint16_t ADC_ConvertedValue;


/**
  * @brief  ADC GPIO ��ʼ��
  * @param  ��
  * @retval ��
  */
static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;		
	ADC_GPIO_APBxClock_FUN ( ADC_GPIO_CLK, ENABLE );				// �� ADC IO�˿�ʱ��		
	
	GPIO_InitStructure.GPIO_Pin = ADC_PIN;									// ���� ADC IO ����ģʽ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;						// ����Ϊģ������	
	GPIO_Init(ADC_PORT, &GPIO_InitStructure);								// ��ʼ�� ADC IO			
}


/**
  * @brief  ����ADC����ģʽ
  * @param  ��
  * @retval ��
  */
static void ADCx_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;		
	ADC_APBxClock_FUN ( ADC_CLK, ENABLE );																	// ��ADCʱ��	
	// ADC ģʽ����	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;											// ֻʹ��һ��ADC�����ڶ���ģʽ		
	ADC_InitStructure.ADC_ScanConvMode = DISABLE ; 													// ��ֹɨ��ģʽ����ͨ����Ҫ����ͨ������Ҫ	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;											// ����ת��ģʽ	
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;			// �����ⲿ����ת���������������	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;									// ת������Ҷ���	
	ADC_InitStructure.ADC_NbrOfChannel = 1;																	// ת��ͨ��1��			
	ADC_Init(ADCx, &ADC_InitStructure);																			// ��ʼ��ADC	
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 																			// ����ADCʱ��ΪPCLK2��8��Ƶ����9MHz		
	ADC_RegularChannelConfig(ADCx, ADC_CHANNEL, 1, ADC_SampleTime_55Cycles5);// ���� ADC ͨ��ת��˳��Ͳ���ʱ��		
	ADC_ITConfig(ADCx, ADC_IT_EOC, ENABLE);																	// ADC ת�����������жϣ����жϷ�������ж�ȡת��ֵ		
	ADC_Cmd(ADCx, ENABLE);																									// ����ADC ������ʼת��		 
	ADC_ResetCalibration(ADCx);																							// ��ʼ��ADC У׼�Ĵ��� 	
	while(ADC_GetResetCalibrationStatus(ADCx));															// �ȴ�У׼�Ĵ�����ʼ�����	
	
	ADC_StartCalibration(ADCx);																							// ADC��ʼУ׼	
	while(ADC_GetCalibrationStatus(ADCx));																	// �ȴ�У׼���		 
	ADC_SoftwareStartConvCmd(ADCx, ENABLE);																	// ����û�в����ⲿ����������ʹ���������ADCת��
}


static void ADC_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);										// ���ȼ�����
  
  NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQ;											// �����ж����ȼ�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


/**
  * @brief  ADC��ʼ��
  * @param  ��
  * @retval ��
  */
void ADCx_Init(void)
{
	ADCx_GPIO_Config();
	ADCx_Mode_Config();
	ADC_NVIC_Config();
}



