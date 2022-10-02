/**
  ******************************************************************************
  * @file    main.c
  * @author  lolita
  * @brief   ϵͳʱ������
  ******************************************************************************
  */ 
	
#include 	"stm32f10x.h"
#include 	"bsp_led.h"
#include	"bsp_mcooutput.h"
#include	"bsp_clkconfig.h"


/* �򵥵���ʱ���� */
void Delay( uint32_t nCount)
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
	// ��������main����֮ǰ�������ļ��Ѿ�����,ϵͳʱ���ѱ���ʼ����72MHZ	
	// ��������ϵͳʱ��ʱ,��ѡ��ʹ��HSE����HSI	
	
	//HSE_SetSysClock(RCC_PLLMul_9);						// ʹ��HSE��SYSCLK = 8M * RCC_PLLMul_x, x:[2,3,...16],�����128M
	  HSI_SetSysClock(RCC_PLLMul_16);						// ʹ��HSIʱ��SYSCLK = 4M * RCC_PLLMul_x, x:[2,3,...16],�����64M	
	
		/* ע�⣺��Ͽ�J20,J21����ñ���Թرշ�����*/
	  MCO_GPIO_Config();												// MCO ���ų�ʼ��
	
	// ����MCO�������ʱ�ӣ���ʾ��������PA8�����������ʱ���ź�
	// ��PLLCLK/2��ΪMCO���ŵ�ʱ�������ϵͳʱ���Ƿ�����׼ȷ
	
	//RCC_MCOConfig(RCC_MCO_HSE);	             	
	//RCC_MCOConfig(RCC_MCO_HSI);	                   
	//RCC_MCOConfig(RCC_MCO_PLLCLK_Div2);    	
	  RCC_MCOConfig(RCC_MCO_SYSCLK);		      	//MCO�������������HSE,HSI,PLLCLK/2,SYSCLK	
	
		LED_GPIO_Config();												//LED �˿ڳ�ʼ��
		while (1)
		{
			LED2(ON);			  			// ��
			Delay(0x0FFFFF);
			LED2( OFF );		 			 // �� 
			Delay(0x0FFFFF);		
		}
}










