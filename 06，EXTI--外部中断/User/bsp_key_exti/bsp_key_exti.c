/**
  ******************************************************************************
  * @file    bsp_key_exti.c
  * @author  lolita
  * @brief   I/O线中断应用bsp
  ******************************************************************************
  */

#include "bsp_key_exti.h"


/**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;    
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);								/* 配置NVIC为优先级组1 */
  
  NVIC_InitStructure.NVIC_IRQChannel = KEY1_INT_EXTI_IRQ;  			/* 配置中断源：按键1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  		/* 配置抢占优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  					/* 配置子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  							/* 使能中断通道 */
  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = KEY2_INT_EXTI_IRQ;  			/* 配置中断源：按键2，其他配置同上 */  
  NVIC_Init(&NVIC_InitStructure);
}


 /**
  * @brief  配置IO为EXTI中断口，并设置中断优先级
  * @param  无
  * @retval 无
  */
void EXTI_Key_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(KEY1_INT_GPIO_CLK | KEY2_INT_GPIO_CLK,ENABLE);			/*开启按键GPIO口的时钟*/												
	NVIC_Configuration();																											/* 配置 NVIC 中断*/	
/*--------------------------KEY1配置-----------------------------*/
  GPIO_InitStructure.GPIO_Pin = KEY1_INT_GPIO_PIN;													/* 选择按键用到的GPIO */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  										/* 配置为浮空输入 */	
  GPIO_Init(KEY1_INT_GPIO_PORT, &GPIO_InitStructure);

  GPIO_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE, KEY1_INT_EXTI_PINSOURCE); 	/* 选择EXTI的信号源 */
  EXTI_InitStructure.EXTI_Line = KEY1_INT_EXTI_LINE;	
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;												/* EXTI为中断模式 */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;										/* 上升沿中断 */
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;  																/* 使能中断 */	
  EXTI_Init(&EXTI_InitStructure);	
  /*--------------------------KEY2配置-----------------------------*/
  GPIO_InitStructure.GPIO_Pin = KEY2_INT_GPIO_PIN;													/* 选择按键用到的GPIO */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  										/* 配置为浮空输入 */	
  GPIO_Init(KEY2_INT_GPIO_PORT, &GPIO_InitStructure);

  GPIO_EXTILineConfig(KEY2_INT_EXTI_PORTSOURCE, KEY2_INT_EXTI_PINSOURCE); 	/* 选择EXTI的信号源 */
  EXTI_InitStructure.EXTI_Line = KEY2_INT_EXTI_LINE;	
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;												/* EXTI为中断模式 */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;										/* 下降沿中断 */
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;  																/* 使能中断 */	
  EXTI_Init(&EXTI_InitStructure);
}




