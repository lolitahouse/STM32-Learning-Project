/**
  ******************************************************************************
  * @file    main.c
  * @author  lolita
  * @brief   系统时钟配置
  ******************************************************************************
  */ 
	
#include 	"stm32f10x.h"
#include 	"bsp_led.h"
#include	"bsp_mcooutput.h"
#include	"bsp_clkconfig.h"


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
	// 程序来到main函数之前，启动文件已经调用,系统时钟已被初始化成72MHZ	
	// 重新设置系统时钟时,可选择使用HSE还是HSI	
	
	//HSE_SetSysClock(RCC_PLLMul_9);						// 使用HSE，SYSCLK = 8M * RCC_PLLMul_x, x:[2,3,...16],最高是128M
	  HSI_SetSysClock(RCC_PLLMul_16);						// 使用HSI时，SYSCLK = 4M * RCC_PLLMul_x, x:[2,3,...16],最高是64M	
	
		/* 注意：需断开J20,J21间跳帽，以关闭蜂鸣器*/
	  MCO_GPIO_Config();												// MCO 引脚初始化
	
	// 设置MCO引脚输出时钟，用示波器可在PA8测量到输出的时钟信号
	// 将PLLCLK/2作为MCO引脚的时钟来检测系统时钟是否配置准确
	
	//RCC_MCOConfig(RCC_MCO_HSE);	             	
	//RCC_MCOConfig(RCC_MCO_HSI);	                   
	//RCC_MCOConfig(RCC_MCO_PLLCLK_Div2);    	
	  RCC_MCOConfig(RCC_MCO_SYSCLK);		      	//MCO引脚输出可以是HSE,HSI,PLLCLK/2,SYSCLK	
	
		LED_GPIO_Config();												//LED 端口初始化
		while (1)
		{
			LED2(ON);			  			// 亮
			Delay(0x0FFFFF);
			LED2( OFF );		 			 // 灭 
			Delay(0x0FFFFF);		
		}
}










