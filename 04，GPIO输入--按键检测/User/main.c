/**
  ******************************************************************************
  * @file    main.c
  * @author  lolita
  * @brief   测试按键
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_key.h"


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
		LED_GPIO_Config();							/* LED端口初始化 */
		LED1(ON);
	
		Key_GPIO_Config();							/* 按键端口初始化 */
	
		while (1)												/* 轮询按键状态，若按键按下则反转LED */
		{
				if( Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON  )
				{						
						LED1_TOGGLE;						/*LED1反转*/
				}
				
				if( Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON  )
				{						
						LED2_TOGGLE;						/*LED2反转*/
				}	
		}
}










