/**
  ******************************************************************************
  * @file    main.c
  * @author  lolita
  * @brief   DMA �洢�������裨���ڣ����ݴ���ʵ��
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart_dma.h"


//�򵥵���ʱ����
void Delay( uint32_t nCount)	 
{
	for(; nCount != 0; nCount--);
}

/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{
		uint16_t i = 0;
  
		USART_Config(); 											/* ��ʼ��USART */ 
		USARTx_DMA_Config();									/* ����ʹ��DMAģʽ */			 
		LED_GPIO_Config(); 										/* ����RGB��ɫ�� */		
	 
		for(i=0;i<SENDBUFF_SIZE;i++) 					/*��佫Ҫ���͵�����*/
			 SendBuff[i]	 = 'P';

/* ��ʾDMA�����������ݣ����ǳ��󣬳�ʱ�����п��ܵ��´��ڵ������ֻῨ��������ҷɵ������
  *���DMA�����е�ѭ��ģʽ��Ϊ����ģʽ*/		 
  
		USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Tx, ENABLE);			/* USART1 �� DMA����TX���� */
  /* ��ʱCPU�ǿ��еģ����Ը����������飬����ͬʱ����LED */  
  while(1)
  {
			LED3_TOGGLE;
			Delay(0xFFFFF);
  }
}









