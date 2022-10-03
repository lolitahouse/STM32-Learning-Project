/**
  ******************************************************************************
  * @file    main.c
  * @author  lolita
  * @brief   测试按键（中断模式/EXTI模式）
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_key_exti.h"


/* 简单的延时函数 */
void Delay( uint32_t nCount)
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
		LED_GPIO_Config();							/* LED 端口初始化 */  	
		EXTI_Key_Config(); 							/* 初始化EXTI中断，按下按键会触发中断 */	
	
		while(1);												/* 等待中断，由于使用中断方式，CPU不用轮询按键 */
}










