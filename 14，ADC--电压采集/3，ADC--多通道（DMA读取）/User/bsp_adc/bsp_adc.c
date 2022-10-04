#include "bsp_adc.h"

__IO uint16_t ADC_ConvertedValue[NOFCHANEL]={0,0,0,0};


/**
  * @brief  ADC GPIO ��ʼ��
  * @param  ��
  * @retval ��
  */
static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;		
	ADC_GPIO_APBxClock_FUN ( ADC_GPIO_CLK, ENABLE );																			// �� ADC IO�˿�ʱ��		
	
	GPIO_InitStructure.GPIO_Pin = ADC_PIN1|ADC_PIN2|ADC_PIN3|ADC_PIN4|ADC_PIN5|ADC_PIN6;	// ���� ADC IO ����ģʽ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;																					// ����Ϊģ������	
	GPIO_Init(ADC_PORT, &GPIO_InitStructure);																							// ��ʼ�� ADC IO			
}


/**
  * @brief  ����ADC����ģʽ
  * @param  ��
  * @retval ��
  */
static void ADCx_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;	

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);													// ��DMAʱ��	
	ADC_APBxClock_FUN ( ADC_CLK, ENABLE );																			// ��ADCʱ��
	
	DMA_DeInit(ADC_DMA_CHANNEL);																								// ��λDMA������		
	DMA_InitStructure.DMA_PeripheralBaseAddr = ( uint32_t ) ( & ( ADC_x->DR ) );	// �����ַΪ��ADC ���ݼĴ�����ַ		
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValue;				// �洢����ַ��ʵ���Ͼ���һ���ڲ�SRAM�ı���		
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;													// ����Դ��������		
	DMA_InitStructure.DMA_BufferSize = NOFCHANEL;																// �������Ĵ�СӦ�õ��ڴ洢���Ĵ�С	
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;						// ����Ĵ���ֻ��һ������ַ���õ���	
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 										// �洢����ַ�̶�		
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	// �������ݴ�СΪ���֣��������ֽ�	
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;					// �洢�����ݴ�СҲΪ���֣����������ݴ�С��ͬ	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;															// ѭ������ģʽ	
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;													// DMA ����ͨ�����ȼ�Ϊ��
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;																// ��ֹ�洢�����洢��ģʽ����Ϊ�Ǵ����赽�洢��		
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);															// ��ʼ��DMA		
	DMA_Cmd(ADC_DMA_CHANNEL , ENABLE);																					// ʹ�� DMA ͨ��		
	// ADC ģʽ����	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;											// ֻʹ��һ��ADC�����ڶ���ģʽ		
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 													// ��ͨ����Ҫ����ͨ������Ҫ	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;											// ����ת��ģʽ	
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;			// �����ⲿ����ת���������������	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;									// ת������Ҷ���	
	ADC_InitStructure.ADC_NbrOfChannel = NOFCHANEL;													// �������Ĵ�СӦ�õ��ڴ洢���Ĵ�С			
	ADC_Init(ADC_x, &ADC_InitStructure);																			// ��ʼ��ADC		
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 																			// ����ADCʱ��ΪPCLK2��8��Ƶ����9MHz	

	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL1, 1, ADC_SampleTime_55Cycles5);	// ���� ADC ͨ��ת��˳��Ͳ���ʱ��	
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL2, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL3, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL4, 4, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL5, 5, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL6, 6, ADC_SampleTime_55Cycles5);
	
	ADC_DMACmd(ADC_x, ENABLE);																								// ʹ��ADC DMA ����	
	ADC_Cmd(ADC_x, ENABLE);																										// ����ADC ������ʼת��		 
	ADC_ResetCalibration(ADC_x);																							// ��ʼ��ADC У׼�Ĵ��� 	
	while(ADC_GetResetCalibrationStatus(ADC_x));															// �ȴ�У׼�Ĵ�����ʼ�����	
	
	ADC_StartCalibration(ADC_x);																							// ADC��ʼУ׼	
	while(ADC_GetCalibrationStatus(ADC_x));																		// �ȴ�У׼���		 
	ADC_SoftwareStartConvCmd(ADC_x, ENABLE);																	// ����û�в����ⲿ����������ʹ���������ADCת�� 
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
}



