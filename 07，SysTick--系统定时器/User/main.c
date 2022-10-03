/**
  ******************************************************************************
  * @file    main.c
  * @author  lolita
  * @brief   Systick��ʱ
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_systick.h"


/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{					
		LED_GPIO_Config();		/* LED �˿ڳ�ʼ�� */
	
		while(1)
		{
				LED1( ON ); 
				SysTick_Delay_Ms( 1000 );
				LED1( OFF );
	  
				LED2( ON );
				SysTick_Delay_Ms( 1000 );
				LED2( OFF );
			
				LED3( ON );
				SysTick_Delay_Ms( 1000 );
				LED3( OFF );
	  } 						
}










