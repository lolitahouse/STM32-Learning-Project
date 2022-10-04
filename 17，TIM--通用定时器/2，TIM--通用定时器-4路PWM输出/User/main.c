/**
  ******************************************************************************
  * @file    main.c
  * @author  lolita
  * @brief   TIM—通用定时器-4路PWM输出应用
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_GeneralTim.h" 



/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{				
	LED_GPIO_Config();				/* led 端口配置 */ 		
	GENERAL_TIM_Init();				/* 定时器初始化 */
	
  while(1)
  {     
         
  }
}






