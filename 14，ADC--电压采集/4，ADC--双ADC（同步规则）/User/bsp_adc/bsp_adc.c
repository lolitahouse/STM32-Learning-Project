#include "bsp_adc.h"

__IO uint32_t ADC_ConvertedValue[NOFCHANEL] = {0};


/**
  * @brief  ADC GPIO 初始化
  * @param  无
  * @retval 无
  */
static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;		
	
	ADCx_1_GPIO_APBxClock_FUN ( ADCx_1_GPIO_CLK, ENABLE );			// ADCx_1 GPIO 初始化
	GPIO_InitStructure.GPIO_Pin = ADCx_1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(ADCx_1_PORT, &GPIO_InitStructure);
	
	ADCx_1_GPIO_APBxClock_FUN ( ADCx_2_GPIO_CLK, ENABLE );			// ADCx_2 GPIO 初始化
	GPIO_InitStructure.GPIO_Pin = ADCx_2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(ADCx_2_PORT, &GPIO_InitStructure);	
}


/**
  * @brief  配置ADC工作模式
  * @param  无
  * @retval 无
  */
static void ADCx_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;	

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);													// 打开DMA时钟	
	ADCx_1_APBxClock_FUN ( ADCx_1_CLK, ENABLE );																// 打开ADC时钟
	ADCx_2_APBxClock_FUN ( ADCx_2_CLK, ENABLE );																			
	
	DMA_DeInit(ADC_DMA_CHANNEL);																								// 复位DMA控制器		
	DMA_InitStructure.DMA_PeripheralBaseAddr = ( uint32_t ) ( & ( ADCx_1->DR ) );	// 外设基址为：ADC 数据寄存器地址		
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValue;				// 存储器地址，实际上就是一个内部SRAM的变量		
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;													// 数据源来自外设		
	DMA_InitStructure.DMA_BufferSize = NOFCHANEL;																// 缓冲区的大小应该等于存储器的大小	
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;						// 外设寄存器只有一个，地址不用递增	
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 										// 存储器地址固定		
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;			// 外设数据大小为半字，即两个字节	
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;							// 存储器数据大小也为半字，跟外设数据大小相同	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;															// 循环传输模式	
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;													// DMA 传输通道优先级为高
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;																// 禁止存储器到存储器模式，因为是从外设到存储器		
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);															// 初始化DMA		
	DMA_Cmd(ADC_DMA_CHANNEL , ENABLE);																					// 使能 DMA 通道		
	// ADC1 模式配置	
	ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;																// 双ADC的规则同步		
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 																	// 多通道才要，单通道不需要	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;															// 连续转换模式	
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;							// 不用外部触发转换，软件开启即可	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;													// 转换结果右对齐	
	ADC_InitStructure.ADC_NbrOfChannel = NOFCHANEL;																	// 缓冲区的大小应该等于存储器的大小			
	ADC_Init(ADCx_1, &ADC_InitStructure);																						// 初始化ADC		
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 																							// 配置ADC时钟为PCLK2的8分频，即9MHz	
	ADC_RegularChannelConfig(ADCx_1, ADCx_1_CHANNEL, 1, ADC_SampleTime_239Cycles5);	// 配置 ADC 通道转换顺序和采样时间	
	ADC_DMACmd(ADCx_1, ENABLE);																											// 使能ADC DMA 请求		
	// ADC2 模式配置	
	ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;																// 双ADC的规则同步		
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 																	// 多通道才要，单通道不需要	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;															// 连续转换模式	
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;							// 不用外部触发转换，软件开启即可	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;													// 转换结果右对齐	
	ADC_InitStructure.ADC_NbrOfChannel = NOFCHANEL;																	// 缓冲区的大小应该等于存储器的大小			
	ADC_Init(ADCx_2, &ADC_InitStructure);																						// 初始化ADC		
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 																							// 配置ADC时钟为PCLK2的8分频，即9MHz	
	ADC_RegularChannelConfig(ADCx_2, ADCx_2_CHANNEL, 1, ADC_SampleTime_239Cycles5);	// 配置 ADC 通道转换顺序和采样时间	
	ADC_DMACmd(ADCx_2, ENABLE);																											// 使能ADC DMA 请求	
	// ADCx_1 校准		
	ADC_Cmd(ADCx_1, ENABLE);																									// 开启ADC ，并开始转换
	ADC_ResetCalibration(ADCx_1);																							// 初始化ADC 校准寄存器 	
	while(ADC_GetResetCalibrationStatus(ADCx_1));															// 等待校准寄存器初始化完成		
	ADC_StartCalibration(ADCx_1);																							// ADC开始校准	
	while(ADC_GetCalibrationStatus(ADCx_1));																		// 等待校准完成		 
	// ADCx_1 校准		
	ADC_Cmd(ADCx_2, ENABLE);																									// 开启ADC ，并开始转换
	ADC_ResetCalibration(ADCx_2);																							// 初始化ADC 校准寄存器 	
	while(ADC_GetResetCalibrationStatus(ADCx_2));															// 等待校准寄存器初始化完成		
	ADC_StartCalibration(ADCx_2);																							// ADC开始校准	
	while(ADC_GetCalibrationStatus(ADCx_2));																		// 等待校准完成
	
	ADC_SoftwareStartConvCmd(ADCx_1, ENABLE);																	// 由于没有采用外部触发，所以使用软件触发ADC转换 
}



/**
  * @brief  ADC初始化
  * @param  无
  * @retval 无
  */
void ADCx_Init(void)
{
	ADCx_GPIO_Config();
	ADCx_Mode_Config();
}



