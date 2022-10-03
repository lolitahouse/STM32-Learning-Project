/**
  ******************************************************************************
  * @file    main.c
  * @author  lolita
  * @brief   串口中断接收测试
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_usart.h"


/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{							
		USART_Config();																								/*初始化USART 配置模式为 115200 8-N-1，中断接收*/
	
		Usart_SendString( DEBUG_USARTx,"这是一个串口中断实验\n");			/* 发送一个字符串 */
		printf("实验平台为野火指南者\n\n\n\n");	
		while(1);					
}










