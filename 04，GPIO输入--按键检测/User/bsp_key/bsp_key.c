/**
  ******************************************************************************
  * @file    bsp_key.c
  * @author  lolita
  * @brief   按键应用bsp（扫描模式）
  ******************************************************************************
  */

#include "bsp_key.h"

/**
  * @brief  配置按键用到的I/O口
  * @param  无
  * @retval 无
  */
void Key_GPIO_Config(void)
{
		GPIO_InitTypeDef	GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK | KEY2_GPIO_CLK,ENABLE);								/*开启按键端口的时钟*/
		
		GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN; 																//选择按键的引脚
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;												//设置按键的引脚为浮空输入
		GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);															//使用结构体初始化按键
	
		GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN; 																//选择按键的引脚
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;												//设置按键的引脚为浮空输入
		GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);															//使用结构体初始化按键
}

/**
	* @brief  检测是否有按键按下
	* @param  GPIOx：x 可以是 A，B，C，D或者 E
	*		     	GPIO_Pin：待读取的端口位 	
	* @retval KEY_OFF(没按下按键)、KEY_ON（按下按键）
	*/
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{
		if( GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_ON )											/* 检测是否有按键按下 */
		{
			 while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON); 									/* 等待按键释放 */
			 return 	KEY_ON;
		}
		else
			 return 	KEY_OFF;
}










