/**
  ******************************************************************************
  * @file    main.c
  * @author  lolita
  * @brief   Systick延时
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_systick.h"


/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{					
		LED_GPIO_Config();		/* LED 端口初始化 */
	
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










