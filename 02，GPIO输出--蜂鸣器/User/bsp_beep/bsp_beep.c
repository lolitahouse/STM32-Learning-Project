/**
  ******************************************************************************
  * @file    bsp_beep.c
  * @author  lolita
  * @brief   蜂鸣器应用函数接口
  ******************************************************************************
  */

#include "bsp_beep.h"

/**
  * @brief  初始化控制蜂鸣器的IO
  * @param  无
  * @retval 无
  */
void BEEP_GPIO_Config(void)
{
		
		GPIO_InitTypeDef	GPIO_InitStructure;								/*定义一个GPIO_InitTypeDef类型的结构体*/	
		RCC_APB2PeriphClockCmd(BEEP_GPIO_CLK, ENABLE);			/*开启控制蜂鸣器的GPIO的端口时钟*/
	
		GPIO_InitStructure.GPIO_Pin = BEEP_GPIO_PIN;				/*选择要控制蜂鸣器的GPIO*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		/*设置GPIO模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		/*设置GPIO速率为50MHz */
		GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStructure);			/*调用库函数，初始化控制蜂鸣器的GPIO*/
	
		GPIO_ResetBits(BEEP_GPIO_PORT, BEEP_GPIO_PIN);			/* 关闭蜂鸣器*/
}

