/**
  ******************************************************************************
  * @file    main.c
  * @author  lolita
  * @brief   ADC ˫ͨ���ɼ�ʵ�飬ʹ��DMA
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_adc.h"

// ADC1ת���ĵ�ѹֵͨ��DMA��ʽ����SRAM
extern __IO uint16_t ADC_ConvertedValue[NOFCHANEL];

// �ֲ����������ڱ���ת�������ĵ�ѹֵ 	 
float ADC_ConvertedValueLocal[NOFCHANEL*2]; 


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
	uint16_t temp0 = 0 ,temp1 = 0;
	
	USART_Config();																								// ���ô���		
	ADCx_Init();																									// ADC ��ʼ��	
	printf("\r\n ----����һ��˫ADC����ͬ���ɼ�ʵ��----\r\n");
	
	while (1)
	{	    
		temp0 = (ADC_ConvertedValue[0]&0XFFFF0000) >> 16;						// ȡ��ADC1���ݼĴ����ĸ�16λ�������ADC2��ת������		
		temp1 = (ADC_ConvertedValue[0]&0XFFFF);											// ȡ��ADC1���ݼĴ����ĵ�16λ�������ADC1��ת������
		
		ADC_ConvertedValueLocal[0] =(float) temp0/4096*3.3;
		ADC_ConvertedValueLocal[1] =(float) temp1/4096*3.3;
		
		printf("\r\n ADCx_1 value = %f V \r\n",ADC_ConvertedValueLocal[1]);
		printf("\r\n ADCx_2 value = %f V \r\n",ADC_ConvertedValueLocal[0]);
		
		printf("\r\n\r\n");
		Delay(0xffffee); 
		 
	}  
}



