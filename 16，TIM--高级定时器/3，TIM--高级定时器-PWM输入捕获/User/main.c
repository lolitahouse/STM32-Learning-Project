/**
  ******************************************************************************
  * @file    main.c
  * @author  lolita
  * @brief   TIM—高级定时器-PWM输入捕获应用，通用定时器产生PWM波，高级定时器则捕获这个PWM，并测量周期和占空比
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_AdvanceTim.h"
#include "bsp_usart.h"
#include "bsp_GeneralTim.h" 


/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{	
	USART_Config();								/* 串口初始化 */		
	GENERAL_TIM_Init();						/* 通用定时器初始化，用于生成PWM信号 */		
	ADVANCE_TIM_Init();						/* 高级定时器初始化 ，用户捕获PWM信号*/
	
  while(1)
  { 
		
  }
}






