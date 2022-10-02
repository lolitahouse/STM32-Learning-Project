/**
  ******************************************************************************
  * @file    main.c
  * @author  lolita
  * @brief   测试led
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_led.h"


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
	LED_GPIO_Config();	 											/* LED 端口初始化 */

	while (1)
	{
		LED1(ON);			  												// 亮
		Delay(0x0FFFFF);
		LED1(OFF);		   													// 灭

		LED2(ON);			 													// 亮
		Delay(0x0FFFFF);
		LED2(OFF);		   													// 灭

		LED3(ON);			 													// 亮
		Delay(0x0FFFFF);
		LED3(OFF);		   													// 灭	 
																			
		LED_RED;																/*轮流显示 红绿蓝黄紫青白 颜色*/
		Delay(0x0FFFFF);
		
		LED_GREEN;
		Delay(0x0FFFFF);
		
		LED_BLUE;
		Delay(0x0FFFFF);
		
		LED_YELLOW;
		Delay(0x0FFFFF);
		
		LED_PURPLE;
		Delay(0x0FFFFF);	
				
		LED_CYAN;
		Delay(0x0FFFFF);
		
		LED_WHITE;
		Delay(0x0FFFFF);
		
		LED_RGBOFF;
		Delay(0x0FFFFF);		
	}
}

