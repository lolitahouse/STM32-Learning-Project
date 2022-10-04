#include "bsp_adc.h"

__IO uint32_t ADC_ConvertedValue[NOFCHANEL] = {0};


/**
  * @brief  ADC GPIO ��ʼ��
  * @param  ��
  * @retval ��
  */
static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;		
	
	ADCx_1_GPIO_APBxClock_FUN ( ADCx_1_GPIO_CLK, ENABLE );			// ADCx_1 GPIO ��ʼ��
	GPIO_InitStructure.GPIO_Pin = ADCx_1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(ADCx_1_PORT, &GPIO_InitStructure);
	
	ADCx_1_GPIO_APBxClock_FUN ( ADCx_2_GPIO_CLK, ENABLE );			// ADCx_2 GPIO ��ʼ��
	GPIO_InitStructure.GPIO_Pin = ADCx_2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(ADCx_2_PORT, &GPIO_InitStructure);	
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
	ADCx_1_APBxClock_FUN ( ADCx_1_CLK, ENABLE );																// ��ADCʱ��
	ADCx_2_APBxClock_FUN ( ADCx_2_CLK, ENABLE );																			
	
	DMA_DeInit(ADC_DMA_CHANNEL);																								// ��λDMA������		
	DMA_InitStructure.DMA_PeripheralBaseAddr = ( uint32_t ) ( & ( ADCx_1->DR ) );	// �����ַΪ��ADC ���ݼĴ�����ַ		
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValue;				// �洢����ַ��ʵ���Ͼ���һ���ڲ�SRAM�ı���		
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;													// ����Դ��������		
	DMA_InitStructure.DMA_BufferSize = NOFCHANEL;																// �������Ĵ�СӦ�õ��ڴ洢���Ĵ�С	
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;						// ����Ĵ���ֻ��һ������ַ���õ���	
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 										// �洢����ַ�̶�		
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;			// �������ݴ�СΪ���֣��������ֽ�	
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;							// �洢�����ݴ�СҲΪ���֣����������ݴ�С��ͬ	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;															// ѭ������ģʽ	
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;													// DMA ����ͨ�����ȼ�Ϊ��
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;																// ��ֹ�洢�����洢��ģʽ����Ϊ�Ǵ����赽�洢��		
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);															// ��ʼ��DMA		
	DMA_Cmd(ADC_DMA_CHANNEL , ENABLE);																					// ʹ�� DMA ͨ��		
	// ADC1 ģʽ����	
	ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;																// ˫ADC�Ĺ���ͬ��		
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 																	// ��ͨ����Ҫ����ͨ������Ҫ	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;															// ����ת��ģʽ	
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;							// �����ⲿ����ת���������������	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;													// ת������Ҷ���	
	ADC_InitStructure.ADC_NbrOfChannel = NOFCHANEL;																	// �������Ĵ�СӦ�õ��ڴ洢���Ĵ�С			
	ADC_Init(ADCx_1, &ADC_InitStructure);																						// ��ʼ��ADC		
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 																							// ����ADCʱ��ΪPCLK2��8��Ƶ����9MHz	
	ADC_RegularChannelConfig(ADCx_1, ADCx_1_CHANNEL, 1, ADC_SampleTime_239Cycles5);	// ���� ADC ͨ��ת��˳��Ͳ���ʱ��	
	ADC_DMACmd(ADCx_1, ENABLE);																											// ʹ��ADC DMA ����		
	// ADC2 ģʽ����	
	ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;																// ˫ADC�Ĺ���ͬ��		
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 																	// ��ͨ����Ҫ����ͨ������Ҫ	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;															// ����ת��ģʽ	
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;							// �����ⲿ����ת���������������	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;													// ת������Ҷ���	
	ADC_InitStructure.ADC_NbrOfChannel = NOFCHANEL;																	// �������Ĵ�СӦ�õ��ڴ洢���Ĵ�С			
	ADC_Init(ADCx_2, &ADC_InitStructure);																						// ��ʼ��ADC		
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 																							// ����ADCʱ��ΪPCLK2��8��Ƶ����9MHz	
	ADC_RegularChannelConfig(ADCx_2, ADCx_2_CHANNEL, 1, ADC_SampleTime_239Cycles5);	// ���� ADC ͨ��ת��˳��Ͳ���ʱ��	
	ADC_DMACmd(ADCx_2, ENABLE);																											// ʹ��ADC DMA ����	
	// ADCx_1 У׼		
	ADC_Cmd(ADCx_1, ENABLE);																									// ����ADC ������ʼת��
	ADC_ResetCalibration(ADCx_1);																							// ��ʼ��ADC У׼�Ĵ��� 	
	while(ADC_GetResetCalibrationStatus(ADCx_1));															// �ȴ�У׼�Ĵ�����ʼ�����		
	ADC_StartCalibration(ADCx_1);																							// ADC��ʼУ׼	
	while(ADC_GetCalibrationStatus(ADCx_1));																		// �ȴ�У׼���		 
	// ADCx_1 У׼		
	ADC_Cmd(ADCx_2, ENABLE);																									// ����ADC ������ʼת��
	ADC_ResetCalibration(ADCx_2);																							// ��ʼ��ADC У׼�Ĵ��� 	
	while(ADC_GetResetCalibrationStatus(ADCx_2));															// �ȴ�У׼�Ĵ�����ʼ�����		
	ADC_StartCalibration(ADCx_2);																							// ADC��ʼУ׼	
	while(ADC_GetCalibrationStatus(ADCx_2));																		// �ȴ�У׼���
	
	ADC_SoftwareStartConvCmd(ADCx_1, ENABLE);																	// ����û�в����ⲿ����������ʹ���������ADCת�� 
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



