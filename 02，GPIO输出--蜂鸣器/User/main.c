/**
  ******************************************************************************
  * @file    main.c
  * @author  lolita
  * @brief   测试beep
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_beep.h"


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
	
	BEEP_GPIO_Config();			/* BEEP GPIO 初始化 */
	
	while(1)
	{		
		BEEP( ON ); 			  // 响
		Delay(0x0FFFFF);	
		BEEP( OFF );		  // 不响
		Delay(0x0FFFFF);
	}
}

