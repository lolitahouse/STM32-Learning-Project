/**
  ******************************************************************************
  * @file    main.c
  * @author  lolita
  * @brief   ADC 单通道采集，使用DMA
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_adc.h"

// ADC1转换的电压值通过DMA方式传到SRAM
extern __IO uint16_t ADC_ConvertedValue;

// 局部变量，用于保存转换计算后的电压值 	 
float ADC_ConvertedValueLocal; 


void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}



/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{ 			
	USART_Config();																							// 配置串口				
	ADCx_Init();																								// ADC 初始化		
	printf("\r\n ----这是一个ADC单通道DMA读取实验----\r\n");		
	while (1)
	{
		ADC_ConvertedValueLocal =(float) ADC_ConvertedValue/4096*3.3; 			// 读取转换的AD值	
		printf("\r\n The current AD value = 0x%04X \r\n", ADC_ConvertedValue); 
		printf("\r\n The current AD value = %f V \r\n",ADC_ConvertedValueLocal); 
		Delay(0xffffee);  
	}  
}



