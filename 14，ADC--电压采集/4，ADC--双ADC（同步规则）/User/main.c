/**
  ******************************************************************************
  * @file    main.c
  * @author  lolita
  * @brief   ADC 双通道采集实验，使用DMA
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_adc.h"

// ADC1转换的电压值通过DMA方式传到SRAM
extern __IO uint16_t ADC_ConvertedValue[NOFCHANEL];

// 局部变量，用于保存转换计算后的电压值 	 
float ADC_ConvertedValueLocal[NOFCHANEL*2]; 


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
	uint16_t temp0 = 0 ,temp1 = 0;
	
	USART_Config();																								// 配置串口		
	ADCx_Init();																									// ADC 初始化	
	printf("\r\n ----这是一个双ADC规则同步采集实验----\r\n");
	
	while (1)
	{	    
		temp0 = (ADC_ConvertedValue[0]&0XFFFF0000) >> 16;						// 取出ADC1数据寄存器的高16位，这个是ADC2的转换数据		
		temp1 = (ADC_ConvertedValue[0]&0XFFFF);											// 取出ADC1数据寄存器的低16位，这个是ADC1的转换数据
		
		ADC_ConvertedValueLocal[0] =(float) temp0/4096*3.3;
		ADC_ConvertedValueLocal[1] =(float) temp1/4096*3.3;
		
		printf("\r\n ADCx_1 value = %f V \r\n",ADC_ConvertedValueLocal[1]);
		printf("\r\n ADCx_2 value = %f V \r\n",ADC_ConvertedValueLocal[0]);
		
		printf("\r\n\r\n");
		Delay(0xffffee); 
		 
	}  
}



