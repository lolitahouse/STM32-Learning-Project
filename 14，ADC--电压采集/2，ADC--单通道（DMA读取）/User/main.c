/**
  ******************************************************************************
  * @file    main.c
  * @author  lolita
  * @brief   ADC ��ͨ���ɼ���ʹ��DMA
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_adc.h"

// ADC1ת���ĵ�ѹֵͨ��DMA��ʽ����SRAM
extern __IO uint16_t ADC_ConvertedValue;

// �ֲ����������ڱ���ת�������ĵ�ѹֵ 	 
float ADC_ConvertedValueLocal; 


void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}



/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{ 			
	USART_Config();																							// ���ô���				
	ADCx_Init();																								// ADC ��ʼ��		
	printf("\r\n ----����һ��ADC��ͨ��DMA��ȡʵ��----\r\n");		
	while (1)
	{
		ADC_ConvertedValueLocal =(float) ADC_ConvertedValue/4096*3.3; 			// ��ȡת����ADֵ	
		printf("\r\n The current AD value = 0x%04X \r\n", ADC_ConvertedValue); 
		printf("\r\n The current AD value = %f V \r\n",ADC_ConvertedValueLocal); 
		Delay(0xffffee);  
	}  
}



