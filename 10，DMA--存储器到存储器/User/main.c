/**
  ******************************************************************************
  * @file    main.c
  * @author  lolita
  * @brief   DMA-存储器到存储器模式 ，即内部的FLASH到内部的SRAM
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_dma.h"


//简单的延时函数
void Delay(__IO uint32_t nCount)	 
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
  uint8_t TransferStatus;					/* 定义存放比较结果变量 */
  	
	LED_GPIO_Config();							/* LED 端口初始化 */      
  LED_PURPLE;  										/* 设置RGB彩色灯为紫色 */   
  Delay(0xFFFFFF);  						 	/* 简单延时函数 */  
  
  DMA_Config(); 																														/* DMA传输配置 */    
  while(DMA_GetFlagStatus(DMA_FLAG_TC)==RESET);															/* 等待DMA传输完成 */   
  TransferStatus = Buffercmp(aSRC_Const_Buffer, aDST_Buffer, BUFFER_SIZE);	/* 比较源数据与传输后数据 */    
  if(TransferStatus == 0)  																									/* 判断源数据与传输后数据比较结果*/
	{
			LED_RED;																															/* 数据不相等时RGB彩色灯显示红色 */
	}
  else
	{
			LED_BLUE; 																														/* 数据相等时RGB彩色灯显示蓝色 */
	}

	while (1);
}









