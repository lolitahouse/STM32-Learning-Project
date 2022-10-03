/**
  ******************************************************************************
  * @file    main.c
  * @author  lolita
  * @brief   DMA 存储器到外设（串口）数据传输实验
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart_dma.h"


//简单的延时函数
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
		uint16_t i = 0;
  
		USART_Config(); 											/* 初始化USART */ 
		USARTx_DMA_Config();									/* 配置使用DMA模式 */			 
		LED_GPIO_Config(); 										/* 配置RGB彩色灯 */		
	 
		for(i=0;i<SENDBUFF_SIZE;i++) 					/*填充将要发送的数据*/
			 SendBuff[i]	 = 'P';

/* 演示DMA持续发送数据，量非常大，长时间运行可能导致串口调试助手会卡死，鼠标乱飞的情况，
  *或把DMA配置中的循环模式改为单次模式*/		 
  
		USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Tx, ENABLE);			/* USART1 向 DMA发出TX请求 */
  /* 此时CPU是空闲的，可以干其他的事情，例如同时控制LED */  
  while(1)
  {
			LED3_TOGGLE;
			Delay(0xFFFFF);
  }
}









